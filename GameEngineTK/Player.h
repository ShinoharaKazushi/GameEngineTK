/**
*	@file	Player.h
*
*	@brief	���@
*
*	@date	2015/05/08
*
*	@author
*/
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Obj3d.h"
#include "CollisionNode.h"

// ���@
class Player
{
public:
	// �p�[�c
	enum PARTS {
		PARTS_ASHI,
		PARTS_BODY,
		PARTS_ENGINE,
		PARTS_HAND_L,
		PARTS_HAND_R,
		PARTS_TURRET_L,
		PARTS_TURRET_R,
		PARTS_HEAD,
		PARTS_BALL,

		PARTS_NUM,
	};

	// �R���X�g���N�^
	Player(DirectX::Keyboard* keyboard);
	// �f�X�g���N�^
	~Player();
	// ������
	void Initialize();
	// ���t���[���X�V
	void Update();
	// �s��̌v�Z
	void Calc();
	// �`��
	void Draw();
	// �e����
	void FireBullet();
	// �e��t
	void ResetBullet();

	// ���W���擾
	const DirectX::SimpleMath::Vector3& GetTrans();
	const DirectX::SimpleMath::Vector3 & GetRot();
	// ���W��ݒ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// ��]��ݒ�
	void SetRot(const DirectX::SimpleMath::Vector3& rot);
	// ���[���h�s����擾
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	//�����蔻�苅���擾
	const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }

protected:
	// �����o�ϐ�
	// ���{�b�g�̍\���p�[�c
	std::vector<Obj3d>	m_Obj;
	// �L�[�{�[�h
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	// ���@�̃M�~�b�N�Ɏg���p�x
	float m_sinAngle;
	// �e�̑��x
	DirectX::SimpleMath::Vector3 m_BulletVel;
	//
	bool m_FireFlag;
	//�����蔻��m�[�h��
	SphereNode m_CollisionNodeBullet;
};

