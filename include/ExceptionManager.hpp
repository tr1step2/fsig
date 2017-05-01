#pragma once

#include <list>
#include <string>
#include <mutex>
#include <exception>
#include <stdexcept>

namespace fsig
{

struct ExceptionManager
{
    void add(const std::exception & e);
    void throwAll();

private:
    std::list<std::string> mMessages;
    std::mutex mMutex;
};

} // ns fsig
