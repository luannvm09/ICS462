#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define TRUE 1
#define FALSE 0
#define MAX_WEIGHT 1200
#define CAR_WEIGHT 200
#define VAN_WEIGHT 300
#define CROSSING_TIME 3     // crossing time is 3 seconds for one vehicle

pthread_mutex_t lock;
pthread_cond_t North;
pthread_cond_t South;

int currentWeight;
int vehicleNumber;
int waitingSouth[1];
int waitingNorth[1];

int checkWeight(vehicleNumber){
    currentWeight = 0;
    int result;
    // Since the ratio for the car and van are 50:50
    while(vehicleNumber != 0 && currentWeight < MAX_WEIGHT){
        if( currenWeight < MAX_WEIGHT){
            vehicleNumber--;
            currentWeight =+CAR_WEIGHT;
            result = 1;
    }
        if( currenWeight < MAX_WEIGHT){
            vehicleNumber--;
            currentWeight =+VAN_WEIGHT;
            result = 1;
        }
    }
    result = 0;
    return result;
    
}

void northSouthVehicleNumber(vehicleNumber, northBounce, southBounce){
    int northBounceVehileNumber;
    int southBounceVehileNumber;
    northBounceVehileNumber = vehicleNumber * northBounce;
    southBounceVehileNumber = vehicleNumber * southBounce;
    waitingNorth[1] = northBounceVehileNumber;
    waitingSouth[1] = southBounceVehileNumber;
}

int main(){
    //int vehicleNumber;
    int northBounce, southBounce;
    int delayTime = 0;

    while (delayTime != 0){
        printf("enter the vehicle number: ");
        scanf("%d", &vehicleNumber);
        prinft("Enter the number of ratio north and south bound: ");
        scanf("%f / %f", &northBounce, &southBounce);
        printf("Enter the delay time between two batched: ");
        scanf("%d", delayTime);
    }
    
}