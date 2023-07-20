#pragma once
#include "vanScene.h"

namespace van
{
	class SceneManager
	{
	public:
		static void Init();
		static void Update();
		static void Render(HDC _hdc);

		template <typename T>
		static __inline T* CreateScene(const std::wstring& name)
		{
			T* scene = new T();	
			//scene->SetName(name);							// SetName() ���� �ȵǾ� ����...
			mScenes.insert(std::make_pair(name, scene));	// mScenes �� ������ scene �ֱ�
			mActiveScene = scene;							// mActiveScene ����
			scene->Init();									// GameObject ��ü ����

			return scene;
		}

		static Scene* LoadScene(const std::wstring& name);
		__forceinline static Scene* GetActiveScene() { return mActiveScene; }

	private:
		/*
			Ʈ�� �ڷᱸ�� ����� ����
			-> ���� : �־��� ��츦 �������� �� ���ϴ� �� ã�� �ӵ��� ������.

			[ map ]
			- key,value ������ �̷�����ִ�.
			- key�� ���ϴ� ���� �˻��ϴ� �뵵
			- value�� ����� ��(=ã�� ��)
		 */
		static std::map<std::wstring, Scene*> mScenes;
		static Scene* mActiveScene;
	};
}
