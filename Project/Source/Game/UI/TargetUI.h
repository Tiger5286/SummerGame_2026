#pragma once
#include "UIBase.h"

class TargetManager;

class TargetUI :
    public UIBase
{
public:
    TargetUI();
    ~TargetUI();

    void Init() override;
    void Update() override;
    void Draw() override;

	// 必要な情報を設定する
	void SetInfo(std::shared_ptr<TargetManager> pTargetManager) { m_pTargetManager = pTargetManager; }

	void ResetTargetFrame() { m_targetFrame = 0; }

private:
	std::weak_ptr<TargetManager> m_pTargetManager;

    int m_arrowHandle = -1;

	int m_targetHandle = -1;
    float m_targetAngle = 0.0f;
	int m_targetFrame = 0;
};

