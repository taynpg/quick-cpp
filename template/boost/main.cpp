#include <boost/filesystem.hpp>
#include <iostream>


namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
    std::cout << fs::exists("C:/") << std::endl;
    std::cout << "..." << std::endl;
    return 0;
}
