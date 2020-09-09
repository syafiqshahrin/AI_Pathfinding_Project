#pragma once



template <typename T>
class Node
{
public:


	Node()
	{
		gDistance = -1;
		hDistance = 0;
	}
	~Node()
	{

	}
	Node(T* d) : data(d)
	{
		gDistance = -1;
		hDistance = 0;
	}

	T* data;
	Node* parent;
	int gDistance;
	float hDistance;
	float fDistance;
	void SetGDistance(int gDist)
	{
		gDistance = gDist;
		fDistance = gDistance + hDistance;
	}
	void SetHDistance(float hDist)
	{
		hDistance = hDist;
		fDistance = gDistance + hDistance;
	}
};

struct LessThanDistance
{
	bool operator()(const Node<Cell>* a, const Node<Cell>* b) const;
};