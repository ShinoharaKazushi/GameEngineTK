#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�ÓI�����o�֐��̎���
 Camera* Obj3d::m_pCamera;	//�J�����̃|�C���^
 std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;	//�ėp�X�e�[�g
 Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;	//�f�o�C�X	
 Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;//�R���e�L�X�g
 std::unique_ptr < DirectX::EffectFactory> Obj3d::m_factory;		//�G�t�F�N�g�t�@�N�g��

void Obj3d::InitializeStatic(Camera * pCamera, Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
{
	m_pCamera = pCamera;
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	//�X�e�[�g�̐ݒ�
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	//�G�t�F�N�g�t�@�N�g���̐���
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//�e�N�X�`���̓ǂݍ��݃t�H���_���w��
	m_factory->SetDirectory(L"Resources");
}

Obj3d::Obj3d()
{
	//�ϐ��̏�����
	m_scale = Vector3(1, 1, 1);
	m_pObjParent = nullptr;
}

void Obj3d::LoadModel(const wchar_t * fileName)
{
	//���f���̓ǂݍ���
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
}
void Obj3d::Update()
{
	//�s����v�Z
	//�X�P�[�����O�s��
	Matrix scalemat = Matrix::CreateScale(m_scale);
	//��]�s��
	Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
	Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
	Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
	Matrix rotmat = rotmatZ * rotmatX * rotmatY;
	//���s�ړ��s��
	Matrix transmat = Matrix::CreateTranslation(m_translation);
	//���[���h�s��̍���
	m_world = scalemat * rotmat * transmat;
	//�e�̍s���������
	if (m_pObjParent) 
	{
		m_world *= m_pObjParent->GetWorld();
	}
}
void Obj3d::Draw()
{
	//Model�̕`��
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
	}
}