#include "vanEndingScene.h"
#include "vanObject.h"
#include "vanSpriteRenderer.h"
#include "vanPlayer.h"
#include "vanResourceManager.h"

namespace van
{
	EndingScene::EndingScene()
	{
		// nothing
	}

	EndingScene::~EndingScene()
	{
		// nothing
	}

	void EndingScene::Init()
	{
		/*Player* player = Object::Instantiate<Player>(enums::eLayerType::Player);
		player->AddComponent<SpriteRenderer>();*/
		// 1) Player ��ü
		Player* player = Object::Instantiate<Player>(enums::eLayerType::Player);
		SpriteRenderer* sr = player->AddComponent<SpriteRenderer>();
		Image* image = ResourceManager::Load<Image>(L"TitleBackGroundImage", L"..\\Resources\\Image\\Fighter.bmp");
		sr->SetImage(image);
	}

	void EndingScene::Update()
	{
		Scene::Update();
	}

	void EndingScene::Render(HDC _hdc)
	{
		Scene::Render(_hdc);

		// Scene ����
		const wchar_t* str = L"[ EndingScene ]";
		int len = (int)wcslen(str);
		Text::PrintwString(_hdc, 10, 30, str);
	}
}