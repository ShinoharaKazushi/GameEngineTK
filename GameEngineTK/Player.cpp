//--------------------------------------------------------------------------------------
// �t�@�C����: Player.cpp
// �쐬��:
// �쐬��:
// ����:
//--------------------------------------------------------------------------------------

#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	m_sinAngle = 0.0f;

	Initialize();
}

Player::~Player()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void Player::Initialize()
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
	m_Obj[PARTS_BALL].LoadModel(L"Resources/boll.cmo");

	//�e�q�֌W�̍\�z(�q���ɐe���Z�b�g)
	m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_ASHI]);
	m_Obj[PARTS_ENGINE].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HAND_L].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HAND_R].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_TURRET_L].SetObjParent(&m_Obj[PARTS_HAND_L]);
	m_Obj[PARTS_TURRET_R].SetObjParent(&m_Obj[PARTS_HAND_R]);
	m_Obj[PARTS_BALL].SetObjParent(&m_Obj[PARTS_TURRET_R]);
	
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

	m_Obj[PARTS_BALL].SetTranslation(Vector3(0, 0.3f, -1));
	m_Obj[PARTS_BALL].SetScale(Vector3(0.3f, 0.3f, 0.3f));

	//���˒��ł͂Ȃ�
	m_FireFlag = false;

	//����̂����蔻��m�[�h�ݒ�
	{
		m_CollisionNodeBullet.Initialize();
		//����p�[�c�ɂԂ牺����
		m_CollisionNodeBullet.SetParent(&m_Obj[PARTS_BALL]);
		//����p�[�c����̃I�t�Z�b�g
		m_CollisionNodeBullet.SetTrans(Vector3(0, 0, 0));
		//�����蔻��̔��a
		m_CollisionNodeBullet.SetLocalRadius(0.3f);
	}
}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void Player::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	// ���@�p�[�c�̃M�~�b�N
	{
		// ���@�̊p�x��ϓ�
		Vector3 angle;

		angle = m_Obj[PARTS_ENGINE].GetRotation();
		m_Obj[PARTS_ENGINE].SetRotation(
			angle + Vector3(0, 0, 0.1));

		// �T�C���̈����̊p�x�����񂾂񑝂���
		m_sinAngle += 0.1f;
		// �t�@���̊p�x���������肫���肷��(-1�`+1)
		float fan_angle = sinf(m_sinAngle) * 3.0f;
		/*m_Obj[PARTS_ENGINE].SetRotation(
		Vector3(0, 0, fan_angle));*/
		//m_Obj[PARTS_ENGINE].SetTranslation(
		//	Vector3(sinf(m_sinAngle), 0, cosf(m_sinAngle)*3.0f));
	}


	// A�L�[�������Ă����
	if (keystate.A)
	{
		// ���@�̊p�x��ϓ�
		float angle = m_Obj[0].GetRotation().y;
		m_Obj[0].SetRotation(
			Vector3(0, angle + 0.03f, 0));
	}

	// D�L�[�������Ă����
	if (keystate.D)
	{
		// ���@�̊p�x��ϓ�
		float angle = m_Obj[0].GetRotation().y;
		m_Obj[0].SetRotation(
			Vector3(0, angle - 0.03f, 0));
	}

	// W�L�[�������Ă����
	if (keystate.W)
	{
		// �ړ��x�N�g��
		Vector3 moveV(0, 0, -0.1f);
		// ��]�s��
		float angle = m_Obj[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTranslation();
		pos += moveV;
		m_Obj[0].SetTranslation(pos);

		Vector3 angleE;
		angleE = m_Obj[PARTS_ENGINE].GetRotation();
		m_Obj[PARTS_ENGINE].SetRotation(
			angleE + Vector3(0, 0, 0.4));
	}

	// S�L�[�������Ă����
	if (keystate.S)
	{
		// �ړ��x�N�g��
		Vector3 moveV(0, 0, +0.1f);
		// ��]�s��
		float angle = m_Obj[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// ���@�̍��W���ړ�
		Vector3 pos = m_Obj[0].GetTranslation();
		pos += moveV;
		m_Obj[0].SetTranslation(pos);

		Vector3 angleE;
		angleE = m_Obj[PARTS_ENGINE].GetRotation();
		m_Obj[PARTS_ENGINE].SetRotation(
			angleE + Vector3(0, 0, -0.4));
	}
	if (keystate.Left)
	{
		Vector3 angleH;
		angleH = m_Obj[PARTS_TURRET_R].GetRotation();
		m_Obj[PARTS_TURRET_R].SetRotation(
			angleH + Vector3(0, 0.05, 0));
	}
	if (keystate.Right)
	{
		Vector3 angleH;
		angleH = m_Obj[PARTS_TURRET_R].GetRotation();
		m_Obj[PARTS_TURRET_R].SetRotation(
			angleH + Vector3(0, -0.05, 0));
	}

	// �e�ۂ��i�ޏ���
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		if (m_FireFlag)
		{
			ResetBullet();
		}
		else {
			FireBullet();
		}
	}
	if (m_FireFlag)
	{
		// ���@�̍��W���ړ�
		Vector3 pos = m_Obj[PARTS_BALL].GetTranslation();
		pos += m_BulletVel;
		m_Obj[PARTS_BALL].SetTranslation(pos);
	}

	// �s��X�V
	Calc();

}

//-----------------------------------------------------------------------------
// �s��X�V
//-----------------------------------------------------------------------------
void Player::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}
	//�m�[�h�X�V
	m_CollisionNodeBullet.Update();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Player::Draw()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}
	//
	m_CollisionNodeBullet.Draw();
}

void Player::FireBullet()
{
	if (m_FireFlag)  return;

	m_FireFlag = true;

	// ���[���h�s����擾
	Matrix worldm = m_Obj[PARTS_BALL].GetWorld();

	Vector3 scale;	// ���[���h�X�P�[�����O
	Quaternion rotation;	// ���[���h��]
	Vector3 translation;	// ���[���h���W

	// ���[���h�s�񂩂�e�v�f�����o��
	worldm.Decompose(scale, rotation, translation);

	// �e�q�֌W���������ăp�[�c��Ɨ�������
	m_Obj[PARTS_BALL].SetObjParent(nullptr);
	m_Obj[PARTS_BALL].SetScale(scale);
	m_Obj[PARTS_BALL].SetRotationQ(rotation);
	m_Obj[PARTS_BALL].SetTranslation(translation);

	// �e�̑��x��ݒ�
	m_BulletVel = Vector3(0, 0, -0.3f);
	// �e�̌����ɍ��킹�Đi�s��������]
	m_BulletVel = Vector3::Transform(m_BulletVel, rotation);
}

void Player::ResetBullet()
{
	if (!m_FireFlag) return;
	Quaternion rotation;	// ���[���h��]
	m_Obj[PARTS_BALL].SetRotationQ(rotation);
	m_Obj[PARTS_BALL].SetObjParent(&m_Obj[PARTS_TURRET_R]);
	m_Obj[PARTS_BALL].SetTranslation(Vector3(0, 0.3f, -1));

	m_FireFlag = false;
}

const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return m_Obj[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3& Player::GetRot()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return m_Obj[0].GetRotation();
}

void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].SetTranslation(trans);
}

void Player::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Player::GetLocalWorld()
{
	// �^���N�p�[�c�̃��[���h�s���Ԃ�
	return m_Obj[0].GetWorld();
}
