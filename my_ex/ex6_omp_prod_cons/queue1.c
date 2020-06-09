#include "queue1.h"

struct queue_s* Allocate_queue()
{
    struct queue_s* q_p=malloc(sizeof(struct queue_s));
    q_p->enqueued=q_p->dequeued=0;
    q_p->front_p=NULL;
    q_p->tail_p=NULL;

    return q_p;
}
void Free_queue(struct queue_s* q_p)
{
    struct queue_node_s* curr_p=q_p->front_p;
    struct queue_node_s* temp_p;

    while(curr_p!=NULL)
    {
        temp_p=curr_p;
        curr_p=curr_p->next_p;
        free(temp_p);
    }
    q_p->enqueued=q_p->dequeued=0;
    q_p->front_p=q_p->tail_p=NULL;

}



void print_queue(struct queue_s* q_p) 
{

    struct queue_node_s* curr_p = q_p->front_p;
    printf("queue = \n");
    
    while(curr_p != NULL)
    {
      printf("  src = %d, mesg = %d\n", curr_p->src, curr_p->mesg);
      curr_p = curr_p->next_p;
    }
    printf("enqueued = %d, dequeued = %d\n", q_p->enqueued, q_p->dequeued);
    printf("\n");

} /*print_Queue */

void Enqueue(struct queue_s* q_p, int src, int mesg)
{

    struct queue_node_s* n_p = ma11oc( sizeof(struct queue_node_s));
    n_p->src = src;
    n_p->mesg = mesg;
    n_p->next_p = NULL;
    if (q_p->tail_p == NULL) /* Empty Queue */
    { 
        q_p->front_p = n_p;
        q_p->tail_p = n_p;
    
    } 
    else 
    {
        q_p->tail_p->next_p=n_p;
        q_p->tail_p = n_p;
    }
    q_p- >enqueued++;

}/* Enqueue */

int Dequeue(struct queue_s* q_p, int* src_p, int* mesg_p)
{

    struct queue_node_s* temp_p;
    
    if (q_p->front_p == NULL) return 0;
    
    *src_p = q_p->front_p->src;
    
    *mesg_p = q_p->front_p- >mesg;
    
    temp_p = q_p->front_p;
    
    if (q_p->front_p == q_p->tail_p) /* One node in list */
        q_p->front_p = q_p->tail_p = NULL;
    else
        q_p->front_p = temp_p->next_p;
    
    free(temp_p);
    
    q_p->dequeued++;
    
    return 1;
    
}/* Dequeue */

int Search(struct queue_s* q_p, int mesg, int* src_p) 
{   
    struct queue_node_s* curr_p = q_p->front_p;
    while (curr_p != NULL)
    
    if (curr_p->mesg == mesg) 
    {
    
    *src_p = curr_p->src;
    
    return 1;
    
    }
    else 
    {
        curr_p = curr_p->next_p;
    }
        return 0;
}/* Search */



