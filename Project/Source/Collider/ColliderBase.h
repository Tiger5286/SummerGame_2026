#pragma once
#include "../Utility/Vector3.h"

/// <summary>
/// 当たり判定の種類を表す列挙体
/// </summary>
enum class ColliderType
{
	None,
	Sphere,
	Capsule,
	Polygon,

	Num
};

class ColliderBase
{
public:
	ColliderBase(ColliderType type);
	virtual ~ColliderBase();

	// 当たり判定を描画する
	virtual void Draw() = 0;

	Vector3 GetPos() const { return m_pos; }	// 位置を取得する
	void SetPos(const Vector3& pos) { m_pos = pos; }	// 位置を設定する

protected:

	Vector3 m_pos;	// 位置

	// 当たり判定の種類
	ColliderType m_type = ColliderType::None;
};

