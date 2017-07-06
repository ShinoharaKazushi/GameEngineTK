//--------------------------------------------------------------------------------------
// ファイル名: Player.cpp
// 作成者:
// 作成日:
// 説明:
//--------------------------------------------------------------------------------------

#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	m_sinAngle = 0.0f;

	Initialize();
}

Player::~Player()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void Player::Initialize()
{
	//自機パーツのロード
	m_Obj.resize(PARTS_NUM);
	m_Obj[PARTS_ASHI].LoadModel(L"Resources/ashi.cmo");
	m_Obj[PARTS_BODY].LoadModel(L"Resources/body.cmo");
	m_Obj[PARTS_ENGINE].LoadModel(L"Resources/engine.cmo");
	m_Obj[PARTS_HAND_L].LoadModel(L"Resources/hand.cmo");
	m_Obj[PARTS_HAND_R].LoadModel(L"Resources/handr.cmo");
	m_Obj[PARTS_TURRET_L].LoadModel(L"Resources/turret.cmo");
	m_Obj[PARTS_TURRET_R].LoadModel(L"Resources/turret.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"Resources/Head.cmo");
	m_Obj[PARTS_BALL].LoadModel(L"Resources/boll.cmo");

	//親子関係の構築(子供に親をセット)
	m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_ASHI]);
	m_Obj[PARTS_ENGINE].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HAND_L].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HAND_R].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_TURRET_L].SetObjParent(&m_Obj[PARTS_HAND_L]);
	m_Obj[PARTS_TURRET_R].SetObjParent(&m_Obj[PARTS_HAND_R]);
	m_Obj[PARTS_BALL].SetObjParent(&m_Obj[PARTS_TURRET_R]);
	
	//親からのオフセット(ローカルの座標ずれ)
	m_Obj[PARTS_BODY].SetTranslation(Vector3(0, 0.4f, 0.1f));

	m_Obj[PARTS_ENGINE].SetTranslation(Vector3(0, 0.4f, 0.6f));
	m_Obj[PARTS_ENGINE].SetRotation(Vector3(0, XMConvertToRadians(180), 0));

	m_Obj[PARTS_HEAD].SetTranslation(Vector3(0, 0.5f, 0));

	m_Obj[PARTS_HAND_L].SetTranslation(Vector3(-0.3f, 0.4f, 0));
	m_Obj[PARTS_HAND_L].SetRotation(Vector3(0, 0, 0));

	m_Obj[PARTS_HAND_R].SetTranslation(Vector3(0.3f, 0.4f, 0));

	m_Obj[PARTS_TURRET_L].SetTranslation(Vector3(-0.2, 0.2f, 0));

	m_Obj[PARTS_TURRET_R].SetTranslation(Vector3(0.2, 0.2f, 0));

	m_Obj[PARTS_BALL].SetTranslation(Vector3(0, 0.3f, -1));
	m_Obj[PARTS_BALL].SetScale(Vector3(0.3f, 0.3f, 0.3f));

	//発射中ではない
	m_FireFlag = false;

	//武器のあたり判定ノード設定
	{
		m_CollisionNodeBullet.Initialize();
		//武器パーツにぶら下げる
		m_CollisionNodeBullet.SetParent(&m_Obj[PARTS_BALL]);
		//武器パーツからのオフセット
		m_CollisionNodeBullet.SetTrans(Vector3(0, 0, 0));
		//あたり判定の半径
		m_CollisionNodeBullet.SetLocalRadius(0.3f);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void Player::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	// 自機パーツのギミック
	{
		// 自機の角度を変動
		Vector3 angle;

		angle = m_Obj[PARTS_ENGINE].GetRotation();
		m_Obj[PARTS_ENGINE].SetRotation(
			angle + Vector3(0, 0, 0.1));

		// サインの引数の角度がだんだん増える
		m_sinAngle += 0.1f;
		// ファンの角度がいったりきたりする(-1～+1)
		float fan_angle = sinf(m_sinAngle) * 3.0f;
		/*m_Obj[PARTS_ENGINE].SetRotation(
		Vector3(0, 0, fan_angle));*/
		//m_Obj[PARTS_ENGINE].SetTranslation(
		//	Vector3(sinf(m_sinAngle), 0, cosf(m_sinAngle)*3.0f));
	}


	// Aキーを押している間
	if (keystate.A)
	{
		// 自機の角度を変動
		float angle = m_Obj[0].GetRotation().y;
		m_Obj[0].SetRotation(
			Vector3(0, angle + 0.03f, 0));
	}

	// Dキーを押している間
	if (keystate.D)
	{
		// 自機の角度を変動
		float angle = m_Obj[0].GetRotation().y;
		m_Obj[0].SetRotation(
			Vector3(0, angle - 0.03f, 0));
	}

	// Wキーを押している間
	if (keystate.W)
	{
		// 移動ベクトル
		Vector3 moveV(0, 0, -0.1f);
		// 回転行列
		float angle = m_Obj[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動
		Vector3 pos = m_Obj[0].GetTranslation();
		pos += moveV;
		m_Obj[0].SetTranslation(pos);

		Vector3 angleE;
		angleE = m_Obj[PARTS_ENGINE].GetRotation();
		m_Obj[PARTS_ENGINE].SetRotation(
			angleE + Vector3(0, 0, 0.4));
	}

	// Sキーを押している間
	if (keystate.S)
	{
		// 移動ベクトル
		Vector3 moveV(0, 0, +0.1f);
		// 回転行列
		float angle = m_Obj[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動
		Vector3 pos = m_Obj[0].GetTranslation();
		pos += moveV;
		m_Obj[0].SetTranslation(pos);

		Vector3 angleE;
		angleE = m_Obj[PARTS_ENGINE].GetRotation();
		m_Obj[PARTS_ENGINE].SetRotation(
			angleE + Vector3(0, 0, -0.4));
	}
	if (keystate.Left)
	{
		Vector3 angleH;
		angleH = m_Obj[PARTS_TURRET_R].GetRotation();
		m_Obj[PARTS_TURRET_R].SetRotation(
			angleH + Vector3(0, 0.05, 0));
	}
	if (keystate.Right)
	{
		Vector3 angleH;
		angleH = m_Obj[PARTS_TURRET_R].GetRotation();
		m_Obj[PARTS_TURRET_R].SetRotation(
			angleH + Vector3(0, -0.05, 0));
	}

	// 弾丸が進む処理
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		if (m_FireFlag)
		{
			ResetBullet();
		}
		else {
			FireBullet();
		}
	}
	if (m_FireFlag)
	{
		// 自機の座標を移動
		Vector3 pos = m_Obj[PARTS_BALL].GetTranslation();
		pos += m_BulletVel;
		m_Obj[PARTS_BALL].SetTranslation(pos);
	}

	// 行列更新
	Calc();

}

//-----------------------------------------------------------------------------
// 行列更新
//-----------------------------------------------------------------------------
void Player::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}
	//ノード更新
	m_CollisionNodeBullet.Update();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void Player::Draw()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}
	//
	m_CollisionNodeBullet.Draw();
}

void Player::FireBullet()
{
	if (m_FireFlag)  return;

	m_FireFlag = true;

	// ワールド行列を取得
	Matrix worldm = m_Obj[PARTS_BALL].GetWorld();

	Vector3 scale;	// ワールドスケーリング
	Quaternion rotation;	// ワールド回転
	Vector3 translation;	// ワールド座標

	// ワールド行列から各要素を取り出す
	worldm.Decompose(scale, rotation, translation);

	// 親子関係を解除してパーツを独立させる
	m_Obj[PARTS_BALL].SetObjParent(nullptr);
	m_Obj[PARTS_BALL].SetScale(scale);
	m_Obj[PARTS_BALL].SetRotationQ(rotation);
	m_Obj[PARTS_BALL].SetTranslation(translation);

	// 弾の速度を設定
	m_BulletVel = Vector3(0, 0, -0.3f);
	// 弾の向きに合わせて進行方向を回転
	m_BulletVel = Vector3::Transform(m_BulletVel, rotation);
}

void Player::ResetBullet()
{
	if (!m_FireFlag) return;
	Quaternion rotation;	// ワールド回転
	m_Obj[PARTS_BALL].SetRotationQ(rotation);
	m_Obj[PARTS_BALL].SetObjParent(&m_Obj[PARTS_TURRET_R]);
	m_Obj[PARTS_BALL].SetTranslation(Vector3(0, 0.3f, -1));

	m_FireFlag = false;
}

const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	// タンクパーツの座標を返す
	return m_Obj[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3& Player::GetRot()
{
	// タンクパーツの座標を返す
	return m_Obj[0].GetRotation();
}

void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetTranslation(trans);
}

void Player::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Player::GetLocalWorld()
{
	// タンクパーツのワールド行列を返す
	return m_Obj[0].GetWorld();
}
