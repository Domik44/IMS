#include "Cell.h"


Cell::Cell()
{
	value = 0;
	colour[0] = 0;
	colour[1] = 0;
	colour[2] = 0;
	generator = false;
}

void Cell::setColour(unsigned char r, unsigned char b, unsigned char g)
{
	colour[0] = r;
	colour[1] = b;
	colour[2] = g;
}

void Cell::setColour(Cell* cell)
{
	colour[0] = cell->colour[0];
	colour[1] = cell->colour[1];
	colour[2] = cell->colour[2];
}

void Cell::incrementColour(unsigned char r, unsigned char b, unsigned char g)
{
	colour[0] += r;
	colour[1] += b;
	colour[2] += g;
}

void Cell::incrementColour(Cell* cell)
{
	colour[0] += cell->colour[0];
	colour[1] += cell->colour[1];
	colour[2] += cell->colour[2];
}


void Cell::setColourFromValue(int maxValue)
{
	const int colourMax = 125; 	//GLUT colour4b max
	static int max = 0;

	// int colourValue = this->value * colourMax;
	if (this->value > 125000) //RED
	{
		colour[0] = 125;
		colour[1] = 0;
		colour[2] = 0;
	}
	else if (this->value > colourMax * 100)
	{
		int convertor = this->value / 1000;
		colour[0] = convertor;
		colour[1] = colourMax - convertor;
		colour[2] = 0;
	}
	else if (this->value > colourMax * 10)
	{
		int convertor = this->value / 100;
		colour[0] = 0;
		colour[1] = convertor;
		colour[2] = colourMax - convertor;
	}
	else if (this->value > colourMax)
	{
		int convertor = this->value / 10;
		colour[0] = 0;
		colour[1] = 0;
		colour[2] = convertor;
	}
	else if (this->value > 0)
	{
		colour[0] = colourMax - this->value;
		colour[1] = colourMax - this->value;
		colour[2] = colourMax - this->value;
	}
	else
	{
		colour[0] = colourMax;
		colour[1] = colourMax;
		colour[2] = colourMax;		
	}





	// colour[0] = colourMax - colourValue;
	// colour[1] = colourMax - colourValue;
	// colour[2] = colourMax - colourValue;
}

void Cell::copySrc(Cell* src)
{
	this->value = src->value;
	this->generator = src->generator;
	this->colour[0] = src->colour[0];
	this->colour[1] = src->colour[1];
	this->colour[2] = src->colour[2];
}