#include "Player.h"
#include <string>
#include <cmath>
#include "Utility/Matrix4x4.h"
#include "Utility/MyLib.h"
#include "Singleton/Input.h"
#include "Game/Collider/CapsuleCollider.h"
#include "Game/Collider/SphereCollider.h"
#include "Singleton/CollisionManager.h"

#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"

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

	// 攻撃がホーミングする距離
	constexpr float kTrackingAttackDist = 500.0f;
	// 攻撃の前進をやめる距離
	constexpr float kStopTrackingDist = 150.0f;
}

void Player::Init()
{
	// 位置の初期化
	m_pos = Vector3(0.0f, 0.0f, 0.0f);

	// 当たり判定の生成と初期化
	m_pCollider = std::make_shared<CapsuleCollider>(kColliderRadius, kColliderHeight);
	// 当たり判定の登録
	CollisionManager::GetInstance().Register(shared_from_this());

	// アニメーションの初期化
	m_anim.Init(m_modelHandle, kIdleAnimName);

	// ステートの初期化
	m_pState = std::make_shared<PlayerStateIdle>();
	m_pState->ChangeState(m_pState);
	m_pState->Enter(weak_from_this());
	CheckChangeState();

	// キャラクタータイプをプレイヤーにする
	m_type = Type::Player;
}

void Player::End()
{
}

void Player::Update()
{
	// 次のステートがあったらステートを変更する
	CheckChangeState();

	// ステートの更新
	m_pState->Update();

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
	CheckHitMap(collResult);
	// 当たり判定に使用したメモリを解放
	MV1CollResultPolyDimTerminate(collResult);

	// 接地判定
	CheckGround();

#ifdef _DEBUG
	// デバッグ用の情報を表示
	DrawFormatString(0, 64 + 16, 0xffffff, L"vel.x:%.2f,y:%.2f,z:%.2f", m_vel.x, m_vel.y, m_vel.z);
	DrawFormatString(0, 64 + 32, 0xffffff, L"vel.length:%.2f", m_vel.Length());
	DrawFormatString(0, 64 + 48, 0xffffff, L"pos.x:%.2f,y:%.2f,z:%.2f", m_pos.x, m_pos.y, m_pos.z);
#endif

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
}

void Player::Draw()
{
	// モデルを描画
	MV1DrawModel(m_modelHandle);

	// ステートに描画したい内容があったら描画
	m_pState->Draw();

#ifdef _DEBUG
	// 当たり判定を描画
	m_pCollider->Draw();
	// 接地判定用のレイを描画
	Vector3 layEnd = m_pos - Vector3::Up() * kLineLength;
	DrawLine3D(m_pos.ToDxLib(), layEnd.ToDxLib(), 0xffff00);

	// デバッグ表示
	DrawFormatString(0, 64, 0xffffff, L"comboFrame:%d", m_comboFrame);
	DrawFormatString(0, 48, 0xffffff, L"m_angle:%.2f", m_angle);
#endif
}

Vector3 Player::GetDir() const
{
	return kDefaultDir * Matrix4x4::GetRotY(m_angle);
}

void Player::OnHitAttack()
{
	printfDx(L"プレイヤーが攻撃を食らった！\n");
}

void Player::Move()
{
	// 入力を取得
	auto& input = Input::GetInstance();

	// 入力方向を向く
	RotateInputDir();

	// スティック入力を取得
	auto stick = input.GetStickInput(LR::Left);
	// 移動ベクトルに入力を反映する
	Vector3 moveVec;
	moveVec += Vector3(stick.x, 0.0f, stick.y) * kMoveAccel;
	// カメラの向きに応じて移動ベクトルを回転させる
	moveVec *= Matrix4x4::GetRotY(m_cameraAngleY);
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
	auto stick = input.GetStickInput(LR::Left);
	if (stick.SquaredLength() > 0.0f)
	{
		stick.Normalize();
		m_angle = atan2f(stick.y, -stick.x) + DX_PI_F / 2;
		m_angle += m_cameraAngleY;
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
		}
	}
	else
	{
		m_isGround = false;
	}
}

void Player::CheckChangeState()
{
	auto nextState = m_pState->GetNextState();
	// 次のステートがある場合は切り替え
	if (m_pState != nextState)
	{
		m_pState->Exit();

		m_pState = nextState;

		m_pState->Enter(weak_from_this());

		m_pState->ChangeState(m_pState);
	}
}