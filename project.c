#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NORTH 0;
#define SOUTH 1;
#define car 3;
#define van 4;

sem_t bridge_lock;
sem_t mutex;
int weight;
int car_southbound;
int car_northbound;

void vehicle-Routine(int vehicle-id, int vehicle-type, int direction)
{
	
}

void arrive()
{

}

void cross()
{

}

void leave()
{

]

/*struct vehicle{
	int vehicle_id;
	int vehicle_type;
	int direction;
	int state;
};

struct bridge{
	int weight;
	struct vehicle northVehicles[30];
	struct vehicle southVehicles[30];
	bool twoDirections;
};*/

int main(){
		

	return 0;
}
