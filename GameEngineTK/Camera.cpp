#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera(int m_Width, int m_Height)
{
	//メンバ関数の初期化
	//カメラの位置　視点座標
	m_eyepos=Vector3(0, 0, 5.0f);
	//どこの見るのか（注視点・参照点）
	m_refpos = Vector3(0, 0, 0);
	//上方向ベクトル
	m_upvec = Vector3(0, 1, 0);
	m_upvec.Normalize();
	
	//垂直方向視野角
	m_fovY = XMConvertToRadians(60.0f);
	//画面横幅と縦幅の比率（アスペクト比）
	m_aspect = (float)m_Width / m_Height;
	//手前の表示限界距離
	m_nearclip = 0.1f;
	//奥の表示限界距離
	m_farclip = 1000.0f;

	//ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//射影行列の生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearclip, m_farclip);

}

/// <summary>
/// 仮想デストラクタ
/// </summary>
Camera::~Camera()
{

}

void Camera::Update()
{
	//ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//射影行列の生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearclip, m_farclip);

}
const Matrix& Camera::GetViewMatrix()
{
	return m_view;
}
const Matrix& Camera::GetProjectionMatrix()
{
	return m_proj;
}
void Camera::SetEyePos(const Vector3& eyepos)
{
	m_eyepos = eyepos;
}
void Camera::SetRefPos(const Vector3& refpos)
{
	m_refpos = refpos;
}
void Camera::SetUpVec(const Vector3& upvec)
{
	m_upvec = upvec;
}
void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}
void Camera::Setaspect(float aspect)
{
	m_aspect = aspect;
}
void Camera::Setnearclip(float nearclip)
{
	m_nearclip = nearclip;
}
void Camera::Setfarclip(float farclip)
{
	m_farclip = farclip;
}