#pragma once
#include "Character.h"
#include <memory>
#include <list>
#include "Utility/MyLib.h"

class CharacterStateBase;

class Attack :
    public Character
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
    //void SetData(MyLib::AttackData data) { m_data = data; }

    void SetData(MyLib::AttackData data, std::shared_ptr<CharacterStateBase> pOwner);

    int GetMaxHP() const override { return -1; }

private:
    std::list<int> m_hitIds;
    MyLib::AttackData m_data;
    std::weak_ptr<CharacterStateBase> m_pOwner;
};

