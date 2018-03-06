#include <fstream>
#include "train.cpp"
#include "station.cpp"
#include "schedule.cpp"
#include "dispatcher.cpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Incorrect number of params" << std::endl;
    }
    std::cout << "test" << std::endl;
    // Steam file in
    std::ifstream infile(argv[1]);
    // train direction
    char dir;
    // load and crossing time
    int lt, ct;
    // parse all
    while (infile >> dir >> lt >> ct)
    {
        std::cout << dir << " " << lt << " " << ct << std::endl;
    }
    return 0;
};
