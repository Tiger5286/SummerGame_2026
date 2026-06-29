#include "EnemyBase.h"
#include "DxLib.h"
#include "../Player/Player.h"

EnemyBase::EnemyBase()
{}

EnemyBase::~EnemyBase()
{
	// 敵はDuplicateされたモデルを使用するため敵自身が削除する
	MV1DeleteModel(m_modelHandle);
}

void EnemyBase::RotateToPlayer()
{
	Vector3 toPlayer = m_pPlayer->GetPos() - m_pos;
	float angle = toPlayer.Angle();
	m_angle = angle;
	m_drawAngle = angle;
}
