#include "ExampleLayer.h"
#include "stb_image/stb_image.h"
#include "Renderer.h"

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

	m_Shader = std::unique_ptr<GLCore::Utils::Shader>(Shader::FromGLSLTextFiles(
		"assets/shaders/test.vert.glsl",
		"assets/shaders/test.frag.glsl"
	));
	glUseProgram(m_Shader->GetRendererID());
	auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
	int samplers[32];
	for (int i = 0; i < 32; i++)
		samplers[i] = i;
	glUniform1iv(loc, 32, samplers);

	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
	Renderer::Init();

	m_ChernoTex = LoadTexture("assets/textures/ChernoLogo.png");
	m_HazelTex = LoadTexture("assets/textures/HazelLogo.png");
}

void ExampleLayer::OnDetach()
{
	Renderer::Shutdown();
}

void ExampleLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);

	if (event.GetEventType() == EventType::WindowResize)
	{
		WindowResizeEvent& e = (WindowResizeEvent&)event;
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
	}
}

static void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ExampleLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(m_Shader->GetRendererID());

	auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();
	SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProj", vp);
	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	Renderer::ResetStats();
	Renderer::BeginBatch();

	for (float y = -10.0f; y < 10.0f; y += 0.25f)
	{
		for (float x = -10.0f; x < 10.0f; x += 0.25f)
		{
			glm::vec4 color = { (x+10.0f)/20.0f, 0.2f, (y+10.0f)/20.0f, 1.0f};
			//Renderer::DrawQuad({ x,y }, { 0.25f, 0.25f }, color);
			Renderer::DrawQuad({ x,y }, { 0.2f, 0.2f }, color);
		}
	}
	
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			GLuint tex = (x + y) % 2 == 0 ? m_ChernoTex : m_HazelTex;
			Renderer::DrawQuad({x,y}, {1.0f, 1.0f}, tex);
		}
	}

	Renderer::DrawQuad({ m_Quadposition[0], m_Quadposition[1]}, {1.0f, 1.0f}, m_ChernoTex);
	Renderer::EndBatch();
	Renderer::Flush();
	
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Controls");
	ImGui::DragFloat2("Quad Position", m_Quadposition, 0.1f);
	ImGui::Text("Quads: %d", Renderer::GetStats().QuadCount);
	ImGui::Text("Draw: %d", Renderer::GetStats().DrawCount);
	ImGui::End();
}
