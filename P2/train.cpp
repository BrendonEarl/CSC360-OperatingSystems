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

  //TODO: figure out why dir is empty and everything is cast to ints
  std::cout << "number: "
            << thisTrain->number
            << " Train intialized: dir: "
            << thisTrain->direction
            << " pri: "
            << thisTrain->priority
            << " load time: "
            << thisTrain->loadTime
            << " cross time: "
            << thisTrain->crossTime
            << std::endl;

  std::cout << "train " << trainArgs->train.number << " waiting for signal" << std::endl;
  while (*trainArgs->startSignal == false)
    ;
  std::cout << "train " << trainArgs->train.number << " released and loading" << std::endl;
}
