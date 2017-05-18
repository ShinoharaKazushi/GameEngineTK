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
	m_keyboard = nullptr;
	isFPS = false;
	InitializeTPS();
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

	//�L�[�{�[�h�̏�Ԃ̎擾
	Keyboard::State keystate = m_keyboard->GetState();
	m_keyboardTraker.Update(keystate);

	//C���[����������
	if (m_keyboardTraker.IsKeyPressed(Keyboard::C))
	{
		//�t���O�؂�ւ�
		isFPS = !isFPS;
		if (!isFPS){
			InitializeTPS();
		}
	}
	if (isFPS)
	{
		//FPS�J����
		Vector3 position;
		//���@�̏��0.2m�̈ʒu�ɃJ������u��
		position = m_targetPos + Vector3(0, 0.2f, 0);
		//�Q�Ɠ_���王�_�ւ̍���
		Vector3 cameraV(0.0f, 0.0f, -CAMERA_DISTANCE);
		//���@�̌��ɉ�荞�ނ��߂̉�]
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		//�����x�N�g���̉�]
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		//������Ɛi�񂾈ʒu�����_
		eyepos = position + cameraV * 0.1f;
		//������i�񂾈ʒu���Q�Ɠ_
		refpos = position + cameraV;
	}
	else {
		//TPS�J����
		//�^�[�Q�b�g�̍��W����J�������W�ɒǏ]
		refpos = m_targetPos + Vector3(0, 2, 0);
		//�^�[�Q�b�g���W����J�������W�ւ̍���
		Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);
		//�J�����̎��������̋t�����ɉ�]
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		//�����x�N�g���̉�]
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		//�J�������W���v�Z
		eyepos = refpos + cameraV;
		//���_�����݈ʒu�����Ԃ���
		eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;
		//�Q�Ɠ_�����݈ʒu�����Ԃ���
		refpos = m_refpos + (refpos - m_refpos) * 0.20f;

	}

	SetEyePos(eyepos);
	SetRefPos(refpos);
	//���N���X�̍X�V
	Camera::Update();
}

void FollowCamera::InitializeTPS()
{
	//���_�A�Q�Ɠ_
	Vector3 eyepos, refpos;
	//�^�[�Q�b�g�̍��W����J�������W�ɒǏ]
	refpos = m_targetPos + Vector3(0, 2, 0);
	//�^�[�Q�b�g���W����J�������W�ւ̍���
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);
	//�J�����̎��������̋t�����ɉ�]
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	//�����x�N�g���̉�]
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//�J�������W���v�Z
	eyepos = refpos + cameraV;

	SetEyePos(eyepos);
	SetRefPos(refpos);
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetPos = targetpos;
}

void FollowCamera::SetTargetAngle(float targetangle)
{
	m_targetAngle = targetangle;
}

void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}
