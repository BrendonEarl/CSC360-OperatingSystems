#include <fstream>
#include "train.cpp"
#include "station.cpp"
#include "dispatcher.cpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Incorrect number of params" << std::endl;
    }
    // Stream file in
    std::ifstream infile(argv[1]);
    // Stream line
    std::string trainEntry;
    // parse all
    while (std::getline(infile, trainEntry))
    {
        TrainThreadArgs trainThreadArgs;

        trainThreadArgs.trainNumber = 1;

        std::istringstream iss(trainEntry);
        iss >> trainThreadArgs.travelInput >> trainThreadArgs.loadTimeInput >> trainThreadArgs.crossTimeInput;
        std::cout << "Reading in: " << trainThreadArgs.travelInput << " " << trainThreadArgs.loadTimeInput << " " << trainThreadArgs.crossTimeInput << std::endl;
        createTrain(&trainThreadArgs);
    }
    return 0;
};
