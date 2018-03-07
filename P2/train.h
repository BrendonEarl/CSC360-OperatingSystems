#ifndef TRAIN_H
#define TRAIN_H

#include <sstream>
#include <pthread.h>
#include <queue>

#include <unistd.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sys/time.h>

typedef enum Direction {
    EAST,
    WEST
} Direction;

std::string getDirection(Direction dir)
{
    if (dir == EAST)
    {
        return "East";
    }
    else if (dir == WEST)
    {
        return "West";
    }
    return NULL;
}

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
    long int timeLoaded;
    bool operator<(const Train &rhs) const
    {
        if (priority == rhs.priority)
        {
            return timeLoaded < rhs.timeLoaded;
        }
        return priority < rhs.priority;
    }
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
    std::priority_queue<Train> trainQueue;
    pthread_mutex_t trainQueueMutex;
    Train *stationInput;
    pthread_mutex_t inputMutex;
    pthread_cond_t inputSignal;
    pthread_cond_t inputEmpty;

} Station;

typedef struct Stations
{
    Station *east;
    Station *west;
} Stations;

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
    Stations *stations;
    pthread_mutex_t *coutMutex;
    long int *startTime;
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
    Station *stationInfo;
    pthread_mutex_t *coutMutex;
    pthread_cond_t *waitingTrainSignal;
} StationThreadArgs;

typedef struct DispatcherThreadArgs
{
    Dispatcher dispatcherInfo;
} DispatcherThreadArgs;

long int timestamp()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int timestamp = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return timestamp;
}

void announce(long int time, std::string out)
{
    std::cout
        << std::setfill('0') << std::setw(2)
        << ((time / 1000) / 3600)
        << ":"
        << std::setfill('0') << std::setw(2)
        << ((time / 1000) / 60) % 60
        << ":"
        << std::setfill('0') << std::setw(2)
        << (time / 1000) % 60
        << ":"
        << std::setw(1)
        << (time % 1000) / 100
        << " "
        << out
        << std::endl;
}

#endif
