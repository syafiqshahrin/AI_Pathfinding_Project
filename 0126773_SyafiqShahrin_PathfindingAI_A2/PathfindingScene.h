#pragma once
#include "Scene.h"

class Shader;
class MeshObject;
class Entity;

class UIHandler;

class PathfindingScene : Scene
{
public:
	PathfindingScene(int* wH, int* wW, float* dTime);
	~PathfindingScene();
	void DrawScene();
	void UpdateInput(InputData& input);
	void UpdateScene();
	void ResetPathfinding();
	void SetDiagonal();
	void SetAStar();
	void DisableCrossCorner();
private:
	void Init();
	void LoadTextures();
	void LoadShaders();
	void CreateMeshes();
	void CreateEntities();

	//Textrures
	GLuint textTextureID[12];
	GLuint textAtlasTexture;


	//Meshes
	MeshObject* meshQuad;
	MeshObject* meshQuad2;

	//Entities
	Entity cellEntity;
	Entity buttonPanel;
	Grid grid;
	
	Button<PathfindingScene> startButton;
	Button<PathfindingScene> aStarButton;
	Button<PathfindingScene> diagonalButton;
	Button<PathfindingScene> crossCornerButton;

	Button<PathfindingScene> resetPathButton;
	Button<Grid> clearGridButton;
	Button<Grid> toggleStartCellButton;
	Button<Grid> toggleEndCellButton;

	Button<PathfindingScene> increaseWeightButton;
	Button<PathfindingScene> decreaseWeightButton;
	Entity weightValueText;
	Entity weightText;
	Entity toggleText;


	//Shaders
	Shader cellShader;
	Shader basicColShader;
	Shader basicButtonShader;
	Shader basicToggleButtonShader;
	Shader uiTextShader;

	///Pathfinding stuff
	Pathfinder pathFinder;
	float* deltatime;
	void StartSearch();
	void IncreaseWeight();
	void DecreaseWeight();
	bool Start; 
	bool Diagonal;
	bool aStar;
	bool disableCrossCorner;
	bool toggleUI;
	float weight;

};