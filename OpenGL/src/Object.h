#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ObjAttrib.h"

class Object
{
public:
	Object(const std::string& name = "", Transform& t = (Transform&)Transform());
	~Object();

	virtual void Update() {};
	virtual void Render() {};

public:
	std::string m_Name;
	std::unique_ptr<Transform> m_Transf;
};

class SolidObject : public Object
{
public:
	SolidObject(
		const std::string& name, Transform& transf, Mesh& mesh, std::shared_ptr<Material> mat);
	~SolidObject();

	void Render(Renderer renderer, glm::mat4 proj, glm::mat4 view);
	void Update();

public:
	std::unique_ptr<Mesh> m_Mesh;
	std::shared_ptr<Material> m_Material;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertBuff;
	std::unique_ptr<IndexBuffer> m_IndexBuff;
};

//class DebugObject : public Object
//{
//public:
//	DebugObject(std::string& name, std::string& filepath, std::string& shaderpath);
//
//	void Render(Renderer renderer, glm::mat4 proj, glm::mat4 view);
//	void Update();
//
//public:
//	std::string m_Name;
//
//	std::vector<glm::vec3> m_Vertices;
//	std::vector<unsigned int> m_Indices;
//
//	glm::vec3 m_Position;
//	glm::vec3 m_Scale;
//	glm::mat4 m_Transform;
//
//	std::unique_ptr<VertexArray> m_VAO;
//	std::unique_ptr<VertexBuffer> m_VertBuff;
//	std::unique_ptr<IndexBuffer> m_IndexBuff;
//	std::unique_ptr<Shader> m_Shader;
//};