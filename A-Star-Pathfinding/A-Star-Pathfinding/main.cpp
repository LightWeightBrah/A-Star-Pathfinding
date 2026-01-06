#pragma region generatedMap
//number_of_obstacles = 17
//obstacle number = 1
//obstacle_size = 9
//direction = 1
//x_coordinate = 15
//y_coordinate = 1
//obstacle number = 2
//obstacle_size = 4
//direction = 1
//x_coordinate = 5
//y_coordinate = 1
//obstacle number = 3
//obstacle_size = 7
//direction = 1
//x_coordinate = 4
//y_coordinate = 15
//obstacle number = 4
//obstacle_size = 1
//direction = 1
//x_coordinate = 14
//y_coordinate = 5
//obstacle number = 5
//obstacle_size = 6
//direction = 0
//x_coordinate = 7
//y_coordinate = 12
//obstacle number = 6
//obstacle_size = 1
//direction = 1
//x_coordinate = 8
//y_coordinate = 11
//obstacle number = 7
//obstacle_size = 6
//direction = 1
//x_coordinate = 9
//y_coordinate = 4
//obstacle number = 8
//obstacle_size = 5
//direction = 0
//x_coordinate = 0
//y_coordinate = 17
//obstacle number = 9
//obstacle_size = 3
//direction = 0
//x_coordinate = 8
//y_coordinate = 14
//obstacle number = 10
//obstacle_size = 8
//direction = 0
//x_coordinate = 12
//y_coordinate = 7
//obstacle number = 11
//obstacle_size = 8
//direction = 0
//x_coordinate = 6
//y_coordinate = 8
//obstacle number = 12
//obstacle_size = 2
//direction = 1
//x_coordinate = 2
//y_coordinate = 16
//obstacle number = 13
//obstacle_size = 9
//direction = 1
//x_coordinate = 11
//y_coordinate = 19
//obstacle number = 14
//obstacle_size = 9
//direction = 0
//x_coordinate = 7
//y_coordinate = 11
//obstacle number = 15
//obstacle_size = 6
//direction = 0
//x_coordinate = 10
//y_coordinate = 1
//obstacle number = 16
//obstacle_size = 8
//direction = 0
//x_coordinate = 17
//y_coordinate = 7
//obstacle number = 17
//obstacle_size = 3
//direction = 0
//x_coordinate = 12
//y_coordinate = 17
//
//
//The generated map is the following :
//0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0
//0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 0 0
//0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0
//0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 5 5 5
//5 5 5 5 5 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0
//0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0
//0 0 0 0 0 0 0 5 0 0 5 5 0 0 0 0 0 0 0 0
//0 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 0 0
//0 0 0 5 5 5 5 5 5 5 5 5 0 5 0 0 0 0 0 0
//5 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 0 0
//5 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 5 5
//5 0 0 0 0 0 5 5 0 0 5 5 0 0 0 0 5 0 0 0
//5 0 0 0 0 0 5 5 0 0 5 5 0 0 0 0 5 0 0 0
//5 0 0 0 5 5 5 5 0 0 5 0 0 0 0 0 5 0 0 0
//5 5 5 5 5 5 5 5 5 5 5 0 0 0 0 0 5 0 0 0
//5 0 0 0 0 0 5 0 0 0 5 0 0 0 0 0 0 0 0 0
//0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
//0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
//0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
//0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
//
//Plik  grid.txt  zostal wygenerowany
//Nacisnij ENTER aby zakonczyc

#pragma endregion

#include <iostream>
#include <tuple>
#include "main.h"
#include <string>

const char wallSign = '5';
const char noWallSign = '0';
const char routeSign = '1';

int** GetArrayFromGrid(const char* mapString)
{
	int width = 0;
	int height = 1;

	int i = 0;
	while (mapString[i] != '\0')
	{
		if(height == 1)
			if (mapString[i] != ' ' && mapString[i] != '\n')
				width++;

		if (mapString[i] == '\n')
			height++;

		i++;
	}
	
	std::cout << "width is " << width << std::endl;
	std::cout << "height is " << height << std::endl;

	
	
	return new int*[width * height];
}



int main()
{
	const int size = 20;
	std::tuple<int, int> start(0, 0);
	std::tuple<int, int> end(size - 1, size - 1);
	const char* mapString =
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


	int** map = GetArrayFromGrid(mapString);


	return 0;
}


