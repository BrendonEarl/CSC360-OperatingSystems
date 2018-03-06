#include <iostream>
#include <string>
#include "trainDetails.h"

class Entry
{
  public:
    Entry();
    Entry(char dir, int lt, int ct);
    bool setNext(Entry &nextEntry);
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
    bool nextSet;
    Entry *next;
};

Entry::Entry()
{
    initialized = false;
    nextSet = false;
}

Entry::Entry(char dr, int lt, int ct)
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
    loadTime = float(lt / 10.0);
    crossTime = float(ct / 10.0);

    initialized = true;
    nextSet = false;

    //TODO: figure out why dir is empty and everything is cast to ints
    std::cout << "entry intialized: dir: " << Direction(dir) << " pri: " << Priority(pri) << " load time: " << loadTime << " cross time: " << crossTime << std::endl;
}

bool Entry::setNext(Entry &nextEntry)
{
    if (nextSet == true)
    {
        return false;
    }
    next = &nextEntry;
    nextSet = true;
    return true;
}

Priority Entry::getPriority()
{
    if (initialized == false)
        throw std::runtime_error("Entry not yet initialized");
    return pri;
}

Direction Entry::getDirection()
{
    if (initialized == false)
        throw std::runtime_error("Entry not yet initialized");
    return dir;
}

float Entry::getLoadTime()
{
    if (initialized == false)
        throw std::runtime_error("Entry not yet initialized");
    return loadTime;
}

float Entry::getCrossTime()
{
    if (initialized == false)
        throw std::runtime_error("Entry not yet initialized");
    return crossTime;
}
