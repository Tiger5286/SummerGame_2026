#include "SceneManager.h"
#include "SceneBase.h"

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> pNewScene)
{
	if (m_pScenes.empty())
	{
		m_pScenes.push_back(pNewScene);
	}
	else
	{
		m_pScenes.back()->End();
		m_pScenes.back() = pNewScene;
	}
	pNewScene->Init();
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> pNewScene)
{
	m_pScenes.push_back(pNewScene);
	pNewScene->Init();
}

void SceneManager::PopScene()
{
	if (m_pScenes.size() > 1)
	{
		m_pScenes.back()->End();
		m_pScenes.pop_back();
	}
}

void SceneManager::ResetScene(std::shared_ptr<SceneBase> pNewScene)
{
	for (auto& scene : m_pScenes)
	{
		scene->End();
	}
	m_pScenes.clear();
	m_pScenes.push_back(pNewScene);
	pNewScene->Init();
}

void SceneManager::Update()
{
	m_pScenes.back()->Update();
}

void SceneManager::Draw()
{
	for (auto& scene : m_pScenes)
	{
		scene->Draw();
	}
}

void SceneManager::End()
{
	for (auto& scene : m_pScenes)
	{
		scene->End();
	}
	m_pScenes.clear();
}
