#include <iostream>
#include <string>
#include <unistd.h>
#include "train.h"

void *createStation(void *args)
{
    StationThreadArgs *stationArgs = (StationThreadArgs *)args;
    Station *thisStation = stationArgs->stationInfo;

    while (true)
    {
        // pthread_cond_broadcast(&thisStation->inputEmpty);
        pthread_mutex_lock(&thisStation->trainQueueMutex);
        pthread_cond_wait(&thisStation->inputSignal, &thisStation->trainQueueMutex);
        thisStation->trainQueue.push(thisStation->stationInput);
        thisStation->stationInput = NULL;
        pthread_mutex_unlock(&thisStation->trainQueueMutex);

        pthread_mutex_lock(stationArgs->coutMutex);
        std::stringstream output;
        output << "Train "
               << std::setw(2) << std::left
               << thisStation->trainQueue.top()->train->number
               << " queued and ready to go "
               << getDirection(thisStation->trainQueue.top()->train->direction);
        announce(timestamp() - *(stationArgs->startTime), output.str());
        pthread_mutex_unlock(stationArgs->coutMutex);
    }
}
