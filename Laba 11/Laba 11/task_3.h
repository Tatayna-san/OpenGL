#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

GLuint Program;         // ID ��������� ���������
GLint Attrib_vertex;    // ID ��������
GLint gScaleLocation;   // ID uniform

// ID Vertex Buffer Object
GLuint VBO1;
GLuint VBO2;
GLuint VBO3;

GLint Attrib_color;     // ID �������� ������
GLuint VBO_position;    // ID VBO ������
GLuint VBO_color;      // ID VBO �����

// ��������� ��� �������� ������
struct Vertex {
    GLfloat x;
    GLfloat y;
};

// ���������� ������
const char* VertexShaderSource = R"(
#version 330 core
in vec2 coord;
void main() {
gl_Position = vec4(coord, 0.0, 1.0);
}
)";

// ����������� ������
const char* FragShaderSource = R"(
#version 330 core
uniform vec4 color; 
void main() {
gl_FragColor = color; 
}
)";

void InitShader();
void InitVBO1();
void InitVBO2();
void InitVBO3();
void ReleaseShader();
void ReleaseVBO();

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
    // ������� ��������
    Vertex quadrilateral[4] = {
        { 0.4f, -0.85f },
        { 0.85f, -0.85f },
        { 0.85f, -0.4f },
        { 0.4f, -0.4f }
    };

    // �������� ������� � �����
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadrilateral), quadrilateral, GL_STATIC_DRAW);

    // �������� ������ OpenGL, ���� ����, �� ����� � ������� ��� ������
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
    }
}

void InitVBO2() {
    glGenBuffers(1, &VBO2);
    // ������� ����������� �������������
    Vertex pentagon[5] = {
        { 0.0f, 0.5f },
        { -0.35f, 0.25f },
        { -0.25f, -0.15f },
        { 0.25f, -0.15f },
        { 0.35f, 0.25f }
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pentagon), pentagon, GL_STATIC_DRAW);

    // �������� ������ OpenGL, ���� ����, �� ����� � ������� ��� ������
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
    }
}

void InitVBO3() {
    glGenBuffers(1, &VBO3);
    // ������� �����
    Vertex fan[6] = {
        { -0.5f, 0.5f },
        { -0.9f, 0.8f },
        { -0.7f, 0.88f },
        { -0.5f, 0.9f },
        { -0.3f, 0.88f },
        { -0.1f, 0.8f },
    };

    // �������� ������� � �����
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fan), fan, GL_STATIC_DRAW);

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

    glBindBuffer(GL_ARRAY_BUFFER, VBO1); // ���������� VBO
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0); // ��������� pointer 0 ��� ������������ ������, �� ��������� ��� ������ � VBO
    glDrawArrays(GL_QUADS, 0, 4); // �������� ������ �� ����������(������)

    float Color1[4] = { 0.5f, 0.10f, 1.1f, 0.5f };
    gScaleLocation = glGetUniformLocation(Program, "color");
    glUniform4f(gScaleLocation, Color1[0], Color1[1], Color1[2], Color1[3]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2); // ���������� VBO
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0); // ��������� pointer 0 ��� ������������ ������, �� ��������� ��� ������ � VBO
    glDrawArrays(GL_POLYGON, 0, 5); // �������� ������ �� ����������(������)

    float Color2[4] = { 0.5f, 1.5f, 0.0f, 0.0f };
    gScaleLocation = glGetUniformLocation(Program, "color");
    glUniform4f(gScaleLocation, Color2[0], Color2[1], Color2[2], Color2[3]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3); // ���������� VBO
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0); // ��������� pointer 0 ��� ������������ ������, �� ��������� ��� ������ � VBO
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6); // �������� ������ �� ����������(������)
    
    float Color3[4] = { 1.5f, 0.10f, 0.1f, 0.5f };
    gScaleLocation = glGetUniformLocation(Program, "color");
    glUniform4f(gScaleLocation, Color3[0], Color3[1], Color3[2], Color3[3]);

    glDisableVertexAttribArray(Attrib_vertex); // ��������� ������ ���������
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
}

// ������������ ��������
void ReleaseShader() {
    // ��������� ����, �� ��������� ��������� ���������
    glUseProgram(0);
    // ������� ��������� ���������
    glDeleteProgram(Program);
}
