#pragma once
#include "Dxlib.h"

/// <summary>
/// 3次元ベクトルを表すクラス
/// </summary>
class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);
	~Vector3();

	// ベクトルの成分
	float x, y, z;

// ----- ユーティリティ関数 -----
	static inline Vector3 Zero()  { return Vector3(0.0f, 0.0f, 0.0f); }
	static inline Vector3 Up()    { return Vector3(0.0f, 1.0f, 0.0f); }
	static inline Vector3 Down()  { return Vector3(0.0f, -1.0f, 0.0f); }
	static inline Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
	static inline Vector3 Left()  { return Vector3(-1.0f, 0.0f, 0.0f); }
	static inline Vector3 Front() { return Vector3(0.0f, 0.0f, 1.0f); }
	static inline Vector3 Back()  { return Vector3(0.0f, 0.0f, -1.0f); }

	static void Draw(const Vector3& start, const Vector3& end);

// ----- 変換関数 -----
	// DxLib用のベクトルに変換して返す
	VECTOR ToDxLib() const;
	// DxLib用のベクトルをこのクラスのベクトルに変換して返す
	static Vector3 FromDxLib(const VECTOR& vec);

// ----- 計算関数 -----
	// ベクトルの大きさの二乗を返す
	float SquaredLength() const;
	// ベクトルの大きさを返す
	float Length() const;

	// 自身のベクトルを正規化する
	void Normalize();
	// 正規化されたベクトルを返す
	Vector3 Normalized() const;

	// ベクトルの内積を返す
	float Dot(const Vector3& v) const;
	// ベクトルの外積を返す
	Vector3 Cross(const Vector3& v) const;

	// ベクトルの線形補間をする
	void Lerp(const Vector3& v, float t);

// ----- 演算子オーバーロード -----
	// 反転したベクトルを返す
	Vector3 operator-() const;
	// ベクトルの足し算
	Vector3 operator+(const Vector3& v) const;
	void operator+=(const Vector3& v);
	// ベクトルの引き算
	Vector3 operator-(const Vector3& v) const;
	void operator-=(const Vector3& v);
	// ベクトルの掛け算
	Vector3 operator*(float s) const;
	void operator*=(float s);
	// ベクトルの割り算
	Vector3 operator/(float s) const;
	void operator/=(float s);

	// ベクトルが同じかどうか
	bool operator==(const Vector3& v) const;
	// ベクトルが同じでないかどうか
	bool operator!=(const Vector3& v) const;
};

