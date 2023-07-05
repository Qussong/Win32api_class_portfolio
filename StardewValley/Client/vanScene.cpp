#include "vanScene.h"

namespace van
{
	Scene::Scene()
	{
		mLayers.resize((int)/*enums::*/eLayerType::End);
	}
	Scene::~Scene()
	{
	}
	void Scene::Init()
	{
		mLayers[(int)/*enums::*/eLayerType::Player].AddGameObject();
	}
	void Scene::Update()
	{
		// mLayers ���� Layer ��ü���� ������ �����ؼ� �������� ������ �����ڷ� ó���Ѵ�. (�޸� ��뷮 ��)
		for (Layer& layer : mLayers)
			layer.Update();

	}
	void Scene::Render(HDC _hdc)
	{
		for (Layer& layer : mLayers)
			layer.Render(_hdc);
	}
}