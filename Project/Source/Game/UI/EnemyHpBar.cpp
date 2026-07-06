#include "EnemyHpBar.h"
#include "DxLib.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../Camera/Camera.h"
#include "Singleton/CameraSetter.h"
#include "../Character/Player/Player.h"

namespace
{
	constexpr float kDrawDist = 1500;
}

EnemyHpBar::EnemyHpBar() : 
	UIBase(0)
{

}

EnemyHpBar::~EnemyHpBar()
{
}

void EnemyHpBar::SetInfo(std::shared_ptr<EnemyBase> pEnemy, std::shared_ptr<Player> pPlayer, int maxHp)
{
	m_pEnemy = pEnemy;
	m_pPlayer = pPlayer;
	m_maxHp = maxHp;
}

void EnemyHpBar::Init()
{
	m_RTHandle = MakeScreen(100, 10);
	m_currentHp = m_maxHp;
}

void EnemyHpBar::Update()
{
	// 敵が死んでいたらUIを消す
	if (m_pEnemy.expired())
	{
		m_isAlive = false;
		return;
	}

	auto pEnemy = m_pEnemy.lock();
	if (pEnemy->IsDying())
	{
		m_isAlive = false;
	}
}

void EnemyHpBar::Draw()
{
	auto pEnemy = m_pEnemy.lock();

	// Hpバーを描画
	SetDrawScreen(m_RTHandle);
	ClearDrawScreen();

	float hpRate = static_cast<float>(pEnemy->GetHP()) / static_cast<float>(m_maxHp);

	float x = 100 * hpRate;
	DrawBox(0, 0, 100 * hpRate, 10, 0xff0000, true);
	DrawBox(0, 0, 100, 10, 0x000000, false, 2);

	SetDrawScreen(DX_SCREEN_BACK);

	// レンダーターゲットの変更により、カメラがリセットされるので戻す
	CameraSetter::GetInstance().SetCameraSetting();

	// UIの描画位置(2D)を算出
	auto screenPos = ConvWorldPosToScreenPos((pEnemy->GetPos() + Vector3(0, 200, 0)).ToDxLib());
	// UIを描画
	bool isInScreen = screenPos.z > 0.0f && screenPos.z < 1.0f;
	float sqrDist = (m_pPlayer.lock()->GetPos() - pEnemy->GetPos()).SquaredLength();
	bool isDrawDist = sqrDist < kDrawDist * kDrawDist;
	if (isInScreen && isDrawDist)
	{
		DrawRotaGraph(screenPos.x, screenPos.y, 1.0, 0.0, m_RTHandle, false);
	}
}
