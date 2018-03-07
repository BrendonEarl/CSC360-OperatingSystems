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
    // Start signal
    bool startLoadingSignal = false;
    // Stream file in
    std::ifstream infile(argv[1]);
    // Stream line
    std::string trainEntry;
    // parse all
    int trainNum = 1;
    while (std::getline(infile, trainEntry))
    {
        TrainThreadArgs nextTrainThreadArgs;

        nextTrainThreadArgs.numberInput = trainNum;
        nextTrainThreadArgs.startSignal = &startLoadingSignal;

        std::istringstream iss(trainEntry);
        iss >> nextTrainThreadArgs.travelInput >> nextTrainThreadArgs.loadTimeInput >> nextTrainThreadArgs.crossTimeInput;
        std::cout << "Reading in: " << nextTrainThreadArgs.travelInput << " " << nextTrainThreadArgs.loadTimeInput << " " << nextTrainThreadArgs.crossTimeInput << std::endl;
        pthread_t some_thread;
        pthread_create(&some_thread, NULL, &createTrain, (void *)&nextTrainThreadArgs);
        std::cout
            << "number: "
            << nextTrainThreadArgs.train.number
            << " Train intialized: dir: "
            << nextTrainThreadArgs.train.direction
            << " pri: "
            << nextTrainThreadArgs.train.priority
            << " load time: "
            << nextTrainThreadArgs.train.loadTime
            << " cross time: "
            << nextTrainThreadArgs.train.crossTime
            << std::endl;
        trainNum += 1;
    }

    startLoadingSignal = true;

    return 0;
};
