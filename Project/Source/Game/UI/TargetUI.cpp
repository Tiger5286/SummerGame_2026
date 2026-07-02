#include "TargetUI.h"
#include "DxLib.h"
#include <cassert>
#include "../Managers/TargetManager.h"
#include "../Managers/EnemyManager.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../Character/Player/Player.h"
#include "../Camera/Camera.h"
#include "../Collider/CapsuleCollider.h"
#include "Game.h"

TargetUI::TargetUI() :
	UIBase(1)
{

}

TargetUI::~TargetUI()
{
	DeleteGraph(m_arrowHandle);
	DeleteGraph(m_targetHandle);
}

void TargetUI::Init()
{
	m_arrowHandle = LoadGraph(L"data/Graphs/EnemyArrow.png");
	assert(m_arrowHandle != -1);
	m_targetHandle = LoadGraph(L"data/Graphs/target.png");
	assert(m_targetHandle != -1);
}

void TargetUI::Update()
{
	// TargetManagerが破棄されていたら自分も破棄する
	if (m_pTargetManager.expired())
	{
		m_isAlive = false;
		return;
	}

	m_targetFrame++;
}

void TargetUI::Draw()
{
	auto pTM = m_pTargetManager.lock();
	// 敵がいなかったらreturn
	auto enemies = pTM->m_pEnemyManager->GetEnemies();
	if (enemies.empty()) return;

	// 画面外の敵のみのリストを作成
	auto inScreenEnemies = pTM->GetInScreenEnemies(enemies);
	auto outScreenEnemies = enemies;
	outScreenEnemies = pTM->GetAliveEnemies(outScreenEnemies);
	outScreenEnemies = pTM->GetInSearchAreaEnemies(outScreenEnemies);
	for (auto& enemy : inScreenEnemies)
	{
		outScreenEnemies.remove(enemy);
	}
	// 画面外の敵でforを回す
	for (auto& enemy : outScreenEnemies)
	{
		// プレイヤーから敵へのベクトルを作成
		Vector3 vec = enemy->GetPos() - pTM->m_pPlayer->GetPos();
		// ベクトルの角度を算出			// カメラの方向を足す
		float angle = atan2(vec.z, vec.x) + pTM->m_pCamera->GetAngleY();
		// 2D座標にする
		float x = cos(angle);
		float y = sin(angle);
		// カメラがある程度上を向いていたらyを反転
		if (pTM->m_pCamera->GetAngleX() > -0.3f)
		{
			y = -y;
			angle = -angle;
		}
		// 方向を描画
		int graphX = Game::kScreenWidth / 2 + x * (Game::kScreenWidth / 2 - Game::kScreenWidth / 10);
		int graphY = Game::kScreenHeight / 2 + y * (Game::kScreenHeight / 2 - Game::kScreenHeight / 10);
		DrawRotaGraph(graphX, graphY, 0.5, angle, m_arrowHandle, true);
	}

	// ターゲットがいないなら処理しない
	if (pTM->m_pTarget == nullptr)
	{
		return;
	}

	// ターゲットのコライダーの種類によって描画位置を変える
	auto pos = Vector3::Zero();
	auto colType = pTM->m_pTarget->GetCollider()->GetType();
	if (colType == ColliderType::Sphere)
	{	// 球なら球の中心を描画位置にする
		auto col = pTM->m_pTarget->GetCollider();
		pos = col->GetPos();
	}
	else if (colType == ColliderType::Capsule)
	{	// カプセルならカプセルの真ん中を描画位置にする
		auto capsule = std::dynamic_pointer_cast<CapsuleCollider>(pTM->m_pTarget->GetCollider());
		pos = capsule->GetPos() + Vector3::Up() * capsule->GetHeight() / 2;
	}
	auto screenPos = ConvWorldPosToScreenPos(pos.ToDxLib());
	// スクリーン座標のzが0.0~1.0の範囲でなければ無効
	if (screenPos.z > 0.0f && screenPos.z < 1.0f)
	{
		m_targetAngle += 0.01f;

		if (m_targetFrame > 120)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		}

		DrawRotaGraph(screenPos.x, screenPos.y, 0.1, m_targetAngle, m_targetHandle, true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
