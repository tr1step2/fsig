#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <memory>

namespace fsig
{

struct CycleWorker
{
    CycleWorker();
    ~CycleWorker();

    void start(std::function<void()> work, size_t sleepms);
    void stop();
    void wait();

private:
    std::unique_ptr<std::thread> mWorkThread;
    std::atomic<bool> mStop;
};

} //ns
