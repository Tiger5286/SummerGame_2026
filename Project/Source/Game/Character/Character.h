#pragma once
#include "../../Utility/Vector3.h"
#include "DxLib.h"
#include "Utility/MyLib.h"
#include <memory>

class ColliderBase;
class CharacterStateBase;

class Character : public std::enable_shared_from_this<Character>
{
public:
	Character();
	virtual ~Character();

	virtual void Init() abstract;
	virtual void End() abstract;
	void Update();
	virtual void Draw() abstract;

protected:
	virtual void OnUpdate() abstract;
public:

	/// <summary>
	/// モデルのハンドルを設定する。Initの前に実行すること
	/// </summary>
	/// <param name="handle">ロードしたモデルのハンドル</param>
	void SetHandle(int handle) { m_modelHandle = handle; }

	// 位置を取得する
	Vector3 GetPos() const { return m_pos; }
	// 位置を設定する
	void SetPos(const Vector3& pos) { m_pos = pos; }
	// 加速度を取得する
	Vector3 GetVel() const { return m_vel; }
	// 加速度を設定する
	void SetVel(const Vector3& vel) { m_vel = vel; }
	// 当たり判定を取得する
	std::shared_ptr<ColliderBase> GetCollider() const { return m_pCollider; }
	// キャラクタータイプを取得する
	MyLib::CharacterType GetType() const { return m_type; }
	// IDを取得する
	const int GetID() const { return m_id; }
	// HPを取得する
	int GetHP() const { return m_hp; }
	// 最大HPを取得する
	virtual int GetMaxHP() const = 0;

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

	/// <summary>
	/// 攻撃を喰らったときに呼ばれる関数
	/// </summary>
	/// <param name="damage"></param>
	virtual void OnHitAttack(const MyLib::AttackData& atkData);

	// マップに当たったときの処理
	void CheckHitMapCapsule(MV1_COLL_RESULT_POLY_DIM coll);
	void CheckHitMapSphere(MV1_COLL_RESULT_POLY_DIM coll);

	// ステートの切り替え処理
	void CheckChangeState();

	// ヒットストップのフレーム数を設定する
	void SetHitStop(int frame) { m_hitStopFrame = frame; }

	bool IsCanHitAttack() const { return m_isCanHitAttack; }

protected:
	// モデルのハンドル
	int m_modelHandle = -1;

	// 当たり判定
	std::shared_ptr<ColliderBase> m_pCollider = nullptr;

	std::shared_ptr<CharacterStateBase> m_pState = nullptr;

	// hp
	int m_hp = -1;

	Vector3 m_pos;	// 位置
	Vector3 m_vel;	// 速度
	float m_angle = 0.0f;	// 向き
	float m_drawAngle = 0.0f;	// 描画時の向き

	// キャラクターのタイプ(タグ)
	MyLib::CharacterType m_type = MyLib::CharacterType::None;

	bool m_isGround = false;	// 接地しているかどうか

	const int m_id;	// ID

	int m_hitStopFrame = 0;	// ヒットストップのフレーム数 (0以上ならヒットストップ中)

	bool m_isCanHitAttack = true;	// 攻撃を喰らえるかどうか
};

