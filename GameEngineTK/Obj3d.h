/// <summary>
/// 3D�I�u�W�F�N�g�̃N���X
/// </summary>
/// 
#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <memory>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include "Camera.h"

class Obj3d
{
	//�ÓI�����o
public:
	//�ÓI�����o�̏�����
	static void InitializeStatic(Camera* pCamera, 
		Microsoft::WRL::ComPtr<ID3D11Device>d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>d3dContext);

private:
	static Camera* m_pCamera;	//�J�����̃|�C���^
	static std::unique_ptr<DirectX::CommonStates> m_states;	//�ėp�X�e�[�g
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;	//�f�o�C�X	
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;//�R���e�L�X�g
	static std::unique_ptr < DirectX::EffectFactory>m_factory;		//�G�t�F�N�g�t�@�N�g��

public:
	Obj3d();	//�R���X�g���N�^

	void LoadModel(const wchar_t* fileName);	//���f���̓ǂݍ���
	void Update();		//�X�V
	void Draw();		//�`��
	//setter
	//�X�P�[�����O�p
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	//���s�ړ��p
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }
	//��]�p�p
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; }
	//
	void SetObjParent(Obj3d* pObjParent) { m_pObjParent = pObjParent; }
;
	//getter
	//�X�P�[�����O�p
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }
	//���s�ړ��p
	const DirectX::SimpleMath::Vector3& GetTransmat() { return m_translation; }
	//��]�p�p
	const DirectX::SimpleMath::Vector3& GetRotation() { return m_rotation; }
	//���[���h�s����擾
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }
	//
	Obj3d* GetObjParent() { return m_pObjParent; }

private:
	//���f���̃��j�[�N�|�C���^
	std::unique_ptr<DirectX::Model>m_model;
	//�X�P�[�����O
	DirectX::SimpleMath::Vector3 m_scale;
	//��]�p
	DirectX::SimpleMath::Vector3 m_rotation;
	//���s�ړ�
	DirectX::SimpleMath::Vector3 m_translation;
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�e�ƂȂ�3D�I�u�W�F�N�g�̃N���X�̃|�C���^
	Obj3d* m_pObjParent;

};

