#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "shaders/shaderFile.h"
#include "tool/cameraFile.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
using namespace std;

const GLuint SreenWidth = 800, SreenHeight = 600;
// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = SreenWidth / 2.0, lastY = SreenHeight / 2.0;
bool firstMouse = true;
bool leftButtonPressed = false; // 左键按下状态

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		leftButtonPressed = (action == GLFW_PRESS);
		if (leftButtonPressed) {
			// 获取当前鼠标位置
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			lastX = xpos;
			lastY = ypos;
		}
	}
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (leftButtonPressed)
	{
		if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

int main()
{
	glfwInit();//初始化
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//配置GLFW
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32); // 设置深度缓冲位数为24位

	GLFWwindow* window = glfwCreateWindow(SreenWidth, SreenHeight, "OpenGL", nullptr, nullptr);
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();//terminate the glfw(end the glfw)
		return -1;
	}

	glfwMakeContextCurrent(window);//设置当前openGL上下

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	glViewport(0, 0, SreenWidth, SreenHeight);
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	// 设置深度测试函数 GL_LESS GL_LEQUAL
	glDepthFunc(GL_LESS);

	Shader outShader("./shaders/trans-vert.vs", "./shaders/trans-frag.frag");

	float value = 0.0;

	GLuint VAO, VBO;

	while (!glfwWindowShouldClose(window))
	{

		// 开始新帧
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// 创建UI窗口
		ImGui::Begin("Vertex Control");

		// 添加用于修改顶点坐标
		ImGui::InputFloat("Value", &value, 0.000001f, 0.1f, "%.8f");

		ImGui::End();
		// Rendering
		ImGui::Render();

		//三角顶点
		GLfloat vertices[] = {
			// Positions         // Colors
			0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
		   -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Left
			0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // Top 

			0.5f, -0.5f, value,   0.0f, 1.0f, 0.0f,  // Bottom Right
		   -0.5f, -0.5f, value,   0.0f, 1.0f, 0.0f,  // Bottom Left
			0.0f,  0.5f, value,   0.0f, 1.0f, 0.0f   // Top 
		};
		//vectices data
		glGenVertexArrays(1, &VAO);//获得一个顶点数组对象
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		outShader.Use();

		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SreenWidth / (float)SreenHeight, 0.1f, 100.0f);
		// Get the uniform locations
		//GLint modelLoc = glGetUniformLocation(outShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(outShader.Program, "view");
		GLint projLoc = glGetUniformLocation(outShader.Program, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f); // 调整偏移值，具体值可以根据场景调整
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 恢复偏移状态
		glDisable(GL_POLYGON_OFFSET_FILL);

		glDrawArrays(GL_TRIANGLES, 3, 3);
		glBindVertexArray(0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);//双缓冲（替换窗口的每一个像素值）
		glfwPollEvents();//检查有没有触发事件
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// 清理
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}