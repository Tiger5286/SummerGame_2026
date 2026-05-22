#pragma once
#include "DxLib.h"
#include "Vector3.h"
#include "Matrix4x4.h"

namespace MyLib
{
	const Vector3 kDefaultDir = Vector3(0, 0, -1);

	/// <summary>
	/// 扇形を描画する
	/// </summary>
	/// <param name="pos">中心点の位置</param>
	/// <param name="angle">扇がどっちを向いているか(ラジアン)</param>
	/// <param name="fanAngle">扇の中心角(ラジアン)</param>
	/// <param name="length">扇の半径</param>
	/// <param name="divNum">何分割の扇を描画するか</param>
	void DrawFan3D(const Vector3& pos,float angle,float fanAngle,float radius,int divNum)
	{
		Vector3 lineVec1 = kDefaultDir * Matrix4x4::GetRotY(fanAngle) * radius;
		Vector3 lineVec2 = kDefaultDir * Matrix4x4::GetRotY(-fanAngle) * radius;
		Vector3 lineVec3 = kDefaultDir * radius;
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
}