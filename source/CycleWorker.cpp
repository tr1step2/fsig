#include <chrono>

#include "CycleWorker.hpp"

fsig::CycleWorker::CycleWorker()
    : mStop(false)
{
}

fsig::CycleWorker::~CycleWorker()
{
    stop();
    wait();
}

void fsig::CycleWorker::start(std::function<void ()> work, size_t sleepms)
{
    if (mWorkThread != nullptr)
        throw std::runtime_error("Worker busy");

    mWorkThread.reset(new std::thread([this, work, sleepms]()
    {
        while(!mStop)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepms));
            work();
        }

        //we should call work for last time
        work();
    }));
}

void fsig::CycleWorker::stop()
{
    mStop = true;
}

void fsig::CycleWorker::wait()
{
    if(mWorkThread)
        mWorkThread->join();
}
