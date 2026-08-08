#include "TutorialManager.h"
#include "Singleton/EventManager.h"
#include "Singleton/UIManager.h"
#include "UI/TutorialUI.h"

namespace
{
	constexpr int kTutorialStepNum = static_cast<int>(TutorialManager::TutorialStep::Num);

	constexpr TutorialManager::ConditionsForProgression kTutorialConditions[kTutorialStepNum] = {
		TutorialManager::ConditionsForProgression::EnemySpawned,	// 移動→攻撃
		TutorialManager::ConditionsForProgression::EnemySpawned,	// 攻撃→ロックオン＆シフト
		TutorialManager::ConditionsForProgression::EnemyDefeated,	// ロックオン＆シフト→ジャンプ
		TutorialManager::ConditionsForProgression::EnemySpawned,	// ジャンプ→スキル
		TutorialManager::ConditionsForProgression::EnemySpawned,	// スキル→回避
		TutorialManager::ConditionsForProgression::EnemySpawned,	// 回避→必殺技
		TutorialManager::ConditionsForProgression::EnemyDefeated,	// 必殺技→チュートリアル終了
		TutorialManager::ConditionsForProgression::None				// チュートリアル終了(次に行かない)
	};
}

void TutorialManager::Init()
{
	auto& eventManager = EventManager::GetInstance();
	eventManager.Register("OnEnemySpawned", [this]() { OnEnemySpawned(); }, shared_from_this());
	eventManager.Register("OnEnemyDefeated", [this]() { OnEnemyDefeated(); }, shared_from_this());
	m_tutorialUI = std::make_shared<TutorialUI>();
	UIManager::GetInstance().AddUI(m_tutorialUI);
}

void TutorialManager::Update()
{
}

void TutorialManager::OnEnemySpawned()
{
	if (kTutorialConditions[static_cast<int>(m_currentStep)] == ConditionsForProgression::EnemySpawned)
	{
		ProceedTutorial();
	}
}

void TutorialManager::OnEnemyDefeated()
{
	if (kTutorialConditions[static_cast<int>(m_currentStep)] == ConditionsForProgression::EnemyDefeated)
	{
		ProceedTutorial();
	}
}

void TutorialManager::ProceedTutorial()
{
	m_currentStep = static_cast<TutorialStep>(static_cast<int>(m_currentStep) + 1);
	m_tutorialUI->SetTutorialStep(m_currentStep);
}
