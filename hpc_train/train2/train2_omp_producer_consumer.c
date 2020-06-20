/* File:     prog5.6_omp_producer_comsumer.c
 * Purpose:  Implement a producer-consumer program in which some of the
 *           threads are producers and others are consumers.  The 
 *           producers read text from a collection of files, one per 
 *           producer.  They insert lines of text into a single shared 
 *           queue.  The consumers take the lines of text and tokenize 
 *           them -- i.e., identify strings of characters separated by
 *           whitespace from the rest of the line.  When a consumer 
 *           finds a token, it writes to stdout.
 *
 * Compile:  gcc -g -Wall -fopenmp -o train2_omp_producer_comsumer 
 *                 train2_omp_producer_comsumer.c
 *          gcc -g -Wall -fopenmp -o train2_omp_producer_comsumer  ./train2_omp_producer_consumer.c 

 * Run:      ./train2_omp_producer_comsumer <producers> <consumers>
 *           wei1.txt wei2.txt wei3.txt
 * Input:    A file that contains list of filename to read as input text
 * Output:   Output tokens or strings of characters separated from the
 *           rest of the input by whitespace
 *
 * Notes:     
 * 1. There must be at least one producer and one consumer
 * 2. There are two critical sections
 *    - One is protected by an atomic clause in Prod_cons
 *    - The other is protected by critical directives: one
 *      in Enqueue and the other in Dequeue
 *
 * IPP:     Programming Assignment 5.6
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#define DEBUG
const int MAX_FILES = 50;
const int MAX_CHAR = 100;

/* struct for queue */
struct list_node_s {
   char* data;
   struct list_node_s* next;
};

void Usage(char* prog_name);
void Prod_cons(int prod_count, int cons_count, FILE* files[], int file_count);
void Get_files(FILE* files[], int* file_count_p);
void Read_file(FILE* file, struct list_node_s** queue_head, 
      struct list_node_s** queue_tail, int my_rank);
void Enqueue(char* line, struct list_node_s** queue_head, 
      struct list_node_s** queue_tail);
struct list_node_s* Dequeue(struct list_node_s** queue_head, 
      struct list_node_s** queue_tail, int my_rank);
void Tokenize(char* data, int my_rank);
void Print_queue(int my_rank, struct list_node_s* queue_head);

int main(int argc, char* argv[]) {
   int   prod_count, cons_count;
   FILE* files[MAX_FILES];
   int   file_count;
   
   if (argc != 3) Usage(argv[0]);
   prod_count = strtol(argv[1], NULL, 10);
   cons_count = strtol(argv[2], NULL, 10);
   
   /* Read in list of filenames and open files */
   Get_files(files, &file_count);

#  ifdef DEBUG
   printf("prod_count = %d, cons_count = %d, file_count = %d\n", 
         prod_count, cons_count, file_count);
#  endif

   /* Producer-consumer */
   Prod_cons(prod_count, cons_count, files, file_count);
   
   return 0;
}  /* main */


/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {
   
   fprintf(stderr, "usage: %s <producer count> <consumer count>\n", 
         prog_name);
   exit(0);
}  /* Usage */


/*--------------------------------------------------------------------
 * Function:    Get_files
 * Purpose:     Read list of files and open them
 * Out args:    files: array of files
 *              file_count_p: total number of files
 */
void Get_files(FILE* files[], int* file_count_p) {   
   int i = 0;
   char filename[MAX_CHAR]; 
   
   while (scanf("%s", filename) != -1) {
      files[i] = fopen(filename, "r");
#     ifdef DEBUG
      printf("file %d = %s, desc = %p\n", i, filename, files[i]);
#     endif
      if (files[i] == NULL) {
         fprintf(stderr, "Can't open %s\n", filename);
         fprintf(stderr, "Quitting . . . \n");
         exit(-1);
      }
      i++;
   }
   *file_count_p = i;
}  /* Get_files */


/*--------------------------------------------------------------------
 * Function:   Prod_cons
 * Purpose:    Divides tasks among threads
 * In args:    thread_count, files, file_count
 */
void Prod_cons(int prod_count, int cons_count, FILE* files[], 
      int file_count) {
   int thread_count = prod_count + cons_count;
   struct list_node_s* queue_head = NULL;
   struct list_node_s* queue_tail = NULL;
   int prod_done_count = 0;

#  pragma omp parallel num_threads(thread_count) default(none) \
      shared(file_count, queue_head, queue_tail, files, prod_count, \
            cons_count, prod_done_count)
   {
      int my_rank = omp_get_thread_num(), f;
      if (my_rank < prod_count) {
         /* Producer code */
         /* A cyclic partition of the files among the producers */
         for (f = my_rank; f < file_count; f += prod_count) {
#           ifdef DEBUG
            printf("Th %d > about to read file %d\n", my_rank, f);
#           endif
            Read_file(files[f], &queue_head, &queue_tail, my_rank);
#           ifdef DEBUG
          Print_queue(my_rank, queue_head);
#           endif
         }
         //atomic保护临界区
#        pragma omp atomic
         prod_done_count++;
      } else {
         /* Consumer code */
         struct list_node_s* tmp_node;
        //生产者还在生产,消费者进行消费，出队一行然后分词
         while (prod_done_count < prod_count) {
            tmp_node = Dequeue(&queue_head, &queue_tail, my_rank);
            if (tmp_node != NULL) {
               Tokenize(tmp_node->data, my_rank);
               free(tmp_node);
            }
         }
#        ifdef DEBUG
         Print_queue(my_rank, queue_head);
#        endif
         //队列非空的时候消费者消费
         while (queue_head != NULL) {
            tmp_node = Dequeue(&queue_head, &queue_tail, my_rank);
            if (tmp_node != NULL) {
               Tokenize(tmp_node->data, my_rank);
               free(tmp_node);
            }
         }
      }
   } /* pragma omp parallel */
}  /* Prod_cons */

/*--------------------------------------------------------------------
 * Function:    Read_file
 * Purpose:     read text line from file into the queue linkedlist
 * In arg:      file, my_rank 
 * In/out arg:  queue_head, queue_tail
 */
//按行读取文件中内容，并加入队列
void Read_file(FILE* file, struct list_node_s** queue_head, 
      struct list_node_s** queue_tail, int my_rank) {
   
   char* line = malloc(MAX_CHAR*sizeof(char));
   while (fgets(line, MAX_CHAR, file) != NULL) {
      printf("Th %d > read line: %s", my_rank, line);
      Enqueue(line, queue_head, queue_tail);
      //free(line);
      line = malloc(MAX_CHAR*sizeof(char));
   }
   //free(line);
   fclose(file);
}  /* Read_file */


/*--------------------------------------------------------------------
 * Function:    Enqueue
 * Purpose:     create data node, add into queue linkedlist
 * In arg:      line
 * In/out arg:  queue_head, queue_tail
 */
//入队，　有临界区，使用critical 指令保护
void Enqueue(char* line, struct list_node_s** queue_head, 
      struct list_node_s** queue_tail) {
   struct list_node_s* tmp_node = NULL;
   
   tmp_node = malloc(sizeof(struct list_node_s));
   tmp_node->data = line;
   tmp_node->next = NULL;
   
#  pragma omp critical
   if (*queue_tail == NULL) { // list is empty
      *queue_head = tmp_node;
      *queue_tail = tmp_node;
   } else {
      (*queue_tail)->next = tmp_node;
      *queue_tail = tmp_node;
   }   
}  /* Enqueue */


/*--------------------------------------------------------------------
 * Function:    Dequeue
 * Purpose:     remove a node from queue linkedlist and tokenize them
 * In arg:      my_rank
 * In/out arg:  queue_head, queue_tail
 * Ret val:     Node at head of queue, or NULL if queue is empty
 */
//出队，有临界区，使用critical 指令保护
struct list_node_s* Dequeue(struct list_node_s** queue_head, 
      struct list_node_s** queue_tail, int my_rank) {
   struct list_node_s* tmp_node = NULL;
   
   if (*queue_head == NULL) // empty
      return NULL;
   
#  pragma omp critical
   {
      if (*queue_head == *queue_tail) // last node
         *queue_tail = (*queue_tail)->next;
   
      tmp_node = *queue_head;
      *queue_head = (*queue_head)->next;
   }
   
   return tmp_node;
}  /* Dequeue */

/*--------------------------------------------------------------------
 * Function:    Tokenize
 * Purpose:     Identify words (i.e., sequences of characters separated 
 *                 by white space and print them out)
 * In arg:      data, my_rank
 *
 */
//分词函数
void Tokenize(char* data, int my_rank) {
   char* my_token, *word;

#  ifdef DEBUG
   printf("Th %d > dequeue line: %s\n", my_rank, data);
#  endif
   my_token = strtok_r(data, " \t\n", &word);
   while (my_token != NULL) {
      printf("Th %d token: %s\n", my_rank, my_token);
      my_token = strtok_r(NULL, " \t\n", &word);
   }
}  /* Tokenize */

/*--------------------------------------------------------------------
 * Function:    Print_queue
 * Purpose:     Print the contents of the queue
 * In args:     queue_head, queue_tail
 */
//将队列中元素全部打印
void Print_queue(int my_rank, struct list_node_s* queue_head) {
   struct list_node_s* curr_p = queue_head;

   printf("Th %d > queue = \n", my_rank);
#  pragma omp critical
   while (curr_p != NULL) {
      printf("%s", curr_p->data);
      curr_p = curr_p->next;
   }
   printf("\n");
}  /* Print_queue */





// base) wei@wei-PC:~/hpc_train/train2$ gcc -g -Wall -fopenmp -o train2_omp_producer_consumer  ./train2_omp_producer_consumer.c  && ./train2_omp_producer_consumer 3 3
// wei1.txt
// file 0 = wei1.txt, desc = 0x55fc77d81440
// wei2.txt
// file 1 = wei2.txt, desc = 0x55fc77d81a80
// wei3.txt
// file 2 = wei3.txt, desc = 0x55fc77d81cb0
// prod_count = 3, cons_count = 3, file_count = 3
// Th 0 > about to read file 0
// Th 0 > read line: wei1_11 wei1_12
// Th 1 > about to read file 1
// Th 5 > dequeue line: wei1_11 wei1_12

// Th 0 > read line: wei1_21 wei1_22Th 4 > dequeue line: wei1_21 wei1_22
// Th 4 token: wei1_21
// Th 4 token: wei1_22
// Th 0 > queue = 

// Th 1 > read line: wei2_11 wei2_12
// Th 4 > dequeue line: wei2_11 wei2_12

// Th 5 token: wei1_11
// Th 1 > read line: wei2_21 wei2_22Th 2 > about to read file 2
// Th 3 > dequeue line: wei2_21 wei2_22
// Th 3 token: wei2_21
// Th 3 token: wei2_22
// Th 5 token: wei1_12
// Th 4 token: wei2_11
// Th 4 token: wei2_12
// Th 1 > queue = 
// (null)
// Th 2 > read line: wei3_11 wei3_12
// Th 2 > read line: wei3_21 wei3_22
// Th 2 > read line: 
// Th 3 > dequeue line: wei3_21 wei3_22

// Th 3 token: wei3_21
// Th 3 token: wei3_22
// Th 5 > dequeue line: 

// Th 2 > queue = 

// Th 5 > queue = 
// Th 4 > dequeue line: wei3_11 wei3_12

// Th 4 token: wei3_11
// Th 4 token: wei3_12
// Th 4 > queue = 

// Th 3 > queue =

 


