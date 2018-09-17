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
#define COUNTDOWN_TIME 12 //set countdown time in seconds
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
    printf("\nTime's UP!\n");
    done = 1;
    return NULL;
}

void *accept_input(void *answer_void_ptr){
    int i=0;
    char *answer_ptr = (char *)answer_void_ptr;
    printf("GO!\n\n");
    for(; done != 1;i++){
        scanf("%s",answers[i]);
    }
    return NULL;
}

int main(){

    int timer, count = 0, y = 0, i=0, j=0, score=0;
    int sub_score, k, exist=0;
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
    if(pthread_create(&accept_thread, NULL, accept_input, &answer)){
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    /* wait for the 2nd thread to finish */
    if(pthread_join(counter_thread, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    pthread_cancel(accept_thread);

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

    printf("\nPlayer's Answers: \n");
    for(i=0;i<MAX_ANSWER;i++){
        if(strcmp(answer[i],"")==0){
            count = i;
            break;
        }
        printf("%s\n",answer[i]);
    }

    //printf("\nFiltered answers:\n");
    /* get unique answers*/
    for(i=0,k=0;i<count;i++){
        //printf("Checking %d \"%s\" \n",i,answer[i]);
        for(j=0,exist=0;j<k;j++){
            if(strcmp(answer[i],filtered[j])==0){
                //printf("\"%s\" exists\n",answer[i]);
                exist++;
            }
        }
        if(exist==0){
            strcpy(filtered[j],answer[i]);
            //printf("%d added \"%s\" to list\n",k,filtered[k]);
            k++;
        }
    }
    count = k;
    /* compute score */
    for(i=0,k=0; i<count; i++){
        for(j=0, sub_score=0; j<MAX_ANSWER; j++){
            if(strcmp(filtered[i],key[j])==0){
                score++;
                sub_score++;
            }
        }
        if(sub_score==0){
            strcpy(wrong[k],filtered[i]);
            k++;
        }
    }
    
    printf("\nCorrect Answers: \n");
    for(i=0;i<MAX_ANSWER;i++){
        printf("%s\n",key[i]);
    }

    printf("\nIncorrect Answers (%d): \n",k);
    if(k!=0){
        for(i=0;i<k;i++){
            if(strcmp(wrong[i],"")==0){
                break;
            }
            printf("%s\n",wrong[i]);
        }
    }
    printf("\nYou got %d/%d numbers correctly\n",score,MAX_ANSWER);

    return 0;

}