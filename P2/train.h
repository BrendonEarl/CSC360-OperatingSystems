#ifndef TRAIN_H
#define TRAIN_H

#include <sstream>
#include <pthread.h>
#include <stdlib.h>
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

typedef struct TrainThread
{
    Train *train;
    pthread_t *thread;
} TrainThread;

struct LessThanByTrainThread : public std::binary_function<TrainThread *, TrainThread *, bool>
{
    bool operator()(const TrainThread *lhs, const TrainThread *rhs) const
    {
        if (lhs->train->priority == rhs->train->priority)
        {
            return lhs->train->timeLoaded < rhs->train->timeLoaded;
        }
        return lhs->train->priority < rhs->train->priority;
    }
};

TrainThread *newTrainThread(void)
{
    TrainThread *nextTrainThread = (TrainThread *)malloc(sizeof(TrainThread));
    nextTrainThread->train = newTrain();
    return nextTrainThread;
}

void delTrainThread(TrainThread *trainThread)
{
    if (trainThread != NULL)
    {
        free(trainThread->train);
        free(trainThread);
    }
}

typedef struct Station
{
    std::priority_queue<TrainThread *, std::vector<TrainThread *>, LessThanByTrainThread> trainQueue;
    pthread_mutex_t trainQueueMutex;
    TrainThread *stationInput;
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
    Direction lastDirection;
    std::priority_queue<TrainThread *, std::vector<TrainThread *>, LessThanByTrainThread> *westStationQueue;
    pthread_mutex_t *westStationQueueMutex;
    std::priority_queue<TrainThread *, std::vector<TrainThread *>, LessThanByTrainThread> *eastStationQueue;
    pthread_mutex_t *eastStationQueueMutex;
    bool waitingTrainSignal;
} Dispatcher;

typedef struct TrainThreadArgs
{
    std::string travelInput;
    int numberInput;
    int loadTimeInput;
    int crossTimeInput;
    TrainThread *trainThread;
    bool *startSignal;
    Stations *stations;
    pthread_mutex_t *coutMutex;
    long int *startTime;
} TrainThreadArgs;

TrainThreadArgs *newTrainThreadArgs()
{
    TrainThreadArgs *nextTrainthreadArgs = (TrainThreadArgs *)malloc(sizeof(TrainThreadArgs));
    nextTrainthreadArgs->trainThread = newTrainThread();
    return nextTrainthreadArgs;
}

void delTrainThreadArgs(TrainThreadArgs *trainThreadArgs)
{
    if (trainThreadArgs != NULL)
    {
        free(trainThreadArgs->trainThread);
        free(trainThreadArgs);
    }
}

typedef struct StationThreadArgs
{
    Station *stationInfo;
    pthread_mutex_t *coutMutex;
    bool *waitingTrainSignal;
    long int *startTime;
} StationThreadArgs;

typedef struct DispatcherThreadArgs
{
    pthread_mutex_t *coutMutex;
    Dispatcher *dispatcherInfo;
    long int *startTime;
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
