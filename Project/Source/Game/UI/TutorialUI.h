#pragma once
#include "UIBase.h"
#include "Game/TutorialManager.h"
#include <vector>

class TutorialUI :
    public UIBase
{
public:
    TutorialUI();
    ~TutorialUI();

	void Init() override;
	void Update() override;
	void Draw() override;

	void SetTutorialStep(TutorialManager::TutorialStep step) { m_currentStep = step; m_iconScaleAngle = 0.0f; }

private:
	TutorialManager::TutorialStep m_currentStep = TutorialManager::TutorialStep::Move;

	int m_fontHandle = -1;
	int m_iconHandle = -1;
	float m_iconScaleAngle = 0.0f;

	std::vector<int> m_buttonHandles;
};