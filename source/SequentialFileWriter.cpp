#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>

#include "SequentialFileWriter.hpp"

fsig::SequentialFileWriter::SequentialFileWriter(const std::string & filename, size_t sequencesSize)
    : mFileName(filename)
    , mSequencesSize(sequencesSize)
    , mStop(false)
    , mStorage(sequencesSize)
{
    mWriteThread.reset(new std::thread ([this]()
    {
        while(!mStop)
        {
            std::unique_lock<std::mutex> lock(mEventMutex);
            mWriteEvent.wait(lock);

            if(mStop)
                return;

            std::vector<std::string> storageCopy;
            {
                std::lock_guard<std::mutex> lock(mStorageMutex);

                //calc sequunce
                size_t i = 0;
                for(; i < mStorage.size(); ++i)
                {
                    if(mStorage[i].empty())
                        break;
                }

                mLastIndex += i;

                //copy
                storageCopy.reserve(storageCopy.size());
                std::copy(mStorage.begin(), mStorage.begin() + i, std::back_inserter(storageCopy));

                //delete
                mStorage.erase(mStorage.begin(), mStorage.begin() + i);
            }

            std::ofstream ofs(mFileName, std::ios_base::app);

            std::ostream_iterator<std::string> outIterator(ofs, "");
            std::copy(storageCopy.begin(), storageCopy.end(), outIterator);
        }
    }));
}

fsig::SequentialFileWriter::~SequentialFileWriter()
{
    mStop = true;

	std::unique_lock<std::mutex> lock(mEventMutex);
    mWriteEvent.notify_one();

	mWriteThread->join();
}

bool fsig::SequentialFileWriter::write_data(const size_t index, const std::string & data)
{
	std::unique_lock<std::mutex> lockEvent(mEventMutex);
    mWriteEvent.notify_one();

	std::lock_guard<std::mutex> lockStorage(mStorageMutex);
	mStorage[index - mLastIndex] = data;

    return true;
}
