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
bool leftButtonPressed = false; // �������״̬

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

static void Do_Movement()
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

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		leftButtonPressed = (action == GLFW_PRESS);
		if (leftButtonPressed) {
			// ��ȡ��ǰ���λ��
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
		GLfloat yoffset = ypos - lastY;

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void camera_bind(Shader ourShader)
{
	glm::mat4 view;
	view = camera.GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SreenWidth / (float)SreenHeight, 0.1f, 100.0f);
	// Get the uniform locations
	//GLint modelLoc = glGetUniformLocation(outShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
	GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

//���Ƕ���
GLfloat trangle_One[] = {
	 //Positions         // Colors
	0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  // Bottom Right
   -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  // Bottom Left
	0.0f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f   // Top 
};
GLfloat trangle_Two[] = {
	 //Positions         // Colors
	0.8f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,  // Bottom Right
   -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,  // Bottom Left
	0.0f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f   // Top 
};
GLfloat trangle_Three[] = {
	 //Positions         // Colors
	1.0f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Right
   -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
	0.0f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f   // Top 
};

//�����Դ�һ��floatƫ�Ƶ����Ƕ���
//GLfloat trangle_One[] = {
//	// Positions         // Colors
//	0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  // Bottom Right
//   -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  // Bottom Left
//	0.0f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f   // Top 
//};
//GLfloat trangle_Two[] = {
//	// Positions         // Colors
//	0.5f, -0.5f, 0.000001f,   0.0f, 1.0f, 1.0f,  // Bottom Right
//   -0.5f, -0.5f, 0.000001f,   0.0f, 1.0f, 1.0f,  // Bottom Left
//	0.0f,  0.5f, 0.000001f,   0.0f, 1.0f, 1.0f   // Top 
//};
//GLfloat trangle_Three[] = {
//	// Positions         // Colors
//	0.5f, -0.5f, 0.000001f,   0.0f, 1.0f, 0.0f,  // Bottom Right
//   -0.5f, -0.5f, 0.000002f,   0.0f, 1.0f, 0.0f,  // Bottom Left
//	0.0f,  0.5f, 0.000003f,   0.0f, 1.0f, 0.0f   // Top 
//};

//�߶ζ���
GLfloat line_One[] = {
	// Positions         // Colors
	0.4f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
   -0.4f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Left
	0.0f,  0.4f, 0.0f,   1.0f, 0.0f, 0.0f   // Top 
};
GLfloat line_Two[] = {
	// Positions         // Colors
	0.7f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Right
   -0.4f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
	0.0f,  0.4f, 0.0f,   0.0f, 1.0f, 0.0f   // Top 
};
GLfloat line_Three[] = {
	// Positions         // Colors
	0.9f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  // Bottom Right
   -0.4f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  // Bottom Left
	0.0f,  0.4f, 0.0f,   0.0f, 0.0f, 1.0f   // Top 
};

unsigned int VAO[6], VBO[6];

//���ջ�ɫʵ�塢��ɫʵ�塢��ɫʵ�塢��ɫ�߿���ɫ�߿���ɫ�߿�˳���ύ����
void data_bind_one()
{
	memset(VAO, 0, size(VAO));
	memset(VBO, 0, size(VBO));
	glGenVertexArrays(6, VAO);
	glGenBuffers(6, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle_One), trangle_One, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle_Two), trangle_Two, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle_Three), trangle_Three, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_One), line_One, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_Two), line_Two, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_Three), line_Three, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

//��ɫʵ�塢��ɫʵ�塢��ɫʵ�塢��ɫ�߿򡢺�ɫ�߿���ɫ�߿�˳���ύ����
void data_bind_two()
{
	memset(VAO, 0, size(VAO));
	memset(VBO, 0, size(VBO));
	glGenVertexArrays(6, VAO);
	glGenBuffers(6, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle_Three), trangle_Three, GL_STATIC_DRAW);
	glBindVertexArray(VAO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle_Two), trangle_Two, GL_STATIC_DRAW);
	glBindVertexArray(VAO[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle_One), trangle_One, GL_STATIC_DRAW);
	glBindVertexArray(VAO[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_Two), line_Two, GL_STATIC_DRAW);
	glBindVertexArray(VAO[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_One), line_One, GL_STATIC_DRAW);
	glBindVertexArray(VAO[4]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_Three), line_Three, GL_STATIC_DRAW);
	glBindVertexArray(VAO[5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

//���ջ�ɫʵ�塢��ɫʵ�塢��ɫʵ�塢��ɫ�߿���ɫ�߿���ɫ�߿�˳�����
void data_draw_one()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonOffset(2.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������

	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonOffset(4.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonOffset(6.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[2]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0);

	glPolygonOffset(1.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[3]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonOffset(3.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[4]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonOffset(5.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[5]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

//��ɫʵ�塢��ɫʵ�塢��ɫʵ�塢��ɫ�߿򡢺�ɫ�߿���ɫ�߿�˳�����
void data_draw_two()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonOffset(6.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonOffset(4.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonOffset(2.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[2]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0);

	glPolygonOffset(3.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[3]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonOffset(1.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[4]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonOffset(5.0f, 1.0f); // ����ƫ��ֵ������ֵ���Ը��ݳ�������
	glBindVertexArray(VAO[5]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
	glfwInit();//��ʼ��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//����GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//����GLFW
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32); // ������Ȼ���λ��Ϊ24λ

	GLFWwindow* window = glfwCreateWindow(SreenWidth, SreenHeight, "OpenGL", nullptr, nullptr);
	
	glfwSetKeyCallback(window, key_callback); //�󶨼���
	glfwSetMouseButtonCallback(window, mouse_button_callback); //����갴��
	glfwSetCursorPosCallback(window, mouse_callback); //������ƶ�
	glfwSetScrollCallback(window, scroll_callback); //��������

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate(); //terminate the glfw(end the glfw)
		return -1;
	}

	glfwMakeContextCurrent(window);//���õ�ǰopenGL����

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	Shader outShader("./shaders/trans-vert.vs", "./shaders/trans-frag.frag");

	glViewport(0, 0, SreenWidth, SreenHeight);
	glEnable(GL_DEPTH_TEST);// ������Ȳ���
	glDepthFunc(GL_LESS);// ������Ȳ��Ժ��� GL_LESS GL_LEQUAL

	//data_bind_one();
	data_bind_two();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();//�����û�д����¼�
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		outShader.Use();

		camera_bind(outShader);

		glEnable(GL_POLYGON_OFFSET_FILL);// ���ö����FILLƫ��
		glEnable(GL_POLYGON_OFFSET_LINE);// ���ö����LINEƫ��

		//data_draw_one();
		data_draw_two();

		glfwSwapBuffers(window);//˫���壨�滻���ڵ�ÿһ������ֵ��
	}

	glDeleteVertexArrays(6, VAO);
	glDeleteBuffers(6, VBO);
	glfwTerminate();
	return 0;
}