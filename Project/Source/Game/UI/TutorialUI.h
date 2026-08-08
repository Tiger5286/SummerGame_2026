#pragma once
#include "UIBase.h"
#include "Game/TutorialManager.h"

class TutorialUI :
    public UIBase
{
public:
    TutorialUI();
    ~TutorialUI();

	void Init() override;
	void Update() override;
	void Draw() override;

	void SetTutorialStep(TutorialManager::TutorialStep step) { m_currentStep = step; }

private:
	TutorialManager::TutorialStep m_currentStep = TutorialManager::TutorialStep::Move;
};