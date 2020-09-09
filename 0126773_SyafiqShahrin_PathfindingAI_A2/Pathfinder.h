#pragma once
//Includes functions and data structures to be used for pathfinding
//Includes booleans for changing how the pathfinding works (e.g. diagonals,cross corners
//type of algorithms,etc...)


class Pathfinder
{
public:
	Pathfinder();
	~Pathfinder();

	void StartSearch(Cell* s, Cell* e, bool d, bool a, bool c, int gX, int gY, float w);
	void UpdatePathfinding(float timestep);
	void Clear();
private:
	Cell* startCell;
	Cell* endCell;
	Node<Cell>* currentCell;
	std::list<Node<Cell>*> openList;
	std::list<Node<Cell>*> closedList;
	


	void CheckCurrentNeighbours();
	void GetNextCell();
	void SortOpenList();
	bool CheckIfInList(Node<Cell>* n, int gD, Node<Cell>* p);
	void GeneratePath();
	

	bool enableDiagonal;
	bool disableCrossCorners;
	bool update;
	bool enableAStar;
	float time;
	float weight;

	int gridMaxX;
	int gridMaxY;


	//bool LessThanDistance(const Node<Cell>* a, const Node<Cell>* b);
};
