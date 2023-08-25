#include "vanYggdrasill.h"
#include "vanCollider.h"
#include "vanPlayerAttack.h"
#include "vanPlayer.h"
#include "vanObject.h"

#define INIT_POS_X		Window_X / 2
#define INIT_POS_Y		Window_Y / 2
#define FIST_SLAM_CNT	2
#define SWIPE_CNT		2

namespace van
{
	Yggdrasill::Yggdrasill()
	{
		// nothing
	}

	Yggdrasill::~Yggdrasill()
	{
		// nothing
	}

	void Yggdrasill::Init()
	{
		Boss::Init();		// �ƹ��͵� ����

		MakeAnimation();	// �ƹ��͵� ����

		//SetBossState(BossState::Gen);
		mState = BossState::Idle;
		SetBossDirection(BossDirection::Center);
		mInitPos = math::Vector2(INIT_POS_X, INIT_POS_Y);

		mBody = Object::Instantiate<YggdrasillBody>(enums::eLayerType::Yggdrasill_Body);
		mHead = Object::Instantiate<YggdrasillHead>(enums::eLayerType::Yggdrasill_Head);
		mChin = Object::Instantiate<YggdrasillChin>(enums::eLayerType::Yggdrasill_Chin);
		mHandLeft = Object::Instantiate<YggdrasillHand>(enums::eLayerType::Yggdrasill_Hand);
		mHandRight = Object::Instantiate<YggdrasillHand>(enums::eLayerType::Yggdrasill_Hand);

		mBody->SetOwner(this);
		mHead->SetOwner(this);
		mChin->SetOwner(this);
		mHandLeft->SetOwner(this);
		mHandRight->SetOwner(this);
		
		mHandLeft->SetHandPos(YggdrasillHand::HandPos::Left);
		mHandRight->SetHandPos(YggdrasillHand::HandPos::Right);

		mBody->SetPos(mInitPos);
	}

	void Yggdrasill::Update()
	{
		Boss::Update();	// �ش� ��ü�� ������ �ִ� Component �Ӽ����� ���� ������Ʈ���ش�.

		switch (mState)
		{
		case BossState::Gen:
			Gen();
			break;
		case BossState::Idle:
		{
			Idle();
			break;
		}
		case BossState::AttackReady:
			AttackReady();
			break;
		case BossState::Attack:
			Attack();
			break;
		case BossState::AttackEnd:
			AttackEnd();
			break;
		case BossState::Hit:
			Hit();
			break;
		case BossState::Dead:
			Dead();
			break;
		default:
			__noop;
		}
	}

	void Yggdrasill::Render(HDC _hdc)
	{
		Boss::Render(_hdc);	// GameObject�� ������ �ִ� Component Ŭ������ ����ϰ� �ִ� Ŭ�������� Render()ȣ��
	}

	void Yggdrasill::MakeAnimation()
	{
		// nothing
	}

	void Yggdrasill::OnCollisionEnter(Collider* _other)
	{
		// nothing
	}

	void Yggdrasill::OnCollisionStay(Collider* _other)
	{
		// nothing
	}

	void Yggdrasill::OnCollisionExit(Collider* _other)
	{
		// nothing
	}

	void Yggdrasill::Gen()
	{

	}

	void Yggdrasill::Idle()
	{
		mBody->SetState(YggdrasillBody::BodyState::Idle);
		mHead->SetState(YggdrasillHead::HeadState::Idle);
		mChin->SetState(YggdrasillChin::ChinState::Idle);
		mHandLeft->SetState(YggdrasillHand::HandState::Idle);
		mHandRight->SetState(YggdrasillHand::HandState::Idle);

		mTime += Time::GetDeltaTime();
		// Idle --(3��)--> Attack Ready
		if (mTime >= 3.0f)
		{
			mTime = 0.0f;
			mState = BossState::AttackReady;
		}
	}

	void Yggdrasill::AttackReady()
	{
		if (mbChooseSkill == false)
		{
			/*
				FistSlam,	// 0
				Swipe,		// 1
				MagicOrbs,	// 2
			*/
			srand((UINT)time(NULL));
			//mAttackCase = (BossSkill)(rand() % 3);
			mAttackCase = (BossSkill)(0);
			mbChooseSkill = true;
		}

		if (mbChooseSkill == true)
		{
			switch (mAttackCase)
			{
			case BossSkill::FistSlam:
				FistSlamReady();
				break;
			case BossSkill::Swipe:
				SwipeReady();
				break;
			case BossSkill::MagicOrbs:
				MagicOrbsReady();
				break;
			default:
				__noop;
			}
		}
	}

	void Yggdrasill::Attack()
	{
		switch (mAttackCase)
		{
		case BossSkill::FistSlam:
			FistSlamAttack();
			break;
		case BossSkill::Swipe:
			SwipeAttack();
			break;
		case BossSkill::MagicOrbs:
			MagicOrbsAttack();
			break;
		default:
			__noop;
		}
	}

	void Yggdrasill::AttackEnd()
	{
		switch (mAttackCase)
		{
		case BossSkill::FistSlam:
		{
			FistSlamEnd();
			break;
		}
		case BossSkill::Swipe:
			SwipeEnd();
			break;
		case BossSkill::MagicOrbs:
			MagicOrbsEnd();
			break;
		default:
			__noop;
		}
	}

	void Yggdrasill::Hit()
	{

	}

	void Yggdrasill::Dead()
	{

	}

	void Yggdrasill::FistSlamReady()
	{
		mHandLeft->SetState(YggdrasillHand::HandState::AttackReady);
		mHandRight->SetState(YggdrasillHand::HandState::AttackReady);

		if (mHandLeft->GetFinishFlag() == true
			&& mHandRight->GetFinishFlag() == true)
		{
			mHandLeft->SetFinishFlag(false);
			mHandRight->SetFinishFlag(false);

			mState = BossState::Attack;
		}
	}
	
	void Yggdrasill::SwipeReady()
	{
		mHandLeft->SetState(YggdrasillHand::HandState::AttackReady);
		mHandRight->SetState(YggdrasillHand::HandState::AttackReady);

		if (mHandLeft->GetFinishFlag() == true
			&& mHandRight->GetFinishFlag() == true)
		{
			mHandLeft->SetFinishFlag(false);
			mHandRight->SetFinishFlag(false);

			mState = BossState::Attack;
		}
	}

	void Yggdrasill::MagicOrbsReady()
	{

	}

	void Yggdrasill::FistSlamAttack()
	{
		// �� ������ ���°� ����
		mHandLeft->SetState(YggdrasillHand::HandState::Attack);
		mHandRight->SetState(YggdrasillHand::HandState::Attack);

		GameObject* target = GetTarget();
		math::Vector2 targetPos = target->GetComponent<Transform>()->GetPosition();
		math::Vector2 ownPos = GetComponent<Transform>()->GetPosition();

		// ������ Hand ���� ����
		if(mAttackDir == AttackHandDir::None)
		{
			// target �� own ���� ���ʿ� ���� �� (target.x < own.x)
			if (targetPos.x <= ownPos.x)
			{
				mAttackDir = AttackHandDir::Left;
			}
			// target �� own ���� �����ʿ� ���� �� (target.x > own.x)
			if (targetPos.x > ownPos.x)
			{
				mAttackDir = AttackHandDir::Right;
			}
		}

		// ������ Ƚ����ŭ ���ݼ���
		if (mFistSlamCnt < FIST_SLAM_CNT
			&& mAttackDir != AttackHandDir::None)
		{
			if (mAttackDir == AttackHandDir::Left)
			{
				mHandLeft->FistSlam();
				// ���� �Ϸ�� ����Ƚ�� ī��Ʈ
				if (mHandLeft->GetFistSlamFlag() == true)
				{
					mHandLeft->SetFistSlamFlag(false);
					mAttackDir = AttackHandDir::None;
					++mFistSlamCnt;
				}
			}
			if (mAttackDir == AttackHandDir::Right)
			{
				mHandRight->FistSlam();
				// ���� �Ϸ�� ����Ƚ�� ī��Ʈ
				if (mHandRight->GetFistSlamFlag() == true)
				{
					mHandRight->SetFistSlamFlag(false);
					mAttackDir = AttackHandDir::None;
					++mFistSlamCnt;
				}
			}
		}
		// ������ �Ϸ����� ��
		else
		{
			mHandLeft->FistSlamAfter();
			mHandRight->FistSlamAfter();

			if (mHandLeft->GetFinishFlag() == true
				&& mHandRight->GetFinishFlag() == true)
			{
				// �ʱ�ȭ
				mFistSlamCnt = 0;
				mHandLeft->SetFinishFlag(false);
				mHandRight->SetFinishFlag(false);
				// ���º��� (Attack --> Attack End)
				mState = BossState::AttackEnd;
			}
		}
	}

	void Yggdrasill::SwipeAttack()
	{
		GameObject* target = GetTarget();
		math::Vector2 targetPos = target->GetComponent<Transform>()->GetPosition();
		math::Vector2 ownPos = GetComponent<Transform>()->GetPosition();

		// �� ������ ���°� ����
		mHandLeft->SetState(YggdrasillHand::HandState::Attack);
		mHandRight->SetState(YggdrasillHand::HandState::Attack);

		// ������ Hand ���� ����
		if (mAttackDir == AttackHandDir::None)
		{
			// target �� own ���� ���ʿ� ���� �� (target.x < own.x)
			if (targetPos.x <= ownPos.x)
			{
				mAttackDir = AttackHandDir::Right;
			}
			// target �� own ���� �����ʿ� ���� �� (target.x > own.x)
			if (targetPos.x > ownPos.x)
			{
				mAttackDir = AttackHandDir::Left;
			}
		}

		// ������ Ƚ����ŭ ���ݼ���
		if (mSwipCnt < FIST_SLAM_CNT
			&& mAttackDir != AttackHandDir::None)
		{
			if (mAttackDir == AttackHandDir::Left)
			{
				mHandLeft->Swip();
				// ���� �Ϸ�� ����Ƚ�� ī��Ʈ
				if (mHandLeft->GetSwipeFlag() == true)
				{
					mHandLeft->SetSwipeFlag(false);
					mAttackDir = AttackHandDir::None;
					++mSwipCnt;
				}
			}
			if (mAttackDir == AttackHandDir::Right)
			{
				mHandRight->Swip();
				// ���� �Ϸ�� ����Ƚ�� ī��Ʈ
				if (mHandRight->GetSwipeFlag() == true)
				{
					mHandRight->SetSwipeFlag(false);
					mAttackDir = AttackHandDir::None;
					++mSwipCnt;
				}
			}
		}
		// ������ �Ϸ����� ��
		else
		{
			mSwipCnt = 0;
			// ���º��� (Attack --> Attack End)
			mState = BossState::AttackEnd;
		}

	}

	void Yggdrasill::MagicOrbsAttack()
	{

	}

	void Yggdrasill::FistSlamEnd()
	{
		mHandLeft->SetState(YggdrasillHand::HandState::AttackEnd);
		mHandRight->SetState(YggdrasillHand::HandState::AttackEnd);

		if (mHandLeft->GetFinishFlag() == true
			&& mHandRight->GetFinishFlag() == true)
		{
			// �ʱ�ȭ
			mHandLeft->SetFinishFlag(false);
			mHandRight->SetFinishFlag(false);
			mbChooseSkill = false;
			// ���º��� (Attack End --> Idle)
			mState = BossState::Idle;
		}
	}

	void Yggdrasill::SwipeEnd()
	{
		mHandLeft->SetState(YggdrasillHand::HandState::AttackEnd);
		mHandRight->SetState(YggdrasillHand::HandState::AttackEnd);

		if (mHandLeft->GetFinishFlag() == true
			&& mHandRight->GetFinishFlag() == true)
		{
			// �ʱ�ȭ
			mHandLeft->SetFinishFlag(false);
			mHandRight->SetFinishFlag(false);
			mbChooseSkill = false;

			// ���º��� (Attack End --> Idle)
			mState = BossState::Idle;
		}
	}

	void Yggdrasill::MagicOrbsEnd()
	{
	}
}