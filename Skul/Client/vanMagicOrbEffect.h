#pragma once
#include "vanSkill.h"

namespace van
{
	class MagicOrbEffect : public Skill
	{
	public:
		enum class MagicOrbEffectState
		{
			Gen,
			Active,
			Dead,
			None,
		};

	public:
		MagicOrbEffect();
		virtual ~MagicOrbEffect();

		virtual void Init() override;
		virtual void Update() override;
		virtual void Render(HDC _hdc) override;
		virtual void MakeAnimation() override;

		virtual void OnCollisionEnter(class Collider* _other) override;	// �浹�� ������ ����
		virtual void OnCollisionStay(class Collider* _other) override;	// �浹����
		virtual void OnCollisionExit(class Collider* _other) override;	// �浹���� ���

		__forceinline void SetState(MagicOrbEffectState _state) { mState = _state; }
		__forceinline MagicOrbEffectState GetState() { return mState; }

		__forceinline void SetOffset(math::Vector2 _offset) { mOffset = _offset; }

		void Gen();
		void Active();
		void Dead();

	private:
		MagicOrbEffectState mState = MagicOrbEffectState::None;
		bool mbSetFlag = false;		// �ִϸ��̼� ����� ���� ���� �Ϸ� ����
		math::Vector2 mOffset = math::Vector2::Zero;
	};
}