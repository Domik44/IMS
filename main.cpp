#include <GL/gl.h>
#include <GL/glut.h>
 
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include "DB.h"


using namespace std;

int MAX_SIZE = 800; // == 8km in real life (8x8 = 64 km^2, represented as 800x800 picture)
DB* db = NULL;
const int pixelSize = 4; // 800/4 means we have 200 cells in one row (200x200 cells overall) 
// -> thats (8*10^3)/200 = one side of cell is 40m in real life (40x40 = 1600 m^2)

int clickX;
int clickY;
bool clicked = false;


void printSimHelp()
{
    printf("NAME\n\tsimulator - air pollution simulator\n");
    printf("\n\n");
    printf("SYNOPSIS\n\tsimulator [-g] [-l] [-w [0-4]] [-c gass_concentration] [-t [0-1]] [-p] [-m maximum_time] [-h]\n");
    printf("\n\n");
    printf("DESCRIPTION\n\tSimulate air pollution via a cellular automat");
    printf("\n\n");
    printf("OPTIONS\n");
    printf("\t-g\tuse GUI, requires X11 to function (VUT merlin.fit.vutbr.cz Usage -> connect from Unix machine via ssh with argument -Y \n\t\t[ssh -Y xlogin00@merlin.fit.vutbr.cz]\n\n");
    printf("\t-l\tstart logging info to stdout (implicitly enabled if run without GUI\n\n");
    printf("\t-w\tselect wind preset [0-4]\n\n");
    printf("\t-c\tselect gass concentration from gass generator\n\n");
    printf("\t-t\tset if gass generator should continually generate gass [1] OR  one time explosion [0] (concentration will be set only once)\n\n");
    printf("\t-p\tthe wind will change direction after 200s to a set preset\n\n");
    printf("\t-m\tset the maximum time counter (off by default)\n\n");
    printf("\t-h\tprint help\n\n");
    printf("\n\n");
}

void mouseCallback(int button, int state, int x, int y)
{
    // Save the button state
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        cout << "CLICK " << x << " " << y <<"\n";
        Cell* ptr = db->getNewCell(x / pixelSize, y / pixelSize);
        ptr->generator = true;
    }
}

//Glut does not allow disabling window rezice, this is a workaround
void resizeDisabler(int width, int height)
{
    glutReshapeWindow( MAX_SIZE, MAX_SIZE);
}


void paintSquare(int x, int y, Cell* cell)
{            
    glColor4b(cell->colour[0], cell->colour[1], cell->colour[2], 0); //TODO set colour as enum?
   
    glBegin(GL_QUADS); //squares
        glVertex2f(pixelSize*x,           pixelSize*y);
        glVertex2f(pixelSize*(x+pixelSize) ,pixelSize*y);
        glVertex2f(pixelSize*(x+pixelSize) ,pixelSize*(y+pixelSize));
        glVertex2f(pixelSize*x,           pixelSize*(y+pixelSize));
    glEnd();
}

// Display function will draw the image.
void display() 
{  
    // glClear(GL_COLOR_BUFFER_BIT);       //clear buffers to preset values
    // glLoadIdentity();                   //replaces the current matrix with the identity matrix

    if (clicked)
    {
        clicked = false;
        Cell* ptr = db->getNewCell(clickX / pixelSize, clickY / pixelSize);
        ptr->generator = true;
        ptr = db->getOldCell(clickX / pixelSize, clickY / pixelSize);
        ptr->generator = true;
    }

    //HERE UPDATE BUFFER WITH CA LOGIC
    if (db->t < db->maxTime)
    {
        db->updateCells();
        if (db->print)
            db->printLogRow();
        db->t ++;
    }

    //Paint to GUI
    glMatrixMode( GL_PROJECTION );      //Applies subsequent matrix operations to the projection matrix stack
    glLoadIdentity();
    gluOrtho2D(0.0, MAX_SIZE, MAX_SIZE, 0.0); //define a 2D orthographic projection matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, MAX_SIZE, MAX_SIZE);

    for (int y = 0; y < MAX_SIZE/pixelSize; y += 1)
    {
        for (int x = 0; x < MAX_SIZE/pixelSize; x += 1)
        {
            Cell* ptr = db->getNewCell(x, y);
            paintSquare(x, y, ptr);
        }
    }

    glutSwapBuffers();
    glutPostRedisplay();
}


int main( int argc, char** argv ) {  // Initialize GLUT and 

    db = new DB(MAX_SIZE / pixelSize, MAX_SIZE / pixelSize);
    if (db == NULL)
    {
        myExit("DB new", ERR_MALLOC);
    }

    //bool GUI = true;
    bool GUI = false;
    //parse Arguments
    while(true)
    {
        switch(getopt(argc, argv, "gpw:m:lc:t:"))
        {
            case 'g':
                GUI = true;
                continue;
            case 'w':
            {
                // No wind
                float w0[9] = {};
                // Strong South and West wind
                float w1[9] = { 3.0,1.0,1.0,
                                7.0,0.0,1.0,
                                4.0,10.0,2.0
                              };
                // Strong South wind
                float w2[9] = { 0.0,0.0,0.0,
                                0.0,0.0,0.0,
                                3.0,10.0,3.0
                              };
                // Weak South wind
                float w3[9] = { 0.0,0.0,0.0,
                                0.0,0.0,0.0,
                                2.0,2.0,2.0
                              };

                if (atoi(optarg) == 1)
                    db->wind = db->getWindArray(w1);
                else if  (atoi(optarg) == 2)
                    db->wind = db->getWindArray(w2);
                else if  (atoi(optarg) == 3)
                    db->wind = db->getWindArray(w3);
                else
                    db->wind = db->getWindArray(w0);
                continue;
            }
            case 'p':
            {
                float w4[9] = {1.0,1.0,1.0,0.0,0.0,10.0,4.0,1.0,5.0};
                db->wind2 = db->getWindArray(w4);
                db->turnWind = true;
                continue;
            }
            case 'c':
                db->concentration = abs(stoi(optarg));
                continue;
            case 'm':
                db->maxTime = stoi(optarg);
                continue;
            case 'l':
                db->print = true;
                continue;
            case 't':
                if(atoi(optarg) == 1)
                {
                    db->generatorType = true;
                }
                continue;
            case -1:
                break;
            
            default :
            case '?':
            case 'h':
                printSimHelp();
                return 0;

        }

        break;
    }


    //set point of origin (will be replaced by click/)
    db->getNewCell(100, 100)->generator = true;

    if (GUI)
    {
        if (db->print)
            db->printLogHead();

        const int WinX = MAX_SIZE;
        const int WinY = MAX_SIZE;
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE);   // Use single color buffer and no depth buffer.
        glutInitWindowSize(WinX,WinY);      // Size of display area, in pixels.
        glutInitWindowPosition(100,100);    // Location of window in screen coordinates.
        glutCreateWindow("IMS simulator");  // Parameter is window title.
        glutDisplayFunc(display);           // Called when the window needs to be redrawn.
        glutMouseFunc(mouseCallback);       // Callback for mouse action

        //disable window rezise
        glutReshapeFunc(resizeDisabler);


        glutMainLoop(); // Run the event loop!  This function does not return.
                        // Program ends when user closes the window.
    }
    else
    {
        //no GUI -> force Logging
        db->print = true;
        db->printLogHead();
        while (db->t < db->maxTime)
        {
            db->updateCells();
            db->printLogRow();
            db->t++;
        }
    }

    return 0;

}

