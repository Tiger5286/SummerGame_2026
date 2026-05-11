#pragma once
#include "../Utility/Vector3.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init() abstract;
	virtual void End() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

	/// <summary>
	/// モデルのハンドルを設定する。Initの前に実行すること
	/// </summary>
	/// <param name="handle">ロードしたモデルのハンドル</param>
	void SetHandle(int handle) { m_modelHandle = handle; }

protected:
	// モデルのハンドル
	int m_modelHandle = -1;

	Vector3 m_pos;	// 位置
	Vector3 m_vel;	// 速度
};

