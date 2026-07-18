#pragma once
#include "DxLib.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cmath>
#include <cassert>
#include <string>
#include <memory>

class Character;

namespace MyLib
{
	constexpr float kDefaultAnimSpeed = 0.5f;	// デフォルトのアニメーション再生速度

	enum class LR
	{
		Left,
		Right
	};

	enum class CharacterType
	{
		None,
		Player,
		Enemy,

		Num
	};

	// 敵の種類を表す列挙体
	enum class EnemyType
	{
		None,
		Zombie,
		Vulture,
		General,

		Num
	};

	struct AttackData
	{
		float colliderRadius = 0.0f;
		int damage = 0;
		CharacterType hitCharacterType = CharacterType::None;
		bool isKnockDown = false;
		bool isIgnoreInvincible = false;
		int specialCharge = 0;
		std::weak_ptr<Character> attacker;
	};

	const Vector3 kDefaultDir = Vector3(0, 0, -1);

	/// <summary>
	/// 扇形を描画する
	/// </summary>
	/// <param name="pos">中心点の位置</param>
	/// <param name="angle">扇がどっちを向いているか(ラジアン)</param>
	/// <param name="fanAngle">扇の中心角(ラジアン)</param>
	/// <param name="length">扇の半径</param>
	/// <param name="divNum">何分割の扇を描画するか</param>
	inline void DrawFan3D(const Vector3& pos,float angle,float fanAngle,float radius,int divNum)
	{
		Vector3 lineVec1 = kDefaultDir * Matrix4x4::GetRotY(fanAngle) * Matrix4x4::GetRotY(angle) * radius;
		Vector3 lineVec2 = kDefaultDir * Matrix4x4::GetRotY(-fanAngle) * Matrix4x4::GetRotY(angle) * radius;
		Vector3 lineVec3 = kDefaultDir * Matrix4x4::GetRotY(angle) * radius;
		DrawLine3D(pos.ToDxLib(), (pos + lineVec1).ToDxLib(), 0xff0000);
		DrawLine3D(pos.ToDxLib(), (pos + lineVec2).ToDxLib(), 0xff0000);
		DrawLine3D(pos.ToDxLib(), (pos + lineVec3).ToDxLib(), 0xff0000);

		const auto theta = fanAngle / divNum;
		for (int i = 0; i < divNum; i++)
		{
			Vector3 tempV = lineVec2 * Matrix4x4::GetRotY(theta * i);
			Vector3 tempV2 = lineVec2 * Matrix4x4::GetRotY(theta * (i + 1));
			DrawLine3D((pos + tempV).ToDxLib(), (pos + tempV2).ToDxLib(), 0xff0000);

			tempV = lineVec1 * Matrix4x4::GetRotY(-theta * i);
			tempV2 = lineVec1 * Matrix4x4::GetRotY(-theta * (i + 1));
			DrawLine3D((pos + tempV).ToDxLib(), (pos + tempV2).ToDxLib(), 0xff0000);
		}
	}

	/// <summary>
	/// 円形を描画する
	/// </summary>
	/// <param name="pos">中心点の位置</param>
	/// <param name="radius">円の半径</param>
	/// <param name="divNum">何分割で描画するか</param>
	inline void DrawCircle3D(const Vector3& pos, float radius, int divNum)
	{
		for (int i = 0; i < divNum; i++)
		{
			Vector3 tempV = kDefaultDir * Matrix4x4::GetRotY(DX_PI_F * 2 / divNum * i) * radius;
			Vector3 tempV2 = kDefaultDir * Matrix4x4::GetRotY(DX_PI_F * 2 / divNum * (i + 1)) * radius;
			DrawLine3D((pos + tempV).ToDxLib(), (pos + tempV2).ToDxLib(), 0xff0000);
		}
	}

	/// <summary>
	/// 二つの角度の最短距離を求める
	/// </summary>
	/// <param name="angle1">角度1(ラジアン)</param>
	/// <param name="angle2">角度2(ラジアン)</param>
	/// <returns>二つの角度の最短距離</returns>
	inline float GetAngleDif(float angle1, float angle2)
	{
		float diff = angle1 - angle2 - DX_TWO_PI_F;
		while (diff > DX_PI_F) diff -= DX_TWO_PI_F;
		while (diff < -DX_PI_F) diff += DX_TWO_PI_F;
		return diff;
	}

	/// <summary>
	/// ベクトルからatan2で角度を取得する。
	/// </summary>
	/// <param name="y">y</param>
	/// <param name="x">x</param>
	/// <returns>角度(ラジアン)</returns>
	inline float GetAngleVec(float y,float x)
	{
		return atan2f(y, -x) + DX_PI_F / 2;
	}

	/// <summary>
	/// 角度からベクトルを取得する。
	/// </summary>
	/// <param name="angle">角度(ラジアン)</param>
	/// <returns>向いている方向の単位ベクトル</returns>
	inline Vector3 GetVecAngle(float angle)
	{
		return kDefaultDir * Matrix4x4::GetRotY(angle);
	}

	/// <summary>
	/// ワイド文字列をマルチバイト文字列に変換する。
	/// </summary>
	/// <param name="wstr">変換したいワイド文字列</param>
	/// <returns>変換後のマルチバイト文字列</returns>
	inline std::string WStringToString(const std::wstring& wstr)
	{
		std::string ret;
		//一度目の呼び出しは文字列数を知るため
		auto result = WideCharToMultiByte(
			CP_ACP,
			0,
			wstr.c_str(),//入力文字列
			wstr.length(),
			nullptr,
			0,
			nullptr,
			nullptr);
		assert(result >= 0);
		ret.resize(result);//確保する
		//二度目の呼び出しは変換
		result = WideCharToMultiByte(
			CP_ACP,
			0,
			wstr.c_str(),//入力文字列
			wstr.length(),
			ret.data(),
			ret.size(),
			nullptr,
			nullptr);
		return ret;
	}

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換する
	/// </summary>
	/// <param name="str">変換したいマルチバイト文字列</param>
	/// <returns>変換後のワイド文字列</returns>
	inline std::wstring StringToWString(const std::string& str)
	{
		std::wstring ret;
		//一度目の呼び出しは文字列数を知るため
		auto result = MultiByteToWideChar(CP_UTF8,
			0,
			str.c_str(),//入力文字列
			str.length(),
			nullptr,
			0);
		assert(result >= 0);
		ret.resize(result);//確保する
		//二度目の呼び出しは変換
		result = MultiByteToWideChar(CP_UTF8,
			0,
			str.c_str(),//入力文字列
			str.length(),
			ret.data(),
			ret.size());
		return ret;
	}

	inline void ReadVector3(Vector3& vec, int fileHandle)
	{
		FileRead_read(&vec.x, sizeof(float), fileHandle);
		FileRead_read(&vec.y, sizeof(float), fileHandle);
		FileRead_read(&vec.z, sizeof(float), fileHandle);
	}
}