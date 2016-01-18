#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H

#pragma once

#include "technique.h"
#include "math3d.h"

struct DirectionLight
{
  Vector3f  color;
  float     ambientIntensity;
  Vector3f  direction;
  float     diffuseIntensity;
};

class LightingTechnique : public Technique
{
public:

  LightingTechnique ( );
  virtual bool Init ( );

  bool SetShader ( const char* pFilename, char* &pShaderText );

  void SetWVP ( const Matrix4f& WVP );
  void SetWorldMatrix ( const Matrix4f& WorldInverse );
  // ��������� ������� �������� � ��������
  void SetTextureUnit ( unsigned int textureUnit );         
  
  // ������� ���������
  void SetDirectionalLight ( const DirectionLight& light );

  // ���������� ����
  void SetEyeWorldPos ( const Vector3f& eyeWorldPos );  // �������
  void SetMatSpecularIntensity ( float intensity );     // ������������� ���������
  void SetMatSpecularPower ( float power );             // ������� ��������� ���������

private:

  GLuint m_WVPLocation;
  GLuint m_WorldMatrixLocation;

  GLuint m_samplerLocation;
                                                
  GLuint m_eyeWorldPosLocation;           // �������
  GLuint m_matSpecularIntensityLocation;  // ������������� ���������
  GLuint m_matSpecularPowerLocation;      // ������� ��������� ���������
                                          
  struct 
  {
    GLuint color;
    GLuint ambientIntensity;
    GLuint direction;
    GLuint diffuseIntensity;
  } m_dirLightLocation;


};

#endif // LIGHTINGTECHNIQUE_H