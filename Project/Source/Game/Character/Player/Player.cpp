#include "Player.h"
#include <string>
#include <cmath>
#include "Utility/Matrix4x4.h"
#include "Utility/MyLib.h"
#include "Singleton/Input.h"
#include "Game/Collider/CapsuleCollider.h"
#include "Singleton/CollisionManager.h"
#include "Singleton/UIManager.h"
#include "../../Camera/Camera.h"

#include "../../UI/PlayerHPUI.h"
#include "../../UI/PlayerSpecialUI.h"
#include "../../UI/ControlUI.h"

#include "PlayerStateIdle.h"
#include "PlayerStateDodge.h"
#include "PlayerStateHit.h"
#include "PlayerStateBurning.h"
#include "PlayerStateDeath.h"
#include "PlayerStateSpin.h"

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"Player|Idle";

	// モデルのデフォルトの向き(angleが0の時の向き)
	const Vector3 kDefaultDir = Vector3(0, 0, -1);

	// 当たり判定
	constexpr float kColliderRadius = 25.0f;
	constexpr float kColliderHeight = 120.0f;

	// 加速度
	constexpr float kMoveAccel = 3.0f;
	// 最大移動速度
	constexpr float kMaxMoveSpeed = 7.0f;

	// 接地判定に使うレイの長さ
	constexpr float kLineLength = 10.0f;
}

void Player::Init()
{
	// 位置の初期化
	m_pos = Vector3::Zero();
	// 向きの初期化(出現時に背中を向けてほしいため180度回転)
	m_angle = DX_PI_F;
	m_drawAngle = DX_PI_F;

	// 当たり判定の生成と初期化
	m_pCollider = std::make_shared<CapsuleCollider>(kColliderRadius, kColliderHeight);
	// 当たり判定の登録
	CollisionManager::GetInstance().Register(shared_from_this());

	// アニメーションの初期化
	m_anim.Init(m_modelHandle, kIdleAnimName);

	// hpの初期化
	m_hp = kMaxHp;

	// ステートの初期化
	m_pState = std::make_shared<PlayerStateIdle>();
	m_pState->ChangeState(m_pState);
	m_pState->Enter(weak_from_this());
	CheckChangeState();

	// キャラクタータイプをプレイヤーにする
	m_type = MyLib::CharacterType::Player;

	// UIの生成と初期化
	auto& uiManager = UIManager::GetInstance();
	auto player = std::dynamic_pointer_cast<Player>(shared_from_this());

	m_pHPUI = std::make_shared<PlayerHPUI>();
	m_pHPUI->SetInfo(player);
	uiManager.AddUI(m_pHPUI);

	m_pSpecialUI = std::make_shared<PlayerSpecialUI>();
	m_pSpecialUI->SetInfo(player);
	uiManager.AddUI(m_pSpecialUI);

	m_pControlUI = std::make_shared<ControlUI>();
	m_pControlUI->SetInfo(player);
	uiManager.AddUI(m_pControlUI);
}

void Player::End()
{
}

void Player::OnUpdate()
{
	//位置に速度を足す
	m_pos += m_vel;

	// 重力をかける
	Gravity();
	// 速度に抵抗をかける
	Resistance();

	// 当たり判定の更新
	auto capsule = std::dynamic_pointer_cast<CapsuleCollider>(m_pCollider);
	capsule->SetPos(m_pos + Vector3(0.0f, capsule->GetRadius(), 0.0f));

	// マップとの当たり判定
	auto collResult = m_pCollider->CheckCollModel(m_mapHandle);
	CheckHitMapCapsule(collResult);
	// 当たり判定に使用したメモリを解放
	MV1CollResultPolyDimTerminate(collResult);

	// 接地判定
	CheckGround();

	// 必殺技ゲージの上限
	if (m_specialCharge > kMaxSpecialCharge) m_specialCharge = kMaxSpecialCharge;
	// スキルクールタイム
	if (m_skillCooltime < kSkillCooltime) m_skillCooltime++;

	// 奈落に落ちたら初期位置に戻す
	if (m_pos.y < -2000.0f)
	{
		m_pos = Vector3(0.0f, 200.0f, 0.0f);
		m_vel = Vector3::Zero();
	}

	// モデルの回転角度を更新
	float diff = MyLib::GetAngleDif(m_angle, m_drawAngle);
	m_drawAngle += diff * 0.1f;

	// 行列を生成してモデルに適用
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto rotMtx = Matrix4x4::GetRotY(m_drawAngle);
	auto mtx = rotMtx * transMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());

	// アニメーションの更新
	m_anim.Update();

#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_2))
	{
		m_hp = 1;
	}
	if (CheckHitKey(KEY_INPUT_5))
	{
		m_specialCharge = kMaxSpecialCharge;
	}
#endif
}

void Player::Draw()
{
	// 透明フラグがoffのときのみ描画
	if (!m_isInvisible)
	{
		// モデルを描画
		MV1DrawModel(m_modelHandle);
	}

	// ステートに描画したい内容があったら描画
	m_pState->Draw();

#ifdef _DEBUG
	// 当たり判定を描画
	m_pCollider->Draw();
	// 接地判定用のレイを描画
	Vector3 layEnd = m_pos - Vector3::Up() * kLineLength;
	DrawLine3D(m_pos.ToDxLib(), layEnd.ToDxLib(), 0xffff00);

	// デバッグ表示
	//DrawFormatString(0, 48, 0xffffff, L"m_angle:%.2f", m_angle);
#endif
}

Vector3 Player::GetDir() const
{
	return kDefaultDir * Matrix4x4::GetRotY(m_angle);
}

void Player::OnHitAttack(const MyLib::AttackData& atkData)
{
	// 被弾属性を取得
	auto state = std::dynamic_pointer_cast<PlayerStateBase>(m_pState);
	PlayerStateBase::HitAttribute hitAttribute = state->GetHitAttribute();
	// 完全無敵なら食らわない
	if (hitAttribute == PlayerStateBase::HitAttribute::PerfectInvincible)
	{
		return;
	}
	// 無敵かつ攻撃が無敵貫通しないなら食らわない
	if (hitAttribute == PlayerStateBase::HitAttribute::Invincible && !atkData.isIgnoreInvincible)
	{
		return;
	}

	// ダメージを食らう
	m_hp -= atkData.damage;
	// 死ぬダメージを食らったら死ぬ
	if (m_hp <= 0)
	{
		m_pState->ChangeState(std::make_shared<PlayerStateDeath>());
		return;
	}

	// ひるまない状態ならひるまない
	if (hitAttribute == PlayerStateBase::HitAttribute::IgnoreFalter)
	{
		return;
	}

	// 死ななかったら被弾
	m_pState->ChangeState(std::make_shared<PlayerStateHit>());
}

void Player::Move()
{
	// 入力を取得
	auto& input = Input::GetInstance();

	// 入力方向を向く
	RotateInputDir();

	// スティック入力を取得
	auto stick = input.GetStickInput(MyLib::LR::Left);
	// 移動ベクトルに入力を反映する
	Vector3 moveVec;
	moveVec += Vector3(stick.x, 0.0f, stick.y) * kMoveAccel;
	// カメラの向きに応じて移動ベクトルを回転させる
	moveVec *= Matrix4x4::GetRotY(m_pCamera.lock()->GetAngleY());
	// 水平移動速度が上限を超えていなければ移動量を足す
	auto velXZ = m_vel;
	velXZ.y = 0.0f;
	if (velXZ.SquaredLength() < kMaxMoveSpeed * kMaxMoveSpeed)
	{
		m_vel += moveVec;
	}
	else
	{
		// 水平移動速度が上限を超えていたらその値で固定する
		velXZ.Normalize();
		velXZ *= kMaxMoveSpeed;
		m_vel.x = 0.0f;
		m_vel.z = 0.0f;
		m_vel += velXZ;
	}
}

void Player::Jump()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(XINPUT_BUTTON_A) && m_isGround)
	{
		m_vel.y = 10.0f;
		m_isGround = false;
	}
}

void Player::RotateInputDir()
{
	auto& input = Input::GetInstance();
	auto stick = input.GetStickInput(MyLib::LR::Left);
	if (stick.SquaredLength() > 0.0f)
	{
		stick.Normalize();
		m_angle = atan2f(stick.y, -stick.x) + DX_PI_F / 2;
		m_angle += m_pCamera.lock()->GetAngleY();
	}
}

void Player::RotateToTarget(float activeDist)
{
	// ターゲットがいないなら処理しない
	if (m_target == nullptr)
	{
		return;
	}

	// ホーミングするターゲットへのベクトルを生成
	Vector3 toTargetVec = m_target->GetPos() - m_pos;
	// 条件を満たしたらプレイヤーの向きをターゲットのほうへ向ける
	bool isTrackingDist = toTargetVec.SquaredLength() < activeDist * activeDist;	// ホーミングする距離内
	if (isTrackingDist)	// 距離内、かつ一定の距離外
	{
		m_angle = toTargetVec.Angle();
	}
}

void Player::CheckGround()
{
	// 速度が上向きなら処理しない
	if (m_vel.y > 0.0f) return;

	// レイの座標を計算
	Vector3 lineStart = m_pos + Vector3::Up() * kLineLength;
	Vector3 lineEnd = m_pos - Vector3::Up() * kLineLength;
	// マップとレイの当たり判定
	auto result = MV1CollCheck_Line(m_mapHandle, -1, lineStart.ToDxLib(), lineEnd.ToDxLib());
	// レイが当たっていたら
	if (result.HitFlag)
	{
		bool isFloor = result.Normal.y > 0.0f;

		if (isFloor)
		{
			m_pos.y = result.HitPosition.y;
			m_isGround = true;
			m_isCanAirShift = true;
		}
	}
	else
	{
		m_isGround = false;
	}
}