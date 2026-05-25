#pragma once
#include "ColliderBase.h"
#include "DxLib.h"

class CapsuleCollider :
    public ColliderBase
{
public:
	CapsuleCollider(float radius, float height);
	virtual ~CapsuleCollider();

	void Draw() override;

	float GetRadius() const { return m_radius; }
	float GetHeight() const { return m_height; }

	/// <summary>
	/// モデル(主にマップ)との当たり判定を行う。
	/// この関数の返り値は動的にメモリを確保しているので、使用したのちMV1CollResultPolyDimTerminate()で解放する必要がある。
	/// </summary>
	/// <param name="mapHandle">当たり判定用のマップモデルのハンドル</param>
	/// <returns>当たり判定結果</returns>
	MV1_COLL_RESULT_POLY_DIM CheckCollModel(int mapHandle);

private:
	float m_radius = 0.0f;	// 半径
	float m_height = 0.0f;	// 高さ
};