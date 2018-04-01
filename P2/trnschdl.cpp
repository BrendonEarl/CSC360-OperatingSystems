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

    // Various
    long int startTime;
    bool quit = false;
    int trainNum;
    bool waitingTrain = false;
    pthread_mutex_t settingTrainSignal;
    pthread_mutex_init(&settingTrainSignal, NULL);

    // Output magement
    pthread_mutex_t coutMutex;
    pthread_mutex_init(&coutMutex, NULL);

    // ----- Setup Dispatcher ------
    Dispatcher dispatcher;
    dispatcher.lastDirection = WEST;
    dispatcher.waitingTrainSignal = &waitingTrain;
    dispatcher.settingTrainSignal = &settingTrainSignal;
    DispatcherThreadArgs dispatcherThreadArgs;
    dispatcherThreadArgs.coutMutex = &coutMutex;
    dispatcherThreadArgs.startTime = &startTime;
    dispatcherThreadArgs.dispatcherInfo = &dispatcher;
    dispatcherThreadArgs.trainNum = &trainNum;
    dispatcherThreadArgs.quit = &quit;

    // ----- Setup Stations ------
    pthread_t westStationThread, eastStationThread;
    Station westStation, eastStation;
    pthread_mutex_init(&westStation.trainQueueMutex, NULL);
    westStation.stationInput = NULL;
    pthread_cond_init(&westStation.inputSignal, NULL);
    pthread_cond_init(&westStation.inputEmpty, NULL);

    pthread_mutex_init(&eastStation.trainQueueMutex, NULL);
    eastStation.stationInput = NULL;
    pthread_cond_init(&eastStation.inputSignal, NULL);
    pthread_cond_init(&eastStation.inputEmpty, NULL);

    Stations stations;
    stations.east = &eastStation;
    stations.west = &westStation;

    StationThreadArgs westStationThreadArgs;
    westStationThreadArgs.stationInfo = &westStation;
    westStationThreadArgs.coutMutex = &coutMutex;
    westStationThreadArgs.waitingTrainSignal = dispatcher.waitingTrainSignal;
    westStationThreadArgs.startTime = &startTime;
    westStationThreadArgs.settingTrainSignal = &settingTrainSignal;
    pthread_create(&westStationThread, NULL, &createStation, (void *)&westStationThreadArgs);

    StationThreadArgs eastStationThreadArgs;
    eastStationThreadArgs.stationInfo = &eastStation;
    eastStationThreadArgs.coutMutex = &coutMutex;
    eastStationThreadArgs.waitingTrainSignal = dispatcher.waitingTrainSignal;
    eastStationThreadArgs.startTime = &startTime;
    eastStationThreadArgs.settingTrainSignal = &settingTrainSignal;
    pthread_create(&eastStationThread, NULL, &createStation, (void *)&eastStationThreadArgs);

    dispatcher.westStationQueue = &westStation.trainQueue;
    dispatcher.westStationQueueMutex = &westStation.trainQueueMutex;
    dispatcher.eastStationQueue = &eastStation.trainQueue;
    dispatcher.eastStationQueueMutex = &eastStation.trainQueueMutex;
    pthread_t dispatcherThread;
    pthread_create(&dispatcherThread, NULL, &createDispatcher, (void *)&dispatcherThreadArgs);

    // ----- Setup Trains ------
    // Start signal
    bool startLoadingSignal = false;
    // Create file stream and string to stream to
    std::ifstream infile(argv[1]);
    std::string trainEntry;
    // Parse in file
    std::vector<TrainThread> trainThreads;
    trainNum = 0;
    while (std::getline(infile, trainEntry))
    {
        TrainThreadArgs *nextTrainThreadArgs = newTrainThreadArgs();

        nextTrainThreadArgs->numberInput = trainNum;
        nextTrainThreadArgs->startSignal = &startLoadingSignal;
        nextTrainThreadArgs->coutMutex = &coutMutex;
        nextTrainThreadArgs->stations = &stations;
        nextTrainThreadArgs->startTime = &startTime;

        std::istringstream iss(trainEntry);
        iss >> nextTrainThreadArgs->travelInput >> nextTrainThreadArgs->loadTimeInput >> nextTrainThreadArgs->crossTimeInput;

        pthread_t nextThread;
        pthread_create(&nextThread, NULL, &createTrain, (void *)nextTrainThreadArgs);

        nextTrainThreadArgs->trainThread->thread = &nextThread;

        trainThreads.push_back(*nextTrainThreadArgs->trainThread);

        trainNum += 1;
    }

    // Sleep main thread for 1 second to ensure threads finish prepping
    struct timespec tim, tim2;
    tim.tv_sec = 1;
    tim.tv_nsec = 0;
    nanosleep(&tim, &tim2);

    startTime = timestamp();

    // Tell threads to start loading
    startLoadingSignal = true;

    nanosleep(&tim, &tim2);
    pthread_cond_signal(&westStation.inputEmpty);
    pthread_cond_signal(&eastStation.inputEmpty);

    while (quit != true)
        ;

    return 0;
};
