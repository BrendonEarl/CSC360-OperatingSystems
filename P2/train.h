#ifndef TRAIN_H
#define TRAIN_H

#include <sstream>
#include <pthread.h>

typedef enum Direction {
    EAST,
    WEST
} Direction;

typedef enum Priority {
    LOW,
    HIGH
} Priority;

typedef struct Train
{
    Priority priority;
    Direction direction;
    int number;
    float loadTime;
    float crossTime;
} Train;

Train *newTrain(void)
{
    Train *nextTrain = (Train *)malloc(sizeof(Train));
    return nextTrain;
}

void delTrain(Train *train)
{
    if (train != NULL)
    {
        free(train);
    }
    return;
}

typedef struct Station
{
    Train *trainQueue;
    pthread_mutex_t trainQueueMutex;
    Train *stationInput;
    pthread_cond_t inputSignal;
} Station;

typedef struct Dispatcher
{
    bool bridgeBusy;
    Train *westStationQueue;
    pthread_mutex_t westStationQueueMutex;
    Train *eastStationQueue;
    pthread_mutex_t eastStationQueueMutex;
    pthread_cond_t waitingTrainSignal;
} Dispatcher;

typedef struct TrainThreadArgs
{
    std::string travelInput;
    int numberInput;
    int loadTimeInput;
    int crossTimeInput;
    Train train;
    bool *startSignal;
    Train *stationInput;
    pthread_cond_t *inputSignal;
    pthread_mutex_t *coutMutex;
    pthread_cond_t *coutCond;
} TrainThreadArgs;

TrainThreadArgs *newTrainThreadArgs()
{
    TrainThreadArgs *nextTrainthreadArgs = (TrainThreadArgs *)malloc(sizeof(TrainThreadArgs));
    return nextTrainthreadArgs;
}

void delTrainThreadArgs(TrainThreadArgs *trainThreadArgs)
{
    if (trainThreadArgs != NULL)
    {
        free(trainThreadArgs);
    }
}

typedef struct StationThreadArgs
{
    Station stationInfo;
    pthread_cond_t waitingTrainSignal;
} StationThreadArgs;

typedef struct DispatcherThreadArgs
{
    Dispatcher dispatcherInfo;
} DispatcherThreadArgs;

#endif
