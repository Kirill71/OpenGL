
#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GL\glut.h>

#include "glut_backend.h"

static ICallbacks* s_pCallbacks = nullptr;

static void SpecialKeyboardCB ( int key, int x, int y )
{
  s_pCallbacks->SpecialKeyboardCB ( key, x, y );
}

static void KeyboardCB ( unsigned char key, int x, int y )
{
  s_pCallbacks->KeyboardCB ( key, x, y );
}

static void PassiveMouseCB ( int x, int y )
{
  s_pCallbacks->PassiveMouseCB ( x, y );
}

static void RenderSceneCB ( )
{
  s_pCallbacks->RenderSceneCB ( );
}

static void IdleCB ( )
{
  s_pCallbacks->IdleCB ( );
}

static void InitCallbaks ( )
{
  glutDisplayFunc         ( RenderSceneCB );
  glutIdleFunc            ( RenderSceneCB );
  glutSpecialFunc         ( SpecialKeyboardCB );
  glutPassiveMotionFunc   ( PassiveMouseCB );
  glutKeyboardFunc        ( KeyboardCB );
}

void GLUTBackendInit ( int argc, char** argv )
{
  glutInit ( &argc, argv );
  glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA );
}

bool GLUTBackendCreateWindow ( unsigned int width, unsigned int height, unsigned int bpp, bool isFullScreen, const char* pTitle )
{
  if ( isFullScreen )
  {
    char modeString[64] = { 0 };
    sprintf_s ( modeString, "%dx%d@%d", width, height, bpp );
    glutGameModeString ( modeString );
    glutEnterGameMode ( );
  }
  else
  {
    glutInitWindowSize ( width, height );
    glutCreateWindow ( pTitle );
  }

  // Must be done after glut is initialized!
  GLenum res = glewInit ( );
  
  if ( res != GLEW_OK ) 
  {
    fprintf ( stderr, "Error: '%p'\n", glewGetErrorString ( res ) );
    return false;
  }

  return true;
}

void GLUTBackendRun ( ICallbacks* pCallbacks )
{
  if ( !pCallbacks )
  {
    fprintf ( stderr, "%s : callbacks not specified!\n", __FUNCTIONW__ );
    return;
  }

  // GL_COLOR_BUFFER_BIT ������ ���������� �����
  glClearColor ( 0.1f, 0.1f, 0.1f, 1.0f );
  
  // �� ��������� ��, ��� �� �� �����
  // ��������, ��� ������� �������� �� ������� �������
  glFrontFace ( GL_CW );
  // �� ������� �������� ������� 
  glCullFace ( GL_BACK );
  // �� ������� �� ������ ��� ���������
  glEnable ( GL_CULL_FACE );
  // ���� �������
  //glEnable ( GL_DEPTH_TEST );

  s_pCallbacks = pCallbacks;
  
  InitCallbaks ( );

  // GLUT ����
  glutMainLoop ( );
}