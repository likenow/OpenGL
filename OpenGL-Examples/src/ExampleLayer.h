#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class ExampleLayer : public GLCore::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	std::unique_ptr<GLCore::Utils::Shader> m_Shader;
	GLCore::Utils::OrthographicCameraController m_CameraController;

	GLuint m_ChernoTex, m_HazelTex;

	float m_Quadposition[2] = { -1.5f, -0.5f };
};