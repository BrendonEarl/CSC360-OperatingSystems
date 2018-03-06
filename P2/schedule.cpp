#include <iostream>
#include <string>
#include "trainDetails.h"
#include "entry.cpp"

class Schedule
{
  public:
    Schedule();
    bool addEntry(char dir, int lt, int ct);
    Entry getNextEntry();

  private:
    Entry entryHead;
    Entry entryTail;
    Entry currEntry;
    bool entryExists;
};

Schedule::Schedule()
{
    entryExists = false;
    std::cout << "Initializing schedule" << std::endl;
}

bool Schedule::addEntry(char dir, int lt, int ct)
{
    try
    {
        if (entryExists == false)
        {
            entryHead = Entry(dir, lt, ct);
            currEntry = entryHead;
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}
