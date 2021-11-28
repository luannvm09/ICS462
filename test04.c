#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define CAR_WEIGHT 200
#define VAN_WEIGHT 300
#define MAX_WEIGHT 1200
#define FALSE 0
#define TRUE 1
#define CAR 1
#define VAN 2
#define NORTH 1		   // 1 is northbound
#define SOUTH 2		   // 2 is southbound
#define CROSSINGTIME 3 // each vehicle takes 3 seconds to cross the bridge.

int vehicleNumber[100];
int totalWeight;
int currentWeight;
int northBoundVehicleNumber[100];
int southBoundVehicleNumber[100];

int northBoundVehicleNum;
int southBoundVehicleNum;
float northBoundRatio[100], southBoundRatio[100];
int delayTime[100];
int ending = 1;

pthread_mutex_t monitorLock;						  // monitor lock
pthread_cond_t NorthSouth = PTHREAD_COND_INITIALIZER; // blockinng north/south cars

struct VEHICLE
{
	char type[10];
	int id;
	int vehicleWeight;
};

typedef struct VEHICLE Vehicle;
Vehicle northBound[100][100];
Vehicle southBound[100][100];

static int isSafe(Vehicle vehicle)
{
	if ((currentWeight + vehicle.vehicleWeight) <= MAX_WEIGHT)
	{
		printf("Ok for vehicle proceed \n");
		return TRUE;
	}
	else
	{
		printf("overweight. Vehicle cannot proceed \n");
		return FALSE;
	}
}

void northSouthVehicleNumber(int vehicleNumber, float northBoundRatio, float southBoundRatio)
{

	northBoundVehicleNum = vehicleNumber * northBoundRatio;
	southBoundVehicleNum = vehicleNumber * southBoundRatio;
}

// Direction is 1 for northbound and 2 for southbound
void arrive(Vehicle vehicle, int direction, int index)
{
	pthread_mutex_lock(&monitorLock);
	while (isSafe(vehicle) == 0)
	{
		pthread_cond_wait(isSafe(vehicle), &monitorLock); // ??????????????????????
	}
	if (direction == 1)
	{
		for (int i = 0; i < northBoundVehicleNum; i++)
		{
			northBound[index][i] = northBound[index][i + 1];
		}
	}
	else
	{
		for (int i = 0; i < southBoundVehicleNum; i++)
		{
			southBound[index][i] = southBound[index][i + 1];
		}
	}
	printf(" %s id %d has arrived the bridge", vehicle.type, vehicle.id);
	crossing(vehicle, direction);
}

// Direction is 1 for northbound and 2 for southbound
void crossing(Vehicle vehicle, int direction)
{
	char dir[10];
	if (direction == 1)
	{
		strcpy(dir, "northbound");
	}
	else
	{
		strcpy(dir, "southbound");
	}
	printf(" %s id %d is crossing the bridge on %s", vehicle.type, vehicle.id, dir);
	sleep(CROSSINGTIME);
	leaving(vehicle);
}

void leaving(Vehicle vehicle)
{
	printf(" %s id %d has left the bridge", vehicle.type, vehicle.id);
	currentWeight = currentWeight = vehicle.vehicleWeight;
	pthread_cond_signal(isSafe(vehicle)); // ?????????????????????
	pthread_mutex_unlock(&monitorLock);
	pthread_exit(0);
}

void northBoundQueueDisplay(vehicle northBound[])
{
	for (int i = 0; i < northBoundVehicleNum; i++)
	{
		printf(" % s id: %d  \n", northBound[index][i].type, northBound[index][i].id);
	}
}

void southBoundQueueDisplay(vehicle southBound[])
{
	for (int i = 0; i < southBoundVehicleNum; i++)
	{
		printf("% s id: %d  \n", southBound[index][i].type, southBound[index][i].id);
	}
}

int main()
{
	pthread_mutex_init(&MonitorLock, USYNC_THREAD, (void *) NULL);
	int index, loopCount = 0;
	//Vehicle northBound[100][100];
	//Vehicle southBound[100][100];
	while (ending != 0)
	{
		currentWeight = totalWeight = 0;
		printf("enter the vehicle number: ");
		scanf("%d", &vehicleNumber[index]);
		printf("Enter the number of ratio north and south bound: ");
		scanf("%f / %f", &northBoundRatio[index], &southBoundRatio[index]);
		printf("Enter the delay time between two batched: ");
		scanf("%d", &delayTime[index]);
		if (delayTime[index] == 0)
		{
			ending = 0; // the condition to stop the loop for user input
		}

		int vehicleNum = vehicleNumber[index];
		float northRatio = northBoundRatio[index];
		float southRatio = southBoundRatio[index];

		northSouthVehicleNumber(vehicleNum, northRatio, southRatio);
		printf("Northbound Vehicle Number : %d \n", northBoundVehicleNum);
		printf("Southbound Vehicle Number : %d \n", southBoundVehicleNum);
		index++;
		loopCount++;
	}

	index = 0;
	while (index < loopCount)
	{
		
		int vehicleNum = vehicleNumber[index];
		//int i;
		Vehicle x[vehicleNum];
		for (int i = 0; i < vehicleNum; i++)
		{
			if (i < northBoundVehicleNum)
			{
				arrive(x[i], 1, index);
			}
			else
			{
				arrive(x[i], 2, index);
			}
		}
		index++;
	}

	/*
		int vehicleNum = vehicleNumber[index];
		float northRatio = northBoundRatio[index];
		float southRatio = southBoundRatio[index];

		northSouthVehicleNumber(vehicleNum, northRatio, southRatio);
		printf("Northbound Vehicle Number : %d \n", northBoundVehicleNum);
		printf("Southbound Vehicle Number : %d \n", southBoundVehicleNum);
		Vehicle x[vehicleNum];
		
		int southBoundIndex = 0;

		for (int num = 0; num < vehicleNumber[index]; num++){
			
			
			x[num].id = num;
			if (x[num].id % 2 == 0)
			    {
				x[num].vehicleWeight = CAR_WEIGHT;
				strcpy(x[num].type, "Car");
				printf("%s id is %d has %d weight \n", x[num].type, x[num].id, x[num].vehicleWeight);
			    }
		    	else
			{
				x[num].vehicleWeight = VAN_WEIGHT;
				strcpy(x[num].type, "Van");
				printf("%s id is %d has %d weight \n", x[num].type, x[num].id, x[num].vehicleWeight);
			}
			if ( num <northBoundVehicleNum)
				{
				northBound[index][num] = x[num];			
				} else {
					
					southBound[index][southBoundIndex] = x[num];
					southBoundIndex++;
					}
			totalWeight += x[num].vehicleWeight;
			printf("totalWeight : %d \n", totalWeight);	
			currentWeight = totalWeight;
			isSafe();*/


/*
		printf("NorthBound Waiting Array: \n"); 
		for( int i = 0; i < northBoundVehicleNum; i++)
			{
			
			printf("Id: %d and Weight: %d \n", northBound[index][i].id, northBound[index][i].vehicleWeight);
			}
	
		printf("------------------\n"); 
		printf("SouthBound Waiting Array: \n"); 
		for( int i = 0; i < southBoundVehicleNum; i++)
			{
			
			printf("Id: %d and Weight: %d \n", southBound[index][i].id, southBound[index][i].vehicleWeight);
			}
			*/


//
return 0;
}
