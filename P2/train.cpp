#include <iostream>
#include <string>
#include "train.h"

void *createTrain(void *args)
{
  TrainThreadArgs *trainArgs = (TrainThreadArgs *)args;
  Train *thisTrain = &trainArgs->train;

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

  pthread_mutex_lock(trainArgs->coutMutex);
  // pthread_cond_wait(trainArgs->coutCond, trainArgs->coutMutex);
  std::cout << "Train "
            << thisTrain->number
            << " intialized. dir: "
            << thisTrain->direction
            << " pri: "
            << thisTrain->priority
            << " load time: "
            << thisTrain->loadTime
            << " cross time: "
            << thisTrain->crossTime
            << std::endl;

  std::cout << "Train " << trainArgs->train.number << " waiting for signal" << std::endl;
  // pthread_cond_signal(trainArgs->coutCond);
  pthread_mutex_unlock(trainArgs->coutMutex);

  while (*trainArgs->startSignal == 0)
    ;

  pthread_mutex_lock(trainArgs->coutMutex);
  // pthread_cond_wait(trainArgs->coutCond, trainArgs->coutMutex);

  std::cout << "Train " << trainArgs->train.number << " released and loading" << std::endl;
  // pthread_cond_signal(trainArgs->coutCond);
  pthread_mutex_unlock(trainArgs->coutMutex);

  delTrainThreadArgs(trainArgs);
  pthread_exit(NULL);
}
