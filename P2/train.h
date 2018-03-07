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
    float loadTime;
    float crossTime;
} Train;

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
    int trainNumber;
    std::string travelInput;
    int loadTimeInput;
    int crossTimeInput;
    Train train;
    Train *stationInput;
    pthread_cond_t inputSignal;
} TrainThreadArgs;

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
