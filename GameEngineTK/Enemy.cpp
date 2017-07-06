//--------------------------------------------------------------------------------------
// ファイル名: Enemy.cpp
// 作成者:
// 作成日:
// 説明:
//--------------------------------------------------------------------------------------

#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Enemy::Enemy(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	m_sinAngle = 0.0f;

	Initialize();
}

Enemy::~Enemy()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void Enemy::Initialize()
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

	//親子関係の構築(子供に親をセット)
	m_Obj[PARTS_BODY].SetObjParent(&m_Obj[PARTS_ASHI]);
	m_Obj[PARTS_ENGINE].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HAND_L].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_HAND_R].SetObjParent(&m_Obj[PARTS_BODY]);
	m_Obj[PARTS_TURRET_L].SetObjParent(&m_Obj[PARTS_HAND_L]);
	m_Obj[PARTS_TURRET_R].SetObjParent(&m_Obj[PARTS_HAND_R]);
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

	// 初期座標をランダムに決定
	Vector3 pos;

	pos.x = rand() % 10;
	pos.z = rand() % 10;

	SetTrans(pos);

	m_Timer = 0;
	m_DistAngle = 0;

	//武器のあたり判定ノード設定
	{
		m_CollisionNodeBody.Initialize();
		//武器パーツにぶら下げる
		m_CollisionNodeBody.SetParent(&m_Obj[PARTS_BODY]);
		//武器パーツからのオフセット
		m_CollisionNodeBody.SetTrans(Vector3(0, 0.5f, 0));
		//あたり判定の半径
		m_CollisionNodeBody.SetLocalRadius(1.0f);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void Enemy::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	// 自機パーツのギミック
	{
		// 自機の角度を変動
		Vector3 angle;

		angle = m_Obj[PARTS_ENGINE].GetRotation();
		m_Obj[PARTS_ENGINE].SetRotation(
			angle + Vector3(0.2f, 0.1f, 0));

		// サインの引数の角度がだんだん増える
		m_sinAngle += 0.1f;
		// ファンの角度がいったりきたりする(-1～+1)
		float fan_angle = sinf(m_sinAngle) * 3.0f;
		/*m_Obj[PLAYER_PARTS_FAN].SetRotation(
		Vector3(0, 0, fan_angle));*/
		//m_Obj[PARTS_FAN].SetTranslation(
		//	Vector3(sinf(m_sinAngle), 0, cosf(m_sinAngle)*3.0f));
	}

	m_Timer--;
	if (m_Timer < 0)
	{
		// 目標角度の再抽選
		m_Timer = 60;
		// -0.5～+0.5の乱数
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		// -90～+90の乱数
		rnd *= 180.0f;

		rnd = XMConvertToRadians(rnd);

		m_DistAngle += rnd;
	}

	{
		// 自機の角度を変動
		Vector3 rot = GetRot();

		float angle = m_DistAngle - rot.y;

		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		rot.y += angle *0.05f;

		SetRot(rot);
	}

	// 機体の向いている方向に前進
	{
		// 移動ベクトル
		Vector3 moveV(0, 0, -0.1f);
		// 今の角度に合わせて移動ベクトルを回転
		// 回転行列
		float angle = m_Obj[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		// 自機の座標を移動
		Vector3 pos = m_Obj[0].GetTranslation();
		pos += moveV;
		m_Obj[0].SetTranslation(pos);
	}

	// 行列更新
	Calc();
}

//-----------------------------------------------------------------------------
// 行列更新
//-----------------------------------------------------------------------------
void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Update();
	}
	//ノード更新
	m_CollisionNodeBody.Update();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin();
		it != m_Obj.end();
		it++)
	{
		it->Draw();
	}
	//
	m_CollisionNodeBody.Draw();
}

const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	// タンクパーツの座標を返す
	return m_Obj[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3& Enemy::GetRot()
{
	// タンクパーツの座標を返す
	return m_Obj[0].GetRotation();
}

void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetTranslation(trans);
}

void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
{
	// タンクパーツのワールド行列を返す
	return m_Obj[0].GetWorld();
}
