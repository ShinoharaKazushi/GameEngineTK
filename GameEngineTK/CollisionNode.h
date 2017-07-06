/// <summary>
/// 当たり判定ノード
/// </summary>
#pragma once
#include "Obj3d.h"
#include "Collision.h"

class CollisionNode
{
public:
	enum TYPE
	{
		TYPE_SPHERE,	// 球
		TYPE_CAPSULE,	// カプセル

		TYPE_NUM
	};
public:
	// デバッグ表示のON/OFFを設定
	static void SetDebugVisible(bool flag) { m_DebugVisible = flag; }
	// デバッグ表示のON/OFFを取得
	static bool GetDebugVisible(void) { return m_DebugVisible; }
protected:
	// デバッグ表示のON/OFFフラグ
	static bool m_DebugVisible;
public:
	virtual void Initialize() = 0;
	//毎フレーム更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() = 0;
	//親をセット
	void SetParent(Obj3d* parent);
	//親からのオフセットをセット
	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_Trans = trans; };
	
protected:
	//デバック表示オブジェクト
	Obj3d m_Obj;
	//親からのオフセット
	DirectX::SimpleMath::Vector3 m_Trans;

	
};

//当たり判定球ノード
class SphereNode : public CollisionNode, public Sphere
{
public:
	SphereNode();
	//初期化
	void Initialize();
	//毎フレーム更新
	void Update();
	//描画
	void Draw();
	void SetLocalRadius(float radius) { m_LocalRadius = radius; };
protected:
	//ローカル半径
	float m_LocalRadius;

};