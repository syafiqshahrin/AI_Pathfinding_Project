#pragma once
//create a 2d vector of cells
//can be resized
//generate grid function that initialises 2d vector (initialises cell data)
//

class Grid
{
public:
	Grid();
	Grid(int dimension, int w, int h, Entity* e);
	~Grid();
	
	void ResizeGrid(int dimension, int w, int h);
	void CheckInput(InputData& input);
	void ClearGrid();
	void ResetGrid();
	void DrawGrid();
	void SetStartToggle();
	void SetEndToggle();
	//Grid Data
	std::vector<std::vector<Cell*>> grid;
	std::vector<float> cellMatrices;
	std::vector<float> cellStatus;
	Cell* startNode;
	Cell* endNode;
	int gridDimensionX;
	int gridDimensionY;
	bool startToggle;
	bool endToggle;
private:
	void GenerateGrid();
	void SetCellNeighbours();

	int gridWidth;
	int gridHeight;



	Entity* cellEntity;
};
