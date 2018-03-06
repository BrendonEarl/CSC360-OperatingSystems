#include <iostream>
#include <string>
#include "trainDetails.h"

struct Train
{
public:
  Train();
  Train(std::string dir, int lt, int ct);
  Priority getPriority();
  Direction getDirection();
  float getLoadTime();
  float getCrossTime();
  pthread_t thread;

private:
  bool initialized;
  Priority pri;
  Direction dir;
  float loadTime;
  float crossTime;
};

Train::Train()
{
  initialized = false;
  thread = NULL;
}

Train::Train(std::string dr, int lt, int ct)
{
  dir = WEST;

  if (dr.compare("E"))
  {
    pri = HIGH;
    dir = EAST;
  }
  else if (dr.compare("e"))
  {
    pri = LOW;
    dir = EAST;
  }
  else if (dr.compare("W"))
  {
    pri = HIGH;
    dir = WEST;
  }
  else if (dr.compare("w"))
  {
    pri = LOW;
    dir = WEST;
  }
  loadTime = lt;
  crossTime = ct;

  initialized = true;

  //TODO: figure out why dir is empty and everything is cast to ints
  std::cout << "Train intialized: dir: " << Direction(dir) << " pri: " << Priority(pri) << " load time: " << loadTime << " cross time: " << crossTime << std::endl;
}

Priority Train::getPriority()
{
  if (initialized == false)
    throw std::runtime_error("Train not yet initialized");
  return pri;
}

Direction Train::getDirection()
{
  if (initialized == false)
    throw std::runtime_error("Train not yet initialized");
  return dir;
}

float Train::getLoadTime()
{
  if (initialized == false)
    throw std::runtime_error("Train not yet initialized");
  return loadTime;
}

float Train::getCrossTime()
{
  if (initialized == false)
    throw std::runtime_error("Train not yet initialized");
  return crossTime;
}

void load()
{
}