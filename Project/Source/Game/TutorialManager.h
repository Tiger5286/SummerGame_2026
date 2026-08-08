#pragma once
#include <memory>

class TutorialUI;

class TutorialManager : public std::enable_shared_from_this<TutorialManager>
{
public:
	TutorialManager() = default;
	~TutorialManager() = default;

	void Init();
	void Update();

	enum class TutorialStep
	{
		Move,
		Attack,
		LockOnAndShift,
		Jump,
		Skill,
		Dodge,
		SpecialAttack,
		End,

		Num
	};
	enum class ConditionsForProgression
	{
		EnemySpawned,
		EnemyDefeated,
		None
	};

private:
	void OnEnemySpawned();
	void OnEnemyDefeated();

	void ProceedTutorial();

private:
	TutorialStep m_currentStep = TutorialStep::Move;
	std::shared_ptr<TutorialUI> m_tutorialUI;
};

