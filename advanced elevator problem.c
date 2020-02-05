#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
int currentTime=0;
typedef struct {
int id; /* 0, 1, 2, 3, … */
int from_floor, to_floor; /* i.e., from where to where */
double arrival_time; /* The time at which the person arrives */
int pickedUp;
int droppedOff;
 /* You can add more fields if you want */
} person;
typedef struct {
int id; /* 0, 1, 2, 3, … */
int current_floor; /* Current location of the elevator */
pthread_mutex_t lock;
//pthread_cond_v cv; /* Will be used to block the elevator if there is no request */
person *people;/* All the people currently inside the elevator */
 /* You can add more fields if you want */
int currentCount;
int index;
} elevator;
typedef struct { /* All the global information related to the elevator simulation */
int num_elevators;
int num_floors;
int beginning_time;
int elevator_speed;
int simulation_time;
int random_seed;
int num_people_started; /* statistics */
int num_people_finished; /* statistics */
pthread_mutex_t *lock;
}  gv;
int count=0;
int totalPeopleGenerated = 0;
int totalPeopleCompleted = 0;
gv obj1;
person *waiting_people; //people waiting for elevator
void* generatePeople(void *arg){
	waiting_people = malloc(1000*sizeof(person)); //Assuming Size of doubly linked list to be 1000
	int index = 0;
	while(1){ //keep on generating people
		sleep(obj1.beginning_time);
		currentTime+=obj1.beginning_time;
		person p;
		p.id = index;
		p.arrival_time = index;
		p.from_floor = rand() % obj1.num_floors + 1;
		p.to_floor = rand() % obj1.num_floors + 1;
		p.pickedUp = 0;
		p.droppedOff = 0;
		while(p.from_floor==p.to_floor)
		p.to_floor = rand() % obj1.num_floors + 1;
		waiting_people[index] = p; //person waiting for elevator
		printf("[%d]Person id = %d arrives on floor = %d, waiting to go to floor = %d\n",currentTime,p.id,p.from_floor,p.to_floor);
		count++;
		index++;
		totalPeopleGenerated++;
	}
	
} 
int pickUpIndex = 0;
void* moveElevator(void *arg){
		elevator e;
		e.current_floor=0; //Assuming each elevator starts from ground floor
		e.id=(int)arg;
		e.people = malloc(1000*sizeof(person)); //Assuming Size of doubly linked list to be 1000
		e.index = 0;
	while(1){
	while(count==0){
	sleep(1);
	}
	person p = waiting_people[pickUpIndex++]; //increment pickup index so that other thread works on another person
	p.pickedUp=1;
	count--;
		if(e.current_floor != p.from_floor)
		printf("[%d]Elevator %d moving from %d to %d\n",currentTime,e.id,e.current_floor,p.from_floor);
		if(e.current_floor<p.from_floor){ // elevator needs to go up
			while(e.current_floor < p.from_floor){
				sleep(obj1.elevator_speed);
				e.current_floor++;
				int i;
				printf("[%d]Elevator %d arrives at floor %d\n",currentTime,e.id,e.current_floor);
				for(i=0;i<totalPeopleGenerated;i++){
					if(waiting_people[i].pickedUp == 0 && waiting_people[i].from_floor==e.current_floor && waiting_people[i].to_floor <=p.from_floor)  { //last condition ensures it only picks someone who wanna go up
						e.people[e.index++] = waiting_people[i];
						waiting_people[i].pickedUp = 1;
						e.currentCount++; //increment number of people currently on elevator
						printf("[%d]Elevator %d picks up person %d\n",currentTime,e.id,waiting_people[i].id);
					}
				}
				
				for(i=0;i<e.currentCount;i++){
					if(e.people[i].droppedOff==0 && e.people[i].to_floor == e.current_floor) {
						totalPeopleCompleted++;
						e.people[i].droppedOff = 1;
						printf("[%d]Elevator %d drops person %d\n",currentTime,e.id,e.people[i].id);
					}
				}
			}
			
		} else{ //elevator needs to go down
		
			while(e.current_floor > p.from_floor){
				sleep(obj1.elevator_speed);
				e.current_floor--;
				int i;
				printf("[%d]Elevator %d arrives at floor %d\n",currentTime,e.id,e.current_floor);
				for(i=0;i<totalPeopleGenerated;i++){
					if(waiting_people[i].pickedUp == 0 && waiting_people[i].from_floor==e.current_floor && waiting_people[i].to_floor >=p.from_floor) { //last condition ensures it only picks someone who wanna go down
						e.people[e.index++] = waiting_people[i];
						waiting_people[i].pickedUp = 1;
						e.currentCount++; //increment number of people currently on elevator
						printf("[%d]Elevator %d picks up person %d\n",currentTime,e.id,waiting_people[i].id);
					}
				}
				
				for(i=0;i<e.currentCount;i++){
					if(e.people[i].droppedOff==0 && e.people[i].to_floor == e.current_floor) {
						totalPeopleCompleted++;
						e.people[i].droppedOff = 1;
						printf("[%d]Elevator %d drops person %d\n",currentTime,e.id,e.people[i].id);
					}
				}
			}
		}
		printf("[%d]Elevator %d picks up person %d\n",currentTime,e.id,p.id); //eventually elevator reaches person's floor
		
	}
	
}
int main(int argc, char *argv[]){
	obj1.num_elevators = atoi(argv[1]);
	obj1.num_floors =  atoi(argv[2]);
	obj1.beginning_time = atoi(argv[3]);
	obj1.elevator_speed = atoi(argv[4]);
	obj1.simulation_time = atoi(argv[5]);
	obj1.random_seed = atoi(argv[6]);
	
	
	pthread_t pgt,et[obj1.num_elevators];
	pthread_create(&pgt, NULL, generatePeople, NULL);
	int i;
	for(i=0;i<obj1.num_elevators;i++){
		pthread_create(&et[i],NULL,moveElevator,(void*) i);
	}

	sleep(obj1.simulation_time);
	pthread_cancel(pgt);
		for(i=0;i<obj1.num_elevators;i++){
		pthread_cancel(et[i]);
	}

	printf("Simulation Result: %d people started, %d people finished during %d seconds\n",totalPeopleGenerated,totalPeopleCompleted,obj1.simulation_time);
	pthread_exit(NULL);
	
}
