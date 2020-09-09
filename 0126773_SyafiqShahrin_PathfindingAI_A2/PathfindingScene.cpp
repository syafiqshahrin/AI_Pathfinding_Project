#include <glad/glad.h>
#include <glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
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
#include "PathfindingScene.h"

PathfindingScene::PathfindingScene(int* wH, int* wW, float* dTime)
{
	deltatime = dTime;
	Init();
}

PathfindingScene::~PathfindingScene()
{

}
void PathfindingScene::Init()
{

	LoadTextures();
	LoadShaders();
	CreateMeshes();
	CreateEntities();
	Start = false;
	Diagonal = false;
	aStar = false;
	disableCrossCorner = false;
	toggleUI = true;
	weight = 1.0f;
}
void PathfindingScene::LoadTextures()
{
	glGenTextures(12, textTextureID);
	Utility::loadTexturePNG("../Media/astartext5.png", textTextureID[0]);
	Utility::loadTexturePNG("../Media/diagonaltext1.png", textTextureID[1]);
	Utility::loadTexturePNG("../Media/crosscornertext1.png", textTextureID[2]);
	Utility::loadTexturePNG("../Media/startnodetext1.png", textTextureID[3]);
	Utility::loadTexturePNG("../Media/endnodetext1.png", textTextureID[4]);
	Utility::loadTexturePNG("../Media/starttext4.png", textTextureID[5]);
	Utility::loadTexturePNG("../Media/cleargridtext1.png", textTextureID[6]);
	Utility::loadTexturePNG("../Media/clearpathtext1.png", textTextureID[7]);
	Utility::loadTexturePNG("../Media/increasebutton.png", textTextureID[8]);
	Utility::loadTexturePNG("../Media/decreasebutton.png", textTextureID[9]);
	Utility::loadTexturePNG("../Media/weighttext3.png", textTextureID[10]);
	Utility::loadTexturePNG("../Media/toggletext2.png", textTextureID[11]);

	glGenTextures(1, &textAtlasTexture);
	Utility::loadTexturePNG("../Media/NumberAtlas2.png", textAtlasTexture);
}
void PathfindingScene::LoadShaders()
{
	cellShader = Shader("../Shader/CellVert.vert", "../Shader/CellFrag.frag");
	basicColShader = Shader("../Shader/basicColVert.vert", "../Shader/basicColFrag.frag");
	basicButtonShader = Shader("../Shader/basicButtonVert.vert", "../Shader/basicButtonFrag.frag");
	basicToggleButtonShader = Shader("../Shader/uiToggleVert.vert", "../Shader/uiToggleFrag.frag");
	uiTextShader = Shader("../Shader/uiTextVert.vert", "../Shader/uiTextFrag.frag");
}
void PathfindingScene::CreateMeshes()
{
	meshQuad = new MeshObject(true,0);
	meshQuad2 = new MeshObject();
}
void PathfindingScene::CreateEntities()
{
	cellEntity = Entity(meshQuad, &cellShader);
	grid = Grid(40,2,2, &cellEntity);
	buttonPanel = Entity(meshQuad2, &basicColShader);
	buttonPanel.ScaleEntity(Vector3(0.3f / (1600.0f/1000.0f), 0.35f, 1.0f));
	buttonPanel.TranslateEntity(Vector3(- 0.8f , 0.55f, 0.0f));
	
	
	toggleText = Entity(meshQuad2, &basicButtonShader);
	toggleText.ScaleEntity(Vector3(0.3f / (1600.0f / 1000.0f), 0.03f, 1.0f));
	toggleText.TranslateEntity(Vector3(-0.8f, 0.95f, 0.0f));
	

	weightText = Entity(meshQuad2, &basicButtonShader, &buttonPanel);
	weightText.ScaleEntity(Vector3(0.4f, 0.1f, 1.0f));
	weightText.TranslateEntity(Vector3(0.4f, -0.45f, 0.0f));

	aStarButton = Button<PathfindingScene>(meshQuad2, &basicToggleButtonShader, &buttonPanel);
	aStarButton.ScaleEntity(Vector3(0.8f, 0.1f, 1.0f));
	aStarButton.TranslateEntity(Vector3(0.0f, 0.8f, 0.0f));
	aStarButton.BindFunction(this, &PathfindingScene::SetAStar);

	diagonalButton = Button<PathfindingScene>(meshQuad2, &basicToggleButtonShader, &buttonPanel);
	diagonalButton.ScaleEntity(Vector3(0.8f, 0.1f, 1.0f));
	diagonalButton.TranslateEntity(Vector3(0.0f, 0.55f, 0.0f));
	diagonalButton.BindFunction(this, &PathfindingScene::SetDiagonal);

	crossCornerButton = Button<PathfindingScene>(meshQuad2, &basicToggleButtonShader, &buttonPanel);
	crossCornerButton.ScaleEntity(Vector3(0.8f, 0.1f, 1.0f));
	crossCornerButton.TranslateEntity(Vector3(0.0f, 0.3f, 0.0f));
	crossCornerButton.BindFunction(this, &PathfindingScene::DisableCrossCorner);

	toggleStartCellButton = Button<Grid>(meshQuad2, &basicToggleButtonShader, &buttonPanel);
	toggleStartCellButton.ScaleEntity(Vector3(0.8f, 0.1f, 1.0f));
	toggleStartCellButton.TranslateEntity(Vector3(0.0f, 0.05f, 0.0f));
	toggleStartCellButton.BindFunction(&grid, &Grid::SetStartToggle);

	toggleEndCellButton = Button<Grid>(meshQuad2, &basicToggleButtonShader, &buttonPanel);
	toggleEndCellButton.ScaleEntity(Vector3(0.8f, 0.1f, 1.0f));
	toggleEndCellButton.TranslateEntity(Vector3(0.0f, -0.2f, 0.0f));
	toggleEndCellButton.BindFunction(&grid, &Grid::SetEndToggle);

	startButton = Button<PathfindingScene>(meshQuad2, &basicButtonShader);
	startButton.ScaleEntity(Vector3(0.08f / (1600.0f / 1000.0f), 0.05f, 1.0f));
	startButton.TranslateEntity(Vector3(-0.935f, 0.1f, 0.0f));
	startButton.BindFunction(this, &PathfindingScene::StartSearch);

	resetPathButton = Button<PathfindingScene>(meshQuad2, &basicButtonShader);
	resetPathButton.ScaleEntity(Vector3(0.08f / (1600.0f / 1000.0f), 0.05f, 1.0f));
	resetPathButton.TranslateEntity(Vector3(-0.8f, 0.1f, 0.0f));
	resetPathButton.BindFunction(this, &PathfindingScene::ResetPathfinding);

	clearGridButton = Button<Grid>(meshQuad2, &basicButtonShader);
	clearGridButton.ScaleEntity(Vector3(0.08f / (1600.0f / 1000.0f), 0.05f, 1.0f));
	clearGridButton.TranslateEntity(Vector3(-0.675f, 0.1f, 0.0f));
	clearGridButton.BindFunction(&grid, &Grid::ClearGrid);

	weightValueText = Entity(meshQuad2, &uiTextShader, &buttonPanel);
	weightValueText.ScaleEntity(Vector3(0.2f, 0.1f, 1.0f));
	weightValueText.TranslateEntity(Vector3(-0.45f, -0.45f, 0.0f));
	
	decreaseWeightButton = Button<PathfindingScene>(meshQuad2, &basicButtonShader, &buttonPanel);
	decreaseWeightButton.ScaleEntity(Vector3(0.05f, 0.1f, 1.0f));
	decreaseWeightButton.TranslateEntity(Vector3(-0.75f, -0.45f, 0.0f));
	decreaseWeightButton.BindFunction(this, &PathfindingScene::DecreaseWeight);

	increaseWeightButton = Button<PathfindingScene>(meshQuad2, &basicButtonShader, &buttonPanel);
	increaseWeightButton.ScaleEntity(Vector3(0.05f, 0.1f, 1.0f));
	increaseWeightButton.TranslateEntity(Vector3(-0.15f, -0.45f, 0.0f));
	increaseWeightButton.BindFunction(this, &PathfindingScene::IncreaseWeight);
	
}
void PathfindingScene::UpdateInput(InputData& input)
{
	//float st = glfwGetTime();
	toggleUI = input.uiKeyToggle;
	if (toggleUI)
	{
		startButton.CheckInput(input);
		aStarButton.CheckInput(input);
		diagonalButton.CheckInput(input);
		crossCornerButton.CheckInput(input);
		resetPathButton.CheckInput(input);
		clearGridButton.CheckInput(input);
		toggleStartCellButton.CheckInput(input);
		toggleEndCellButton.CheckInput(input);
		increaseWeightButton.CheckInput(input);
		decreaseWeightButton.CheckInput(input);
	}
	if (Start)
	{
		Start = false;
		ResetPathfinding();
		pathFinder.StartSearch(grid.startNode, grid.endNode, Diagonal,aStar, disableCrossCorner, grid.gridDimensionX, grid.gridDimensionY, weight);
		//input.play = false;
	}
	else
	{
		grid.CheckInput(input);
	}
	//st = glfwGetTime() - st;
	//std::cout << "Check Grid Time: " << st * 1000 << std::endl;
}
void PathfindingScene::UpdateScene()
{
	pathFinder.UpdatePathfinding(*deltatime);
}
void PathfindingScene::DrawScene()
{
	float st = glfwGetTime();
	grid.DrawGrid();
	if (toggleUI)
	{
		buttonPanel.DrawEntity(0);
		toggleText.DrawEntity(textTextureID[11]);
	
		aStarButton.m_meshShader->use();
		aStarButton.m_meshShader->setBool("isToggled", aStar);
		aStarButton.m_meshShader->setFloat("Time", glfwGetTime());
		aStarButton.DrawEntity(textTextureID[0]);
		startButton.DrawEntity(textTextureID[5]);
		clearGridButton.DrawEntity(textTextureID[6]);
		resetPathButton.DrawEntity(textTextureID[7]);
		diagonalButton.m_meshShader->use();
		diagonalButton.m_meshShader->setBool("isToggled", Diagonal);
		diagonalButton.m_meshShader->setFloat("Time", glfwGetTime());
		diagonalButton.DrawEntity(textTextureID[1]);
		crossCornerButton.m_meshShader->use();
		crossCornerButton.m_meshShader->setBool("isToggled", disableCrossCorner);
		crossCornerButton.m_meshShader->setFloat("Time", glfwGetTime());
		crossCornerButton.DrawEntity(textTextureID[2]);
		toggleStartCellButton.m_meshShader->use();
		toggleStartCellButton.m_meshShader->setBool("isToggled", grid.startToggle);
		toggleStartCellButton.m_meshShader->setFloat("Time", glfwGetTime());
		toggleStartCellButton.DrawEntity(textTextureID[3]);
		toggleEndCellButton.m_meshShader->use();
		toggleEndCellButton.m_meshShader->setBool("isToggled", grid.endToggle);
		toggleEndCellButton.m_meshShader->setFloat("Time", glfwGetTime());
		toggleEndCellButton.DrawEntity(textTextureID[4]);
		weightText.DrawEntity(textTextureID[10]);
		weightValueText.m_meshShader->use();
		float intVal;
		float fractVal;
		fractVal = modff(weight, &intVal) * 10.0f;
		if (fractVal > 9.1f)
		{
			fractVal = 0.0f;
			intVal += 1.0f;
		}
		weightValueText.m_meshShader->setFloat("fractVal", fractVal);
		weightValueText.m_meshShader->setFloat("intVal", intVal);
		weightValueText.DrawEntity(textAtlasTexture);
		decreaseWeightButton.DrawEntity(textTextureID[9]);
		increaseWeightButton.DrawEntity(textTextureID[8]);
	}
	
	st = glfwGetTime() - st;
	//std::cout << "Draw Grid Time: " << st * 1000 << std::endl;
}

void PathfindingScene::ResetPathfinding()
{
	grid.ResetGrid();
	pathFinder.Clear();
}


void PathfindingScene::StartSearch()
{
	Start =  true;
}

void PathfindingScene::SetDiagonal()
{
	std::cout << "diagonal" << std::endl;
	Diagonal = !Diagonal;
}

void PathfindingScene::SetAStar()
{
	std::cout << "astar" << std::endl;
	aStar = !aStar;
}

void PathfindingScene::DisableCrossCorner()
{
	std::cout << "corner" << std::endl;
	disableCrossCorner = !disableCrossCorner;
}
void PathfindingScene::IncreaseWeight()
{
	weight += 0.1f;
	if (weight > 9.0f)
	{
		weight = 9.0f;
	}
}

void PathfindingScene::DecreaseWeight()
{
	weight -= 0.1f;
	if (weight < 0.0f)
	{
		weight = 0.0f;
	}
}