#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_WEIGHT 1200
#define CAR_WEIGHT 200
#define VAN_WEIGHT 300
#define TRUE 1
#define FALSE 0

pthread_mutex_t Northlock, Southlock;
pthread_cond_t Cond;
int currentWeight = 0;

struct VEHICLE
{
    char type[10];
    int id;
    int vehicleWeight;
};

typedef struct VEHICLE Vehicle;

Vehicle northVehicleArr[100][100];
Vehicle southVehicleArr[100][100];

static int isSafe(Vehicle vehicle)
{
    if ((currentWeight + vehicle.vehicleWeight) <= MAX_WEIGHT)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void arrive(Vehicle *vehicle)
{
    while (isSafe(*vehicle) != 1)
    {
        pthread_cond_wait(&Cond, &Northlock);
    }

    printf(" %s %d has arrived to the bridge\n\n", vehicle->type, vehicle->id);
    currentWeight = currentWeight + vehicle->vehicleWeight;
}

void crossing(Vehicle *vehicle)
{
    printf("%s %d is crossing the bridge \n\n", vehicle->type, vehicle->id);
    sleep(3);
}

void leaving(Vehicle *vehicle)
{
    printf("%s %d has finished crossing the bridge \n\n", vehicle->type, vehicle->id);
    currentWeight = currentWeight - vehicle->vehicleWeight;
}

void *northBound(void *arg)
{

    pthread_mutex_lock(&Northlock);

    printf("NorthBound ");
    arrive((Vehicle *)arg);
    pthread_mutex_unlock(&Northlock);

    pthread_mutex_lock(&Northlock);
    printf("Northbound ");
    crossing((Vehicle *)arg);
    pthread_mutex_unlock(&Northlock);

    pthread_mutex_lock(&Northlock);
    printf("Northbound ");
    leaving((Vehicle *)arg);

    pthread_mutex_unlock(&Northlock);

    return NULL;
}

void *southBound(void *arg)
{
    pthread_mutex_lock(&Southlock);

    printf("SouthBound ");
    arrive((Vehicle *)arg);
    pthread_mutex_unlock(&Southlock);

    pthread_mutex_lock(&Southlock);
    printf("Southbound ");
    crossing((Vehicle *)arg);
    pthread_mutex_unlock(&Southlock);

    pthread_mutex_lock(&Southlock);
    printf("Southbound ");
    leaving((Vehicle *)arg);

    pthread_mutex_unlock(&Southlock);

    return NULL;
}

int main()
{

    int count = 0;
    int n[count], s[count];
    int tempN, tempS;
    int totalNum[100];
    float nRatio[100], sRatio[100];

    int delayTime[100];
    delayTime[0] = -1;

    while (delayTime[count] != -2)
    {
        printf("Enter the number of total: ");
        scanf("%d", &totalNum[count]);

        printf("Enter the northbound and southbound ratio: ");
        scanf("%f/%f", &nRatio[count], &sRatio[count]);

        n[count] = nRatio[count] * totalNum[count];
        s[count] = sRatio[count] * totalNum[count];

        printf("Enter the delay time: ");
        scanf("%d", &delayTime[count]);

        if (delayTime[count] == -2)
        {
            delayTime[count + 1] = -2;
            delayTime[count] = 0;
        }

        count++;
    }

    count = 0;

    int tempId = 0;
    while (delayTime[count] != -2)
    {
        printf("Northbound: %d, Southbound: %d \n", n[count], s[count]);

        tempN = n[count];
        tempS = s[count];
        

        for (int i = 0; i < totalNum[count]; )
        {
            
            for (int a = 0; a < tempN; a++)
            {
                 
                int temp = rand() % 2;
                northVehicleArr[count][a].id = tempId ;
                if (temp == 0)
                {
                    strcpy(northVehicleArr[count][a].type, "Car");
                    northVehicleArr[count][a].vehicleWeight = 200;
                }
                else
                {
                    strcpy(northVehicleArr[count][a].type, "Van");
                    northVehicleArr[count][a].vehicleWeight = 300;
                }
                tempId++;
                i++;
            }

            for (int b = 0; b < tempS; b++)
            {
                
                int temp = rand() % 2;
                southVehicleArr[count][b].id = tempId ;
                if (temp == 0)
                {
                    strcpy(southVehicleArr[count][b].type, "Car");
                    southVehicleArr[count][b].vehicleWeight = 200;
                }
                else
                {
                    strcpy(southVehicleArr[count][b].type, "Van");
                    southVehicleArr[count][b].vehicleWeight = 300;
                }
                tempId++;
                i++;
            }
        }

        pthread_t northVehicles[tempN], southVehicles[tempS];

        
        pthread_mutex_init(&Northlock, NULL);
        pthread_mutex_init(&Southlock, NULL);
        pthread_cond_init(&Cond, NULL);

        for (int i = 0; i < tempN; i++)
            pthread_create(&northVehicles[i], NULL, &northBound, &northVehicleArr[count][i]);

        for (int i = 0; i < tempS; i++)
            pthread_create(&southVehicles[i], NULL, &southBound, &southVehicleArr[count][i]);

        for (int i = 0; i < tempN; i++)
            pthread_join(northVehicles[i], NULL);
        for (int i = 0; i < tempS; i++)
            pthread_join(southVehicles[i], NULL);


        pthread_mutex_destroy(&Northlock);
        pthread_mutex_destroy(&Southlock);
        sleep(delayTime[count]);
        printf("----------- \n");

        count++;
    }

    pthread_exit(NULL);

    return 0;
}
