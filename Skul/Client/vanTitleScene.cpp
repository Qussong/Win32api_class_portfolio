#include "vanTitleScene.h"
#include "vanPlayer.h"
#include "vanObject.h"
#include "vanTransform.h"
#include "vanSpriteRenderer.h"
#include "vanTexture.h"
#include "vanResourceManager.h"
#include "vanBackGround.h"
#include "vanCamera.h"

#include "vanMonster.h"
#include "vanAnimator.h"

namespace van
{
	TitleScene::TitleScene()
	{
		// nothing
	}

	TitleScene::~TitleScene()
	{
		// nothing
	}

	void TitleScene::Init()
	{
		// 1) Title Art
		BackGround* bg = Object::Instantiate<BackGround>(enums::eLayerType::BackGround);
		bg->GetComponent<Transform>()->SetPosition(math::Vector2(Window_X / 2, Window_Y / 2));
		SpriteRenderer* bgsr = bg->AddComponent<SpriteRenderer>();
		bgsr->SetTexture(ResourceManager::Find<Texture>(L"BG_Title_Art"));
		bgsr->SetScale(math::Vector2(0.67f,0.67f));
		bgsr->SetAffectCamera(false);

		// 2) Title Logo
		BackGround* logo = Object::Instantiate<BackGround>(enums::eLayerType::BackGround);
		logo->GetComponent<Transform>()->SetPosition(math::Vector2(Window_X / 2, Window_Y / 2 + 200));
		bgsr = logo->AddComponent<SpriteRenderer>();
		bgsr->SetTexture(ResourceManager::Find<Texture>(L"BG_Title_Logo"));
		bgsr->SetScale(math::Vector2(0.67f,0.67f));
		bgsr->SetAffectCamera(false);

		//SetSceneTarget(nullptr);	// 기본값 nullptr이라 생략 가능
		Camera::SetTarget(GetSceneTarget());
	}

	void TitleScene::Update()
	{
		Camera::SetTarget(GetSceneTarget());
		Scene::Update();						// 부모의 Update 함수 호출
	}

	void TitleScene::Render(HDC _hdc)
	{
		Scene::Render(_hdc);					// 부모의 Render 함수 호출

		// Scene 구분
		const wchar_t* str = L"[ TitleScene ]";
		Text::PrintwString(_hdc, 10, 30, str);	// 아래 두면 가려지지 않는다.
	}
	void TitleScene::SceneIN()
	{
	}
	void TitleScene::SceneOut()
	{
	}
}