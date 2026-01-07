#include <iostream>
#include <tuple>
#include "main.h"
#include <string>

const int WALL_SIGN = 5;
const int NO_WALL_SIGN = 0;
const int ROUTE_SIGN = 1;

void Print2dArray(int** array, int rows, int collumns)
{
	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < collumns; y++)
		{
			std::cout << array[x][y] << " ";
		}

		std::cout << std::endl;
	}
}

void MeasureMapDimensions(const char* mapText, int& rows, int& collumns)
{
	int i = 0;
	while (mapText[i] != '\0')
	{
		if (rows == 1)
			if (mapText[i] != ' ' && mapText[i] != '\n')
				collumns++;

		if (mapText[i] == '\n')
			rows++;

		i++;
	}

	std::cout << "rows is " << rows << std::endl;
	std::cout << "collumns is " << collumns << std::endl;
	std::cout << std::endl;
}


void Set2dArrayValuesFromText(const char* mapText, int** array)
{
	int x = 0;
	int y = 0;

	int i = 0;
	while (mapText[i] != '\0')
	{
		if (mapText[i] != ' ' && mapText[i] != '\n')
		{
			array[x][y] = mapText[i] - '0'; //znaki liczb sa po kolei w ASCII wiec jezeli odejme poczatkowy znak 0 to jest gitara
			y++;
		}

		if (mapText[i] == '\n')
		{
			y = 0;
			x++;
		}

		i++;
	}
}

int** InitEmpty2dArray(int rows, int collumns)
{
	int** array = new int* [rows];
	for (int i = 0; i < rows; i++)
		array[i] = new int[collumns];

	return array;
}

int** GetInited2dArray(const char* mapText, int rows, int collumns)
{
	int** array = InitEmpty2dArray(rows, collumns);
	Set2dArrayValuesFromText(mapText, array);

	return array;
}

int** GetArrayFromGrid(const char* mapText)
{
	int rows = 1;
	int collumns = 0;

	MeasureMapDimensions(mapText, rows, collumns);
	int** array = GetInited2dArray(mapText, rows, collumns);
	Print2dArray(array, rows, collumns);
	
	return array;
}

int main()
{
	const char* mapText =
R"(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 5 5 5
5 5 5 5 5 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0
0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 5 0 0 5 5 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 0 0
0 0 0 5 5 5 5 5 5 5 5 5 0 5 0 0 0 0 0 0
5 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 0 0
5 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 5 5
5 0 0 0 0 0 5 5 0 0 5 5 0 0 0 0 5 0 0 0
5 0 0 0 0 0 5 5 0 0 5 5 0 0 0 0 5 0 0 0
5 0 0 0 5 5 5 5 0 0 5 0 0 0 0 0 5 0 0 0
5 5 5 5 5 5 5 5 5 5 5 0 0 0 0 0 5 0 0 0
5 0 0 0 0 0 5 0 0 0 5 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0)";



	const char* mapText30x20 =
		R"(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 1 1 1 1 1 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 1
5 5 5 5 5 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 5 0 0 5 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 5 5 5 5 5 5 5 5 5 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 5 5 5 5 5 5 5 5 5 5 5 5
5 0 0 0 0 0 5 5 0 0 5 5 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 0 0 5 5 0 0 5 5 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 5 5 5 5 0 0 5 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
5 5 5 5 5 5 5 5 5 5 5 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 0 0 5 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)";


	int one = 1;
	std::cout << (int)WALL_SIGN << std::endl;


	int** map = GetArrayFromGrid(mapText30x20);

	


	return 0;
}


