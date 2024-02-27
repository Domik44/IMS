#include "DB.h"
#include <math.h> 
DB::DB(int WinX, int WinY) : WinX(WinX), WinY(WinY)
{
	cells= (Cell*)calloc(sizeof(Cell), WinY * WinX);
	if (cells == NULL)
	{
		myExit("Cell DB calloc", ERR_MALLOC);
	}

	oldcells = (Cell*)calloc(sizeof(Cell), WinY * WinX);
	if (oldcells == NULL)
	{
		myExit("oldCell DB calloc", ERR_MALLOC);
	}
	
	print = false;
	t = 0;
	maxTime = INT_MAX;
	turnWind = false;
	concentration = 300000;
	generatorType = false;

	float w0[9] = {};
	wind = getWindArray(w0);
}
int DB::getWinX()
{
	return WinX;
}

int DB::getWinY()
{
	return WinY;
}


Cell* DB::getOldCell(int x, int y)
{
	if (x < 0 || y < 0 || x >= WinX || y >= WinY)
		return NULL;
	if (this->oldcells != NULL)
		return (this->oldcells + y * WinX + x);
	return NULL;
}

Cell* DB::getNewCell(int x, int y)
{
	if (x < 0 || y < 0 || x >= WinX || y >= WinY)
		return NULL;
	if (this->cells != NULL)
		return (this->cells + y * WinX + x);
	return NULL;
}



int DB::RGBSum(Cell* p)
{
	if (p != NULL)
		return p->colour[0] + p->colour[1] + p->colour[2];
	return 0;
}


//TODO RANDOM MATRIX, PUT YOUR CA LOGIN HERE
void DB::updateCell(Cell* cell, unsigned int cellX, unsigned int cellY)
{
	if (cell->generator)
	{
		Cell* ptr = getNewCell(cellX, cellY);
		ptr->value = concentration;
		ptr->setColourFromValue( this->concentration);
		ptr->generator = generatorType;
	}
	else
	{
		// Constants 
		const float r = sqrt(2)*CELL_SIDE_LENGTH;  			// Distance between middle cell and neibours
		const float alfa = 1/(2*pow(CELL_SIDE_LENGTH, 2)); 	// Weight coefficient of N, E, W, S neighbours
		const float beta = 1/(4*pow(CELL_SIDE_LENGTH, 2)); 	// Weight coefficient of NE, NW, SE, SW neighbours
		const float first_order_coef = 0.003; 				// First order coefficient

		float diff = 0;
		float wind_diff = 0;
		float diffusion_constant = 0;
		float weight_constant = 0;

		// New cell 
		Cell* ptr = getNewCell(cellX, cellY);
		ptr->generator = false;
		ptr->value = cell->value; // Old concentration

		int cnt = 0;

		// Go through all the neibours
		for (int y = -1 ; y <= 1; y++)
		{
			for (int x = -1 ; x <= 1; x++)
			{
				if (x == 0 && y == 0) // Dont count itself
					continue;
					
				Cell* neighbour = getOldCell(x + cellX, y + cellY); // Getting neighbour
				if (neighbour)
				{
					if (abs(x) == 1 && abs(y) == 1) // Decide what neighbour it is
					{
						diffusion_constant = DIFFUSION_CORNER;
						weight_constant = beta;
					}
					else
					{
						diffusion_constant = DIFFUSION_NEIGHBOUR;
						weight_constant = alfa;
					}

					diff += diffusion_constant * weight_constant * (neighbour->value - cell->value); // Diffusion
					float wind_val = wind[y+1][x+1];
					if(turnWind && t > 50){
						wind_val = wind2[y+1][x+1];
					}
					wind_diff += (wind_val/r) * (neighbour->value - cell->value); // Aplying wind

					cnt ++;
				}
			}
		}

		ptr->value += std::round(diff+wind_diff-(first_order_coef*cell->value)); // Next state value
		ptr->setColourFromValue( this->concentration);
	}

}


void DB::updateCells()
{
	Cell* switcher = cells;
	cells = oldcells;
	oldcells = switcher;
	for (unsigned int y = 0; y < getWinY() - 1; ++y)
	{
		for (unsigned int x = 0; x < getWinX() - 1; ++x)
		{
			Cell* cellptr = getOldCell(x, y);
			if (cellptr != NULL)
			{
				//update new cells from old cells
				updateCell(cellptr, x, y);
				std::cout << "";
			}
		}
	}
}

void DB::printLogHead()
{
	printf("-------------LOG------------------\n");
	printf("  Time| ToxicCells Count| Total Value\n");
}

void DB::printLogRow()
{
	int totalToxic = 0;
	int totalValue = 0;
	for (unsigned int y = 0; y < getWinY() - 1; ++y)
	{
		for (unsigned int x = 0; x < getWinX() - 1; ++x)
		{
			Cell* cellptr = getOldCell(x, y);
			if (cellptr->value != 0)
			{
				totalToxic++;
				totalValue += cellptr->value;
			}
		}
	}
	printf("%6d|%16d |%12d\n", t, totalToxic, totalValue);
}

float **DB::getWindArray(float k[9]){
    int l = 0;
    float **p = new float*[3];
    for (int i = 0; i < 3; ++i){
        p[i] = new float[3];
        for(int j = 0; j < 3; ++j){
            p[i][j] = k[l];
            l++;
        }
    }
    
    return p;
}
