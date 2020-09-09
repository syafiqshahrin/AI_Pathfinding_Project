#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue> 
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




bool LessThanDistance::operator()(const Node<Cell>* a, const Node<Cell>* b) const
{
	return a->fDistance < b->fDistance;
}