#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

int BUFF_SIZE;           /* total number of slots */

int TTP;//total items to produce                
int NP; //number of producers
int NC;//number of consumers
int num_sleep;//numbers of sleep


typedef struct
{
    int* buf;   //shared variable
    int curr_buf_size; //current size of buffer         
} sbuf_t;

sbuf_t shared; //name of buffer
sem_t mutex;
int item = 0;
void *Producer(void *arg)
{
    int i, index;

    index = (int)arg;

    while(item<TTP) //loop to run until every producer has been produced
    {
	int sleep_time = rand() % num_sleep +1;
	sleep(sleep_time); //pretending to produce
	sem_wait(&mutex); //locks so that multiple producers donot produce values simultaneously and making total count inconsistent
	while(shared.curr_buf_size==BUFF_SIZE) //buffer is full, wait until it is consumed
                sleep(1);
	if(item==TTP){
	sem_post(&mutex);
		break;
	}
	shared.buf[item]=item; 
        printf("item %d produced by producer %d\n",item,index);
	item++;
	shared.curr_buf_size++;
	sem_post(&mutex);

       
    }
    return NULL;
}
sem_t cons_mutex;
int item_cons=0;
int successPrint=1;
void *Consumer(void *arg)
{
    int i, index;
    index = (int)arg;
    while(item_cons<=TTP){
    int sleep_time = rand()%num_sleep +1;
    sleep(sleep_time);
    sem_wait(&cons_mutex); //lock so that multiple consumers donot consume same value and make total inconsistent
     if(item_cons>=TTP){ //no more items to consume
	if(successPrint){
        printf("Success\n");
	successPrint=0;
	}
        sem_post(&cons_mutex);
        break;
        }
    while(shared.curr_buf_size==0){ //no items in buffer so wait, until one is produced
	sleep(1);
	}
    printf("item %d consumed by %d consumer\n",item_cons,index);
    item_cons++;
    shared.curr_buf_size--;
    sem_post(&cons_mutex);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t idP, idC;
    int index;

	NP  = atoi(argv[1]);//argument for number of producers
        NC = atoi(argv[2]);//argument for number of consumers
        num_sleep  = atoi(argv[3]);//arguments for number of sleep
        TTP  = atoi(argv[4]);//argument for total number of items to produce
	BUFF_SIZE=atoi(argv[5]);
	shared.buf=(int*)malloc(BUFF_SIZE*sizeof(int));
	shared.curr_buf_size=0;
	sem_init(&mutex,0,1);
	sem_init(&cons_mutex,0,1);
   	for (index = 0; index < NP; index++)//creates producer thread
   	 {
        printf("producing thread %d\n",index);
        pthread_create(&idP, NULL, Producer, (void*)index);
    	}
    
    for(index=0; index<NC; index++)//creates consumer thread
    {
	printf("making consumer thread %d\n",index);
       pthread_create(&idC, NULL, Consumer, (void*)index);
    }

    pthread_exit(NULL);
}
