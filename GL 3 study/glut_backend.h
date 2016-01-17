#ifndef GLUT_BACKEND_H
#define GLUT_BACKEND_H

#include "callback.h"

// ������������� GLUT`a
void GLUTBackendInit ( int argc, char** argv );

// �������� ����
bool GLUTBackendCreateWindow ( unsigned int width, unsigned int height, unsigned int bpp, bool isFullScreen, const char* pTitle );

// ������ �������� ����� GLUT`a
void GLUTBackendRun ( ICallbacks* pCallbacks );

#endif /* GLUT_BACKEND_H */
