#include <iostream>
#include <string>
#include <unistd.h>
#include "train.h"

void *createDispatcher(void *args)
{
    DispatcherThreadArgs *dispatcherThreadArgs = (DispatcherThreadArgs *)args;
    Dispatcher *thisDispatcher = dispatcherThreadArgs->dispatcherInfo;

    while (true)
    {
        while (thisDispatcher->waitingTrainSignal == false)
            ;

        pthread_mutex_lock(thisDispatcher->westStationQueueMutex);
        pthread_mutex_lock(thisDispatcher->eastStationQueueMutex);
        Train westTrain;
        Train eastTrain;
        if (!thisDispatcher->westStationQueue->empty())
        {
            westTrain = *thisDispatcher->westStationQueue->top()->train;
        }
        if (!thisDispatcher->eastStationQueue->empty())
        {
            eastTrain = *thisDispatcher->eastStationQueue->top()->train;
        }

        Train selectedTrain;
        if (!thisDispatcher->westStationQueue->empty() && !thisDispatcher->eastStationQueue->empty())
        {
            if (westTrain.priority == eastTrain.priority)
            {
                if (thisDispatcher->lastDirection == WEST)
                {
                    selectedTrain = eastTrain;
                    thisDispatcher->eastStationQueue->pop();
                    thisDispatcher->lastDirection = EAST;
                }
                else if (thisDispatcher->lastDirection == EAST)
                {
                    selectedTrain = westTrain;
                    thisDispatcher->westStationQueue->pop();
                    thisDispatcher->lastDirection = WEST;
                }
            }
            else if (westTrain.priority == HIGH)
            {
                selectedTrain = eastTrain;
                thisDispatcher->eastStationQueue->pop();
                thisDispatcher->lastDirection = EAST;
            }
            else if (eastTrain.priority == HIGH)
            {
                selectedTrain = eastTrain;
                thisDispatcher->eastStationQueue->pop();
                thisDispatcher->lastDirection = EAST;
            }
        }
        else if (!thisDispatcher->westStationQueue->empty())
        {
            if (westTrain.priority == HIGH)
            {
                selectedTrain = westTrain;
                thisDispatcher->westStationQueue->pop();
                thisDispatcher->lastDirection = WEST;
            }
            else if (westTrain.priority == LOW)
            {
                selectedTrain = westTrain;
                thisDispatcher->westStationQueue->pop();
                thisDispatcher->lastDirection = WEST;
            }
        }
        else if (!thisDispatcher->eastStationQueue->empty())
        {
            if (eastTrain.priority == HIGH)
            {
                selectedTrain = eastTrain;
                thisDispatcher->eastStationQueue->pop();
                thisDispatcher->lastDirection = EAST;
            }
            else if (eastTrain.priority == LOW)
            {
                selectedTrain = eastTrain;
                thisDispatcher->eastStationQueue->pop();
                thisDispatcher->lastDirection = EAST;
            }
        }
        long int startCrossingTime = timestamp() - *(dispatcherThreadArgs->startTime);

        if (thisDispatcher->westStationQueue->empty() && thisDispatcher->eastStationQueue->empty())
        {
            thisDispatcher->waitingTrainSignal = false;
        }
        pthread_mutex_unlock(thisDispatcher->westStationQueueMutex);
        pthread_mutex_unlock(thisDispatcher->eastStationQueueMutex);

        pthread_mutex_lock(dispatcherThreadArgs->coutMutex);
        std::stringstream output;
        output << "Train "
               << std::setw(2) << std::left
               << selectedTrain.number
               << " is ON the main track going "
               << getDirection(selectedTrain.direction);
        announce(startCrossingTime, output.str());
        pthread_mutex_unlock(dispatcherThreadArgs->coutMutex);

        struct timespec tim, tim2;
        tim.tv_sec = (long)selectedTrain.crossTime;
        tim.tv_nsec = (long)(selectedTrain.crossTime * 1000000000L) % 1000000000L;
        nanosleep(&tim, &tim2);

        pthread_mutex_lock(dispatcherThreadArgs->coutMutex);
        long int endCrossingTime = timestamp() - *(dispatcherThreadArgs->startTime);
        output.str(std::string());
        output << "Train "
               << std::setw(2) << std::left
               << selectedTrain.number
               << " is OFF the main track after going "
               << getDirection(selectedTrain.direction);
        announce(endCrossingTime, output.str());
        pthread_mutex_unlock(dispatcherThreadArgs->coutMutex);
    }
}
