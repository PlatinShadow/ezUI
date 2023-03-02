#include "ez/Core/UIApp.h"
#include "ez/Core/Base.h"
#include "ez/Core/Profiling.h"
#include "ez/Graphics/Renderer/Renderer2D.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ez {
	gfx::Brush defaultBrush;
	gfx::Brush defaultBrush2;
	gfx::Brush gradientBrush;

	std::vector<gfx::Brush> brushes;

	UIApp* UIApp::Create(const UIApp::Specification& settings) {
		return new UIApp(settings);
	}

	void UIApp::OnFramebufferSizeChanged(GLFWwindow* window, int width, int height) {
		if (width > 0 && height > 0) {
			gfx::Renderer2D::SetRenderSize(width, height);
		}
	}

	UIApp::UIApp(const UIApp::Specification& settings) {
		ez::Logger::Init();
		
		EZ_PROFILE_BEGIN_SESSION("Startup");

		EZ_CORE_LOG("ezUI Version v", ez::VERSION_MAJOR, ".", ez::VERSION_MINOR, ".", ez::VERSION_PATCH);

		m_Spec = settings;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef EZ_BUILD_DEBUG_MODE
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		#endif

		m_Window = glfwCreateWindow(m_Spec.Width, m_Spec.Height, m_Spec.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetFramebufferSizeCallback(m_Window, UIApp::OnFramebufferSizeChanged);
		glfwSetWindowUserPointer(m_Window, this);

		gfx::Renderer2D::Init(m_Spec.Width, m_Spec.Height);

		defaultBrush = gfx::Renderer2D::CreateSolidColorBrush(glm::vec4(1.0, 1.0, 1.0, 1.0));
		defaultBrush2 = gfx::Renderer2D::CreateSolidColorBrush(glm::vec4(0.0, 1.0, 1.0, 1.0));

		gradientBrush = gfx::Renderer2D::CreateGradientBrush(glm::vec4(0.98f, 0.18f, 0.63f, 1.0f), glm::vec4(0.94f, 0.24f, 0.3f, 1.0f));
			 
		for (int i = 0; i < 2048; i++) {
			brushes.push_back(gfx::Renderer2D::CreateSolidColorBrush(glm::vec4(rand(), rand(), rand(), 1.0f)));
		}

		EZ_PROFILE_END_SESSION();
	}

	UIApp::~UIApp() {
		EZ_PROFILE_BEGIN_SESSION("Shutdown");

		gfx::Renderer2D::Shutdown();
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

			gfx::Renderer2D::BeginFrame();
			gfx::Renderer2D::SetViewMatrix(glm::mat4(1.0f));
			
			for (int x = 0; x < 500; x++) {
				for (int y = 0; y < 300; y++) {
					gfx::Renderer2D::DrawRect(brushes[(x+y) % 1024], glm::vec3(x * 4 + 4, y * 4 + 4, sin(x+y+currentFrameTime*10) * 4), glm::vec2(4,4), glm::vec3(0));
				}
			}

			gfx::Renderer2D::EndFrame();
		

			glfwSwapBuffers(m_Window);
			lastFrameTime = currentFrameTime;
		}

		EZ_PROFILE_END_SESSION();
	}
}