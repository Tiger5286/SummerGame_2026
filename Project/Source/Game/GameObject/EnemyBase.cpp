#include "EnemyBase.h"
#include "DxLib.h"

EnemyBase::EnemyBase()
{}

EnemyBase::~EnemyBase()
{
	// 敵はDuplicateされたモデルを使用するため敵自身が削除する
	MV1DeleteModel(m_modelHandle);
}
