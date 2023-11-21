#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

// ID ������� ���������� �����
GLint Unif_xmove;
GLint Unif_ymove;

// ID uniform
GLint gScaleLocation;

GLuint Program;         // ID ��������� ���������
GLint Attrib_vertex;    // ID ��������

// ID Vertex Buffer Object
GLuint VBO1;
GLuint VBO2;
GLuint VBO3;

GLint Attrib_color;     // ID �������� ������
GLuint VBO_position;    // ID VBO ������

// ID VBO �����
GLuint VBO1_color;      
GLuint VBO2_color;     
GLuint VBO3_color;

// ��������� ��� �������� ������
struct Vertex {
    GLfloat x;
    GLfloat y;
};

// ���������� ������
const char* VertexShaderSource = R"(
#version 330 core
in vec3 coord;
in vec4 color;
uniform float x_move;
uniform float y_move;
out vec4 vert_color;
void main() {
vec3 position = vec3(coord) + vec3(x_move, y_move, 0);
gl_Position = vec4(position[0], position[1], 0.0, 1.0);
vert_color = color;
}
)";

// ����������� ������
const char* FragShaderSource = R"(
#version 330 core
in vec4 vert_color;
out vec4 color;
void main() {
color = vert_color;
}
)";

float moveX = 0;
float moveY = 0;

void InitShader();
void InitVBO1();
void InitVBO2();
void InitVBO3();
void ReleaseShader();
void ReleaseVBO();

void moveShape(float moveXinc, float moveYinc) {
    moveX += moveXinc;
    moveY += moveYinc;
}

void Init() {
    // �������
    InitShader();
    // ��������� �����
    InitVBO1();
    InitVBO2();
    InitVBO3();
}

void ShaderLog(unsigned int shader)
{
    int infologLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
    if (infologLen > 1)
    {
        int charsWritten = 0;
        std::vector<char> infoLog(infologLen);
        glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
        std::cout << "InfoLog: " << infoLog.data() << std::endl;
    }
}

// ������������� ������ ������
void InitVBO1() {
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO1_color);
    // ������� ��������
    Vertex quadrilateral[4] = {
        { 0.4f, -0.85f },
        { 0.85f, -0.85f },
        { 0.85f, -0.4f },
        { 0.4f, -0.4f }
    };

    float colors1[8][4] = {
        { 0.0, 1.0, 1.0, 0.0 }, { 0.0, 1.0, 1.0, 0.0 },
        { 1.0, 1.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0, 0.0 },
        { 1.0, 1.0, 1.0, 0.0 }, { 1.0, 1.0, 1.0, 0.0 },
        { 1.0, 1.0, 1.0, 0.0 }, { 1.0, 1.0, 1.0, 0.0 },
    };

    // �������� ������� � �����
    glBindBuffer(GL_ARRAY_BUFFER, VBO1); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadrilateral), quadrilateral, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STATIC_DRAW);

    // �������� ������ OpenGL, ���� ����, �� ����� � ������� ��� ������
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
    }
}

void InitVBO2() {
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &VBO2_color);
    // ������� ����������� �������������
    Vertex pentagon[5] = {
        { 0.0f, 0.5f },
        { -0.35f, 0.25f },
        { -0.25f, -0.15f },
        { 0.25f, -0.15f },
        { 0.35f, 0.25f }
    };
    float colors2[10][4] = {
        { 0.0, 1.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0, 0.0 },
        { 1.0, 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0, 0.0 },
        { 1.0, 1.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0, 0.0 },
        { 1.0, 0.0, 1.0, 0.0 }, { 1.0, 0.0, 1.0, 0.0 },
        { 0.0, 1.0, 1.0, 0.0 }, { 0.0, 1.0, 1.0, 0.0 },
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pentagon), pentagon, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), colors2, GL_STATIC_DRAW);

    // �������� ������ OpenGL, ���� ����, �� ����� � ������� ��� ������
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
    }
}

void InitVBO3() {
    glGenBuffers(1, &VBO3);
    glGenBuffers(1, &VBO3_color);
    // ������� �����
    Vertex fan[6] = {
        { -0.5f, 0.5f },
        { -0.9f, 0.8f },
        { -0.7f, 0.88f },
        { -0.5f, 0.9f },
        { -0.3f, 0.88f },
        { -0.1f, 0.8f },
    };
    float colors3[12][4] = {
       { 0.1, 0.4, 1.0, 0.5 }, { 0.1, 0.4, 1.0, 0.5 },
       { 1.1, 1.4, 1.0, 0.5 }, { 0.1, 0.4, 1.0, 0.5 },
       { 1.1, 1.4, 1.0, 0.5 }, { 0.1, 0.4, 1.0, 0.5 },
       { 1.1, 1.4, 1.0, 0.5 }, { 0.1, 0.4, 1.0, 0.5 },
       { 1.1, 1.4, 1.0, 0.5 }, { 0.1, 0.4, 1.0, 0.5 },
       { 1.1, 1.4, 1.0, 0.5 }, { 0.1, 0.4, 1.0, 0.5 },
    };

    // �������� ������� � �����
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fan), fan, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors3), colors3, GL_STATIC_DRAW);

    // �������� ������ OpenGL, ���� ����, �� ����� � ������� ��� ������
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
    }
}

// �������
void InitShader() {
    // ������� ��������� ������
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    // �������� �������� ���
    glShaderSource(vShader, 1, &VertexShaderSource, NULL);
    // ����������� ������
    glCompileShader(vShader);
    std::cout << "vertex shader compilation result:" << std::endl;
    // ������� ������ ���� �������
    ShaderLog(vShader);

    // ������� ����������� ������
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    // �������� �������� ���
    glShaderSource(fShader, 1, &FragShaderSource, NULL);
    // ����������� ������
    glCompileShader(fShader);
    std::cout << "fragment shader compilation result:" << std::endl;
    // ������� ������ ���� �������
    ShaderLog(fShader);

    // ������� ��������� � ����������� ������� � ���
    Program = glCreateProgram();
    glAttachShader(Program, vShader);
    glAttachShader(Program, fShader);
    // ������� ��������� ���������
    glLinkProgram(Program);

    // ��������� ������ ������
    GLint linkStatus;
    glGetProgramiv(Program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        std::cout << "Failed to link shader program" << std::endl;
        return;
    }

    // ���������� ID �������� �� ��������� ���������
    const char* attr_name = "coord"; // ��� � �������
    Attrib_vertex = glGetAttribLocation(Program, attr_name);
    if (Attrib_vertex == -1) {
        std::cout << "Could not bind attrib " << attr_name << std::endl;
        return;
    }
}

// ������� ���������
void Draw() {
    glUseProgram(Program); // ������������� ��������� ��������� �������
    glEnableVertexAttribArray(Attrib_vertex); // �������� ������ ���������
    glEnableVertexAttribArray(Attrib_color);

    glUniform1f(Unif_xmove, moveX);
    glUniform1f(Unif_ymove, moveY);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1); // ���������� VBO
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0); // ��������� pointer 0 ��� ������������ ������, �� ��������� ��� ������ � VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO1_color);
    glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_QUADS, 0, 4); // �������� ������ �� ����������(������)

    glBindBuffer(GL_ARRAY_BUFFER, VBO2); // ���������� VBO
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0); // ��������� pointer 0 ��� ������������ ������, �� ��������� ��� ������ � VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO2_color); 
    glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 0, 0); 
    glDrawArrays(GL_POLYGON, 0, 5); // �������� ������ �� ����������(������)

    glBindBuffer(GL_ARRAY_BUFFER, VBO3); // ���������� VBO
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0); // ��������� pointer 0 ��� ������������ ������, �� ��������� ��� ������ � VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO3_color);
    glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6); // �������� ������ �� ����������(������)

    glDisableVertexAttribArray(Attrib_vertex); // ��������� ������ ���������
    glDisableVertexAttribArray(Attrib_color); 
    glUseProgram(0); // ��������� ��������� ���������
}

// ������������ ��������
void Release() {
    // �������
    ReleaseShader();
    // ��������� �����
    ReleaseVBO();
}

// ������������ ������
void ReleaseVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &VBO3);
    glDeleteBuffers(1, &VBO1_color);
    glDeleteBuffers(1, &VBO2_color);
    glDeleteBuffers(1, &VBO3_color);
}

// ������������ ��������
void ReleaseShader() {
    // ��������� ����, �� ��������� ��������� ���������
    glUseProgram(0);
    // ������� ��������� ���������
    glDeleteProgram(Program);
}