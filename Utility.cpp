#include "Utility.h"


//TODO free memory
void myExit(std::string errMsg, int errNum)
{
    std::cerr << "Error: " << errMsg << "\n";
    exit(errNum);
}