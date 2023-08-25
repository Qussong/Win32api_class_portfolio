#pragma once
#include "vanSkill.h"

namespace van
{
	class RangeFire : public Skill
	{
	public:
		enum class RangeFireState
		{
			Gen,
			Active,
			Dead,
			None,
		};
	public:
		RangeFire();
		virtual ~RangeFire();

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render(HDC _hdc) override;
		virtual void MakeAnimation() override;

		virtual void OnCollisionEnter(class Collider* _other) override;	// �浹�� ������ ����
		virtual void OnCollisionStay(class Collider* _other) override;	// �浹����
		virtual void OnCollisionExit(class Collider* _other) override;	// �浹���� ���

		void SetState(RangeFireState _state) { mState = _state; }
		RangeFireState GetState() { return mState; }

		void Gen();
		void Active();
		void Dead();
		void SetRangeFirePos();

	private:
		RangeFireState mState = RangeFireState::None;
		bool mbPlayAnimation = true;
		float mDelay = 0.0f;
	};
}