//-------------------------------------------------------------------------
// カメラを制御をするクラス
//-------------------------------------------------------------------------

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>


class Camera
{
public:
	Camera(int m_Width ,int m_Height);			//コンストラクタ
	virtual ~Camera();	//仮想デストラクタ
	virtual void Update();		//更新
	const DirectX::SimpleMath::Matrix& GetViewMatrix();	//ビュー座標を取得
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix();	//射影行列を取得
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);	//視点座標をセット
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);	//参照点座標をセット
	void SetUpVec(const DirectX::SimpleMath::Vector3& upvec);	//上方向ベクトルをセット
	void SetFovY(float fovY);//垂直方向視野角
	void Setaspect(float aspect);//アスペクト比
	void Setnearclip(float nearclip);//ニアクリップ
	void Setfarclip(float farclip);//ファークリップ

protected:
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Vector3 m_eyepos;	//カメラの位置　視点座標
	DirectX::SimpleMath::Vector3 m_refpos;	//どこの見るのか（注視店・参照点）
	DirectX::SimpleMath::Vector3 m_upvec;		//上方向ベクトル

	//射影行列
	DirectX::SimpleMath::Matrix m_proj;
	float m_fovY;			//垂直方向視野角
	float m_aspect;			//画面横幅と縦幅の比率（アスペクト比）
	float m_nearclip;		//手前の表示限界距離
	float m_farclip;		//奥の表示限界距離
};
