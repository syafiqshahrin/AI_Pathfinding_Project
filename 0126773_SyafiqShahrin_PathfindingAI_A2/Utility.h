#pragma once
struct Vector3;


class Utility
{
public:
	static void loadTexturePNG(const char* path, GLuint textureID);
};

struct Transform
{
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;

	Transform();
	Transform(Vector3 position, Vector3 rotation, Vector3 scale);
};

struct InputData
{
	Vector2 mousePosition;

	bool isLeftClicked;
	bool isRightClicked;
	bool uiKeyToggle;
	bool mouseInWindow;
	//temporary (to be converted to buttons)
	bool setStartToggle;
	bool setGoalToggle;
	bool clearGrid;
	bool play;
	bool setDiagonalToggle;
	//
	InputData();
};