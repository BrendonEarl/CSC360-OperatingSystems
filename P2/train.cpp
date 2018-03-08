#include <iostream>
#include <string>
#include <sys/time.h>
#include <iomanip>
#include "train.h"

void *createTrain(void *args)
{
  TrainThreadArgs *trainArgs = (TrainThreadArgs *)args;
  Train *thisTrain = trainArgs->trainThread->train;

  if (trainArgs->travelInput.compare("E") == 0)
  {
    thisTrain->priority = HIGH;
    thisTrain->direction = EAST;
  }
  else if (trainArgs->travelInput.compare("e") == 0)
  {
    thisTrain->priority = LOW;
    thisTrain->direction = EAST;
  }
  else if (trainArgs->travelInput.compare("W") == 0)
  {
    thisTrain->priority = HIGH;
    thisTrain->direction = WEST;
  }
  else if (trainArgs->travelInput.compare("w") == 0)
  {
    thisTrain->priority = LOW;
    thisTrain->direction = WEST;
  }
  thisTrain->number = trainArgs->numberInput;
  thisTrain->loadTime = float(trainArgs->loadTimeInput) / 10;
  thisTrain->crossTime = float(trainArgs->crossTimeInput) / 10;

  while (*trainArgs->startSignal == 0)
    ;

  pthread_mutex_lock(trainArgs->coutMutex);
  std::cout << "Train " << thisTrain->number << " released and loading for " << thisTrain->loadTime << "sec" << std::endl;
  pthread_mutex_unlock(trainArgs->coutMutex);

  struct timespec tim, tim2;
  tim.tv_sec = (long)thisTrain->loadTime;
  tim.tv_nsec = (long)(thisTrain->loadTime * 1000000000L) % 1000000000L;
  nanosleep(&tim, &tim2);

  struct timeval tp;
  gettimeofday(&tp, NULL);
  thisTrain->timeLoaded = tp.tv_sec * 1000 + tp.tv_usec / 1000;

  if (thisTrain->direction == EAST)
  {
    pthread_mutex_lock(&trainArgs->stations->west->inputMutex);
    pthread_cond_wait(&trainArgs->stations->west->inputEmpty, &trainArgs->stations->west->inputMutex);
    trainArgs->stations->west->stationInput = trainArgs->trainThread;
    pthread_mutex_unlock(&trainArgs->stations->west->inputMutex);
    pthread_cond_signal(&trainArgs->stations->west->inputSignal);
  }
  else if (thisTrain->direction == WEST)
  {
    pthread_mutex_lock(&trainArgs->stations->east->inputMutex);
    pthread_cond_wait(&trainArgs->stations->east->inputEmpty, &trainArgs->stations->east->inputMutex);
    trainArgs->stations->east->stationInput = trainArgs->trainThread;
    pthread_mutex_unlock(&trainArgs->stations->east->inputMutex);
    pthread_cond_signal(&trainArgs->stations->east->inputSignal);
  }

  // delTrainThreadArgs(trainArgs);
  pthread_exit(NULL);
}
