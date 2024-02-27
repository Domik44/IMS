#include "Cell.h"

#define CELL_SIDE_LENGTH 40		// Length of one side of cell
#define DIFFUSION_NEIGHBOUR 10 	// Diffusive coefficient for N, E, W, S directions -> m^2/s
#define DIFFUSION_CORNER 2 		// Diffusive coefficient for NE, NW, SE, SW directions -> m^2/s

/**
 * @brief Holds most simulation info
 *	glwf callbacks cant have more arguments, but are able to have one global pointer with glfwSetWindowUserPointer() -> one class for most info
*/
class DB
{
private:
	int const WinX;
	int const WinY;


public:
	Cell* cells;
	Cell* oldcells;
	float **wind; 
	float **wind2; 
	bool turnWind;
	bool print;
	int t;
	int maxTime;
	int concentration;
	bool generatorType;


	DB(int WinX, int WinY);

	/**
	 * @brief get cell from OLDcells buffer;
	 * @return found cell or null
	*/
	Cell* getOldCell(int x, int y);

	Cell* getNewCell(int x, int y);


	void updateCell(Cell* cell, unsigned int cellX, unsigned int cellY);

	void updateCells();

	void printLogHead();
	void printLogRow();
	
	int RGBSum(Cell* p);

	int getWinX();

	int getWinY();

	float **getWindArray(float k[9]);
};

