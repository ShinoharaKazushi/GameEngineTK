//-------------------------------------------------------------------------
// �J�����𐧌������N���X
//-------------------------------------------------------------------------

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>


class Camera
{
public:
	Camera(int m_Width ,int m_Height);			//�R���X�g���N�^
	virtual ~Camera();	//���z�f�X�g���N�^
	virtual void Update();		//�X�V
	const DirectX::SimpleMath::Matrix& GetViewMatrix();	//�r���[���W���擾
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix();	//�ˉe�s����擾
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);	//���_���W���Z�b�g
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);	//�Q�Ɠ_���W���Z�b�g
	void SetUpVec(const DirectX::SimpleMath::Vector3& upvec);	//������x�N�g�����Z�b�g
	void SetFovY(float fovY);//������������p
	void Setaspect(float aspect);//�A�X�y�N�g��
	void Setnearclip(float nearclip);//�j�A�N���b�v
	void Setfarclip(float farclip);//�t�@�[�N���b�v

protected:
	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Vector3 m_eyepos;	//�J�����̈ʒu�@���_���W
	DirectX::SimpleMath::Vector3 m_refpos;	//�ǂ��̌���̂��i�����X�E�Q�Ɠ_�j
	DirectX::SimpleMath::Vector3 m_upvec;		//������x�N�g��

	//�ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;
	float m_fovY;			//������������p
	float m_aspect;			//��ʉ����Əc���̔䗦�i�A�X�y�N�g��j
	float m_nearclip;		//��O�̕\�����E����
	float m_farclip;		//���̕\�����E����
};
