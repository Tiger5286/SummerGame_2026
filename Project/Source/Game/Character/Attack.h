#pragma once
#include "Character.h"
#include <memory>
#include <list>

class Attack :
    public Character, public std::enable_shared_from_this<Attack>
{
public:
    struct Data
    {
        float colliderRadius = 0.0f;
        int damage = 0;
        Character::Type hitCharacterType = Character::Type::None;
    };

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
    void SetData(Data data) { m_data = data; }

private:
    std::list<int> m_hitIds;
    Data m_data;
};

