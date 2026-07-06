#include "EnemyBase.h"
#include "DxLib.h"
#include "../Player/Player.h"
#include "../../UI/EnemyHpBar.h"
#include "Singleton/UIManager.h"

EnemyBase::EnemyBase()
{}

EnemyBase::~EnemyBase()
{
	// 敵はDuplicateされたモデルを使用するため敵自身が削除する
	MV1DeleteModel(m_modelHandle);
}

void EnemyBase::BaseInit(int maxHp)
{
	m_pHpBar = std::make_shared<EnemyHpBar>();
	auto enemy = std::dynamic_pointer_cast<EnemyBase>(shared_from_this());
	m_pHpBar->SetInfo(enemy, m_pPlayer,maxHp);
	UIManager::GetInstance().AddUI(m_pHpBar);
}

void EnemyBase::RotateToPlayer()
{
	Vector3 toPlayer = m_pPlayer->GetPos() - m_pos;
	float angle = toPlayer.Angle();
	m_angle = angle;
	m_drawAngle = angle;
}
