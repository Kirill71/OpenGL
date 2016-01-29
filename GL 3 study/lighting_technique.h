#ifndef LIGHTINGTECHNIQUE_H
#define LIGHTINGTECHNIQUE_H

#pragma once

#include "technique.h"
#include "math3d.h"

const unsigned int MAX_POINT_LIGHTS = 3u; // ������������ ���������� ���������� ��������� �����
const unsigned int MAX_SPOT_LIGHTS  = 2u; // ������������ ���������� ���������� ������������� �����


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base light for diferent sourse
struct BaseLight
{
  Vector3f  color;            // ���� �����
  float     ambientIntensity; // ������������� �������� ����� (������������)
  float     diffuseIntensity; // ������������� ����������� ����� (�� ���������)

  BaseLight ( ) :
    color             ( Vector3f ( 0.0f, 0.0f, 0.0f ) ),
    ambientIntensity  (0.0f),
    diffuseIntensity  (0.0f)
  { }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Direction light
struct DirectionLight : BaseLight
{
  Vector3f  direction;  // ����������� ��������� (�� ������������ �����)

  DirectionLight ( ) :
    direction ( Vector3f ( 0.0f, 0.0f, 0.0f ) )
  { }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Point light
struct PointLight : BaseLight
{
  Vector3f  position;

  // ��������� ��������� ��� ����� ������� ���������� �������� ������
  struct
  {
    float constant;
    float linear;
    float exp;
  } attenuation; // ���������

  PointLight ( ):
    position ( Vector3f ( 0.0f, 0.0f, 0.0f ) )
  {
    attenuation.constant = 1.0f;
    attenuation.linear = 0.0f;
    attenuation.exp = 0.0f;
  }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Spot light
struct SpotLight : public PointLight
{
  Vector3f  direction;
  float     cutOff;    // ������� ���������, ������������ ���� ����� ������������ ����� � �������� �� ��������, ������� ��� ������� ��� ������� �����

  SpotLight ( ) :
    direction   ( Vector3f ( 0.0f, 0.0f, 0.0f ) ),
    cutOff      (0.0f)
  { }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LightingTechnique : public Technique
{
public:

  LightingTechnique ( );
  virtual bool Init ( );

  // ������ ������
  bool SetShader ( const char* pFilename, char* &pShaderText );
  // ������� �������������� (world view projection)
  void SetWVP ( const Matrix4f& WVP );
  // ������� ������� ��������������
  void SetWorldMatrix ( const Matrix4f& WorldInverse );
  // ��������� ������� �������� � ��������
  void SetTextureUnit ( unsigned int textureUnit ); 

  // ������������ ���������:
  // ������������� ������������ ���������
  void SetDirectionalLight ( const DirectionLight& light );

  // ���������� ����:
  // ������������� � ������� ������� �������
  void SetEyeWorldPos           ( const Vector3f& eyeWorldPos );  
  // ������������� � ������� ������������� ����������� �����
  void SetMatSpecularIntensity  ( float intensity );              
  // ������������� � ������� ������� ��������� ���������
  void SetMatSpecularPower      ( float power );                  

  // �������� ����:
  // ������������� ��������� ���������� �����
  void SetPointLights ( unsigned int numLights, const PointLight* pLights );

  // ������������ ����:
  // ������������� ��������� ���������� �����
  void LightingTechnique::SetSpotLights ( unsigned int numLights, const SpotLight* pLights );
  
private:

  // ������������ ������� (����� �) � ��������:
  // 1 ������ ��������������:

  GLuint m_WVPLocation;
  GLuint m_WorldMatrixLocation;

  GLuint m_samplerLocation; // �-������� �������� ��������
                               
  // 2 ����������� �����:

  GLuint m_eyeWorldPosLocation;           // �-������� �������
  GLuint m_matSpecularIntensityLocation;  // �-������� ������������� ���������
  GLuint m_matSpecularPowerLocation;      // �-������� ������� ��������� ���������

  // 3 ������������� �����
  struct 
  {
    GLuint color;
    GLuint ambientIntensity;
    GLuint direction;
    GLuint diffuseIntensity;

  } m_dirLightLocation; // �-������� ������������� ��������� �����

  // 4 ��������� �����
  struct 
  {
    GLuint color;
    GLuint ambientIntensity;
    GLuint diffuseIntensity;
    GLuint position;

    struct
    {
      GLuint constant;
      GLuint linear;
      GLuint exp;
    } atten;

  } m_pointLightsLocation[MAX_POINT_LIGHTS]; // ������ �-������� ���������� ��������� �����

  GLuint m_numPointLightsLocation; // �-������� ���������� �������� ���������� �����

  // 5 ������������� �����
  struct 
  {
    GLuint color;
    GLuint ambientIntensity;
    GLuint diffuseIntensity;
    GLuint position;
    GLuint direction;
    GLuint cutOff;
    
    struct
    {
      GLuint constant;
      GLuint linear;
      GLuint exp;
    } atten;

  } m_spotLightsLocation[MAX_SPOT_LIGHTS]; // ������ �-������� ���������� ������������� �����

  GLuint m_numSpotLightsLocation; // �-������� ���������� ������������ ���������� �����

};

#endif /* LIGHTINGTECHNIQUE_H */