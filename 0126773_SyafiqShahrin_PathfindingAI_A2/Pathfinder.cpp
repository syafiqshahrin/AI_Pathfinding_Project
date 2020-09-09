#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue> 
#include <algorithm>
#include <iterator>
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
#include "Node.h"
#include "Pathfinder.h"

Pathfinder::Pathfinder()
{
	startCell = nullptr;
	endCell = nullptr;
	currentCell = nullptr;
	update = false;
	enableDiagonal = false;
	enableAStar = true;
	disableCrossCorners = true;
	time = 0.0f;
	weight = 0.0f;
}
Pathfinder::~Pathfinder()
{

}

void Pathfinder::StartSearch(Cell* s, Cell* e, bool d, bool a, bool c, int gX, int gY, float w)
{
	startCell = s;
	endCell = e;
	gridMaxX = gX;
	gridMaxY = gY;
	currentCell = new Node<Cell>(startCell);
	currentCell->SetGDistance(0);
	openList.push_back(currentCell);
	update = true;
	enableDiagonal = d;
	enableAStar = a;
	disableCrossCorners = c;
	weight = w;
}

void Pathfinder::CheckCurrentNeighbours()
{
	int hvNeighbourSize = currentCell->data->m_horiVertiNeighbours.size();
	int dNeighbourSize = currentCell->data->m_diagonalNeighbours.size();
	int hvObstacleNeighbours = 0;
	for (int i = 0; i < hvNeighbourSize; i++)
	{
		//tally horizontal and vertical neighbours 
		//to be used later to remove diagonal neighbours in between two hor/ver obstacle neighbours (disable checking through obstacles)
		if (enableDiagonal || disableCrossCorners)
		{
			if (currentCell->data->m_horiVertiNeighbours[i]->m_Obstacle) {
				hvObstacleNeighbours++;
			}

		}
		/*
		//for avoiding wall sticking and crossing obstacle corners
		//check for horizontal and vertical neighbours and increasing the value of the current cell based on the amount
		if (currentCell->data->m_horiVertiNeighbours[i]->m_Obstacle) {
			currentCell->SetGDistance(currentCell->gDistance + 14);
		}
		//for avoiding wall sticking and crossing obstacle corners
		//check for diagonal neighbours and increasing the value of the current cell based on the amount
		if (i < dNeighbourSize) {
			if (currentCell->data->m_diagonalNeighbours[i]->m_Obstacle)
			{
				currentCell->SetGDistance(currentCell->gDistance + 14);
			}
		}
		*/
		if (!currentCell->data->m_horiVertiNeighbours[i]->m_Visited && !currentCell->data->m_horiVertiNeighbours[i]->m_Obstacle)
		{
			Node<Cell>* node = new Node<Cell>();
			node->data = currentCell->data->m_horiVertiNeighbours[i];
			int newGDistance = currentCell->gDistance + 10;
			Node<Cell>* parent = currentCell;
			

			if (enableAStar)
			{
				//Calculate HDistance
				int horValue = abs(endCell->gridX - node->data->gridX);
				//int horValue = (endCell->gridX - node->data->gridX) * (endCell->gridX - node->data->gridX);
				int vertValue = abs(endCell->gridY - node->data->gridY);
				//int vertValue = (endCell->gridY - node->data->gridY) * (endCell->gridY - node->data->gridY);
				float total = horValue + vertValue;
				//float total = sqrtf(horValue + vertValue);
				//std::cout << "hval: " << total << std::endl;
				node->SetHDistance((total  * weight) * 10);
			}

			
			if (!CheckIfInList(node, newGDistance, parent))
			{
				node->SetGDistance(currentCell->gDistance + 10);
				node->parent = currentCell;
				node->data->m_InOpenList = true;
				openList.push_back(node);
			}
			
		}
	}
	if (enableDiagonal) //diagonal check
	{
		for (int i = 0; i < dNeighbourSize; i++)
		{

			if (!currentCell->data->m_diagonalNeighbours[i]->m_Visited && !currentCell->data->m_diagonalNeighbours[i]->m_Obstacle)
			{
				//to remove diagonal cells between two ver/hor obstacle neighbours (disable checking through obstacles)
	
				if (hvObstacleNeighbours > 0 && disableCrossCorners)
				{
					switch (currentCell->data->m_horiVertiNeighbours.size())
					{
						case 2:
							if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle || currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && (i == 0))
							{
								continue;
							}
							break;
						case 3:
							if (currentCell->data->gridY == (gridMaxY - 1))
							{
								if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle || currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && (i == 0))
								{
									continue;
								}
								if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle || currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 1))
								{
									continue;
								}
							}
							if (currentCell->data->gridY == (0))
							{
								if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle || currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && (i == 1))
								{
									continue;
								}
								if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle || currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 0))
								{
									continue;
								}
							}
							if (currentCell->data->gridX == (gridMaxX - 1))
							{
								if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle || currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 0))
								{
									continue;
								}
								if (currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle || currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 1))
								{
									continue;
								}
							}
							if (currentCell->data->gridX == (0))
							{
								if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle || currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 1))
								{
									continue;
								}
								if (currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle || currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 0))
								{
									continue;
								}
							}
							break;
						case 4:
							if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle && (i == 0 || i == 2))
							{
								continue;
							}
							if (currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && (i == 1 || i == 3))
							{
								continue;
							}
							if (currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 0 || i == 3))
							{
								continue;
							}
							if (currentCell->data->m_horiVertiNeighbours[3]->m_Obstacle && (i == 1 || i == 2))
							{
								continue;
							}
							break;
						default:
							break;
					}
				}
				else if (hvObstacleNeighbours > 0)
				{
					switch (currentCell->data->m_horiVertiNeighbours.size())
					{
					case 2:
						if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && (i == 0))
						{
							continue;
						}
						break;
					case 3:
						if (currentCell->data->gridY == (gridMaxY - 1))
						{
							if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && (i == 0))
							{
								continue;
							}
							if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 1))
							{
								continue;
							}
						}
						if (currentCell->data->gridY == (0))
						{
							if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && (i == 1))
							{
								continue;
							}
							if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 0))
							{
								continue;
							}
						}
						if (currentCell->data->gridX == (gridMaxX - 1))
						{
							if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 0))
							{
								continue;
							}
							if (currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 1))
							{
								continue;
							}
						}
						if (currentCell->data->gridX == (0))
						{
							if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 1))
							{
								continue;
							}
							if (currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && (i == 0))
							{
								continue;
							}
						}
						break;
					case 4:
				
						if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && i == 0)
						{
							continue;
						}
						if (currentCell->data->m_horiVertiNeighbours[0]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[3]->m_Obstacle && i == 2)
						{
							continue;
						}
						if (currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[2]->m_Obstacle && i == 3)
						{
							continue;
						}
						if (currentCell->data->m_horiVertiNeighbours[1]->m_Obstacle && currentCell->data->m_horiVertiNeighbours[3]->m_Obstacle && i == 1)
						{
							continue;
						}
						break;
					default:
						break;
					}
				}
				

				Node<Cell>* node = new Node<Cell>();
				node->data = currentCell->data->m_diagonalNeighbours[i];
				int newGDistance = currentCell->gDistance + 14;
				Node<Cell>* parent = currentCell;

				if (enableAStar)
				{
					int horValue = abs(endCell->gridX - node->data->gridX);
					//int horValue = (endCell->gridX - node->data->gridX) * (endCell->gridX - node->data->gridX);
					int vertValue = abs(endCell->gridY - node->data->gridY);
					//int vertValue = (endCell->gridY - node->data->gridY) * (endCell->gridY - node->data->gridY);
					float total = horValue + vertValue;
					//float total = sqrtf(horValue + vertValue);
					//std::cout << "hval: " << total << std::endl;
					node->SetHDistance((total * weight) * 10);
				}
				
				
				if (!CheckIfInList(node, newGDistance, parent))
				{
					node->parent = currentCell;
					node->SetGDistance(currentCell->gDistance + 14);
					node->data->m_InOpenList = true;
					openList.push_back(node);
				}

			}

		}
	}
	
	
	SortOpenList();
}
bool Pathfinder::CheckIfInList(Node<Cell>* n, int gD, Node<Cell>* p)
{
	for (auto const& i : openList) {
		if (i->data == n->data)
		{
			if (gD < i->gDistance)
			{
				
				i->SetGDistance(gD);
				i->parent = p;
				
			}
			return true;
		}
	}
	return false;
}

void Pathfinder::SortOpenList()
{
	openList.sort(LessThanDistance());
}
void Pathfinder::GetNextCell()
{
	currentCell->data->m_Visited = true;
	currentCell->data->m_InOpenList = false;
	closedList.push_back(currentCell);
	openList.remove(currentCell);

	if (openList.size() > 0)
	{
		currentCell = openList.front();
		//std::cout << currentCell->data->gridX << " , " << currentCell->data->gridY <<  " : Next cell"<< std::endl;
	}
	else
	{
		std::cout << "No path found!" << std::endl;
	}
	
}
void Pathfinder::UpdatePathfinding(float timestep)
{
	
	if (update)
	{
		time += timestep;
		
		if (time > 0.01f)
		{
			time = 0.0f;
			if (openList.size() > 0)
			{
				CheckCurrentNeighbours();
				GetNextCell();
				if (currentCell->data == endCell)
				{
					std::cout << "Path found!" << std::endl;
					GeneratePath();
					update = false;
				}
			}
				
		}
	}
	
}

void Pathfinder::GeneratePath()
{
	Cell* temp = new Cell();
	while (temp != startCell)
	{

		temp = currentCell->parent->data;
		temp->m_Path = true;
		currentCell = currentCell->parent;
	}
	Clear();
}

void Pathfinder::Clear()
{
	openList.clear();
	closedList.clear();
}