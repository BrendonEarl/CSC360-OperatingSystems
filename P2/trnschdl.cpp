#include <fstream>
#include "train.cpp"
#include "station.cpp"
#include "dispatcher.cpp"

int main(int argc, char *argv[])
{
    // Ensure input has correct number of params
    if (argc != 2)
    {
        std::cout << "Incorrect number of params" << std::endl;
    }

    // Output magement
    pthread_mutex_t coutMutex;
    pthread_mutex_init(&coutMutex, NULL);
    pthread_cond_t coutCond;
    pthread_cond_init(&coutCond, NULL);

    // Start signal
    bool startLoadingSignal = false;
    // Create file stream and string to stream to
    std::ifstream infile(argv[1]);
    std::string trainEntry;
    // Parse in file
    int trainNum = 1;
    while (std::getline(infile, trainEntry))
    {
        TrainThreadArgs *nextTrainThreadArgs = newTrainThreadArgs();

        nextTrainThreadArgs->numberInput = trainNum;
        nextTrainThreadArgs->startSignal = &startLoadingSignal;
        nextTrainThreadArgs->coutMutex = &coutMutex;
        nextTrainThreadArgs->coutCond = &coutCond;

        std::istringstream iss(trainEntry);
        iss >> nextTrainThreadArgs->travelInput >> nextTrainThreadArgs->loadTimeInput >> nextTrainThreadArgs->crossTimeInput;

        pthread_mutex_lock(&coutMutex);
        std::cout << "Reading in: " << nextTrainThreadArgs->travelInput << " " << nextTrainThreadArgs->loadTimeInput << " " << nextTrainThreadArgs->crossTimeInput << std::endl;
        pthread_mutex_unlock(&coutMutex);

        pthread_t some_thread;
        pthread_create(&some_thread, NULL, &createTrain, (void *)nextTrainThreadArgs);

        trainNum += 1;
    }

    for (int i = 0; i < 10000000; i++)
        ;

    startLoadingSignal = true;

    while (true)
        ;

    return 0;
};
