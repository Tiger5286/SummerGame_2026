#include "Player.h"
#include <string>
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
	constexpr float kMoveAccel = 2.0f;
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

	// 移動処理
	Move();

	// 行列を生成してモデルに適用
	auto transMtx = Matrix4x4::GetTranslate(m_pos);
	auto rotMtx = Matrix4x4::GetRotY(m_angle);
	auto mtx = rotMtx * transMtx;
	MV1SetMatrix(m_modelHandle, mtx.ToDxLib());

	UpdateState();

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
	// 速度が上限を超えていなければ移動量を足す
	if (m_vel.SquaredLength() < kMaxMoveSpeed * kMaxMoveSpeed)
	{
		m_vel += moveVec;
	}
	// 位置に速度を足す
	m_pos += m_vel;
	// 速度に抵抗をつける
	Resistance();

	// モデルを回転させる
	// 移動ベクトルがある時のみ回る
	if (stick.SquaredLength() > 0.0f)
	{
		m_angle = atan2f(stick.y, -stick.x) + DX_PI_F / 2;
		m_angle += m_cameraAngleY;
	}
	DrawFormatString(0, 32, 0xffffff, L"stick.x:%.2f,y:%.2f", stick.x, stick.y);
	DrawFormatString(0, 48, 0xffffff, L"m_angle:%.2f", m_angle);
}

void Player::UpdateState()
{
	if (m_vel.SquaredLength() > 0.0f)
	{
		m_state = State::Run;
	}
	else
	{
		m_state = State::Idle;
	}
}
