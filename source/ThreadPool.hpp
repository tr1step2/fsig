#pragma once

#include <boost/thread.hpp>
#include <boost/asio.hpp>

namespace fsig
{

class ThreadPool
{
public:
    ThreadPool(size_t size);
    ~ThreadPool();

    void wait_all();

    template <typename Work>
    void add(Work work)
    {
        mService.post(work);
    }

private:
    boost::asio::io_service mService;
    boost::thread_group mThreadGroup;

    std::unique_ptr<boost::asio::io_service::work> mWorking;
};

}// ns fsig
