#include <iostream>

#include "Signature.hpp"

int main(int argc, char ** argv)
{
    try
    {
        fsig::Signature fsig({argc, argv});
        fsig.wait();
    }
    catch(const std::exception & e)
    {
        std::cout << "Error occured. Message: " << e.what();
    }
}
