#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include "Matrix.h"
#include "Vector.h"
#include "Utility.h"
#include "Shader.h"
#include "MeshObject.h"
#include "Entity.h"

Entity::Entity()
{
	m_EntityTransform = Transform();
	m_EntityMesh = nullptr;
	useIdentity = false;
	Parent = nullptr;
	gOthorMatrix = Matrix4::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 30.0f);
	gViewMatrix = Matrix4::translate(Vector3(0.0f, 0.0f, -1.0f));
}

Entity::Entity(MeshObject* mesh, Shader* shader)
{
	m_EntityTransform = Transform();
	m_EntityMesh = mesh;
	m_meshShader = shader;
	useIdentity = false;
	Parent = nullptr;
	gOthorMatrix = Matrix4::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 30.0f);
	gViewMatrix = Matrix4::translate(Vector3(0.0f, 0.0f, -1.0f));

}
Entity::Entity(MeshObject* mesh, Shader* shader, Entity* parent)
{
	Parent = parent;
	m_EntityTransform = Transform();
	m_EntityMesh = mesh;
	m_meshShader = shader;
	useIdentity = false;
	gOthorMatrix = Matrix4::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 30.0f);
	gViewMatrix = Matrix4::translate(Vector3(0.0f, 0.0f, -1.0f));

}

void Entity::TranslateEntity(Vector3 newPosition)
{
	m_EntityTransform.Position = newPosition;

}
void Entity::ScaleEntity(Vector3 newScale)
{
	m_EntityTransform.Scale = newScale;

}
void Entity::RotateEntity()
{

}
void Entity::UseIdentityMatrix(bool identity)
{
	useIdentity = identity;
}
void Entity::DrawEntity(GLuint texture)
{

	m_meshShader->use();

	modelMatrix =
		Matrix4::translate(m_EntityTransform.Position) *
		Matrix4::scale(m_EntityTransform.Scale) *
		Matrix4::rotate(0.0f, m_EntityTransform.Rotation);
	if (Parent == nullptr)
	{
		OrthoModelMatrix = gOthorMatrix * gViewMatrix * modelMatrix;
	}
	else
	{
		OrthoModelMatrix = Parent->OrthoModelMatrix * modelMatrix;
	}
	m_meshShader->setMat4("uModelMatrix", modelMatrix.data);
	m_meshShader->setMat4("uOrthoModelMatrix", OrthoModelMatrix.data);
	if (useIdentity)
	{
		m_meshShader->setMat4("uOrthoModelMatrix", Matrix4::identity().data);
	}

	m_meshShader->setInt("Main_Tex", 0);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_EntityMesh->DrawMesh();

}
void Entity::DrawEntityInstance(GLuint texture)
{
	m_meshShader->use();
	m_meshShader->setInt("Main_Tex", 0);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_EntityMesh->DrawMesh();
}
void Entity::DrawEntity(GLuint texture, GLuint texture2)
{
	m_meshShader->use();

	modelMatrix =
		Matrix4::translate(m_EntityTransform.Position) *
		Matrix4::scale(m_EntityTransform.Scale) *
		Matrix4::rotate(0.0f, m_EntityTransform.Rotation);
	if (Parent == nullptr)
	{
		OrthoModelMatrix = gOthorMatrix * gViewMatrix * modelMatrix;
	}
	else
	{
		OrthoModelMatrix = Parent->OrthoModelMatrix * modelMatrix;
	}
	m_meshShader->setMat4("uModelMatrix", modelMatrix.data);
	m_meshShader->setMat4("uOrthoModelMatrix", OrthoModelMatrix.data);
	if (useIdentity)
	{
		m_meshShader->setMat4("uOrthoModelMatrix", Matrix4::identity().data);
	}

	m_meshShader->setInt("Main_Tex", 0);
	m_meshShader->setInt("Main_Tex2", 1);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	m_EntityMesh->DrawMesh();

}
void Entity::DrawEntity(GLuint texture, GLuint texture2, GLuint texture3)
{

	m_meshShader->use();

	modelMatrix =
		Matrix4::translate(m_EntityTransform.Position) *
		Matrix4::scale(m_EntityTransform.Scale) *
		Matrix4::rotate(0.0f, m_EntityTransform.Rotation);
	if (Parent == nullptr)
	{
		OrthoModelMatrix = gOthorMatrix * gViewMatrix * modelMatrix;
	}
	else
	{
		OrthoModelMatrix = Parent->OrthoModelMatrix * modelMatrix;
	}

	m_meshShader->setMat4("uModelMatrix", modelMatrix.data);
	m_meshShader->setMat4("uOrthoModelMatrix", OrthoModelMatrix.data);
	if (useIdentity)
	{
		m_meshShader->setMat4("uOrthoModelMatrix", Matrix4::identity().data);
	}

	m_meshShader->setInt("Main_Tex", 0);
	m_meshShader->setInt("Main_Tex2", 1);
	m_meshShader->setInt("Main_Tex3", 2);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, texture3);
	m_EntityMesh->DrawMesh();

}
void Entity::SetShader(Shader* shader)
{
	m_meshShader = shader;
}
