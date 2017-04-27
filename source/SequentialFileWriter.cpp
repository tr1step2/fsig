#include <fstream>
#include <iterator>
#include <algorithm>
#include <vector>

#include "SequentialFileWriter.hpp"

fsig::SequentialFileWriter::SequentialFileWriter(const std::string & filename, size_t sequencesSize)
    : mFileName(filename)
    , mSequencesSize(sequencesSize)
    , mStorage(sequencesSize)
{
}

bool fsig::SequentialFileWriter::write_data(const size_t index, const std::string & data)
{
    std::lock_guard<std::recursive_mutex> lockStorage(mStorageMutex);
    mStorage[index - mLastIndex] = data;

    try_write();

    return true;
}

void fsig::SequentialFileWriter::try_write()
{
    std::vector<std::string> storageCopy;

    {
        std::lock_guard<std::recursive_mutex> lockStorage(mStorageMutex);

        //calc sequence to write on disk
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

    std::lock_guard<std::mutex> writeLock(mWriteMutex);

    std::ofstream ofs(mFileName, std::ios_base::app);

    std::ostream_iterator<std::string> outIterator(ofs, "");
    std::copy(storageCopy.begin(), storageCopy.end(), outIterator);
}
