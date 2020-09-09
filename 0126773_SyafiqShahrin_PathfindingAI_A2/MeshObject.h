#pragma once

class Shader;
struct Vector3;
struct Transform;



class MeshObject{
public:

private:
	//store mesh data
	
	GLfloat m_meshVertices[20] = {
		-1.0f, -1.0f, 0.0f, //position
		0.0f, 0.0f,  //Tex Coord
		-1.0f, 1.0f, 0.0f, //position
		0.0f, 1.0f,  //Tex Coord
		1.0f, -1.0f, 0.0f, //position
		1.0f, 0.0f, //Tex Coord
		1.0f, 1.0f, 0.0f, //position
		1.0f, 1.0f //Tex Coord
	};
	
	GLint m_vertexIndices[6] =
	{
		0, 1, 2,
		3, 2, 1
	};

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint MBO;
	GLuint S3BO;
	GLuint S2BO;
	int size;

	void Init();

public:
	MeshObject();
	MeshObject(bool i, int s);
	~MeshObject();
	//Draw mesh
	void DrawMesh();
	void SetInstanceSize(int s, int sY);

	void InitInstance(std::vector<float> matVec);
	void SendVec4(std::vector<float> vec4D);
	void SendVec2(std::vector<float> vec2D);
	bool instanced;
	//Change Shader

};

