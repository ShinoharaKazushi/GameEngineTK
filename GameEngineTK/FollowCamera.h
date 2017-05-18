/// <summary>
/// 自機に追従するカメラクラス
/// </summary>
#pragma once
#include "Camera.h"
#include <Keyboard.h>

class FollowCamera:public Camera
{
public:
	const static float CAMERA_DISTANCE;	//カメラ距離
	FollowCamera(int m_Width, int m_Height);
	~FollowCamera();
	
	void Update() override;	//毎フレーム更新
	void InitializeTPS();	//TPSカメラの初期化
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);	//追従対象座標をセット
	void SetTargetAngle(float targetangle);	//追従対象角度をセット
	void SetKeyboard(DirectX::Keyboard* keyboard);//キーボードセット
	
protected:
	//追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	//追従対象の角度
	float m_targetAngle;
	//キーボード
	DirectX::Keyboard* m_keyboard;
	//キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTraker;
	//FPSカメラフラグ
	bool isFPS;
};
