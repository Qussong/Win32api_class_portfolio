#include "vanMage.h"
#include "vanRigidBody.h"
#include "vanAnimator.h"
#include "vanResourceManager.h"
#include "vanTexture.h"
#include "vanCollider.h"
#include "vanTransform.h"
#include "vanWall.h"
#include "vanFireBall.h"
#include "vanRangeFire.h"
#include "vanObject.h"
#include "vanPhoenixLanding.h"

#define MAX_HP					100.0f
#define FLY_POS					150.0f
#define WALK_SPEED				200.0f
#define FLY_SPEED				200.0f
#define LANDING_SPEED			200.0f
#define PHOENIX_READY_GAP_POS	100.0f
#define FLY_READY_GAP_POS		150.0f
#define LANDING_TIMER			10.0f

namespace van
{
	Mage::Mage()
	{
		AddComponent<RigidBody>();
	}

	Mage::~Mage()
	{
		// nothing
	}

	void Mage::Init()
	{
		Boss::Init();

		MakeAnimation();

		SetBossState(BossState::Gen);
		SetBossDirection(BossDirection::Left);
		SetHp(MAX_HP);
		SetHpPercent(100.0f);

		Collider* col = GetComponent<Collider>();
		col->SetSize(math::Vector2(33.0f * 2, 65.0f * 2));

		Animator* at = GetComponent<Animator>();
		at->SetScale(math::Vector2(2.0f, 2.0f));

	}

	void Mage::Update()
	{
		Boss::Update();

		SetHpPercent(GetHp() / MAX_HP * 100.0f);
		
		// Boss�� Attack Direction�� �޶������� Ȯ��
		ComparePosWithBossAndTarget();
		// LandingTimer ī��Ʈ
		LandingTimerCnt();
		
		mBossPastState = GetBossState();
		mBossPastDirection = GetBossDirection();

		

		switch (GetBossState())
		{
		case BossState::Gen:
			Gen();
			break;
		case BossState::Idle:
			Idle();
			break;
		case BossState::Walk:
			Walk();
			break;
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
		case BossState::TakeOff:
			TakeOff();
			break;
		default:
			__noop;
		}

		// Boss�� State(����)�� �޶����� ���ο� �ִϸ��̼� ���
		if (mBossPastState != GetBossState()
			|| mBossPastDirection != GetBossDirection())
		{
			mbPlayAnimation = true;
		}
	}

	void Mage::Render(HDC _hdc)
	{
		Boss::Render(_hdc);

	}

	void Mage::MakeAnimation()
	{
		Animator* at = GetComponent<Animator>();
		at->CreateAnimation(L"Intor_1", ResourceManager::Find<Texture>(L"Mage_Intor_1"), math::Vector2(0.0f, 0.0f), math::Vector2(33.0f, 81.0f), 40);
		at->CreateAnimation(L"Intor_2", ResourceManager::Find<Texture>(L"Mage_Intor_2"), math::Vector2(0.0f, 0.0f), math::Vector2(42.0f, 96.0f), 10, math::Vector2(0.0f, -8.0f));
		at->CreateAnimation(L"Idle_L", ResourceManager::Find<Texture>(L"Mage_Idle_L"), math::Vector2(0.0f, 0.0f), math::Vector2(41.0f, 71.0f), 6, math::Vector2(0.0f, 0.0f));
		at->CreateAnimation(L"Idle_R", ResourceManager::Find<Texture>(L"Mage_Idle_R"), math::Vector2(0.0f, 0.0f), math::Vector2(41.0f, 71.0f), 6, math::Vector2(0.0f, 0.0f));
		at->CreateAnimation(L"Walk_Front_L", ResourceManager::Find<Texture>(L"Mage_Walk_Front_L"), math::Vector2(0.0f, 0.0f), math::Vector2(57.0f, 55.0f), 6);
		at->CreateAnimation(L"Walk_Front_R", ResourceManager::Find<Texture>(L"Mage_Walk_Front_R"), math::Vector2(0.0f, 0.0f), math::Vector2(57.0f, 55.0f), 6);
		at->CreateAnimation(L"Walk_Back_L", ResourceManager::Find<Texture>(L"Mage_Walk_Back_L"), math::Vector2(0.0f, 0.0f), math::Vector2(60.0f, 60.0f), 6);
		at->CreateAnimation(L"Walk_Back_R", ResourceManager::Find<Texture>(L"Mage_Walk_Back_R"), math::Vector2(0.0f, 0.0f), math::Vector2(60.0f, 60.0f), 6);
		at->CreateAnimation(L"Attack_Ready_FireBall_L", ResourceManager::Find<Texture>(L"Mage_FireBall_Ready_L"), math::Vector2(0.0f, 0.0f), math::Vector2(55.0f, 96.0f), 13);
		at->CreateAnimation(L"Attack_Ready_FireBall_R", ResourceManager::Find<Texture>(L"Mage_FireBall_Ready_R"), math::Vector2(0.0f, 0.0f), math::Vector2(55.0f, 96.0f), 13);
		at->CreateAnimation(L"Attack_FireBall_L", ResourceManager::Find<Texture>(L"Mage_FireBall_L"), math::Vector2(0.0f, 0.0f), math::Vector2(52.0f, 69.0f), 8, math::Vector2(0.0f, 10.0f), 0.09F);
		at->CreateAnimation(L"Attack_FireBall_R", ResourceManager::Find<Texture>(L"Mage_FireBall_R"), math::Vector2(0.0f, 0.0f), math::Vector2(52.0f, 69.0f), 8, math::Vector2(0.0f, 10.0f), 0.09F);
		at->CreateAnimation(L"Attack_Ready_RangeFire_L", ResourceManager::Find<Texture>(L"Mage_RangeFire_Ready_L"), math::Vector2(0.0f, 0.0f), math::Vector2(61.0f, 109.0f), 15, math::Vector2(0.0f, 15.0f));
		at->CreateAnimation(L"Attack_Ready_RangeFire_R", ResourceManager::Find<Texture>(L"Mage_RangeFire_Ready_R"), math::Vector2(0.0f, 0.0f), math::Vector2(61.0f, 109.0f), 15, math::Vector2(0.0f, 15.0f));
		at->CreateAnimation(L"Attack_RangeFire_L", ResourceManager::Find<Texture>(L"Mage_RangeFire_L"), math::Vector2(0.0f, 0.0f), math::Vector2(62.0f, 109.0f), 3, math::Vector2(0.0f, 8.0f));
		at->CreateAnimation(L"Attack_RangeFire_R", ResourceManager::Find<Texture>(L"Mage_RangeFire_R"), math::Vector2(0.0f, 0.0f), math::Vector2(62.0f, 109.0f), 3, math::Vector2(0.0f, 8.0f));
		at->CreateAnimation(L"Attack_Ready_PhoenixLanding_L", ResourceManager::Find<Texture>(L"Mage_PhoenixRanding_Ready_L"), math::Vector2(0.0f, 0.0f), math::Vector2(58.0f, 58.0f), 3);
		at->CreateAnimation(L"Attack_Ready_PhoenixLanding_R", ResourceManager::Find<Texture>(L"Mage_PhoenixRanding_Ready_R"), math::Vector2(0.0f, 0.0f), math::Vector2(58.0f, 58.0f), 3);
		at->CreateAnimation(L"Attack_PhoenixLanding_L", ResourceManager::Find<Texture>(L"Mage_PhoenixRanding_Land_L"), math::Vector2(0.0f, 0.0f), math::Vector2(47.0f, 54.0f), 9, math::Vector2(0.0f, 7.0f));
		at->CreateAnimation(L"Attack_PhoenixLanding_R", ResourceManager::Find<Texture>(L"Mage_PhoenixRanding_Land_R"), math::Vector2(0.0f, 0.0f), math::Vector2(47.0f, 54.0f), 9, math::Vector2(0.0f, 7.0f));
		
	}

	void Mage::OnCollisionEnter(Collider* _other)
	{
		// ���� �浹�ϸ� Walk ������ �ٲ��ش�.
		GameObject* obj = _other->GetOwner();
		Wall* wall = dynamic_cast<Wall*>(obj);

		if (wall != nullptr)
		{
			if (GetBossDirection() == BossDirection::Left)
			{
				SetBossDirection(BossDirection::Right);
			}
			else if (GetBossDirection() == BossDirection::Right)
			{
				SetBossDirection(BossDirection::Left);
			}

			// ���ο� �ִϸ��̼� ��� ex) Walk
			mbPlayAnimation = true;
		}
	}

	void Mage::OnCollisionStay(Collider* _other)
	{
		// nothing
	}

	void Mage::OnCollisionExit(Collider* _other)
	{
		// nothing
	}

	void Mage::Gen()
	{
		Animator* at = GetComponent<Animator>();
		Transform* tr = GetComponent<Transform>();
		RigidBody* rb = GetComponent<RigidBody>();

		// �������� �ö� �ڿ� �߷��� ������ �� �޵��� ���ش�.
		if (mbIntroFlag == true
			&& rb->GetGround() == false)
		{
			rb->SetGround(true);
		}

		// Mage �� ���� �ٴ¼��� �ִϸ��̼� ��� ����
		// ù��° Intro �ִϸ��̼�
		if (rb->GetGround() == true
			&& mbPlayAnimation == true
			&& mbIntroFlag == false)
		{
			at->PlayAnimation(L"Intor_1", false);
			mbPlayAnimation = false;
			mbIntroFlag = true;
		}

		// �ι�° Intro �ִϸ��̼�
		if (mbPlayAnimation == true
			&& mbIntroFlag == true
			&& mbIntroEndFlag == false)
		{
			// �������� �÷��ش�.
			tr->SetPosition(tr->GetPosition() - math::Vector2(0.0f, FLY_POS));

			at->PlayAnimation(L"Intor_2", false);
			mbPlayAnimation = false;
			mbIntroEndFlag = true;
		}

		// Intro ����
		if (at->IsActiveAnimationComplete() == true
			&& mbIntroEndFlag == true)
		{
			//mInitPos_Y = tr->GetPosition().y;	// ���� ��ġ ����
			SetBossState(BossState::Idle);
		}

		// ���� �ִϸ��̼� ��� �Ϸ�� ���� ���� ���
		if (at->IsActiveAnimationComplete() == true)
		{
			mbPlayAnimation = true;
		}
	}

	void Mage::Idle()
	{
		Animator* at = GetComponent<Animator>();
		
		if (mbAnimationReDirectionFlag == true)
		{
			SetBossDirection(mBossAttackDirection);
			mbAnimationReDirectionFlag = false;
		}

		if (mbPlayAnimation == true)
		{
			if (GetBossDirection() == BossDirection::Left)
			{
				at->PlayAnimation(L"Idle_L", true);
			}

			if (GetBossDirection() == BossDirection::Right)
			{
				at->PlayAnimation(L"Idle_R", true);
			}

			mbPlayAnimation = false;
		}

		AddTimer(Time::GetDeltaTime());

		// Idle ���°� 1.5�� �����Ǹ�
		if (GetTimer() >= 1.5f)
		{
			// ���� �ʱ�ȭ
			SetTimer(0.0f);
			mbAnimationReDirectionFlag = true;

			// ���� �ൿ ����
			/*
				Walk,			// 0
				Attack_Ready,	// 1
				TakeOff			// 2
			*/
			int nextCase = 0;
			if (mbTakeOff == true)
			{
				nextCase = 2;
				mbTakeOff = false;	// �÷��װ� ���ش�.
			}
			else
			{
				srand((UINT)time(NULL));
				nextCase = (rand() % 2);
			}
			switch (nextCase)
			{
			case 0:
				SetBossState(BossState::Walk);
				break;
			case 1:
				SetBossState(BossState::AttackReady);
				break;
			case 2:
				SetBossState(BossState::TakeOff);
				break;
			default:
				__noop;
			}
		}
	}

	void Mage::Walk()
	{
		Animator* at = GetComponent<Animator>();
		Transform* tr = GetComponent<Transform>();
		math::Vector2 pos = tr->GetPosition();
		// �¿� �̵�
		if (GetBossDirection() == BossDirection::Left)
		{
			pos.x -= WALK_SPEED * Time::GetDeltaTime();
			tr->SetPosition(pos);

			if (mbPlayAnimation == true)
			{
				if (mBossAttackDirection == BossDirection::Left)
				{
					at->PlayAnimation(L"Walk_Front_L", true);
				}
				if (mBossAttackDirection == BossDirection::Right)
				{
					at->PlayAnimation(L"Walk_Back_L", true);
				}

				mbPlayAnimation = false;
			}
		}

		if (GetBossDirection() == BossDirection::Right)
		{
			pos.x += WALK_SPEED * Time::GetDeltaTime();
			tr->SetPosition(pos);

			if (mbPlayAnimation == true)
			{
				if (mBossAttackDirection == BossDirection::Left)
				{
					at->PlayAnimation(L"Walk_Back_R", true);
				}
				if (mBossAttackDirection == BossDirection::Right)
				{
					at->PlayAnimation(L"Walk_Front_R", true);
				}

				mbPlayAnimation = false;
			}
		}
		
		// Walk ���� 3�� ������ Idle ���·� ��ȯ
		AddTimer(Time::GetDeltaTime());
		if (GetTimer() >= 3.0f)
		{
			SetTimer(0.0f);
			SetBossState(BossState::Idle);
		}
	}

	void Mage::AttackReady()
	{
		if (mbChooseSkill == false)
		{
			/*
				RangeFire,		// 0
				FireBall,		// 1
				RangeFire,		// 2
				PhoenixLanding,	// 3
				FinishMove		// 4
			*/
			srand((UINT)time(NULL));
			mAttackCase = (BossSkill)(rand() % 4);
			mbChooseSkill = true;
		}

		if (mbChooseSkill == true)
		{
			// �ʻ�� ���� �ٸ� ��ų�� ���õǾ��� ��
			switch (mAttackCase)
			{
			case BossSkill::FireBall1:
			case BossSkill::FireBall2:
				AttackFireBallReady();
				break;
			case BossSkill::RangeFire:
				AttackRangeFireReady();
				break;
			case BossSkill::PhoenixLanding:
				AttackPhoenixLandingReady();
				break;
			default:
				__noop;
			}
		}
	}

	void Mage::Attack()
	{
		switch (mAttackCase)
		{
		case BossSkill::FireBall1:
		case BossSkill::FireBall2:
			AttackFireBall();
			break;
		case BossSkill::RangeFire:
			AttackRangeFire();
			break;
		case BossSkill::PhoenixLanding:
			AttackPhoenixLanding();
			break;
		//case BossSkill::FinishMove:
		//	AttackFinishMove();
		//	break;
		default:
			__noop;
		}
	}

	void Mage::AttackEnd()
	{
		mbChooseSkill = false;				// ���ݽ�ų ���ÿ��� �ʱ�ȭ(false = �����ؾ���)
		mbAnimationReDirectionFlag = true;	// �÷��̾� ���⿡ �°� �ٽ� �ִϸ��̼� ���� ����
		SetBossState(BossState::Idle);		// Idle ���·� ��ȯ
	}

	void Mage::Hit()
	{

	}

	void Mage::Dead()
	{

	}

	void Mage::AttackFireBallReady()
	{
		Animator* at = GetComponent<Animator>();
		
		if (mbPlayAnimation == true)
		{
			if (GetBossDirection() == BossDirection::Left)
			{
				at->PlayAnimation(L"Attack_Ready_FireBall_L");
			}

			if (GetBossDirection() == BossDirection::Right)
			{
				at->PlayAnimation(L"Attack_Ready_FireBall_R");
			}

			mbPlayAnimation = false;
		}

		if (at->IsActiveAnimationComplete() == true)
		{
			SetBossState(BossState::Attack);
		}
	}

	void Mage::AttackRangeFireReady()
	{
		Animator* at = GetComponent<Animator>();

		if (mbPlayAnimation == true)
		{
			if (GetBossDirection() == BossDirection::Left)
			{
				at->PlayAnimation(L"Attack_Ready_RangeFire_L");
			}

			if (GetBossDirection() == BossDirection::Right)
			{
				at->PlayAnimation(L"Attack_Ready_RangeFire_R");
			}

			mbPlayAnimation = false;
		}

		if (at->IsActiveAnimationComplete() == true)
		{
			if (mbPlayAnimation == true)
			{
				if (mBossAttackDirection == BossDirection::Left)
				{
					at->PlayAnimation(L"Attack_FireBall_L", true);
				}

				if (mBossAttackDirection == BossDirection::Right)
				{
					at->PlayAnimation(L"Attack_FireBall_R", true);
				}

				mbPlayAnimation = false;
			}
			if (at->IsActiveAnimationComplete() == true)
			{
				SetBossState(BossState::Attack);
			}
		}
	}

	void Mage::AttackPhoenixLandingReady()
	{
		if (mbSky == false)
		{
			SetBossState(BossState::AttackEnd);
			return;
		}

		Transform* tr = GetComponent<Transform>();
		RigidBody* rb = GetComponent<RigidBody>();
		Animator* at = GetComponent<Animator>();
		math::Vector2 pos = tr->GetPosition();

		if (mbRecordPosY == false)
		{
			mInitPos = pos;
			mbRecordPosY = true;
		}

		if (mbFly == true)
		{
			// ���� �̵�( -pos.y)
			pos.y -= 100.0f * Time::GetDeltaTime();
			tr->SetPosition(pos);
			float gap = mInitPos.y - pos.y;
			if (fabs(gap) >= 200.0f)	// Ư����ġ�� ���޽�
			{
				mbFly = false;			// �� �̻� �� �ö󰡵� �ȴ�.
			}
		}

		// �ö��� ���� && �������� ���� == ���ϼ��� ��
		if(mbFly == false
			&& mbLand == false)
		{
			// Ready �ִϸ��̼� ����
			if (mbPlayAnimation == true)
			{
				if (GetBossDirection() == BossDirection::Left)
				{
					at->PlayAnimation(L"Attack_Ready_PhoenixLanding_L");
				}
				if (GetBossDirection() == BossDirection::Right)
				{
					at->PlayAnimation(L"Attack_Ready_PhoenixLanding_R");
				}

				mbPlayAnimation = false;
			}

			// �ִϸ��̼� �Ϸ�� ���ϼ���
			if (at->IsActiveAnimationComplete() == true)
			{
				mbLand = true;				// ���Ͻ���
				rb->SetGround(false);		// ����Ȯ���� ���� �߷��� ����޵��� �Ѵ�.
				rb->SetVelocity(math::Vector2(0.0f, 300.0f));
			}
		}
		
		// ���Ͻ�
		if (mbLand == true)
		{
			// ���� �����ϸ�
			if (rb->GetGround() == true)
			{
				mbSky = false;	// ���� ������ �˷��ش�.
				// Attack���� ��ȯ
				SetBossState(BossState::Attack);
			}
		}
	}

	void Mage::AttackFireBall()
	{
		Animator* at = GetComponent<Animator>();
		Transform* tr = GetComponent<Transform>();

		if (mbPlayAnimation == true)
		{
			if (mBossAttackDirection == BossDirection::Left)
			{
				at->PlayAnimation(L"Attack_FireBall_L", true);
			}

			if (mBossAttackDirection == BossDirection::Right)
			{
				at->PlayAnimation(L"Attack_FireBall_R", true);
			}

			mbPlayAnimation = false;
		}

		// FireBall ������
		if (mbShoot == false)
		{
			AddTimer(Time::GetDeltaTime());

			if (GetTimer() >= 0.3f)
			{
				SetTimer(0.0f);
				mbShoot = true;
			}
		}

		// FireBall ����
		if (mFireBallCnt < 6
			&& mbShoot == true)
		{
			FireBall* fireBall = Object::Instantiate<FireBall>(enums::eLayerType::Boss_Mage_Skill_FireBall);
			fireBall->SetOwner(this);
			fireBall->GetComponent<Transform>()->SetPosition(tr->GetPosition());

			mListFireBall.push_back(fireBall);

			++mFireBallCnt;
			mbShoot = false;
		}

		// FireBall ��������
		if (mFireBallCnt == 6)
		{
			mFireBallCnt = 0;
			mbShoot = true;
			mListFireBall.clear();
			SetBossState(BossState::AttackEnd);
		}
	}

	void Mage::AttackRangeFire()
	{
		Animator* at = GetComponent<Animator>();
		Transform* tr = GetComponent<Transform>();

		if (mbPlayAnimation == true)
		{
			if (GetBossDirection() == BossDirection::Left)
			{
				at->PlayAnimation(L"Attack_RangeFire_L", true);
			}

			if (GetBossDirection() == BossDirection::Right)
			{
				at->PlayAnimation(L"Attack_RangeFire_R", true);
			}

			mbPlayAnimation = false;
		}

		// RangeFire ������
		if (mbShoot == false)
		{
			AddTimer(Time::GetDeltaTime());

			if (mbShoot == false
				&& GetTimer() >= 0.7f)
			{
				SetTimer(0.0f);
				mbShoot = true;
			}
		}

		// RangeFire ����
		if (mRangeFire < 3
			&& mbShoot == true)
		{
			RangeFire* rangeFire = Object::Instantiate<RangeFire>(enums::eLayerType::Boss_Mage_Skill_RangeFire);
			rangeFire->SetOwner(this);
			rangeFire->GetComponent<Transform>()->SetPosition(tr->GetPosition());

			mListRangeFire.push_back(rangeFire);

			++mRangeFire;
			mbShoot = false;
		}

		// RangeFire ��������
		if (mRangeFire == 3)
		{
			mRangeFire = 0;
			mbShoot = true;
			mListRangeFire.clear();
			SetBossState(BossState::AttackEnd);
		}
	}

	void Mage::AttackPhoenixLanding()
	{
		Animator* at = GetComponent<Animator>();
		
		if (mbPlayAnimation == true
			&& PhoenixLandingAnimation == true)
		{
			if (GetBossDirection() == BossDirection::Left)
			{
				at->PlayAnimation(L"Attack_PhoenixLanding_L");
			}
			if (GetBossDirection() == BossDirection::Right)
			{
				at->PlayAnimation(L"Attack_PhoenixLanding_R");
			}

			mbPlayAnimation = false;
			PhoenixLandingAnimation = false;
		}

		if (at->IsActiveAnimationComplete())
		{
			mbRecordPosY = false;
			mbFly = true;
			mbLand = false;
			mbLandingTimer = true;	// Landing �ð� ī��Ʈ����
			// mbSky, mInitPosY �� �ٽ� �ϴ÷� ������ �� �� �ʱ�ȭ�Ѵ�.
			SetBossState(BossState::AttackEnd);
		}
	}

	void Mage::ComparePosWithBossAndTarget()
	{
		mBossPastAttackDirection = mBossAttackDirection;

		GameObject* target = GetTarget();

		if (target != nullptr)
		{
			Transform* tr_target = target->GetComponent<Transform>();
			math::Vector2 pos_target = tr_target->GetPosition();
			math::Vector2 pos_boss = GetComponent<Transform>()->GetPosition();
			float gapX = pos_target.x - pos_boss.x;	// + : right, - : left

			if (gapX > 0.0f)
			{
				mBossAttackDirection = BossDirection::Right;
			}
			if (gapX < 0.0f)
			{
				mBossAttackDirection = BossDirection::Left;
			}
		}

		// Boss�� Attack Direction�� �޶����� ���ο� �ִϸ��̼� ��� ex) Walk, Attack
		if (GetBossState() == BossState::Walk
			|| GetBossState() == BossState::Attack)
		{
			if (mBossPastAttackDirection != mBossAttackDirection)
			{
				mbPlayAnimation = true;
			}
		}

	}

	void Mage::LandingTimerCnt()
	{
		if (mbLandingTimer == true)
		{
			mLandingTimer += Time::GetDeltaTime();
			if (mLandingTimer >= LANDING_TIMER)
			{
				mLandingTimer = 0.0f;		// Ÿ�̸� �ð� �ʱ�ȭ
				mbLandingTimer = false;		// Ÿ�̸� �������� �ʱ�ȭ (�ʱⰪ : ��������)
				mbTakeOff = true;			// �� -> �ϴ� ��ġ �ʱ�ȭ ON
			}
		}
	}

	void Mage::TakeOff()
	{
		RigidBody* rb = GetComponent<RigidBody>();
		Transform* tr = GetComponent<Transform>();
		math::Vector2 dest = mInitPos;			// ������(y��ǥ)
		math::Vector2 pos = tr->GetPosition();	// ������ġ

		if (rb->GetGround() == false)
		{
			rb->SetGround(true);
		}

		pos.y -= 100.0f * Time::GetDeltaTime();
		tr->SetPosition(pos);

		// ���� ��ġ�� ���������� ���ų� �������� ���
		if (pos.y <= dest.y)
		{
			mbSky = true;					// �ϴÿ� �� �ִ� ���·� ���� (PhoenixLanding ��� ����)
			mInitPos = math::Vector2::Zero;	// PhoenixLanding ���� �ʱ���ġ�� �ʱ�ȭ
			SetBossState(BossState::Idle);
		}
	}

}