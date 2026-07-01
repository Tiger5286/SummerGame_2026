#pragma once
#include "UIBase.h"

class EnemyBase;
class Camera;

class EnemyHpBar : public UIBase
{
public:
	EnemyHpBar();
	~EnemyHpBar();

	void Init() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// 必要な情報を設定する
	/// </summary>
	/// <param name="pEnemy">このUIを持つ敵のポインタ</param>
	/// <param name="maxHp">このUIを持つ敵の最大HP</param>
	void SetInfo(std::shared_ptr<EnemyBase> pEnemy, int maxHp);

private:
	std::weak_ptr<EnemyBase> m_pEnemy;
	int m_maxHp = 0;
	int m_currentHp = 0;
};

