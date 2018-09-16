#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

#define LINE_LEN 30
#define ARRAY_SIZE 20
#define COUNTDOWN_TIME 10 //set countdown time in seconds

int done = 0;
char answers[ARRAY_SIZE][LINE_LEN];

/* this function is run by the second thread */
void *countdown(void *count_void_ptr){
    /* countdown to COUNTDOWN_TIME seconds */
    int *count_ptr = (int *)count_void_ptr;
    while(++(*count_ptr) < COUNTDOWN_TIME){   
        sleep(1);
    }
    printf("Time's UP!\n");
    done = 1;
    return NULL;
}

void *accept_input(void *answer_void_ptr){
    int i=0;
    char *answer_ptr = (char *)answer_void_ptr;
    printf("Enter your answers: ");
    for(; done != 1;i++){
        scanf("%s",answers[i]);
    }
    return NULL;
}

int main(){

    int timer, count = 0, y = 0, i=0;
    char answer[ARRAY_SIZE][LINE_LEN];

    /* initialize reference to second and third thread */
    pthread_t counter_thread, accept_thread;
    printf("Welcome to Family Feud!\nQuestion: According to survey, what are the top 5 activites people do during the summer?\n");
    for(timer=3;timer>0;timer--){
        printf("%d...\n",timer);
        sleep(1);
    }

    /* create a second thread which executes countdown(&count) */
    if(pthread_create(&counter_thread, NULL, countdown, &count)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    /* create a third thread for input */
    if(pthread_create(&accept_thread, NULL, accept_input, &answer)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    /* wait for the 2nd thread to finish */
    if(pthread_join(counter_thread, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }
    //pthread_exit(NULL);
    pthread_cancel(accept_thread);

    for(count = 0, i=0; i < 20; i++){
        if(strlen(answers[i])!=0){
            count++;
            printf("line %d: %s \ncount: %d\n",i, answers[i],count);
        }
    }

    printf("Count: %d\nShe said: \n", count);
    
    for(i=0;i<count;i++){
        printf("%s\n",answers[i]);
    }
    

    return 0;

}