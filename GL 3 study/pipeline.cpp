#include "pipeline.h"

// �������� ������� �������������
const Matrix4f& Pipeline::GetWVPTrans()
{
  // ������� ��������������
  Matrix4f ScaleTrans;
  ScaleTrans.InitScaleTransform ( m_scale.x, m_scale.y, m_scale.z );

  // ������� ��������
  Matrix4f RotateTrans;
  RotateTrans.InitRotateTransform ( m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z );

  // ������� �����������
  Matrix4f TranslationTrans;
  TranslationTrans.InitTranslationTransform ( m_worldPos.x, m_worldPos.y, m_worldPos.z );
  
  // ������� ����������� ������
  Matrix4f CameraTranslationTrans;
  CameraTranslationTrans.InitTranslationTransform ( -m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z );
  
  // ������� �������� ������
  Matrix4f CameraRotateTrans;
  CameraRotateTrans.InitCameraTransform ( m_camera.Target, m_camera.Up );
  
  // ������� �������� ����������� ������
  Matrix4f PersProjTrans;
  PersProjTrans.InitPersProjTransform ( m_persProjInfo );

  // ����������� ��� ��������� ������ ������� �������������
  m_transformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * TranslationTrans * RotateTrans * ScaleTrans;
  
  return m_transformation;
}

const Matrix4f& Pipeline::GetWorldTrans ( )
{
  Matrix4f ScaleTrans, RotateTrans, TranslationTrans;

  ScaleTrans.InitScaleTransform ( m_scale.x, m_scale.y, m_scale.z );
  RotateTrans.InitRotateTransform ( m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z );
  TranslationTrans.InitTranslationTransform ( m_worldPos.x, m_worldPos.y, m_worldPos.z );

  m_WorldTransformation = TranslationTrans * RotateTrans * ScaleTrans;
  return m_WorldTransformation;
}