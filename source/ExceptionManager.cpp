#include <sstream>

#include "ExceptionManager.hpp"

void fsig::ExceptionManager::add(const std::exception & e)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMessages.push_back(e.what());
}

void fsig::ExceptionManager::throwAll()
{
    std::lock_guard<std::mutex> lock(mMutex);

    if (mMessages.empty())
        return;

    std::stringstream ss;

    for(const auto & message : mMessages)
        ss << message << "\n";

    throw std::runtime_error(ss.str());
}
