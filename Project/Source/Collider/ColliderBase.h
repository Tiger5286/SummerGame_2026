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

	// 位置を設定する
	void SetPos(const Vector3& pos) { m_pos = pos; }

protected:

	Vector3 m_pos;	// 位置

	// 当たり判定の種類
	ColliderType m_type = ColliderType::None;
};

