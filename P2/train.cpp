#include <iostream>
#include <string>
#include "train.h"

void createTrain(void *args)
{
  TrainThreadArgs *trainArgs = (TrainThreadArgs *)args;

  if (trainArgs->travelInput.compare("E"))
  {
    trainArgs->train.priority = HIGH;
    trainArgs->train.direction = EAST;
  }
  else if (trainArgs->travelInput.compare("e"))
  {
    trainArgs->train.priority = LOW;
    trainArgs->train.direction = EAST;
  }
  else if (trainArgs->travelInput.compare("W"))
  {
    trainArgs->train.priority = HIGH;
    trainArgs->train.direction = WEST;
  }
  else if (trainArgs->travelInput.compare("w"))
  {
    trainArgs->train.priority = LOW;
    trainArgs->train.direction = WEST;
  }
  trainArgs->train.loadTime = float(trainArgs->loadTimeInput) / 10;
  trainArgs->train.crossTime = float(trainArgs->crossTimeInput) / 10;

  //TODO: figure out why dir is empty and everything is cast to ints
  std::cout << "Train intialized: dir: "
            << trainArgs->train.direction
            << " pri: "
            << trainArgs->train.priority
            << " load time: "
            << trainArgs->train.loadTime
            << " cross time: "
            << trainArgs->train.crossTime
            << std::endl;
}
