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
#define MAX_ANSWER 5 /* Number of answers to be considered */

int done = 0;
char answers[ARRAY_SIZE][LINE_LEN];  /* store answers from scanf */

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

    int timer, count = 0, y = 0, i=0, j=0, score=0;
    int sub_score, k=0;
    char answer[MAX_ANSWER][LINE_LEN]; /* store first 5 answers to this variable */
    char filtered[MAX_ANSWER][LINE_LEN]; /* store filetered unique answers */
    char key[MAX_ANSWER][LINE_LEN] = {"swimming","eating","vacation","hiking","sleeping"};
    char wrong[MAX_ANSWER][LINE_LEN];

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

    pthread_cancel(accept_thread);

    /* count number of elements in answer
    for(count = 0, i=0; i < ARRAY_SIZE; i++){
        if(strlen(answers[i])!=0){
            count++;
            printf("line %d: %s \ncount: %d\n",i, answers[i],count);
        }
    } */

    /* get first five elements of answer */
    for(i = 0; i<5; i++){
        if(strcmp(answers[i],"")==0){
            count = i;
            //printf("count %d\n",count);
            break;
        }
        else{
            strcpy(answer[i],answers[i]);
            printf("%d %s\n",i, answer[i]);
        }
    }

    printf("Player's Answers: \n");
    for(i=0;i<count;i++){
        printf("%s\n",answer[i]);
    }

    /* compute score */
    for(i=0; i<count; i++){
        for(j=0, sub_score=0; j<MAX_ANSWER; j++){
            if(strcmp(answer[i],key[j])==0){
                score++;
                sub_score++;
            }
        }
        if(sub_score==0){
            strcpy(wrong[k],answer[i]);
            k++;
        }
    }
    
    printf("Correct Answers: \n");
    for(i=0;i<5;i++){
        printf("%s\n",key[i]);
    }

    printf("Incorrect Answers: \n");
    if(k!=0){
        for(i=0;i<k;i++){
            printf("%s\n",wrong[i]);
        }
    }
    printf("Your score is: %d out of 5\n",score);

    return 0;

}