#pragma once
#include "vanGameObject.h"

namespace van
{
	class Camera
	{
	public:
		static void Init();
		static void Update();

		__forceinline static math::Vector2 CalculatePosition(math::Vector2 pos) { return pos - mDistance; }
		__forceinline static GameObject* GetTarget() { return mTarget; }
		__forceinline static void SetTarget(GameObject* _target) { mTarget = _target; }
		__forceinline static void SetLimitDistance(math::Vector2 _widthLimit, math::Vector2 _heightLimit)
		{
			mWidthLimit = _widthLimit;
			mHeightLimit = _heightLimit;
		}

	private:
		
		static math::Vector2 mResolution;	// 화면 해상도
		static math::Vector2 mLookPosition;	// 카메라가 바라보고 있는 위치
		static math::Vector2 mDistance;		// 가운데를 원점으로 이동한 거리
		static GameObject* mTarget;			// 카메라가 바라보는 GameObject 객체

		// 카메라 이동 제한값 설정
		static math::Vector2 mWidthLimit;	// width.x = Left, width.y = Right
		static math::Vector2 mHeightLimit;	// height.x = Up, height.y = Down
	};
}

