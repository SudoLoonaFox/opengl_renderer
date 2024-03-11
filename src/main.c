#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MAX_SHADER_BUFFER_SIZE 2000
// TODO add error logging function
int  success;
char infoLog[512];

const char* vertexFormatIn = "v %f %f %f\n";
const char* faceFormatIn = "f %i %i %i\n";

typedef struct {
	float* vertices;
	unsigned int* indices;
	unsigned int vertices_len;
	unsigned int indices_len;
}Model;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";
/*
// Triangle
float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};
*/
// Cube
float vertices[] = {
    // Front face
     0.5,  0.5,  0.5,
    -0.5,  0.5,  0.5,
    -0.5, -0.5,  0.5,
     0.5, -0.5,  0.5,

    // Back face
     0.5,  0.5, -0.5,
    -0.5,  0.5, -0.5,
    -0.5, -0.5, -0.5,
     0.5, -0.5, -0.5,
};

unsigned int indices[] = {
      // Front
      0, 1, 2,
      2, 3, 0,

      // Right
      0, 3, 7,
      7, 4, 0,

      // Bottom
      2, 6, 7,
      7, 3, 2,

      // Left
      1, 5, 6,
      6, 2, 1,

      // Back
      4, 7, 6,
      6, 5, 4,

      // Top
      5, 1, 0,
      0, 4, 5,
};

int compileShaderFromFile(char* path, GLuint shader){
	char buffer[MAX_SHADER_BUFFER_SIZE]; // Will this work?
	FILE* file = fopen(path, "r");
	if(file==NULL){
		return -1;
	}
	size_t i = 0;
	while(i<MAX_SHADER_BUFFER_SIZE){
		char c = fgetc(file);
		if(c == EOF)break;
		buffer[i++] = c;
	}
	buffer[i] = '\0';

	const char* buf = buffer;
	glShaderSource(shader, 1, &buf, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("%s", infoLog);
	}
	return 0;
}

int main(void){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Renderer", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	glewInit();

	// set callback for resizing
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// create vertex shader
	unsigned int vertexShader;
	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShaderFromFile("src/vertexShader.glsl", vertexShader);
	
	// create fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShaderFromFile("src/fragmentShader.glsl", fragmentShader);

	//TODO Create variadic function to make shader program
	// create shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success){
    	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("%s", infoLog);
	}

	// bind and delete
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(shaderProgram);

	// create vertex buffer object
	unsigned int vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	// create vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// bind buffer here
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// create element buffer object	
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// copy indices to element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 1. bind Vertex Array Object
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	// 3. copy our index array in a element buffer for OpenGL to use
	// 4. then set the vertex attributes pointers

    /* Loop until the user closes the window */

	// set wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// unset wireframe
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while (!glfwWindowShouldClose(window)){
        /* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
	//	glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
