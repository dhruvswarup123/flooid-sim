#pragma once

// C++ Libs
#include <stdio.h>
#include <windows.h>
#include <optional>

// OpenGL/GLEW -- must be before other gl imports
#define GLEW_STATIC
#include <GL/glew.h>

// Imgui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// GLFW
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include <GL/gl.h>  // GLUT, includes glu.h and gl.h


class App final {
public:
	bool init();

	void setup();

	bool windowShouldClose();

	void loop();

	void deinit();
private:
	static void glfwErrorCallback(int error, const char* description) {
		fprintf(stderr, "GLFW Error %d: %s\n", error, description);
	}
	
	// Constants
	static constexpr ImVec4 kClearColor{ 25/255.0f, 65/255.0f, 105/255.0f, 1.00f };

	// Setup variables
	GLFWwindow* window_{};
	ImGuiIO* io_{};
};

