#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//静的メンバ変数の
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int m_Width, int m_Height)
	: Camera(m_Width, m_Height)
{

	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
}

FollowCamera::~FollowCamera()
{
}

void FollowCamera::Update()
{
	//カメラ距離
	const float CAMERA_DISTANCE = 5.0f;
	//視点、参照点
	Vector3 eyepos, refpos;
	//ターゲットの座標からカメラ座標に追従
	refpos = m_targetPos + Vector3(0, 2, 0);
	//ターゲット座標からカメラ座標への差分
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);
	//カメラの視線方向の逆方向に回転
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//カメラ座標を計算
	eyepos = refpos + cameraV;

	SetEyePos(eyepos);
	SetRefPos(refpos);
	//基底クラスの更新
	Camera::Update();
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetPos = targetpos;
}

void FollowCamera::SetTargetAngle(float targetangle)
{
	m_targetAngle = targetangle;
}
