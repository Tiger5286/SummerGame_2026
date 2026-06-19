#pragma once
#include "../../Utility/Vector3.h"

/// <summary>
/// 当たり判定の種類を表す列挙体
/// </summary>
enum class ColliderType
{
	None,
	Sphere,
	Capsule,
	//Polygon,

	Num
};

class ColliderBase
{
public:
	ColliderBase(ColliderType type);
	virtual ~ColliderBase();

	// 当たり判定を描画する
	virtual void Draw() = 0;

	/// <summary>
	/// モデル(主にマップ)との当たり判定を行う。
	/// この関数の返り値は動的にメモリを確保しているので、使用したのちMV1CollResultPolyDimTerminate()で解放する必要がある。
	/// </summary>
	/// <param name="mapHandle">当たり判定用のマップモデルのハンドル</param>
	/// <returns>当たり判定結果</returns>
	virtual MV1_COLL_RESULT_POLY_DIM CheckCollModel(int mapHandle) = 0;

	Vector3 GetPos() const { return m_pos; }	// 位置を取得する
	void SetPos(const Vector3& pos) { m_pos = pos; }	// 位置を設定する

	bool IsEnable() const { return m_isEnable; }	// 有効か無効かを取得する
	void SetEnable(bool isEnable) { m_isEnable = isEnable; }	// 有効か無効かを設定する

	ColliderType GetType() const { return m_type; }

protected:
	Vector3 m_pos;	// 位置
	bool m_isEnable = true;

	// 当たり判定の種類
	ColliderType m_type = ColliderType::None;
};

