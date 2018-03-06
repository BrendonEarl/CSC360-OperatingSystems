#include <iostream>
#include <string>
#include "trainDetails.h"

class Entry
{
  public:
    Entry();
    Entry(std::string entryTxt);
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

Entry::Entry(std::string entryTxt)
{
    // code snippet inspired by SO answer: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c/14266139
    std::string token;
    size_t pos;

    // parse entry text
    for (int i = 0; i < 3; i++)
    {
        pos = entryTxt.find(" ");
        token = entryTxt.substr(0, pos);

        if (i == 0)
        {
            if (token.compare("E") != 0)
            {
                pri = HIGH;
                dir = EAST;
            }
            else if (token.compare("e") != 0)
            {
                pri = LOW;
                dir = EAST;
            }
            else if (token.compare("W") != 0)
            {
                pri = HIGH;
                dir = WEST;
            }
            else if (token.compare("w") != 0)
            {
                pri = LOW;
                dir = WEST;
            }
        }
        else if (i == 1)
        {
            loadTime = float(atoi(token.c_str()) / 10);
        }
        else if (i == 2)
        {
            crossTime = float(atoi(token.c_str()) / 10);
        }

        entryTxt.erase(0, pos + 1);
    }

    initialized = true;
    nextSet = false;
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
