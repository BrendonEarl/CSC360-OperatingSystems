#include <iostream>
#include <string>
#include "trainDetails.h"

class Train
{
public:
  Train();
  Train(char dir, int lt, int ct);
  Priority getPriority();
  Direction getDirection();
  float getLoadTime();
  float getCrossTime();

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
}

Train::Train(char dr, int lt, int ct)
{
  // code snippet inspired by SO answer: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c/14266139
  std::string token;
  size_t pos;

  dir = WEST;

  if (dr == *"E")
  {
    pri = HIGH;
    dir = EAST;
  }
  else if (dr == *"e")
  {
    pri = LOW;
    dir = EAST;
  }
  else if (dr == *"W")
  {
    pri = HIGH;
    dir = WEST;
  }
  else if (dr == *"w")
  {
    pri = LOW;
    dir = WEST;
  }
  loadTime = float(lt) / 10.0;
  crossTime = float(ct) / 10.0;

  initialized = true;

  //TODO: figure out why dir is empty and everything is cast to ints
  std::cout << "entry intialized: dir: " << Direction(dir) << " pri: " << Priority(pri) << " load time: " << loadTime << " cross time: " << crossTime << std::endl;
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
