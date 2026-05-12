#include "Player.h"
#include <string>
#include <cmath>
#include "../Utility/Matrix4x4.h"

#include "../System/Input.h"

namespace
{
	// アニメーションの名前
	const std::wstring kIdleAnimName    = L"Player|Idle";
	const std::wstring kRunAnimName     = L"Player|Run";
	const std::wstring kFallAnimName    = L"Player|Fall";
	const std::wstring kRollingAnimName = L"Player|Rolling";
	const std::wstring kCombo1AnimName  = L"Player|Combo1";
	const std::wstring kCombo2AnimName  = L"Player|Combo2";
	const std::wstring kCombo3AnimName  = L"Player|Combo3";

	// 加速度
	constexpr float kMoveAccel = 3.0f;
	// 最大移動速度
	constexpr float kMaxMoveSpeed =7.0f;
}

Player::Player(Input& input):
	m_input(input)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_anim.Init(m_modelHandle, kIdleAnimName);
}

void Player::End()
{
}

void Player::Update()
{
	// 過去のステートを保存
	m_prevState = m_state;

	// ジャンプ処理
	Jump();
	// 移動処理
	Move();

	// 設置判定
	if (m_pos.y <= 0.0f)
	{
		m_isGround = true;
	}

	// 状態を更新
	UpdateState();

	// 重力をかける
	Gravity();
	// 速度に抵抗をかける
	Resistance();

	// モデルの回転角度を更新
	float diff = m_angle - m_drawAngle - DX_PI_F * 2;
	while (diff > DX_PI_F) diff -= 2.0f * DX_PI_F;
	while (diff < -DX_PI_F) diff += 2.0f * DX_PI_F;
	m_drawAngle += diff * 0.1f;

	// 行列を生成してモデルに適用
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto rotMtx = Matrix4x4::GetRotY(m_drawAngle);
	auto mtx = rotMtx * transMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());

	DrawFormatString(0, 64 + 16, 0xffffff, L"vel.x:%.2f,y:%.2f,z:%.2f", m_vel.x, m_vel.y, m_vel.z);
	DrawFormatString(0, 64 + 32, 0xffffff, L"vel.length:%.2f", m_vel.Length());

	// アニメーションの更新
	m_anim.Update();
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
}

void Player::Move()
{
	// スティック入力を取得
	auto stick = m_input.GetStickInput(LR::Left);
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
	// 位置に速度を足す
	m_pos += m_vel;

	// モデルを回転させる
	// 入力があるときのみ回る
	if (stick.SquaredLength() > 0.0f)
	{
		m_angle = atan2f(stick.y, -stick.x) + DX_PI_F / 2;
		m_angle += m_cameraAngleY;
	}
	DrawFormatString(0, 32, 0xffffff, L"stick.x:%.2f,y:%.2f", stick.x, stick.y);
	DrawFormatString(0, 48, 0xffffff, L"m_angle:%.2f", m_angle);
}

void Player::Jump()
{
	if (m_input.IsTriggerd(XINPUT_BUTTON_A) && m_isGround)
	{
		m_vel.y = 10.0f;
		m_isGround = false;
	}
}

void Player::UpdateState()
{
	// 現在の状態からステートを決定
	auto velXZ = m_vel;
	velXZ.y = 0.0f;
	if (!m_isGround)	// 地面にいなければ落下
	{
		m_state = State::Fall;
	}
	else if (velXZ.SquaredLength() > 0.0f)	// 水平方向への速度が0より大きければ移動
	{
		m_state = State::Run;
	}
	else	// どれにも当てはまらなければ待機
	{
		m_state = State::Idle;
	}
	DrawFormatString(0, 64, 0xffffff, L"state:%d", m_state);

	// ステート切り替えに応じてアニメーションも切り替え
	if (TriggerdChangeState(State::Idle))
	{
		m_anim.ChangeAnim(kIdleAnimName);
	}
	if (TriggerdChangeState(State::Run))
	{
		m_anim.ChangeAnim(kRunAnimName);
	}
	if (TriggerdChangeState(State::Fall))
	{
		m_anim.ChangeAnim(kFallAnimName);
	}
}

bool Player::TriggerdChangeState(State state)
{
	return m_state == state && m_prevState != state;
}
