#include "Player.h"
#include <string>
#include <cmath>
#include "Utility/Matrix4x4.h"
#include "Utility/MyLib.h"
#include "Singleton/Input.h"
#include "Game/Collider/CapsuleCollider.h"

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

	// コンボのフレーム数
	constexpr int kCombo1Frame = 33*2;
	constexpr int kCombo2Frame = 34*2;
	constexpr int kCombo3Frame = 43*2;
	// コンボ一つに最短どれくらいの時間かかるか
	constexpr int kCombo1MinFrame = 10 * 2;
	constexpr int kCombo2MinFrame = 10 * 2;
	// コンボ中の前進する速度
	constexpr float kAttackMoveSpeed = 10.0f;
	// コンボ中の前進する時間
	constexpr int kCombo1MoveFrame = 10;
	constexpr int kCombo2MoveFrame = 10;
	constexpr int kCombo3MoveFrame = 15;
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
	m_collider = std::make_shared<CapsuleCollider>(kColliderRadius, kColliderHeight);

	// アニメーションの初期化
	m_anim.Init(m_modelHandle, kIdleAnimName);

	// ステートの初期化
	m_pState = std::make_shared<PlayerStateIdle>();
	m_pState->ChangeState(m_pState);
	m_pState->Enter(shared_from_this());
	CheckChangeState();
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
	auto capsule = std::dynamic_pointer_cast<CapsuleCollider>(m_collider);
	capsule->SetPos(m_pos + Vector3(0.0f, capsule->GetRadius(), 0.0f));

	// マップとの当たり判定
	auto collResult = m_collider->CheckCollModel(m_mapHandle);
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
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	// 当たり判定を描画
	m_collider->Draw();
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

void Player::Attack()
{
	/*
	Vector3 toTargetDir = Vector3::Zero();
	float toTargetDistSquare = 0.0f;
	bool isTrackingAttack = false;
	if (m_target != nullptr)
	{
		// ターゲットとの距離を計算しておく
		toTargetDistSquare = (m_target->GetPos() - m_pos).SquaredLength();
		// 攻撃が敵にホーミングするかどうか		// ターゲットが存在する　かつ　ホーミングする距離内にいる
		isTrackingAttack = m_target != nullptr && toTargetDistSquare < kTrackingAttackDist * kTrackingAttackDist;

		// ターゲットへの方向を計算しておく
		toTargetDir = (m_target->GetPos() - m_pos);
		toTargetDir.y = 0.0;
		toTargetDir.Normalize();
	}

	// Xボタンで攻撃、回避中は不可
	if (m_input.IsTriggerd(XINPUT_BUTTON_X) && m_state != State::Dodge)
	{
		if (m_state == State::Combo1)	// 1段目→2段目
		{
			// コンボフレームが受付時間内なら次のコンボに移行する
			if (m_comboFrame < kCombo1Frame)
			{
				m_isTransferNextCombo = true;
			}
		}
		else if (m_state == State::Combo2)	// 2段目→3段目
		{
			// コンボフレームが受付時間内なら次のコンボに移行する
			if (m_comboFrame < kCombo1Frame + kCombo2Frame)
			{
				m_isTransferNextCombo = true;
			}
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
			m_isCanControll = false;

			if (isTrackingAttack)
			{
				m_angle = atan2(toTargetDir.z, -toTargetDir.x) + DX_PI_F / 2;
			}
			else
			{
				RotateInputDir();
			}
		}
	}
	// 攻撃中なら
	bool isAttacking = m_state == State::Combo1 || m_state == State::Combo2 || m_state == State::Combo3;
	if (isAttacking)
	{	// コンボ用のフレームカウントを更新
		m_comboFrame++;

		switch (m_state)
		{
		case State::Combo1:
			// コンボ移行処理
			if (m_isTransferNextCombo)
			{
				if (m_comboFrame > kCombo1MinFrame)
				{
					m_state = State::Combo2;
					m_anim.ChangeAnim(kCombo2AnimName, 0.5f, false);
					m_isTransferNextCombo = false;
					m_comboFrame = kCombo1MinFrame;

					if (isTrackingAttack)
					{
						m_angle = atan2(toTargetDir.z, -toTargetDir.x) + DX_PI_F / 2;
					}
					else
					{
						RotateInputDir();
					}
				}
			}
			else
			{
				if (m_comboFrame == kCombo1Frame)
				{
					m_comboFrame = 0;
					m_isCanControll = true;
					m_isTransferNextCombo = false;
				}
			}
			// コンボ移動処理
			if (m_comboFrame < kCombo1MoveFrame)
			{
				float moveSpeed = -kAttackMoveSpeed;
				if (toTargetDistSquare < kStopTrackingDist * kStopTrackingDist)
				{
					moveSpeed = 0.0f;
				}
				Vector3 moveVec = Vector3(0,0, moveSpeed);
				moveVec *= Matrix4x4::GetRotY(m_angle);
				m_pos += moveVec;
			}
			// 攻撃中は浮く
			if (m_comboFrame < kCombo1MinFrame)
			{
				m_vel.y = 0.0f;
			}
			break;
		case State::Combo2:
			if (m_isTransferNextCombo)
			{
				if (m_comboFrame > kCombo2MinFrame + kCombo1MinFrame)
				{
					m_state = State::Combo3;
					m_anim.ChangeAnim(kCombo3AnimName, 0.5f, false);
					m_isTransferNextCombo = false;
					m_comboFrame = kCombo2MinFrame + kCombo1MinFrame;

					if (isTrackingAttack)
					{
						m_angle = atan2(toTargetDir.z, -toTargetDir.x) + DX_PI_F / 2;
					}
					else
					{
						RotateInputDir();
					}
				}
			}
			else
			{
				if (m_comboFrame == kCombo2Frame + kCombo1MinFrame)
				{
					m_comboFrame = 0;
					m_isCanControll = true;
					m_isTransferNextCombo = false;
				}
			}
			// コンボ移動処理
			if (m_comboFrame < kCombo1MoveFrame + kCombo1MinFrame)
			{
				float moveSpeed = -kAttackMoveSpeed;
				if (toTargetDistSquare < kStopTrackingDist * kStopTrackingDist)
				{
					moveSpeed = 0.0f;
				}
				Vector3 moveVec = Vector3(0, 0, moveSpeed);
				moveVec *= Matrix4x4::GetRotY(m_angle);
				m_pos += moveVec;
			}
			// 攻撃中は浮く
			if (m_comboFrame < kCombo1MinFrame + kCombo2MinFrame)
			{
				m_vel.y = 0.0f;
			}
			break;
		case State::Combo3:
			if (m_comboFrame == kCombo3Frame + kCombo2MinFrame + kCombo1MinFrame)
			{
				m_comboFrame = 0;
				m_isCanControll = true;
				m_isTransferNextCombo = false;
			}
			// コンボ移動処理
			if (m_comboFrame < kCombo3MoveFrame + kCombo2MinFrame + kCombo1MinFrame)
			{
				float moveSpeed = -kAttackMoveSpeed;
				if (toTargetDistSquare < kStopTrackingDist * kStopTrackingDist)
				{
					moveSpeed = 0.0f;
				}
				Vector3 moveVec = Vector3(0, 0, moveSpeed);
				moveVec *= Matrix4x4::GetRotY(m_angle);
				m_pos += moveVec;
			}
			break;
		}
	}
	*/
}

void Player::CancelAttack()
{
	m_comboFrame = 0;
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

void Player::CheckHitMap(MV1_COLL_RESULT_POLY_DIM coll)
{
	auto capsule = std::dynamic_pointer_cast<CapsuleCollider>(m_collider);
	for (int i = 0; i < coll.HitNum; i++)
	{
		// 当たったポリゴンの法線を取得
		auto normal = Vector3::FromDxLib(coll.Dim[i].Normal);

		// 法線が少しでも上を向いていれば床判定
		bool isFloor = normal.y > 0.0f;	// true:床 / false:壁
		// 床判定なら法線を真上向きにする
		if (isFloor) normal = Vector3::Up();

		// カプセルの位置を取得
		Vector3 capsuleStart = capsule->GetPos();
		Vector3 capsuleEnd = capsuleStart + Vector3::Up() * capsule->GetHeight();

		// ポリゴンとカプセルの線との最短距離を計算
		auto minDist = Segment_Triangle_MinLength(capsuleStart.ToDxLib(), capsuleEnd.ToDxLib(),
								   coll.Dim[i].Position[0], coll.Dim[i].Position[1], coll.Dim[i].Position[2]);
		// 押し戻し量を計算
		auto pushDist = capsule->GetRadius() - minDist;

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

void Player::CheckChangeState()
{
	auto nextState = m_pState->GetNextState();
	// 次のステートがある場合は切り替え
	if (m_pState != nextState)
	{
		m_pState->Exit();

		m_pState = nextState;

		m_pState->Enter(shared_from_this());

		m_pState->ChangeState(m_pState);
	}
}