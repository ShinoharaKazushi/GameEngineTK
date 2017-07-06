#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// 静的メンバ関数の実体
// カメラ
Camera* Obj3d::m_pCamera;
// 汎用ステート
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;
// デバイス
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;
// コンテキスト
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
// エフェクトファクトリ
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;
//
ID3D11BlendState* Obj3d::s_pBlendStateSubtract;


void Obj3d::InitializeStatic(Camera * pCamera, Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
{
	m_pCamera = pCamera;
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;

	// ステートの設定
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

	// エフェクトファクトリ生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// テクスチャの読み込みフォルダを指定
	m_factory->SetDirectory(L"Resources");
}

void Obj3d::SetSubtractive()
{
	// 減算描画を設定
	m_d3dContext->OMSetBlendState(s_pBlendStateSubtract, nullptr, 0xffffff);
}


Obj3d::Obj3d()
{
	// 変数の初期化
	m_scale = Vector3(1, 1, 1);

	m_pObjParent = nullptr;
	// デフォルトではクォータニオンを使わない
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
	// 行列を計算
	// スケーリング行列
	Matrix scalemat = Matrix::CreateScale(m_scale);
	// 回転行列
	Matrix rotmat;
	if (m_UseQuternion)
	{// クォータニオンから回転行列を計算
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{// オイラー角から回転行列を計算（Z→X→Y）
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
		rotmat = rotmatZ * rotmatX * rotmatY;
	}
	// 平行移動行列
	Matrix transmat = Matrix::CreateTranslation(m_translation);
	// ワールド行列を合成
	m_world = scalemat * rotmat * transmat;
	// 親の行列をかける
	if (m_pObjParent)
	{
		m_world *= m_pObjParent->GetWorld();
	}
}

/**
*	@brief オブジェクトのライティングを無効にする
*/
void Obj3d::DisableLighting()
{
	if (m_model)
	{
		// モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// 自己発光を最大値に
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// エフェクトに含む全ての平行光源分について処理する
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ライトを無効にする
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

	// ワールド行列をSRTの順に合成
	m_world = Matrix::Identity;
	m_world *= scalem;
	m_world *= rotm;
	m_world *= transm;

	// 親行列があれば
	if (m_pObjParent)
	{
		// 親行列を掛ける
		m_world = m_world * m_pObjParent->GetWorld();
	}
}

void Obj3d::Draw()
{
	// モデルを描画
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
//		// 減算描画用の設定関数を渡して描画
//		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, view, projection, false, Obj3d::SetSubtractive);
//	}
//}