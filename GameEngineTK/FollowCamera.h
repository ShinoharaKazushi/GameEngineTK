/// <summary>
/// ���@�ɒǏ]����J�����N���X
/// </summary>
#pragma once
#include "Camera.h"
#include <Keyboard.h>

class FollowCamera:public Camera
{
public:
	const static float CAMERA_DISTANCE;	//�J��������
	FollowCamera(int m_Width, int m_Height);
	~FollowCamera();
	
	void Update() override;	//���t���[���X�V
	void InitializeTPS();	//TPS�J�����̏�����
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);	//�Ǐ]�Ώۍ��W���Z�b�g
	void SetTargetAngle(float targetangle);	//�Ǐ]�Ώۊp�x���Z�b�g
	void SetKeyboard(DirectX::Keyboard* keyboard);//�L�[�{�[�h�Z�b�g
	
protected:
	//�Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	//�Ǐ]�Ώۂ̊p�x
	float m_targetAngle;
	//�L�[�{�[�h
	DirectX::Keyboard* m_keyboard;
	//�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTraker;
	//FPS�J�����t���O
	bool isFPS;
};
