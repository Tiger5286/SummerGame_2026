#pragma once
#include "Character.h"
#include <memory>
#include <list>
#include "Utility/MyLib.h"

class Attack :
    public Character, public std::enable_shared_from_this<Attack>
{
public:
    Attack() = default;
    ~Attack() = default;

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

    void OnCollision(Character& other) override;

    /// <summary>
    /// 攻撃の情報を設定する。Initの前に実行する。
    /// </summary>
    /// <param name="data"></param>
    void SetData(MyLib::AttackData data) { m_data = data; }

private:
    std::list<int> m_hitIds;
    MyLib::AttackData m_data;
};

