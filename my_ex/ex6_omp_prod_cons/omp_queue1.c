#include <stdio.h>
#include<stdlib.h>
#include "queue1.h"
#define USE_MAIN

#ifdef  USE_MAIN
int main()
{
    char op;
    int src,mesg;
    struct queue_s* q_p=Allocate_queue();
    printf("Op? (e,d,p,s,f,q)\n");
    scanf(" %c",&op);
    while(op!='q'&&op!='Q')
    {
        switch(op)
        {
           case 'e':
           case 'E':
               printf("Src? Mesg? \n");
               scanf("%d%d",&src,&mesg);
               Enqueue(q_p,src,mesg);
               break;
            case 'd':
            case 'D':
                if(Dequeue(q_p,&src,&mesg))
                    printf("Dequeued src=%d, mesg=%d\n",src,mesg);
                else
                    printf("Queue is empty");
                break;
            case 's':
            case 'S':
                printf("Mesg?\n");
                scanf("%d",&mesg);
                if(Search(q_p,mesg,&src))
                    printf("Found %d from %d ",mesg,src);
                else
                    printf("Didn't find %d ",mesg);
                break;
            case 'p':
            case 'P':
                print_queue(q_p);
                break;
            case 'f':
            case 'F':
                Free_queue(q_p);
                break;
            default:
                printf("%c isn't a valid command \n",op);
                printf("please try again\n");
    }
    printf("Op? (e,d,p,s,f,q)\n");
    scanf(" %c",&op);
  }

    Free_queue(q_p);
    free(q_p);
    return 0;
}//main
#endif