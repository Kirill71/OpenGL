#pragma once

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/glut.h>


#include "pipeline.h"
#include "Camera.h"
#include "Texture.h"

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

struct Vertex
{
    Vector3f m_pos;
    Vector2f m_tex;

    Vertex() { }
    Vertex(Vector3f pos, Vector2f tex) : 
      m_pos(pos), 
      m_tex(tex) { }
};


GLuint VBO[2]; // ���������� ���������� ��� �������� ��������� �� ����� ������
GLuint IBO[2];
GLuint gWVPLocation;
GLuint gSampler;
Camera *pGameCamera = 0;
Texture *pTexture = 0;

// -------------------------------------------------------------------------------------------------------
static void RenderSceneCB()
{
  pGameCamera->OnRender();

  glClear(GL_COLOR_BUFFER_BIT);

  static float Scale = 0.0f;
  Scale += 0.01;

  Pipeline p;
  p.Rotate(0.0f, Scale, 0.0f);
  p.WorldPos(0.0f, 0.0f, 3.0f);
  p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
  p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
  
  // �������� �������� � ������ �� �������
  // ��� ��� ���� ������ ������� glUniform* ��� 
  // �������� ������ � uniform-���������� �������
  // 1: ������ unifor variable
  // 2: ���������� ����������� ������
  // 3: �� ������� ��� �� ��������
  // 4: ��������� �� ����� �������
  glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());

  // �������� �������� � ������ �� �������
  //glUniform1f(gScaleLocation, sinf(Scale));

  // ���������� ������� (��� �� ��� � � ��������� �������)
  // 1: ����� ��������
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  
  // �������� �����
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  
  // ��� ��������� ������������ ������ ������ (������ ������� � �����������)
  // 1: ����� �������� (������� �������, ����� ��� ��������)
  // 2: ���������� ��������� � �������� (x y z) = 3
  // 3: ��� ������� ���������� (x: float)
  // 4: ��������������� �������� ��� ���
  // 5: ����� ������ ����� 2 ���������� (������ ����)
  // 6: �������� � ��������� (� ����� ������� ���������� ������ ������ ��������)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(GLfloat) * 3));
  
  // �������� �����
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
  
  // �������� �������� � "������ �������"
  pTexture->Bind(GL_TEXTURE0);

  // ������ ��� ����
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

  // ��������� ��������
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glutSwapBuffers();
}

// -------------------------------------------------------------------------------------------------------
static void SpecialKeyboardCB(int Key, int x, int y)
{
  pGameCamera->OnKeyboard(Key);
}

static void KeyboardCB(unsigned char Key, int x, int y)
{
     switch (Key) {
        case 'q':
        exit(0);
        break;
        case '+':
          Camera::STEP_SIZE_MOUSE *= 10;
          break;
        case '-':
          Camera::STEP_SIZE_MOUSE /= 10;
          break;
     }
}
 
static void PassiveMouseCB(int x, int y)
{
     pGameCamera->OnMouse(x, y);
}

// -------------------------------------------------------------------------------------------------------
static void InitializeGlutCallbacks()
{
  glutDisplayFunc(RenderSceneCB);
  glutIdleFunc(RenderSceneCB);
  glutSpecialFunc(SpecialKeyboardCB);
  glutPassiveMotionFunc(PassiveMouseCB);
  glutKeyboardFunc(KeyboardCB);
}

// -------------------------------------------------------------------------------------------------------
// �������� ������ ������
static void CreateVertexBuffer()
{
  // 3 vertices
  Vertex Vertices[4] = {  Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
                          Vertex(Vector3f(0.0f, -1.0f, -1.15475), Vector2f(0.5f, 1.0f)),
                          Vertex(Vector3f(1.0f, -1.0f, 0.5773f),  Vector2f(1.0f, 0.0f)),
                          Vertex(Vector3f(0.0f, 1.0f, 0.0f),      Vector2f(0.5f, 0.3271484f)) };
  /*Vector3f Vertices[4];
  Vertices[0] = Vector3f(-1.0f, -1.0f, 0.5773f);
  Vertices[1] = Vector3f(0.0f, -1.0f, -1.15475f);
  Vertices[2] = Vector3f(1.0f, -1.0f, 0.5773f);
  Vertices[3] = Vector3f(0.0f, 1.0f, 0.0f);*/

  Vector3f cubeV[8];
  cubeV[0] = Vector3f(-1.0f, 1.0f, 1.0f);
  cubeV[1] = Vector3f(1.0f, 1.0f, 1.0f);
  cubeV[2] = Vector3f(1.0f, -1.0f, 1.0f);
  cubeV[3] = Vector3f(-1.0f, -1.0f, 1.0f);
  cubeV[4] = Vector3f(-1.0f, 1.0f, -1.0f);
  cubeV[5] = Vector3f(1.0f, 1.0f, -1.0f);
  cubeV[6] = Vector3f(1.0f, -1.0f, -1.0f);
  cubeV[7] = Vector3f(-1.0f, -1.0f, -1.0f);

  // Generate of buffer 
  //  | 1: ��� VBO
  //  | 2: c����� �� ����� ��� �������� ID �������
  glGenBuffers(2, VBO);
  std::cout << "\n\n&VBO: " << VBO << "\nVBO[0]: " << VBO[0] << "\nVBO[1]: " << VBO[1] << std::endl << std::endl;

  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeV), cubeV, GL_STATIC_DRAW);

  // �������� ��������� � �������� ����
  // ������ VBO ���������� ��������
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

  // ��������� ������ � �������� VBO
  // �������� ������ ������ � ���������������
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
  // indeces
  unsigned int cubeI[] = 
  { 0, 1, 2, 3,
    3, 7, 6, 2,
    2, 1, 5, 6,
    6, 7, 4, 5,
    5, 1, 0, 4,
    4, 7, 3, 0
  };

  // indeces
  unsigned int indeces[] =
    { 0, 3, 1, 
    1, 3, 2,
    2, 3, 0,
    0, 1, 2 };
  /*{ 0, 3, 1, 
    1, 3, 2,
    2, 3, 0,
    0, 2, 1 };*/

  glGenBuffers(2, IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeI), cubeI, GL_STATIC_DRAW);

  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);
}

// -------------------------------------------------------------------------------------------------------
// ���������� ������� � ��������� ��������
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
  // ������� ������ ������
  GLuint ShaderObj = glCreateShader(ShaderType);
  if (ShaderObj == 0) {
    fprintf(stderr, "Error creating shader type %d\n", ShaderType);
    system("pause");
    exit(0);
  }

  // ������ �������� ����������
  // �������� ����� ������� �� ��������� ������
  GLint Lengths[1];
  Lengths[0] = strlen(pShaderText);

  const GLchar* p[1];
  p[0] = pShaderText;

  // ��������� ����� (���) � ������
  //  | 2: cout of bloks
  glShaderSource(ShaderObj, 1, p, Lengths);

  // Compiling shader
  glCompileShader(ShaderObj);

  // Status of compile
  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
    system("pause");
    exit(1);
  }

  // program obj + compil obj
  glAttachShader(ShaderProgram, ShaderObj);
}

bool SetShader(GLuint shaderProgram, const char* filename, GLenum ShaderType)
{
  std::ifstream is(filename, std::ios::in | std::ios::binary | std::ios::ate);
  if(!is.is_open())
  {
    std::cerr << "Unable to open file " << filename << std::endl;
    return false;
  }

  long size = is.tellg();
  char* pShaderText = new char[size + 1];

  is.seekg(0, std::ios::beg);
  is.read(pShaderText, size);
  is.close();
  
  pShaderText[size] = 0;

  std::cout << pShaderText << std::endl << std::endl;

  // Add our shader
  AddShader(shaderProgram, pShaderText, ShaderType);

  delete[] pShaderText;

  return true;
}

// -------------------------------------------------------------------------------------------------------
// ���������� ��������
static void CompileShaders()
{
  // Program object
  GLuint ShaderProgram = glCreateProgram();
  if (ShaderProgram == 0) {
    fprintf(stderr, "Error creating shader program\n");
    system("pause");
    exit(1);
  }

  SetShader(ShaderProgram, "VS.glsl", GL_VERTEX_SHADER);
  SetShader(ShaderProgram, "FS.glsl", GL_FRAGMENT_SHADER);

  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  // link of shader program
  glLinkProgram(ShaderProgram);

  // status of linker
  glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
  if (Success == 0) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
    system("pause");
    exit(1);
  }

  // C����� �� ��������� �������� �����������
  glValidateProgram(ShaderProgram);
  glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
  if (!Success) {
    glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
    fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
    system("pause");
    exit(1);
  }

  // �������, ��� ������������� ������������� ��������� 
  // �������� �� ��������� � ��� ���������
  glUseProgram(ShaderProgram);

  // ����������� ������� (index for acces) uniform variable
  gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
  assert(gWVPLocation != 0xFFFFFFFF);

  //gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
  //assert(gSampler != 0xFFFFFFFF);
}

// -------------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  //glutInitWindowPosition(100, 100);
  //glutCreateWindow("Sample Project");
  glutGameModeString("1366x768:32@72");
  glutEnterGameMode();
  glutSetWindowTitle("Sample project");
  
  // ������������� �������� ������� GLUT�
  InitializeGlutCallbacks();

  pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

  // Must be done after glut is initialized!
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    system("pause");
    return 1;
  }

  // GL_COLOR_BUFFER_BIT ������ ���������� �����
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  
  // �� ��������� ��, ��� �� �� �����
  // ��������, ��� ������� �������� �� ������� �������
  glFrontFace(GL_CW);
  // �� ������� �������� ������� 
  glCullFace(GL_BACK);
  // �� ������� �� ������ ��� ���������
  glEnable(GL_CULL_FACE);


  // ������� ����� ������
  CreateVertexBuffer();
  CreateIndexBuffer();

  // ���������� � ����������� �������
  CompileShaders();
  
  glUniform1i(gSampler, 0);

  pTexture = new Texture(GL_TEXTURE_2D, "Content/test_rotate_02.png");

  if(!pTexture->Load())
    return 1;

  // GLUT ����
  glutMainLoop();

  return 0;
}


