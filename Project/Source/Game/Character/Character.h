#pragma once
#include "../../Utility/Vector3.h"
#include "DxLib.h"
#include <memory>

class ColliderBase;

class Character
{
public:
	Character();
	virtual ~Character();

	virtual void Init() abstract;
	virtual void End() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

	enum class Type
	{
		None,
		Player,
		Enemy,

		Num
	};

	/// <summary>
	/// モデルのハンドルを設定する。Initの前に実行すること
	/// </summary>
	/// <param name="handle">ロードしたモデルのハンドル</param>
	void SetHandle(int handle) { m_modelHandle = handle; }

	// 位置を取得する
	Vector3 GetPos() const { return m_pos; }
	// 位置を設定する
	void SetPos(const Vector3& pos) { m_pos = pos; }
	// 当たり判定を取得する
	std::shared_ptr<ColliderBase> GetCollider() const { return m_pCollider; }
	// キャラクタータイプを取得する
	Type GetType() const { return m_type; }

	// IDを取得する
	const int GetID() const { return m_id; }

	/// <summary>
	/// 速度に抵抗をつける(なにもしなかったら徐々に速度が遅くなる)
	/// </summary>
	void Resistance();

	/// <summary>
	/// 重力をかける
	/// </summary>
	void Gravity(float power = 0.5f);

	/// <summary>
	/// ほかのオブジェクトと当たった時に呼ばれる関数
	/// </summary>
	/// <param name="other">当たったオブジェクト</param>
	virtual void OnCollision(Character& other);

	virtual void OnHitAttack() {};

	// マップに当たったときの処理
	void CheckHitMap(MV1_COLL_RESULT_POLY_DIM coll);

protected:
	// モデルのハンドル
	int m_modelHandle = -1;

	std::shared_ptr<ColliderBase> m_pCollider = nullptr;

	Vector3 m_pos;	// 位置
	Vector3 m_vel;	// 速度

	Type m_type = Type::None;

	bool m_isGround = false;	// 接地しているかどうか

	const int m_id;
};

