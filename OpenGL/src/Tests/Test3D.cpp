#include "Test3D.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

namespace test {

	Test3D::Test3D()
		: m_W(960), m_H(540),
		m_Eye(0.0f, 0.0f, 500), m_Up(0.0f, 1.0f, 0.0f), m_Amount(5), m_Center(0.0f, 0.0f, 0.0f),
		m_LightInt(0.0f)
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_Proj = glm::perspective(
			90.0f * glm::pi<float>() / 180.0f, (float)m_W / (float)m_H, 0.1f, 1500.0f
		);
		m_View = glm::lookAt(m_Eye, m_Center, m_Up);

		m_Teapot = std::make_unique<Object>(
			"teapot", "res/models/teapot.obj", "res/shaders/Test3D.shader"
		);
		m_Teapot->m_Scale = glm::vec3(50, 50, 50);
	}

	Test3D::~Test3D()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void Test3D::OnUpdate(float deltaTime)
	{
		m_Teapot->m_Shader->Bind();
		m_Teapot->m_Shader->SetUniform4fv("u_Model", &m_TeapotCol[0]);

		m_Teapot->m_Shader->SetUniform1f("u_LightInt", m_LightInt);

		glm::vec4 light(-1.0, -1.0, -1.0, 0.0f);
		light = m_View * light;
		m_Teapot->m_Shader->SetUniform3fv("u_LightDir", &light[0]);

		m_Teapot->m_Shader->SetUniform4fv("u_Light", &m_LightCol[0]);
		m_Teapot->m_Shader->SetUniform4fv("u_Ambient", &m_AmbientCol[0]);

		m_View = glm::lookAt(m_Eye, m_Center, m_Up);

		m_Teapot->Update();
	}

	void Test3D::OnRender()
	{
		GLCall(glClearColor(0, 0, 1, 0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		m_Teapot->Render(renderer, m_Proj, m_View);
	}

	void Test3D::OnImGuiRender()
	{
		glm::vec4 camera = m_View * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		ImGui::SliderFloat3("Model", &m_Teapot->m_Position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("Scale", &m_Teapot->m_Scale.x, 0.0f, 100.0f);
		ImGui::ColorEdit4("Model Color", m_TeapotCol);

		ImGui::SliderFloat("Light Intensity", &m_LightInt, 0.0f, 10.0f);
		ImGui::ColorEdit4("Light Color", m_LightCol);
		ImGui::ColorEdit4("Ambient Color", m_AmbientCol);

		ImGui::Text("Camera Position (%f, %f, %f)", -camera.x, -camera.y, -camera.z);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// This function gets called when the window size gets changed
	void Test3D::WindowSizeCallback(GLFWwindow* window, int width, int height) {
		m_W = width;
		m_H = height;
		glViewport(0, 0, m_W, m_H);
		// Set the projection matrix based on the current perspective, then pass
		// the calculated matrix onto the shader program. Remember that the perspective
		// field of view should be in radians!
		m_Proj = glm::perspective(
			90.0f * glm::pi<float>() / 180.0f, (float)m_W / (float)m_H, 0.1f, 1500.0f
		);
	}

	void Test3D::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			m_Lmb = action;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			m_Rmb = action;
			break;
		}
	}

	void Test3D::MousePosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		float mousedx = m_Mousex - (float)xpos;
		float mousedy = m_Mousey - (float)ypos;
		float speed = 3;
		float angle;
		if (m_Lmb)
		{
			angle = atan(mousedx / glm::length(m_Eye)) * speed;
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
			m_Eye = rot * glm::vec4(m_Eye, 1.0f);
			m_Up = rot * glm::vec4(m_Up, 1.0f);

			angle = -atan(mousedy / glm::length(m_Eye)) * speed;
			m_Piv = glm::normalize(glm::cross(m_Eye, m_Up));
			m_Eye = glm::rotate(glm::mat4(1.0f), angle, m_Piv) * glm::vec4(m_Eye, 1.0f);
			m_Up = glm::cross(m_Piv, glm::normalize(m_Eye));
		}
		m_Mousex = (float)xpos;
		m_Mousey = (float)ypos;
	}

	void Test3D::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		float amt = glm::sqrt(glm::length(m_Eye)) * 4;
		glm::vec3 en = glm::normalize(m_Eye);

		m_Eye += en * amt * (float)yoffset;
	}
}

// Set up initial position for eye,up and amount
// As well as booleans 

// The lighting is enabled using the same framework as in mytest 3 
// Except that we use two point lights
// For now, lights and materials are set in display.  Will move to init 
// later, per update lights

// need these uniforms
//islight = glGetUniformLocation(m_Shader, "islight");
//light0posn = glGetUniformLocation(m_Shader, "light0posn");
//light0color = glGetUniformLocation(m_Shader, "light0color");
//light1posn = glGetUniformLocation(m_Shader, "light1posn");
//light1color = glGetUniformLocation(m_Shader, "light1color");
//ambient = glGetUniformLocation(m_Shader, "ambient");
//diffuse = glGetUniformLocation(m_Shader, "diffuse");
//specular = glGetUniformLocation(m_Shader, "specular");
//shininess = glGetUniformLocation(m_Shader, "shininess");
//color = glGetUniformLocation(m_Shader, "color");

// Get the uniform locations of the transformation matrices
//projectionPosition = glGetUniformLocation(m_Shader, "projection");
//modelviewPos = glGetUniformLocation(m_Shader, "modelview");

// Set Light and Material properties for the teapot
// Lights are transformed by current modelview matrix. 
// The shader can't do this globally. 
// So we need to do so manually.  

// need to implement in Test3D()
//transformvec(light_position, light0);
//transformvec(light_position1, light1);
//glUniform4fv(light0posn, 1, &light0[0]);
//glUniform4fv(light0color, 1, &light_specular[0]);
//glUniform4fv(light1posn, 1, &light1[0]);
//glUniform4fv(light1color, 1, &light_specular1[0]);

//glUniform4fv(ambient, 1, &small[0]);
//glUniform4fv(diffuse, 1, &small[0]);
//glUniform4fv(specular, 1, &one[0]);
//glUniform1f(shininess, high);
//glUniform1i(islight, true);