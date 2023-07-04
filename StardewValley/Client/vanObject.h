#pragma once
#include "CommonInclude.h"	// <windows.h>_HDC

#define DIRECTION_CNT  8

namespace van
{
	enum class Direction
	{
		Top,			// 0
		TopRight,		// 1
		Right,			// 2
		BottomRight,	// 3
		Bottom,			// 4
		BottomLeft,		// 5
		Left,			// 6
		TopLeft,		// 7
	};

	class Object
	{
	public:
		Object();
		Object(HWND hwnd, float _x1, float _y1, float _x2, float y2);
		void setDirNum();									// ���� ����
		void setDirNum2(int _directNum);					// ���� ����
		void setDelta(float _delta);						// set ��ŸŸ�� 
		void setSpeed(float _speed);						// set �ӵ�
		/*
			[ �ݻ� ����� �� ]
			top ----> bottom	0 -> 4

			topRight --x-->		1 -> 3 (+2)
			topRight --y-->		1 -> 7 (-2)

			right ---->			2 -> 6

			bottomRight --x-->	3 -> 1 (-2)
			bottomRight --y-->	3 -> 5 (+2)

			bottom ---->		4 -> 0

			bottomLeft --x-->	5 -> 7 (+2)
			bottomLeft --y-->	5 -> 3 (-2)

			left ---->			6 -> 2

			topleft --x-->		7 -> 5 (-2)
			topleft --y-->		7 -> 1 (+2)

			[ ��Ģ ]
			topRight, bottomLeft --(x)--> +2
			topRight, bottomLeft --(y)--> -2

			bottomRight, topLeft --(x)--> -2
			bottomRight, topLeft --(y)--> +2

			top,right,bottom,left ----> +4
		*/
		void ReflectX();									// x�࿡ �ε�������
		void ReflectY();									// y�࿡ �ε�������
		void MoveTop();										// ��
		void MoveTopRight();								// ��-��
		void MoveRight();									// ��
		void MoveBottomRight();								// ��-��
		void MoveBottom();									// ��
		void MoveBottomLeft();								// ��-��
		void MoveLeft();									// ��
		void MoveTopLeft();									// ��-��
		void Update();										// ��ü�� ��ǥ�� ������Ʈ
		void Render(HDC hdc);								// ��ü ȭ�鿡 �׷��ֱ�
		bool ConditionMaxX(); 								// FHD �ʺ� ����� ���� ����
		bool ConditionMaxY();								// FHD ���� ����� ���� ����
		__forceinline bool NoneReflection() { return isReflect == false; }	// �ݻ� ���°� �ƴϸ� true
		
	private:
		HWND mHwnd;		// ������ �ڵ�
		float x1;		// 1�� ��ǥ�� x��
		float y1;		// 1�� ��ǥ�� y��
		float x2;		// 2�� ��ǥ�� x��
		float y2;		// 2�� ��ǥ�� y��
		float maxX;		// x�� �ִ밪
		float maxY;		// y�� �ִ밪
		int directNum;	// �����̴� ����
		float speed;	// ��ü�� �����̴� �ӵ� (�ʴ� �����̴� �Ÿ�)
		float delta;	// ��Ÿ Ÿ��
		bool isReflect;	// �ݻ翩�� Ȯ��
	};
}
