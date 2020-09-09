#pragma once
//Cell class


class Cell
{
public:
	Cell();
	~Cell();


	//Cell Data
	bool m_Obstacle;
	bool m_Start;
	bool m_End;
	bool m_Visited;
	bool m_Path;
	bool m_InOpenList;
	Transform m_CellTransform;
	float cellLengthX;
	float cellLengthY;
	//
	int gridX;
	int gridY;
	//Cell Neighbour list
	std::vector<Cell*> m_diagonalNeighbours;
	std::vector<Cell*> m_horiVertiNeighbours;

	//
	bool CheckCellInput(InputData& input, bool s, bool e);
	
	
};