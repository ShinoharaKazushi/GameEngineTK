/// <summary>
/// ���@�ɒǏ]����J�����N���X
/// </summary>
#pragma once
#include "Camera.h"

class FollowCamera:public Camera
{
public:
	const static float CAMERA_DISTANCE;	//�J��������
	FollowCamera(int m_Width, int m_Height);
	~FollowCamera();
	//���t���[���X�V
	void Update() override;
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);	//�Ǐ]�Ώۍ��W���Z�b�g
	void SetTargetAngle(float targetangle);	//�Ǐ]�Ώۊp�x���Z�b�g
	
protected:
	//�Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	//�Ǐ]�Ώۂ̊p�x
	float m_targetAngle;
};
