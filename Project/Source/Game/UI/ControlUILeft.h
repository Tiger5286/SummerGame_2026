#pragma once
#include "UIBase.h"
#include <vector>

class ControlUILeft :
    public UIBase
{
public:
    ControlUILeft();
    ~ControlUILeft();

    void Init() override;
    void Update() override;
    void Draw() override;

    // ControlUIのフォントをもらってくる
    void SetInfo(int fontHandle) { m_fontHandle = fontHandle; }

private:
    std::vector<int> m_handles;
    int m_fontHandle = -1;
};

