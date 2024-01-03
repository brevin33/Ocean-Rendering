#include <pch.h>
#include <GLFW/glfw3.h>
#include <Mesmerize/Renderer.h>
#include <Mesmerize/Defaults.h>

constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;

void main() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, GraphicsAPI, nullptr, nullptr);
	MZ::setup(window, 1, "../../../include/dep/MesmerizeRenderer/");
	MZ::setDefferedShader("../../../include/dep/MesmerizeRenderer/shaders/defferedFrag.spv",nullptr, 0, nullptr, 0);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		MZ::drawFrame();
	}

	MZ::cleanup();
}