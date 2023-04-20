#include "ExampleLayer.h"
#include "stb_image/stb_image.h"

using namespace GLCore;
using namespace GLCore::Utils;

struct Vec2
{
	float x, y;
};
struct Vec3
{
	float x, y, z;
};

struct Vec4
{
	float x, y, z, w;
};

struct Vertex
{
	Vec3 Positions[3];
	Vec4 Color[4];
	Vec2 TexCoords[2];
	float TexID;
};

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

static Vertex* CreateQuad(Vertex* target, float x, float y, float textureID)
{
	float size = 1.0f;

	/*
	* c++ 17
	Vertex v0;
	v0.Positions = { x, y, 0.0f };
	v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	v0.TexCoords = { 0.0f, 0.0f };
	v0.TexID = textureID;

	Vertex v1;
	v1.Positions = { x + size, y, 0.0f };
	v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	v1.TexCoords = { 1.0f, 0.0f };
	v1.TexID = textureID;

	Vertex v2;
	v2.Positions = { x + size, y + size, 0.0f };
	v2.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	v2.TexCoords = { 1.0f, 1.0f };
	v2.TexID = textureID;

	Vertex v3;
	v3.Positions = { x, y + size, 0.0f };
	v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	v3.TexCoords = { 0.0f, 1.0f };
	v3.TexID = textureID;
	*/
	
	target->Positions[0] = {x, y, 0.0f};
	target->Positions[1] = { x, y, 0.0f };
	target->Positions[2] = { x, y, 0.0f };
	target->Color[0] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[1] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[2] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[3] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->TexCoords[0] = {0.0f, 0.0f};
	target->TexCoords[1] = {0.0f, 0.0f};
	target->TexID = textureID;
	target++; // increment the buffer pointer

	target->Positions[0] = {x + size, y, 0.0f};
	target->Positions[1] = {x + size, y, 0.0f};
	target->Positions[2] = {x + size, y, 0.0f};
	target->Color[0] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[1] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[2] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[3] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->TexCoords[0] = {1.0f, 0.0f};
	target->TexCoords[1] = {1.0f, 0.0f};
	target->TexID = textureID;
	target++; // increment the buffer pointer

	target->Positions[0] = {x + size, y + size, 0.0f};
	target->Positions[1] = {x + size, y + size, 0.0f};
	target->Positions[2] = {x + size, y + size, 0.0f};
	target->Color[0] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[1] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[2] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[3] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->TexCoords[0] = {1.0f, 1.0f};
	target->TexCoords[1] = {1.0f, 1.0f};
	target->TexID = textureID;
	target++; // increment the buffer pointer

	target->Positions[0] = {x, y + size, 0.0f};
	target->Positions[1] = {x, y + size, 0.0f};
	target->Positions[2] = {x, y + size, 0.0f};
	target->Color[0] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[1] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[2] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->Color[3] = {0.18f, 0.6f, 0.96f, 1.0f};
	target->TexCoords[0] = {0.0f, 1.0f};
	target->TexCoords[1] = {0.0f, 1.0f};
	target->TexID = textureID;
	target++; // increment the buffer pointer

	return target;
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

	const size_t MaxQuadCount = 1000;
	const size_t MaxVertexCount = MaxQuadCount * 4;
	const size_t MaxIndexCount = MaxQuadCount * 6;

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
	/*
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
	*/

	// glCreateVertexArrays need >=4.5
	glGenVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	// glCreateBuffers need >=4.5
	glGenBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, MaxQuadCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Positions));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

	/*
	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0
	};
	
	*/
	/*
	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};
	*/
	uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;
	for (size_t i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

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

	uint32_t indexCount = 0;
	std::array<Vertex, 1000> vertices;
	Vertex* buffer = vertices.data();
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			buffer = CreateQuad(buffer, x, y, (x+y)%2);
			indexCount += 6;
		}
	}
	buffer = CreateQuad(buffer, m_Quadposition[0], m_Quadposition[1], 0.0f);
	indexCount += 6;
	/*
	// Set dynamic vertex buffer
	auto q0 = CreateQuad(m_Quadposition[0], m_Quadposition[1], 0.0f);
	auto q1 = CreateQuad(0.5f, -0.5f, 1.0f);

	Vertex vertices[8];
	memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
	memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
	*/

	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_Shader->GetRendererID());
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
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

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
	ImGui::Begin("Controls");
	//if (ImGui::ColorEdit4("Square Base Color", glm::value_ptr(m_SquareBaseColor)))
		//m_SquareColor = m_SquareBaseColor;
	//ImGui::ColorEdit4("Square Alternate Color", glm::value_ptr(m_SquareAlternateColor));
	ImGui::DragFloat2("Quad Position", m_Quadposition, 0.1f);
	ImGui::End();
}
