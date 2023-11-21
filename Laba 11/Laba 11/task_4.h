#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

// ID юниформ переменной цвета
GLint Unif_xmove;
GLint Unif_ymove;

// ID uniform
GLint gScaleLocation;

GLuint Program;         // ID шейдерной программы
GLint Attrib_vertex;    // ID атрибута

// ID Vertex Buffer Object
GLuint VBO1;
GLuint VBO2;
GLuint VBO3;

GLint Attrib_color;     // ID атрибута вершин
GLuint VBO_position;    // ID VBO вершин

// ID VBO цвета
GLuint VBO1_color;      
GLuint VBO2_color;     
GLuint VBO3_color;

// Структура для описания вершин
struct Vertex {
    GLfloat x;
    GLfloat y;
};

// Вершинноый шейдер
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

// Фрагментный шейдер
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
    // Шейдеры
    InitShader();
    // Вершинный буфер
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

// Инициализация буфера вершин
void InitVBO1() {
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO1_color);
    // Вершины квадрата
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

    // Передаем вершины в буфер
    glBindBuffer(GL_ARRAY_BUFFER, VBO1); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadrilateral), quadrilateral, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STATIC_DRAW);

    // Проверка ошибок OpenGL, если есть, то вывод в консоль тип ошибки
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
    }
}

void InitVBO2() {
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &VBO2_color);
    // Вершины правильного пятиугольника
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

    // Проверка ошибок OpenGL, если есть, то вывод в консоль тип ошибки
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
    }
}

void InitVBO3() {
    glGenBuffers(1, &VBO3);
    glGenBuffers(1, &VBO3_color);
    // Вершины веера
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

    // Передаем вершины в буфер
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fan), fan, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors3), colors3, GL_STATIC_DRAW);

    // Проверка ошибок OpenGL, если есть, то вывод в консоль тип ошибки
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << error << std::endl;
    }
}

// Шейдеры
void InitShader() {
    // Создаем вершинный шейдер
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    // Передаем исходный код
    glShaderSource(vShader, 1, &VertexShaderSource, NULL);
    // Компилируем шейдер
    glCompileShader(vShader);
    std::cout << "vertex shader compilation result:" << std::endl;
    // Функция печати лога шейдера
    ShaderLog(vShader);

    // Создаем фрагментный шейдер
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Передаем исходный код
    glShaderSource(fShader, 1, &FragShaderSource, NULL);
    // Компилируем шейдер
    glCompileShader(fShader);
    std::cout << "fragment shader compilation result:" << std::endl;
    // Функция печати лога шейдера
    ShaderLog(fShader);

    // Создаем программу и прикрепляем шейдеры к ней
    Program = glCreateProgram();
    glAttachShader(Program, vShader);
    glAttachShader(Program, fShader);
    // Линкуем шейдерную программу
    glLinkProgram(Program);

    // Проверяем статус сборки
    GLint linkStatus;
    glGetProgramiv(Program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        std::cout << "Failed to link shader program" << std::endl;
        return;
    }

    // Вытягиваем ID атрибута из собранной программы
    const char* attr_name = "coord"; // имя в шейдере
    Attrib_vertex = glGetAttribLocation(Program, attr_name);
    if (Attrib_vertex == -1) {
        std::cout << "Could not bind attrib " << attr_name << std::endl;
        return;
    }
}

// Функция отрисовки
void Draw() {
    glUseProgram(Program); // Устанавливаем шейдерную программу текущей
    glEnableVertexAttribArray(Attrib_vertex); // Включаем массив атрибутов
    glEnableVertexAttribArray(Attrib_color);

    glUniform1f(Unif_xmove, moveX);
    glUniform1f(Unif_ymove, moveY);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1); // Подключаем VBO
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0); // Указываем pointer 0 при подключенном буфере, мы указываем что данные в VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO1_color);
    glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_QUADS, 0, 4); // Передаем данные на видеокарту(рисуем)

    glBindBuffer(GL_ARRAY_BUFFER, VBO2); // Подключаем VBO
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0); // Указываем pointer 0 при подключенном буфере, мы указываем что данные в VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO2_color); 
    glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 0, 0); 
    glDrawArrays(GL_POLYGON, 0, 5); // Передаем данные на видеокарту(рисуем)

    glBindBuffer(GL_ARRAY_BUFFER, VBO3); // Подключаем VBO
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0); // Указываем pointer 0 при подключенном буфере, мы указываем что данные в VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO3_color);
    glVertexAttribPointer(Attrib_color, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6); // Передаем данные на видеокарту(рисуем)

    glDisableVertexAttribArray(Attrib_vertex); // Отключаем массив атрибутов
    glDisableVertexAttribArray(Attrib_color); 
    glUseProgram(0); // Отключаем шейдерную программу
}

// Освобождение ресурсов
void Release() {
    // Шейдеры
    ReleaseShader();
    // Вершинный буфер
    ReleaseVBO();
}

// Освобождение буфера
void ReleaseVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &VBO3);
    glDeleteBuffers(1, &VBO1_color);
    glDeleteBuffers(1, &VBO2_color);
    glDeleteBuffers(1, &VBO3_color);
}

// Освобождение шейдеров
void ReleaseShader() {
    // Передавая ноль, мы отключаем шейдерную программу
    glUseProgram(0);
    // Удаляем шейдерную программу
    glDeleteProgram(Program);
}