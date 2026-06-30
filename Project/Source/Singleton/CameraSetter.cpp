#include "CameraSetter.h"
#include "DxLib.h"
#include "../Game/Camera/Camera.h"

CameraSetter& CameraSetter::GetInstance()
{
	static CameraSetter instance;
	return instance;
}

void CameraSetter::Init(std::shared_ptr<Camera> pCamera)
{
	m_pCamera = pCamera;
}

void CameraSetter::SetCameraSetting()
{
	SetCameraPositionAndTarget_UpVecY(m_pCamera->GetPos().ToDxLib(), m_pCamera->GetTargetPos().ToDxLib());
}
