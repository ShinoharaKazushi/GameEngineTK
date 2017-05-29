/// <summary>
/// 3Dオブジェクトのクラス
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
	//静的メンバ
public:
	//静的メンバの初期化
	static void InitializeStatic(Camera* pCamera, 
		Microsoft::WRL::ComPtr<ID3D11Device>d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>d3dContext);

private:
	static Camera* m_pCamera;	//カメラのポインタ
	static std::unique_ptr<DirectX::CommonStates> m_states;	//汎用ステート
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;	//デバイス	
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;//コンテキスト
	static std::unique_ptr < DirectX::EffectFactory>m_factory;		//エフェクトファクトリ

public:
	Obj3d();	//コンストラクタ

	void LoadModel(const wchar_t* fileName);	//モデルの読み込み
	void Update();		//更新
	void Draw();		//描画
	//setter
	//スケーリング用
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	//平行移動用
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }
	//回転角用
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; }
	//
	void SetObjParent(Obj3d* pObjParent) { m_pObjParent = pObjParent; }
;
	//getter
	//スケーリング用
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }
	//平行移動用
	const DirectX::SimpleMath::Vector3& GetTransmat() { return m_translation; }
	//回転角用
	const DirectX::SimpleMath::Vector3& GetRotation() { return m_rotation; }
	//ワールド行列を取得
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }
	//
	Obj3d* GetObjParent() { return m_pObjParent; }

private:
	//モデルのユニークポインタ
	std::unique_ptr<DirectX::Model>m_model;
	//スケーリング
	DirectX::SimpleMath::Vector3 m_scale;
	//回転角
	DirectX::SimpleMath::Vector3 m_rotation;
	//平行移動
	DirectX::SimpleMath::Vector3 m_translation;
	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//親となる3Dオブジェクトのクラスのポインタ
	Obj3d* m_pObjParent;

};

