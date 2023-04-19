#include "ExampleLayer.h"
#include "stb_image/stb_image.h"

using namespace GLCore;
using namespace GLCore::Utils;

ExampleLayer::ExampleLayer()
	: m_CameraController(16.0f / 9.0f)
{

}

ExampleLayer::~ExampleLayer()
{

}

static GLuint LoadTexture(const std::string& path)
{
	int w, h, bits;

	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	stbi_image_free(pixels);
	return textureID;
}

void ExampleLayer::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test.vert.glsl",
		"assets/shaders/test.frag.glsl"
	);
	glUseProgram(m_Shader->GetRendererID());
	auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
	int samplers[2] = { 0, 1 };
	glUniform1iv(loc, 2, samplers);
	/*
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	
	*/
	// 在vertex buffer中，除了 position 和 color，再存入一个 uv（texcoord，即 texture coordinate）
	// 第10列 : 采样 texture
	float vertices[] = {
		-1.5f, -0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,
		-1.5f,  0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,

		 0.5f, -0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,
		 1.5f, -0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,
		 1.5f,  0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f
	};

	// glCreateVertexArrays need >=4.5
	glGenVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	// glCreateBuffers need >=4.5
	glGenBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)12);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)28);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (const void*)36);

	/*
	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0
	};
	
	*/
	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};
	// glCreateBuffers need >=4.5
	glGenBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_ChernoTex = LoadTexture("assets/textures/ChernoLogo.png");
	m_HazelTex = LoadTexture("assets/textures/HazelLogo.png");
}

void ExampleLayer::OnDetach()
{
	glDeleteVertexArrays(1, &m_QuadVA);
	glDeleteBuffers(1, &m_QuadVB);
	glDeleteBuffers(1, &m_QuadIB);
}

void ExampleLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);

	/*
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& e)
		{
			m_SquareColor = m_SquareAlternateColor;
			return false;
		});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e)
		{
			m_SquareColor = m_SquareBaseColor;
			return false;
		});
	*/
}

static void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ExampleLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUseProgram(m_Shader->GetRendererID());
	//glBindTextureUnit(0, m_ChernoTex);
	//glBindTextureUnit(1, m_HazelTex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ChernoTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_HazelTex);

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
	SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProj", vp);
	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	glBindVertexArray(m_QuadVA);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

	/*
	// glDrawElements twice
	glBindVertexArray(m_QuadVA);
	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	*/
}

void ExampleLayer::OnImGuiRender()
{
	/*
	ImGui::Begin("Controls");
	if (ImGui::ColorEdit4("Square Base Color", glm::value_ptr(m_SquareBaseColor)))
		m_SquareColor = m_SquareBaseColor;
	ImGui::ColorEdit4("Square Alternate Color", glm::value_ptr(m_SquareAlternateColor));
	ImGui::End();
	*/
}
