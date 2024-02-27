#include "Utility.h"
class Cell
{
public:
	unsigned char colour[3];
	bool generator;
	int value;

	Cell();

	void setColour(unsigned char r, unsigned char b, unsigned char g);
	void setColour(Cell* cell);
	void incrementColour(unsigned char r, unsigned char b, unsigned char g);
	void incrementColour(Cell* cell);
	void setColourFromValue(int maxValue);
	void copySrc(Cell* src);
};

