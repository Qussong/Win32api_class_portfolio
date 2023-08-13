#pragma once
#include "vanScene.h"

namespace van
{
	class TestScene : public Scene
	{
	public:
		TestScene();
		virtual ~TestScene();

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render(HDC _hdc) override;
		virtual void SceneIN() override;
		virtual void SceneOut() override;

	private:

	};
}
