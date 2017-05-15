#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�ÓI�����o�ϐ���
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int m_Width, int m_Height)
	: Camera(m_Width, m_Height)
{

	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
}

FollowCamera::~FollowCamera()
{
}

void FollowCamera::Update()
{
	//�J��������
	const float CAMERA_DISTANCE = 5.0f;
	//���_�A�Q�Ɠ_
	Vector3 eyepos, refpos;
	//�^�[�Q�b�g�̍��W����J�������W�ɒǏ]
	refpos = m_targetPos + Vector3(0, 2, 0);
	//�^�[�Q�b�g���W����J�������W�ւ̍���
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);
	//�J�����̎��������̋t�����ɉ�]
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//�J�������W���v�Z
	eyepos = refpos + cameraV;

	SetEyePos(eyepos);
	SetRefPos(refpos);
	//���N���X�̍X�V
	Camera::Update();
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetPos = targetpos;
}

void FollowCamera::SetTargetAngle(float targetangle)
{
	m_targetAngle = targetangle;
}
