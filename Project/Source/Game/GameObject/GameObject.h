#pragma once
#include "../../Utility/Vector3.h"
#include <memory>

class ColliderBase;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init() abstract;
	virtual void End() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

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
	std::shared_ptr<ColliderBase> GetCollider() const { return m_collider; }

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
	virtual void OnCollision(GameObject& other);

protected:
	// モデルのハンドル
	int m_modelHandle = -1;

	std::shared_ptr<ColliderBase> m_collider = nullptr;

	Vector3 m_pos;	// 位置
	Vector3 m_vel;	// 速度
};

