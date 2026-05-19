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

	// 当たり判定
	constexpr float kColliderRadius = 25.0f;
	constexpr float kColliderHeight = 120.0f;

	// 加速度
	constexpr float kMoveAccel = 3.0f;
	// 最大移動速度
	constexpr float kMaxMoveSpeed = 7.0f;
	// 回避の速度
	constexpr float kDodgeSpeed = 15.0f;

	// 設置判定に使うレイの長さ
	constexpr float kLineLength = 10.0f;

	// 回避のフレーム数
	constexpr int kDodgeFrame = 30;
	// コンボのフレーム数
	constexpr int kCombo1Frame = 20;
	constexpr int kCombo2Frame = 22;
	constexpr int kCombo3Frame = 60;
}

Player::Player(Input& input):
	m_input(input),
	m_collider(kColliderRadius, kColliderHeight)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_anim.Init(m_modelHandle, kIdleAnimName);

	m_pos = Vector3(0.0f, 200.0f, 0.0f);
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
	// 回避処理
	Dodge();
	// 攻撃処理
	Attack();

	// 状態を更新
	// 回避中でなければ、かつ攻撃中でなければ
	if (m_dodgeFrame == 0 && m_comboFrame == 0)
	{
		UpdateState();
	}
	

	// 重力をかける
	Gravity();
	// 速度に抵抗をかける
	Resistance();

	// コライダーの位置を更新
	m_collider.SetPos(m_pos + Vector3::Up() * m_collider.GetRadius());

	// マップとの当たり判定
	auto collResult = m_collider.CheckCollModel(m_mapHandle);
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
	float diff = m_angle - m_drawAngle - DX_PI_F * 2;
	while (diff > DX_PI_F) diff -= 2.0f * DX_PI_F;
	while (diff < -DX_PI_F) diff += 2.0f * DX_PI_F;
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
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	// 当たり判定を描画
	m_collider.Draw();
	// 接地判定用のレイを描画
	Vector3 layEnd = m_pos - Vector3::Up() * kLineLength;
	DrawLine3D(m_pos.ToDxLib(), layEnd.ToDxLib(), 0xffff00);
#endif
}

void Player::Move()
{
	// スティック入力を取得
	auto stick = m_input.GetStickInput(LR::Left);
	// 移動禁止状態なら入力を消す
	if (!m_isCanControll)
	{
		stick = Vector3::Zero();
	}

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
	else if (m_state != State::Dodge)
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

#ifdef _DEBUG
	DrawFormatString(0, 32, 0xffffff, L"stick.x:%.2f,y:%.2f", stick.x, stick.y);
	DrawFormatString(0, 48, 0xffffff, L"m_angle:%.2f", m_angle);
#endif
}

void Player::Jump()
{
	if (m_input.IsTriggerd(XINPUT_BUTTON_A) && m_isGround && m_isCanControll)
	{
		m_vel.y = 10.0f;
		m_isGround = false;
	}
}

void Player::Dodge()
{
	// ボタンを押した、かつ回避中でなければ回避
	if (m_input.IsTriggerd(XINPUT_BUTTON_B) && m_state != State::Dodge)
	{
		m_anim.ChangeAnim(kRollingAnimName, 0.5f, false);
		m_state = State::Dodge;
		m_dodgeFrame = 0;
		m_isCanControll = false;

		// 攻撃を中止する
		CancelAttack();
	}

	if (m_state == State::Dodge)
	{	
		// 回避中のフレームをカウント
		m_dodgeFrame++;
		// 一定時間経過したら回避終了
		if (m_dodgeFrame > kDodgeFrame)
		{
			m_dodgeFrame = 0;
			m_isCanControll = true;
		}

		// 向いている方向に進む
		Vector3 moveVec = Vector3(0.0f, 0.0f, -kDodgeSpeed);
		moveVec *= Matrix4x4::GetRotY(m_angle);
		m_vel.x = moveVec.x;
		m_vel.z = moveVec.z;
	}
}

void Player::Attack()
{
	// Xボタンで攻撃、回避中は不可
	if (m_input.IsTriggerd(XINPUT_BUTTON_X) && m_state != State::Dodge)
	{
		if (m_state == State::Combo1)	// 1段目→2段目
		{
			m_isTransferNextCombo = true;
		}
		else if (m_state == State::Combo2)	// 2段目→3段目
		{
			m_isTransferNextCombo = true;
		}
		else if (m_state == State::Combo3)	// 3段目
		{
			// 何もしない
		}
		else	// 1段目開始
		{
			m_state = State::Combo1;
			m_anim.ChangeAnim(kCombo1AnimName, 0.5f, false);
			m_comboFrame = 0;
			m_isTransferNextCombo = false;
		}
	}
	// 攻撃中なら
	bool isAttacking = m_state == State::Combo1 || m_state == State::Combo2 || m_state == State::Combo3;
	if (isAttacking)
	{	// コンボ用のフレームカウントを更新
		m_comboFrame++;
		// 1段目→2段目
		if (m_comboFrame == kCombo1Frame)
		{
			// 次のコンボへの移行が予約されていたら
			if (m_isTransferNextCombo)
			{	// 次のコンボへ
				m_state = State::Combo2;
				m_anim.ChangeAnim(kCombo2AnimName,0.5f,false);
				m_isTransferNextCombo = false;
			}
			else
			{	// 次のコンボに行かないならコンボを終わる
				m_comboFrame = 0;
			}
		}
		// 2段目→3段目
		if (m_comboFrame == kCombo2Frame + kCombo1Frame)
		{
			// 次のコンボへの移行が予約されていたら
			if (m_isTransferNextCombo)
			{	// 次のコンボへ
				m_state = State::Combo3;
				m_anim.ChangeAnim(kCombo3AnimName, 0.5f, false);
				m_isTransferNextCombo = false;
			}
			else
			{	// 次のコンボに行かないならコンボを終わる
				m_comboFrame = 0;
			}
		}
		// 3段目
		if (m_comboFrame >= kCombo3Frame + kCombo2Frame + kCombo1Frame)
		{
			m_comboFrame = 0;
			m_isTransferNextCombo = false;
		}
	}
}

void Player::CancelAttack()
{
	m_comboFrame = 0;
}

void Player::CheckHitMap(MV1_COLL_RESULT_POLY_DIM coll)
{
	for (int i = 0; i < coll.HitNum; i++)
	{
		// 当たったポリゴンの法線を取得
		auto normal = Vector3::FromDxLib(coll.Dim[i].Normal);

		// 法線が少しでも上を向いていれば床判定
		bool isFloor = normal.y > 0.0f;	// true:床 / false:壁
		// 床判定なら法線を真上向きにする
		if (isFloor) normal = Vector3::Up();

		// カプセルの位置を取得
		Vector3 capsuleStart = m_collider.GetPos();
		Vector3 capsuleEnd = capsuleStart + Vector3::Up() * m_collider.GetHeight();

		// ポリゴンとカプセルの線との最短距離を計算
		auto minDist = Segment_Triangle_MinLength(capsuleStart.ToDxLib(), capsuleEnd.ToDxLib(),
								   coll.Dim[i].Position[0], coll.Dim[i].Position[1], coll.Dim[i].Position[2]);
		// 押し戻し量を計算
		auto pushDist = m_collider.GetRadius() - minDist;

		// 法線方向に押し戻す
		m_pos += normal * pushDist;
		if (isFloor)
		{
			m_vel.y = 0.0f;
			m_isGround = true;
		}
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
#ifdef _DEBUG
	DrawFormatString(0, 64, 0xffffff, L"state:%d", m_state);
#endif

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
