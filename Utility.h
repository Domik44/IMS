#include <iostream>
#include <climits>
#define ERR_GLUT -5
#define ERR_MALLOC -1

enum WIND { N, NE, E, SE, S, SW, W, NW, None};


void myExit(std::string errMsg, int errNum);
