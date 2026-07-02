#pragma once
#include "Utility/Vector3.h"
#include <memory>

class UIBase
{
public:
	UIBase(int layer);
	virtual ~UIBase();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool IsAlive() const { return m_isAlive; }
	bool IsVisible() const { return m_isVisible; }
	int GetLayer() const { return m_layer; }

protected:
	// x,yしか使わない
	Vector3 m_pos;
	int m_RTHandle = -1;
	bool m_isAlive = true;
	bool m_isVisible = true;
	// UIの描画順を決めるためのレイヤー(増えるほど手前に描画される)
	int m_layer = 0;
};