#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h> 
#include <stdint.h>
#include <unistd.h>

#include "bbuff.h"
#include "stats.h"

_Bool stop_thread = false;


typedef struct{
    int factory_number;
    double time_stamp_in_ms;
}candy_t;

double current_time_in_ms(void){
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec*1000.0 + now.tv_nsec/1000000.0;
}

void* factory_thread(void* arg){
    int fnumber = *((int*)arg);
    while(!stop_thread){
        int time_wait = rand()%4;
        printf("\tFactory %d ships candy & waits %ds\n", fnumber, time_wait);
        candy_t* new_candy = malloc(sizeof(candy_t)); //allocate new cady
        new_candy->factory_number = fnumber;
        new_candy->time_stamp_in_ms = current_time_in_ms();
        bbuff_blocking_insert(new_candy); // add candy to buffer
        stats_record_produced(fnumber);  // add candy to stats
        sleep(time_wait);
    }
    printf("Candy-factory %d done\n", fnumber);
    pthread_exit(NULL);
}

void* kid_thread(void* arg){
    // int knumber = *((int*)arg);
    while(1){
        int time_wait = rand()%2;
        candy_t* candynode = (candy_t*)bbuff_blocking_extract(); //extract from buffer
        int fnumber = candynode->factory_number;
        // printf("\tKid %d eats candy which produced by factory %d & waits %ds\n", knumber, fnumber, time_wait);
        if(candynode != NULL){
            stats_record_consumed(fnumber, current_time_in_ms() - candynode->time_stamp_in_ms); // recorde ino stats
        }
        free(candynode);
        sleep(time_wait);
    }
    pthread_exit(NULL);
}



int main( int argc, char *argv[] ){ // argc start at 1
    //1. Extract arguments
    int factory;
    int kid;
    int second;
    if(argc == 1){
        printf("%s\n", "Error: No argument were passed.");
        exit(1);
    } 
    else if(argc == 4){
        factory = atoi(argv[1]);
        kid = atoi(argv[2]);
        second = atoi(argv[3]);
        if(factory <= 0 || kid <= 0 || second <= 0){
        printf("%s\n", "Error: All arguments must be positive.");
        exit(1);
        }
        // else{
        //     printf("Arguments:\n");
        //     for (int i = 1; i < argc; ++i){
        //         printf("%d. %s\n", i, argv[i]);
        //     }
        // }
    }
    else{
        printf("%s\n", "Error: Need exactly three arguments.");
        exit(1);
    }

    //2. Initialize modules
    bbuff_init();
    stats_init(factory);
    
    //3. Launch candy-factory threads
    pthread_t factory_arr[factory];
    int factory_num[factory];
    for (int i = 0; i < factory; i++){
        factory_num[i] = i;
        if(0 != pthread_create(&factory_arr[i], NULL, factory_thread, (void*)&factory_num[i])){
            printf("%s\n", "Error: Fail to launch factory thread.");
            exit(1);
        }
    }

    //4.  Launch kid threads
    pthread_t kid_arr[kid];
    int kid_num[kid];
    for (int j = 0; j < kid; j++){
        kid_num[j] = j;
        if(0 != pthread_create(&kid_arr[j], NULL, kid_thread, (void*)&kid_num[j])){
            printf("%s\n", "Error: Fail to launch kid thread.");
            exit(1);
        }
    }

    // 5.  Wait for requested time
    for(int k = 0; k < second; k++){
        sleep(1);
        printf("Time %ds\n", k);
    }

    // 6.  Stop candy-factory threads
    stop_thread = true;
    printf("%s\n", "Stopping candy factories...");
    for(int l = 0; l < factory; l++){
        pthread_join(factory_arr[l], NULL); //factories join terminated threads
    }

    // 7.  Wait until no more candy
    while(!bbuff_is_empty()){
        printf("Waiting for all candy to be consumed\n");
        sleep(1);
    }

    // 8.  Stop kid threads
    printf("Stopping kids.\n"); 
    for(int n = 0; n < kid; n++){
        pthread_cancel(kid_arr[n]);	// stop and cancel kid threads
        pthread_join(kid_arr[n], NULL); //kids join the terminated threads
    }
    // 9.  Print statistics
    stats_display();
    // 10. Cleanup any allocated memory
    stats_cleanup();
    // free(factory_arr);
    // free(kid_arr);
    
    return 0;
}
