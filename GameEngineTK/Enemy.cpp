//--------------------------------------------------------------------------------------
// �t�@�C����: Enemy.cpp
// �쐬��:
// �쐬��:
// ����:
//--------------------------------------------------------------------------------------

#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Enemy::Enemy(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	m_sinAngle = 0.0f;

	Initialize();
}

Enemy::~Enemy()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void Enemy::Initialize()
{
	//���@�p�[�c�̃��[�h
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_ASHI].LoadModel(L"Resources/ashi.cmo");
	m_Obj[PARTS_BODY].LoadModel(L"Resources/body.cmo");
	m_Obj[PARTS_ENGINE].LoadModel(L"Resources/engine.cmo");
	m_Obj[PARTS_HAND_L].LoadModel(L"Resources/hand.cmo");
	m_Obj[PARTS_HAND_R].LoadModel(L"Resources/handr.cmo");
	m_Obj[PARTS_TURRET_L].LoadModel(L"Resources/turret.cmo");
	m_Obj[PARTS_TURRET_R].LoadModel(L"Resources/turret.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"Resources/Head.cmo");

	//�e�q�֌W�̍\�z(�q���ɐe���Z�b�g)
	m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_ASHI]);
	m_Obj[PARTS_ENGINE].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HAND_L].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HAND_R].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_TURRET_L].SetObjParent(&m_Obj[PARTS_HAND_L]);
	m_Obj[PARTS_TURRET_R].SetObjParent(&m_Obj[PARTS_HAND_R]);
	//�e����̃I�t�Z�b�g(���[�J���̍��W����)
	m_Obj[PARTS_BODY].SetTranslation(Vector3(0, 0.4f, 0.1f));

	m_Obj[PARTS_ENGINE].SetTranslation(Vector3(0, 0.4f, 0.6f));
	m_Obj[PARTS_ENGINE].SetRotation(Vector3(0, XMConvertToRadians(180), 0));

	m_Obj[PARTS_HEAD].SetTranslation(Vector3(0, 0.5f, 0));

	m_Obj[PARTS_HAND_L].SetTranslation(Vector3(-0.3f, 0.4f, 0));
	m_Obj[PARTS_HAND_L].SetRotation(Vector3(0, 0, 0));

	m_Obj[PARTS_HAND_R].SetTranslation(Vector3(0.3f, 0.4f, 0));

	m_Obj[PARTS_TURRET_L].SetTranslation(Vector3(-0.2, 0.2f, 0));

	m_Obj[PARTS_TURRET_R].SetTranslation(Vector3(0.2, 0.2f, 0));

	// �������W�������_���Ɍ���
	Vector3 pos;

	pos.x = rand() % 10;
	pos.z = rand() % 10;

	SetTrans(pos);

	m_Timer = 0;
	m_DistAngle = 0;

	//����̂����蔻��m�[�h�ݒ�
	{
		m_CollisionNodeBody.Initialize();
		//����p�[�c�ɂԂ牺����
		m_CollisionNodeBody.SetParent(&m_Obj[PARTS_BODY]);
		//����p�[�c����̃I�t�Z�b�g
		m_CollisionNodeBody.SetTrans(Vector3(0, 0.5f, 0));
		//�����蔻��̔��a
		m_CollisionNodeBody.SetLocalRadius(1.0f);
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void Enemy::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	// ���@�p�[�c�̃M�~�b�N
	{
		// ���@�̊p�x��ϓ�
		Vector3 angle;

		angle = m_Obj[PARTS_ENGINE].GetRotation();
		m_Obj[PARTS_ENGINE].SetRotation(
			angle + Vector3(0.2f, 0.1f, 0));

		// �T�C���̈����̊p�x�����񂾂񑝂���
		m_sinAngle += 0.1f;
		// �t�@���̊p�x���������肫���肷��(-1�`+1)
		float fan_angle = sinf(m_sinAngle) * 3.0f;
		/*m_Obj[PLAYER_PARTS_FAN].SetRotation(
		Vector3(0, 0, fan_angle));*/
		//m_Obj[PARTS_FAN].SetTranslation(
		//	Vector3(sinf(m_sinAngle), 0, cosf(m_sinAngle)*3.0f));
	}

	m_Timer--;
	if (m_Timer < 0)
	{
		// �ڕW�p�x�̍Ē��I
		m_Timer = 60;
		// -0.5�`+0.5�̗���
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		// -90�`+90�̗���
		rnd *= 180.0f;

		rnd = XMConvertToRadians(rnd);

		m_DistAngle += rnd;
	}

	{
		// ���@�̊p�x��ϓ�
		Vector3 rot = GetRot();

		float angle = m_DistAngle - rot.y;

		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		rot.y += angle *0.05f;

		SetRot(rot);
	}

	// �@�̂̌����Ă�������ɑO�i
	{
		// �ړ��x�N�g��
		Vector3 moveV(0, 0, -0.1f);
		// ���̊p�x�ɍ��킹�Ĉړ��x�N�g������]
		// ��]�s��
		float angle = m_Obj[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTranslation();
		pos += moveV;
		m_Obj[0].SetTranslation(pos);
	}

	// �s��X�V
	Calc();
}

//-----------------------------------------------------------------------------
// �s��X�V
//-----------------------------------------------------------------------------
void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}
	//�m�[�h�X�V
	m_CollisionNodeBody.Update();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}
	//
	m_CollisionNodeBody.Draw();
}

const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return m_Obj[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3& Enemy::GetRot()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return m_Obj[0].GetRotation();
}

void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].SetTranslation(trans);
}

void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
{
	// �^���N�p�[�c�̃��[���h�s���Ԃ�
	return m_Obj[0].GetWorld();
}
