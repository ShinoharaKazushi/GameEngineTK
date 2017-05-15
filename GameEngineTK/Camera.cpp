#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera(int m_Width, int m_Height)
{
	//�����o�֐��̏�����
	//�J�����̈ʒu�@���_���W
	m_eyepos=Vector3(0, 0, 5.0f);
	//�ǂ��̌���̂��i�����_�E�Q�Ɠ_�j
	m_refpos = Vector3(0, 0, 0);
	//������x�N�g��
	m_upvec = Vector3(0, 1, 0);
	m_upvec.Normalize();
	
	//������������p
	m_fovY = XMConvertToRadians(60.0f);
	//��ʉ����Əc���̔䗦�i�A�X�y�N�g��j
	m_aspect = (float)m_Width / m_Height;
	//��O�̕\�����E����
	m_nearclip = 0.1f;
	//���̕\�����E����
	m_farclip = 1000.0f;

	//�r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//�ˉe�s��̐���
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearclip, m_farclip);

}

/// <summary>
/// ���z�f�X�g���N�^
/// </summary>
Camera::~Camera()
{

}

void Camera::Update()
{
	//�r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//�ˉe�s��̐���
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearclip, m_farclip);

}
const Matrix& Camera::GetViewMatrix()
{
	return m_view;
}
const Matrix& Camera::GetProjectionMatrix()
{
	return m_proj;
}
void Camera::SetEyePos(const Vector3& eyepos)
{
	m_eyepos = eyepos;
}
void Camera::SetRefPos(const Vector3& refpos)
{
	m_refpos = refpos;
}
void Camera::SetUpVec(const Vector3& upvec)
{
	m_upvec = upvec;
}
void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}
void Camera::Setaspect(float aspect)
{
	m_aspect = aspect;
}
void Camera::Setnearclip(float nearclip)
{
	m_nearclip = nearclip;
}
void Camera::Setfarclip(float farclip)
{
	m_farclip = farclip;
}