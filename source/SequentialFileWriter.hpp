#pragma once

#include <deque>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "IWriter.hpp"

namespace fsig
{

struct SequentialFileWriter : IWriter
{
    SequentialFileWriter(const std::string & filename, size_t sequencesSize);
    ~SequentialFileWriter();

    virtual bool write_data(const size_t index, const std::string & data) override;

private:
    size_t mSequencesSize;
    std::deque<std::string> mStorage;

    std::string mFileName;
    std::mutex mStorageMutex;
    std::mutex mEventMutex;
    std::condition_variable mWriteEvent;
    std::unique_ptr<std::thread> mWriteThread;
    std::atomic<bool> mStop;
    size_t mLastIndex = 0;
};

} //ns fsig
