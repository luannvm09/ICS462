#include <stdio.h>

#define CAR_WEIGHT 200
#define VAN_WEIGHT 300
#define MAX_WEIGHT 1200
#define FALSE 0
#define TRUE 1

struct vehicle;
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

typedef struct
{
    int id;
    int vehicleWeight;
   

} vehicle; 

static int isSafe()
{
    if (currentWeight <= MAX_WEIGHT)
    {
	printf("Still ok \n");        
	return TRUE;
    }
    else 
	{
	printf("overweight \n");
        return FALSE;
	}
}

void northSouthVehicleNumber(int vehicleNumber, float northBoundRatio, float southBoundRatio)
{

    northBoundVehicleNum = vehicleNumber * northBoundRatio;
    southBoundVehicleNum = vehicleNumber * southBoundRatio;
    
}

int main(){
	int index, loopCount = 0;
	vehicle northBound[100][100];
	vehicle southBound[100][100];
	while( ending!=0)
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
	while(index < loopCount)
	{	
		int vehicleNum = vehicleNumber[index];
		float northRatio = northBoundRatio[index];
		float southRatio = southBoundRatio[index];

		northSouthVehicleNumber(vehicleNum, northRatio, southRatio);
		printf("Northbound Vehicle Number : %d \n", northBoundVehicleNum);
		printf("Southbound Vehicle Number : %d \n", southBoundVehicleNum);
		vehicle x[vehicleNum];
		
		int southBoundIndex = 0;

		for (int num = 0; num < vehicleNumber[index]; num++){
			
			
			x[num].id = num;
			if (x[num].id % 2 == 0)
			    {
				x[num].vehicleWeight = CAR_WEIGHT;
				printf("Car id is %d has %d weight \n", x[num].id, x[num].vehicleWeight);
			    }
		    	else
			{
				x[num].vehicleWeight = VAN_WEIGHT;
				printf("Van id is %d has %d weight \n", x[num].id, x[num].vehicleWeight);
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
			isSafe();
			}
		
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

		index++;
	}	
return 0;

}

