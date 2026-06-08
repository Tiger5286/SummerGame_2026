#pragma once
#include "Utility/Vector3.h"
#include <memory>

class Player;
class Character;

class Camera
{
public:
	Camera() = default;
	virtual ~Camera() = default;

	void Init();
	void Update();

	float GetAngleY() const { return m_angleY; }
	Vector3 GetPos() const { return m_pos; }
	void SetPos(const Vector3& pos) { m_pos = pos; }

	// マップのハンドルを設定する(Initの前に呼ぶ)
	void SetMapHandle(int handle) { m_mapHandle = handle; }

	// プレイヤーのポインタを設定する
	void SetPlayer(std::weak_ptr<Player> pPlayer) { m_pPlayer = pPlayer; }

	// ターゲットを設定する
	void SetTarget(std::shared_ptr<Character> pTarget) { m_pTarget = pTarget; }

private:
	std::weak_ptr<Player> m_pPlayer;
	int m_mapHandle = -1;

	Vector3 m_pos;
	Vector3 m_targetPos;
	float m_angleY = 0.0f;
	float m_angleX = 0.0f;

	std::shared_ptr<Character> m_pTarget = nullptr;
};

