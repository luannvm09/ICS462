#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/sem.h>
#include "sem_ops.h"

/*number of cars and trucks heading in each direction*/
#define CARS_NORTH 3
#define CARS_SOUTH 3
#define VANS_NORTH 2
#define VANS_SOUTH 2

/*max cars allowed on bridge at any one time*/
#define MAXCARS 30
#define MAX_WEIGHT 1200
#define CAR_WEIGHT 200
#define VAN_WEIGHT 300

/*car or truck randomly arrives within first MAXWAIT seconds*/
#define MAXWAIT 6
/*and takes CROSSINGTIME to make is across the bridge*/
#define CROSSINGTIME 3

/*binary semaphores*/
int count_lock_north;	/*protects cars_count_east*/
int count_lock_south;	/*protects cars_count_west*/
int bridge_lock;		/*protects the bridge*/

int car_sem_north;		/*makes sure no more than MAXCARS cars on bridge heading east*/
int car_sem_south;		/*makes sure no more than MAXCARS cars on bridge heading west*/

int cars_count_north;	/*num cars currently on bridge heading east*/
int cars_count_south;	/*num cars currently on bridge heading west*/

/*cleanup deletes semaphores at end*/
void cleanup(void){
	rm_sem(count_lock_north);
	rm_sem(count_lock_south);
	rm_sem(bridge_lock);
	rm_sem(car_sem_north);
	rm_sem(car_sem_south);
	exit(1);
}

/*cars heading east*/
void *car_north(void * arg){
	int *me = (int *) arg;	/*id of particular car*/

	sleep( rand() % MAXWAIT );	/*wait for a while before showing up at the bridge*/
	
	P(car_sem_north);	/*proceed when there's less then 3 cars
						heading east on the bridge*/
	P(count_lock_north);	/*lock counter access*/
	cars_count_north++;	/*increment num cars heading east*/
	if(cars_count_north == 1) 	/*I'm the first car heading east on the bridge*/
		P(bridge_lock);			/*proceed when the bridge is free*/
	V(count_lock_north);		/*unlock counter access*/
	/*announce it*/
	printf("Car %d going north on the bridge\n", *me );
	/*proceed to cross the bridge*/
	sleep( CROSSINGTIME );
	/*announce it*/
	printf("Car %d going north off the bridge\n", *me );

	
	P(count_lock_north);	/*lock counter access*/
	cars_count_north--;	/*decrement counter*/
	if(cars_count_north == 0)	/*if i'm the last car on the bridge*/
		V(bridge_lock);		/*unlock the bridge for other traffic*/
	V(count_lock_north);	/*unlock counter access*/
	V(car_sem_north);	/*exit the bridge*/
	pthread_exit( NULL );	/*exit the thread*/
}
/*cars heading west*/
void *car_south(void * arg){
	int *me = (int *) arg;	/*id of particular car*/

	sleep( rand() % MAXWAIT );	/*wait for a while before showing up at the bridge*/
	
	P(car_sem_south);	/*proceed when there's less then 3 cars
						heading west on the bridge*/
	P(count_lock_south);	/*lock counter access*/
	cars_count_south++;	/*increment num cars heading west*/
	if(cars_count_south == 1) /*I'm the first car heading west on the bridge*/
		P(bridge_lock);	/*proceed when the bridge is free and lock bridge*/
	V(count_lock_south);	/*unlock counter access*/

	/*announce it*/
	printf( "Car %d going south on the bridge\n", *me );
	/*proceed to cross the bridge*/
	sleep( CROSSINGTIME );
	/*announce it*/
	printf( "Car %d going south off the bridge\n", *me );

	P(count_lock_south);	/*lock counter access*/
	cars_count_south--;	/*decrement counter*/
	if(cars_count_south == 0)	/*if i'm the last car on the bridge*/
		V(bridge_lock);	/*unlock the bridge for other traffic*/
	V(count_lock_south); /*unlock counter access*/
	V(car_sem_south); 	/*exit the bridge*/
	pthread_exit( NULL );	/*exit the thread*/
}
/*trucks heading east*/
void *van_north(void * arg){
	int *me = (int *) arg; /*id of particular truck*/

	sleep( rand() % MAXWAIT );	/*wait for a while before showing up at the bridge*/

	P(bridge_lock); /*proceed when bridge is free, lock the bridge*/
	/*announce it*/
	printf( "Van %d going north on the bridge\n", *me );
	/*proceed to cross the bridge*/
	sleep( CROSSINGTIME );
	/*announce it*/
	printf( "Van %d going north off the bridge\n", *me );
	V(bridge_lock); /*unlock the bridge*/
	pthread_exit( NULL );	/*exit the thread*/
}
/*trucks heading west*/
void *van_south(void * arg){
	int *me = (int *) arg; /*id of particular truck*/

	sleep( rand() % MAXWAIT ); /*wait for a while before showing up at the bridge*/

	P(bridge_lock); /*proceed when bridge is free, lock the bridge*/
	/*announce it*/
	printf( "Van %d going south on the bridge\n", *me );
	/*proceed to cross the bridge*/
	sleep( CROSSINGTIME );
	/*announce it*/
	printf( "Van %d going south off the bridge\n", *me );
	V(bridge_lock); /*unlock the bridge*/
	pthread_exit( NULL ); /*exit the thread*/
}

int main(void){
	int i;	/*index*/
	pthread_t car_tid_north[MAXCARS];	/*thread ids of cars heading east*/
	pthread_t car_tid_south[MAXCARS];	/*thread ids of cars heading west*/
	pthread_t van_tid_north[MAXCARS]; /*thread ids of trucks heading east*/
	pthread_t van_tid_south[MAXCARS]; /*thread ids of trucks heading west*/
	int car_id_north[MAXCARS]; /*car ids of cars heading east*/
	int car_id_south[MAXCARS]; /*car ids of cars heading west*/
	int van_id_north[MAXCARS]; /*truck ids of trucks heading east*/
	int van_id_south[MAXCARS]; /*truck ids of trucks heading west*/

	/*initialise no. cars to 0*/
	cars_count_north = 0;
	cars_count_south = 0;

	/*create semaphores*/
	bridge_lock = semtran(IPC_PRIVATE);	
	count_lock_north = semtran(IPC_PRIVATE);
	count_lock_south = semtran(IPC_PRIVATE);
	car_sem_north = semtran(IPC_PRIVATE);
	car_sem_south = semtran(IPC_PRIVATE);

	/*binary semaphores, initalise to 1*/
	V(count_lock_north);
	V(count_lock_south);
	V(bridge_lock);

	/*initialise car semaphores to size MAXCARS*/
	for(i = 0; i < MAXCARS; i++)
		V(car_sem_north);
	for(i = 0; i < MAXCARS; i++)
		V(car_sem_south);

	/*set ids of vehicles*/
	for(i = 0; i < CARS_SOUTH; i++)
		car_id_south[i] = i;
	for(i = 0; i < CARS_NORTH; i++)
		car_id_north[i] = i;
	for(i = 0; i < VANS_NORTH; i++)
		van_id_north[i] = i;
	for(i = 0; i < VANS_SOUTH; i++)
		van_id_south[i] = i;

	/* create threads -- remembering to check for errors!*/
	for(i = 0; i < CARS_NORTH; i++)
		if(pthread_create( &car_tid_north[i], NULL, car_north, (void *) &car_id_north[i] )){
			perror("Could not create thread");
			exit(EXIT_FAILURE);
		}
	for(i = 0; i < CARS_SOUTH; i++)
		if(pthread_create( &car_tid_south[i], NULL, car_south, (void *) &car_id_south[i] )){
			perror("Could not create thread");
			exit(EXIT_FAILURE);
		}
	for(i = 0; i < VANS_NORTH; i++)
		if(pthread_create( &van_tid_north[i], NULL, van_north, (void *) &van_id_north[i] )){
			perror("Could not create thread");
			exit(EXIT_FAILURE);
		}
	for(i = 0; i < VANS_SOUTH; i++)
		if(pthread_create( &van_tid_south[i], NULL, van_south, (void *) &van_id_south[i] )){
			perror("Could not create thread");
			exit(EXIT_FAILURE);
		}

	/*join threads*/
	for(i = 0; i < CARS_NORTH; i++)
		if(pthread_join( car_tid_north[i], NULL )){
			perror("Thread join failed");
			exit(EXIT_FAILURE);		
		}
	for(i = 0; i < CARS_SOUTH; i++)
		if(pthread_join( car_tid_south[i], NULL )){
			perror("Thread join failed");
			exit(EXIT_FAILURE);		
		}
	for(i = 0; i < VANS_NORTH; i++)
		if(pthread_join( van_tid_north[i], NULL )){
			perror("Thread join failed");
			exit(EXIT_FAILURE);		
		}
	for(i = 0; i < VANS_SOUTH; i++)
		if(pthread_join( van_tid_south[i], NULL )){
			perror("Thread join failed");
			exit(EXIT_FAILURE);		
		}

	/*delete semaphores*/
	cleanup();
	return 0;
	
}
