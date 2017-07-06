#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �ÓI�����o�֐��̎���
// �J����
Camera* Obj3d::m_pCamera;
// �ėp�X�e�[�g
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;
// �f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;
// �R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
// �G�t�F�N�g�t�@�N�g��
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;
//
ID3D11BlendState* Obj3d::s_pBlendStateSubtract;


void Obj3d::InitializeStatic(Camera * pCamera, Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
{
	m_pCamera = pCamera;
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;

	// �X�e�[�g�̐ݒ�
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	// �G�t�F�N�g�t�@�N�g������
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// �e�N�X�`���̓ǂݍ��݃t�H���_���w��
	m_factory->SetDirectory(L"Resources");
}

void Obj3d::SetSubtractive()
{
	// ���Z�`���ݒ�
	m_d3dContext->OMSetBlendState(s_pBlendStateSubtract, nullptr, 0xffffff);
}


Obj3d::Obj3d()
{
	// �ϐ��̏�����
	m_scale = Vector3(1, 1, 1);

	m_pObjParent = nullptr;
	// �f�t�H���g�ł̓N�H�[�^�j�I�����g��Ȃ�
	m_UseQuternion = false;
}

void Obj3d::LoadModel(const wchar_t * fileName)
{
	m_model = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		fileName,
		*m_factory
	);
}

void Obj3d::Update()
{
	// �s����v�Z
	// �X�P�[�����O�s��
	Matrix scalemat = Matrix::CreateScale(m_scale);
	// ��]�s��
	Matrix rotmat;
	if (m_UseQuternion)
	{// �N�H�[�^�j�I�������]�s����v�Z
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{// �I�C���[�p�����]�s����v�Z�iZ��X��Y�j
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
		rotmat = rotmatZ * rotmatX * rotmatY;
	}
	// ���s�ړ��s��
	Matrix transmat = Matrix::CreateTranslation(m_translation);
	// ���[���h�s�������
	m_world = scalemat * rotmat * transmat;
	// �e�̍s���������
	if (m_pObjParent)
	{
		m_world *= m_pObjParent->GetWorld();
	}
}

/**
*	@brief �I�u�W�F�N�g�̃��C�e�B���O�𖳌��ɂ���
*/
void Obj3d::DisableLighting()
{
	if (m_model)
	{
		// ���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// ���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// ���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// ���Ȕ������ő�l��
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// �G�t�F�N�g�Ɋ܂ޑS�Ă̕��s�������ɂ��ď�������
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ���C�g�𖳌��ɂ���
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}

void Obj3d::Calc()
{
	Matrix scalem;
	Matrix rotm;
	Matrix transm;

	scalem = Matrix::CreateScale(m_scale);

	if (m_UseQuternion)
	{
		rotm = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{
		rotm = Matrix::CreateFromYawPitchRoll(m_rotation.y, m_rotation.x, m_rotation.z);
	}

	transm = Matrix::CreateTranslation(m_translation);

	// ���[���h�s���SRT�̏��ɍ���
	m_world = Matrix::Identity;
	m_world *= scalem;
	m_world *= rotm;
	m_world *= transm;

	// �e�s�񂪂����
	if (m_pObjParent)
	{
		// �e�s����|����
		m_world = m_world * m_pObjParent->GetWorld();
	}
}

void Obj3d::Draw()
{
	// ���f����`��
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(),
			*m_states,
			m_world,
			m_pCamera->GetView(),
			m_pCamera->GetProj());
	}
}

//void Obj3d::DrawSubtractive()
//{
//	if (m_model)
//	{
//		assert(s_pCamera);
//		const Matrix& view = m_pCamera->GetView();
//		const Matrix& projection = m_pCamera->GetProj();
//
//		assert(s_pDeviceContext);
//		assert(s_pStates);
//
//		// ���Z�`��p�̐ݒ�֐���n���ĕ`��
//		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, view, projection, false, Obj3d::SetSubtractive);
//	}
//}