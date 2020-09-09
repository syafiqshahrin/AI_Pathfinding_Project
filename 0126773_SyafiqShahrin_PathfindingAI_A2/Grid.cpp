#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "Matrix.h"
#include "Vector.h"
#include "Utility.h"
#include "MeshObject.h"
#include "Entity.h"
#include "UIHandler.h"
#include "Button.h"
#include "Cell.h"
#include "Grid.h"

Grid::Grid()
{

}
Grid::Grid(int dimension, int w, int h, Entity* e)
{
	gridDimensionX = dimension * (1600.0f / 1000.0f);
	gridDimensionY = dimension;
	gridWidth = w;
	gridHeight = h;
	cellEntity = e;
	GenerateGrid();
	SetCellNeighbours();
	cellEntity->m_EntityMesh->SetInstanceSize(gridDimensionX, gridDimensionY);
	cellEntity->m_EntityMesh->InitInstance(cellMatrices);
	startNode = grid[0][0];
	endNode = grid[gridDimensionY - 1][gridDimensionX - 1];
	grid[0][0]->m_Start = true;
	grid[gridDimensionY-1][gridDimensionX-1]->m_End = true;
	startToggle = false;
	endToggle = false;

}

Grid::~Grid()
{

}

void Grid::GenerateGrid()
{
	grid = std::vector<std::vector<Cell*>>(gridDimensionY);
	Matrix4 gOthorMatrix = Matrix4::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 30.0f);
	Matrix4 gViewMatrix = Matrix4::translate(Vector3(0.0f, 0.0f, -1.0f));
	for (int y = 0; y < gridDimensionY; y++)
	{
		for (int x = 0; x < gridDimensionX; x++)
		{
			Cell* cell = new Cell();
			cell->m_CellTransform.Scale = Vector3((1.0f / (1600.0f / 1000.0f)) * (1.0f / ((float)gridDimensionX / (1600.0f / 1000.0f))) , 1.0f * (1.0f / ((float)gridDimensionY)), 1.0f);
			cell->m_CellTransform.Position = Vector3(-1.0f + ((float)gridWidth * cell->m_CellTransform.Scale.x * 0.5f) + ((float)gridWidth * cell->m_CellTransform.Scale.x * 1.0f * (x)), -1.0f + ((float)gridWidth * cell->m_CellTransform.Scale.y * 0.5f) + ((float)gridHeight * cell->m_CellTransform.Scale.y * 1.0f * (y)), 0.0f);
			cell->cellLengthX = (float)gridWidth * cell->m_CellTransform.Scale.x;
			cell->cellLengthY = (float)gridHeight * cell->m_CellTransform.Scale.y;
			Matrix4 modelMatrix =
				Matrix4::translate(cell->m_CellTransform.Position) *
				Matrix4::scale(cell->m_CellTransform.Scale) *
				Matrix4::rotate(0.0f, cell->m_CellTransform.Rotation);
			Matrix4 OrthoModelMatrix = gOthorMatrix * gViewMatrix * modelMatrix;
			for (int i = 0; i < 16; i++)
			{
				cellMatrices.push_back(OrthoModelMatrix.data[i]);
			}
			cell->gridX = x;
			cell->gridY = y;
			//std::cout << "gridXY: " << cell->gridX << " , " << cell->gridY << std::endl;
			grid[y].push_back(cell);
		}
	}

	std::cout << "Grid generated" << std::endl;
}

void Grid::SetCellNeighbours()
{
	for (int y = 0; y < gridDimensionY; y++)
	{
		for (int x = 0; x < gridDimensionX; x++)
		{
			for (int i = -1; i <= 1; i++)
			{
				if ((x + 0) >= 0 && (x + 0) < gridDimensionX)
				{
					if ((y + i) >= 0 && (y + i) < gridDimensionY)
					{
						if (i == 0)
						{
							continue;
						}
						if (grid[y + i][x + 0]->m_Obstacle)
						{
							continue;
						}
						grid[y][x]->m_horiVertiNeighbours.push_back(grid[y + i][x + 0]);
					}
				}
			}
			for (int i = -1; i <= 1; i++)
			{
				if ((x + i) >= 0 && (x + i) < gridDimensionX)
				{
					if ((y + 0) >= 0 && (y + 0) < gridDimensionY)
					{
						if (i == 0)
						{
							continue;
						}
						if (grid[y + 0][x + i]->m_Obstacle)
						{
							continue;
						}
						grid[y][x]->m_horiVertiNeighbours.push_back(grid[y + 0][x + i]);
					}
				}
			}
			for (int i = -1; i <= 1; i++)
			{
				if ((x + i) >= 0 && (x + i) < gridDimensionX)
				{
					if ((y + i) >= 0 && (y + i) < gridDimensionY)
					{
						if (i == 0)
						{
							continue;
						}
						if (grid[y + i][x + i]->m_Obstacle)
						{
							continue;
						}
						grid[y][x]->m_diagonalNeighbours.push_back(grid[y + i][x + i]);
					}
				}
			}
			for (int i = -1; i <= 1; i++)
			{
				if ((x + (i * -1)) >= 0 && (x + (i * -1)) < gridDimensionX)
				{
					if ((y + i) >= 0 && (y + i) < gridDimensionY)
					{
						if (i == 0)
						{
							continue;
						}
						if (grid[y + i][x + (i * -1)]->m_Obstacle)
						{
							continue;
						}
						grid[y][x]->m_diagonalNeighbours.push_back(grid[y + i][x + (i * -1)]);
					}
				}
			}
		}
	}
	
}

void Grid::ResizeGrid(int dimension, int w, int h)
{
	gridDimensionX = dimension * (1600.0f / 1000.0f);
	gridDimensionY = dimension;
	gridWidth = w;
	gridHeight = h;
	cellMatrices.clear();
	GenerateGrid();
	SetCellNeighbours();
	cellEntity->m_EntityMesh->SetInstanceSize(gridDimensionX, gridDimensionY);
	cellEntity->m_EntityMesh->InitInstance(cellMatrices);
	startNode = grid[0][0];
	endNode = grid[gridDimensionY - 1][gridDimensionX - 1];
	grid[0][0]->m_Start = true;
	grid[gridDimensionY - 1][gridDimensionX - 1]->m_End = true;
}

void Grid::CheckInput(InputData& input)
{
	if (endToggle)
	{
		for (int y = 0; y < gridDimensionY; y++)
		{
			for (int x = 0; x < gridDimensionX; x++)
			{
				if (grid[y][x]->CheckCellInput(input, startToggle, endToggle))
				{
					endNode->m_End = false;
					endNode = grid[y][x];
					endToggle = false;
				}
			}
		}
	}
	else if (startToggle)
	{
		for (int y = 0; y < gridDimensionY; y++)
		{
			for (int x = 0; x < gridDimensionX; x++)
			{
				if (grid[y][x]->CheckCellInput(input, startToggle, endToggle))
				{
					startNode->m_Start = false;
					startNode = grid[y][x];
					startToggle = false;
				}
			}
		}
	}
	else
	{
		for (int y = 0; y < gridDimensionY; y++)
		{
			for (int x = 0; x < gridDimensionX; x++)
			{
				grid[y][x]->CheckCellInput(input, startToggle, endToggle);
			}
		}
	}


	
}

void Grid::ClearGrid()
{
	for (int y = 0; y < gridDimensionY; y++)
	{
		for (int x = 0; x < gridDimensionX; x++)
		{
			grid[y][x]->m_Path = false;
			grid[y][x]->m_Obstacle = false;
			grid[y][x]->m_Visited = false;
			grid[y][x]->m_InOpenList = false;
			
		}

	}
}
void Grid::ResetGrid()
{
	for (int y = 0; y < gridDimensionY; y++)
	{
		for (int x = 0; x < gridDimensionX; x++)
		{
			grid[y][x]->m_Path = false;
			grid[y][x]->m_Visited = false;
			grid[y][x]->m_InOpenList = false;

		}

	}
}


void Grid::DrawGrid()
{
	for (int y = 0; y < gridDimensionY; y++)
	{
		for (int x = 0; x < gridDimensionX; x++)
		{
			/*
			cellEntity->m_EntityTransform = grid[y][x].m_CellTransform;
			
			cellEntity->m_meshShader->use();
			cellEntity->m_meshShader->setBool("Obstacle", grid[y][x].m_Obstacle);
			cellEntity->m_meshShader->setBool("Start", grid[y][x].m_Start);
			cellEntity->m_meshShader->setBool("End", grid[y][x].m_End);
			
			cellEntity->DrawEntity(0);
			*/
			cellStatus.push_back(grid[y][x]->m_Obstacle);
			cellStatus.push_back(grid[y][x]->m_Start);
			cellStatus.push_back(grid[y][x]->m_End);
			cellStatus.push_back(grid[y][x]->m_Path);
			cellStatus.push_back(grid[y][x]->m_Visited);
			cellStatus.push_back(grid[y][x]->m_InOpenList);

		}
	}
	cellEntity->m_EntityMesh->SendVec4(cellStatus);
	cellEntity->DrawEntityInstance(0);
	cellStatus.clear();
}

void Grid::SetStartToggle()
{
	endToggle = false;
	startToggle = !startToggle;
}
void Grid::SetEndToggle()
{
	startToggle = false;
	endToggle = !endToggle;
}