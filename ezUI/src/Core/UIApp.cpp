#include "Core/UIApp.h"
#include "Core/Base.h"
#include "Graphics/Renderer2D.h"
#include "Utility/Profiling.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ez {
	UIApp* UIApp::Create(const UIAppSpecification& settings) {
		return new UIApp(settings);
	}

	void UIApp::OnFramebufferSizeChanged(GLFWwindow* window, int width, int height) {
		if (width > 0 && height > 0) {
			Renderer2D::SetRenderSize(width, height);
		}
	}

	UIApp::UIApp(const UIAppSpecification& settings) {
		ez::Logger::Init();
		
		EZ_PROFILE_BEGIN_SESSION("Startup");

		EZ_CORE_LOG("ezUI Version v", ez::VERSION_MAJOR, ".", ez::VERSION_MINOR, ".", ez::VERSION_PATCH);

		m_Spec = settings;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef EZ_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		#endif

		m_Window = glfwCreateWindow(m_Spec.Width, m_Spec.Height, m_Spec.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetFramebufferSizeCallback(m_Window, UIApp::OnFramebufferSizeChanged);
		glfwSetWindowUserPointer(m_Window, this);

		Renderer2D::Init(m_Spec.Width, m_Spec.Height);

		EZ_PROFILE_END_SESSION();
	}

	UIApp::~UIApp() {
		EZ_PROFILE_BEGIN_SESSION("Shutdown");

		Renderer2D::Shutdown();
		glfwTerminate();

		EZ_PROFILE_END_SESSION();
	}

	void UIApp::Run() {
		EZ_PROFILE_BEGIN_SESSION("Runtime");

		while (!glfwWindowShouldClose(m_Window)) {
			//glfwWaitEvents();
			glfwPollEvents();

			static double lastFrameTime = 0;
			double currentFrameTime = glfwGetTime();
			double delta = currentFrameTime - lastFrameTime;
			std::string frameRate = "FPS: ";
			frameRate += std::to_string((1 / delta));
			glfwSetWindowTitle(m_Window, frameRate.c_str());

			Renderer2D::SetViewMatrix(glm::mat4(1.0f));
			Renderer2D::BeginScene();
			for (int i = 0; i < 100; i++) {
				Renderer2D::DrawRect(glm::vec3(i, i, 0), glm::vec2(8, 8), glm::vec3(0));
			}
			Renderer2D::EndScene();
		
			glfwSwapBuffers(m_Window);
			lastFrameTime = currentFrameTime;
		}

		EZ_PROFILE_END_SESSION();
	}
}