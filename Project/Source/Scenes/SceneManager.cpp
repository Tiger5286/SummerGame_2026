#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneLoad.h"
#include "DxLib.h"
#include "Singleton/FadeManager.h"

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> pNewScene, bool isCallLoadScene)
{
	// シーンが空なら新しいシーンを追加する
	if (m_pScenes.empty())
	{
		m_pScenes.push_back(pNewScene);
	}
	else	// シーンが空でないなら最後のシーンを新しいシーンにする
	{
		m_pScenes.back()->End();
		m_pScenes.back() = pNewScene;
	}

	// ロードシーンを使用するなら非同期読み込みを有効にする
	//if (isCallLoadScene) SetUseASyncLoadFlag(true);

	// 新しいシーンの初期化
	pNewScene->Init();

	// ロードシーンを追加する
	if (isCallLoadScene)
	{
		m_pScenes.push_back(std::make_shared<SceneLoad>(*this));
		m_pScenes.back()->Init();
	}
}

void SceneManager::ChangeSceneWithFade(std::shared_ptr<SceneBase> pNewScene, bool isCallLoadScene)
{
	m_pNextScene = pNewScene;
	m_isCallLoadSceneNext = isCallLoadScene;
	m_nextChangeScene = NextChangeScene::Change;
	FadeManager::GetInstance().StartFadeOut();
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

void SceneManager::PopSceneWithFade()
{
	m_nextChangeScene = NextChangeScene::Pop;
	FadeManager::GetInstance().StartFadeOut();
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

void SceneManager::ResetSceneWithFade(std::shared_ptr<SceneBase> pNewScene)
{
	m_pNextScene = pNewScene;
	m_nextChangeScene = NextChangeScene::Reset;
	FadeManager::GetInstance().StartFadeOut();
}

void SceneManager::Update()
{
	m_pScenes.back()->Update();
	auto& fadeManager = FadeManager::GetInstance();

	if (fadeManager.IsNotFading() && m_nextChangeScene != NextChangeScene::None)
	{
		fadeManager.StartFadeIn();
		switch (m_nextChangeScene)
		{
		case NextChangeScene::Change:
			ChangeScene(m_pNextScene, m_isCallLoadSceneNext);
			break;
		case NextChangeScene::Reset:
			ResetScene(m_pNextScene);
			break;
		case NextChangeScene::Pop:
			PopScene();
			break;
		}

		m_pNextScene = nullptr;
		m_isCallLoadSceneNext = false;
		m_nextChangeScene = NextChangeScene::None;
	}
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
