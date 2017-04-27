#include "ThreadPool.hpp"

fsig::ThreadPool::ThreadPool(size_t size)
    : mWorking(new boost::asio::io_service::work(mService))
{
    boost::asio::io_service::work work(mService);

    for(size_t i = 0; i < size; ++i)
    {
        mThreadGroup.create_thread([this]()
        {
            mService.run();
        });
    }
}

fsig::ThreadPool::~ThreadPool()
{
    mService.stop();
    wait_all();
}

void fsig::ThreadPool::wait_all()
{
    mWorking.reset();
    mThreadGroup.join_all();
}
