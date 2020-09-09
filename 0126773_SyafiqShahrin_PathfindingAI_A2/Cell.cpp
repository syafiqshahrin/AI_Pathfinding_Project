#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
#include "Vector.h"
#include "Utility.h"
#include "Cell.h"


Cell::Cell()
{
	m_Obstacle = false;
	m_Start = false;
	m_End = false;
	m_Visited = false;
	m_Path = false;
	m_InOpenList = false;
	gridX = 0;
	gridY = 0;
}
Cell::~Cell()
{

}

bool Cell::CheckCellInput(InputData& input, bool s, bool e)
{
	if (input.mousePosition.x > (m_CellTransform.Position.x - (cellLengthX * 0.5)) && input.mousePosition.x < (m_CellTransform.Position.x + (cellLengthX * 0.5)))
	{
		if (input.mousePosition.y > (m_CellTransform.Position.y - (cellLengthY * 0.5)) && input.mousePosition.y < (m_CellTransform.Position.y + (cellLengthY * 0.5)))
		{
			if (input.isLeftClicked)
			{
				if (e)
				{
					m_End = !m_End;
					m_Obstacle = false;
					//std::cout << "goal is" << m_End <<std::endl;
					input.isLeftClicked = false;
				}
				else if (s)
				{
					m_Start = !m_Start;
					m_Obstacle = false;
					//std::cout << "start is" << m_Start << std::endl;
					input.isLeftClicked = false;
				}
				else
				{
					if (!m_Start && !m_End)
					{
						m_Obstacle = true;
						//std::cout << "obstacle true" << std::endl;
					}
		
				}
				return true;
				
			}
			if (input.isRightClicked)
			{
				m_Obstacle = false;
				//std::cout << "obstacle false;" << std::endl;
				return true;
			}
		}
		
	}
	return false;
}