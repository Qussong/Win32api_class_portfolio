#include "vanObject.h"

namespace van
{
	Object::Object()
		: mHwnd(NULL)
		, x1(0.0f)
		, y1(0.0f)
		, x2(0.0f)
		, y2(0.0f)
		, maxX(0.0f)
		, maxY(0.0f)
		, directNum(0)
		, delta(0.0f)
		, speed(0.0f)
		, isReflect(false)
	{
	}
	Object::Object(HWND hwnd, float _x1, float _y1, float _x2, float _y2)
		: mHwnd(hwnd)
		, x1(_x1)
		, y1(_y1)
		, x2(_x2)
		, y2(_y2)
		, directNum(0)
		, delta(0.0f)
		, speed(0.0f)
		, isReflect(false)
	{
		RECT clientArea;
		GetClientRect(mHwnd, &clientArea);
		maxX = clientArea.right;
		maxY = clientArea.bottom;
	}
	void Object::setDirNum()
	{
		directNum = random(DIRECTION_CNT);
	}
	void Object::setDirNum2(int _directNum)
	{
		directNum = _directNum;
	}
	void Object::setDelta(float _delta)
	{
		delta = _delta;
	}
	void Object::setSpeed(float _speed)
	{
		speed = _speed;
	}
	/*
		[ 규칙 ]
		topRight, bottomLeft --(x)--> +2
		topRight, bottomLeft --(y)--> -2

		bottomRight, topLeft --(x)--> -2
		bottomRight, topLeft --(y)--> +2

		top,right,bottom,left ----> +4
	*/
	void Object::ReflectX()
	{
		isReflect = true;
		if (directNum == (int)Direction::Top
			|| directNum == (int)Direction::Bottom)
			directNum = (directNum + 4) % DIRECTION_CNT;
		else if (directNum == (int)Direction::TopRight 
			|| directNum == (int)Direction::BottomLeft)
			directNum = (directNum + 2) % DIRECTION_CNT;
		else if (directNum == (int)Direction::BottomRight
			|| directNum == (int)Direction::TopLeft)
		{
			directNum = (directNum - 2) % DIRECTION_CNT;
			if (directNum <= 0)
				directNum += DIRECTION_CNT;
		}
		else
			__noop;
	}
	void Object::ReflectY()
	{
		isReflect = true;
		if (directNum == (int)Direction::Right
			|| directNum == (int)Direction::Left)
			directNum = (directNum + 4) % DIRECTION_CNT;
		else if (directNum == (int)Direction::TopRight
			|| directNum == (int)Direction::BottomLeft)
		{
			directNum = (directNum - 2) % DIRECTION_CNT;
			if (directNum <= 0)
				directNum += DIRECTION_CNT;
		}
		else if (directNum == (int)Direction::BottomRight 
			|| directNum == (int)Direction::TopLeft)
			directNum = (directNum + 2) % DIRECTION_CNT;
		else
			__noop;
	}
	void Object::MoveTop()
	{
		if (!ConditionMaxY() && NoneReflection())
			ReflectX();
		else
		{
			isReflect = false;
			y1 -= speed * delta;
			y2 -= speed * delta;
		}
	}
	void Object::MoveTopRight()
	{
		if (!ConditionMaxY() && NoneReflection())
			ReflectX();
		else if (!ConditionMaxX() && NoneReflection())
			ReflectY();
		else
		{
			isReflect = false;
			x1 += speed * delta;
			y1 -= speed * delta;
			x2 += speed * delta;
			y2 -= speed * delta;
		}
	}
	void Object::MoveRight()
	{
		if (!ConditionMaxX() && NoneReflection())
			ReflectY();
		else
		{
			isReflect = false;
			x1 += speed * delta;
			x2 += speed * delta;
		}
	}
	void Object::MoveBottomRight()
	{
		if (!ConditionMaxY() && NoneReflection())
			ReflectX();
		else if (!ConditionMaxX() && NoneReflection())
			ReflectY();
		else
		{
			isReflect = false;
			x1 += speed * delta;
			y1 += speed * delta;
			x2 += speed * delta;
			y2 += speed * delta;
		}
	}
	void Object::MoveBottom()
	{
		if (!ConditionMaxY() && NoneReflection())
			ReflectX();
		else
		{
			isReflect = false;
			y1 += speed * delta;
			y2 += speed * delta;
		}
	}
	void Object::MoveBottomLeft()
	{
		if (!ConditionMaxY() && NoneReflection())
			ReflectX();
		else if (!ConditionMaxX() && NoneReflection())
			ReflectY();
		else
		{
			isReflect = false;
			x1 -= speed * delta;
			y1 += speed * delta;
			x2 -= speed * delta;
			y2 += speed * delta;
		}
	}
	void Object::MoveLeft()
	{
		if (!ConditionMaxX() && NoneReflection())	// 범위안에 안에 있지 않고, 반사상태가 아니면
			ReflectY();
		else
		{
			isReflect = false;
			x1 -= speed * delta;
			x2 -= speed * delta;
		}
	}
	void Object::MoveTopLeft()
	{
		if (!ConditionMaxY() && NoneReflection())
			ReflectX();
		else if (!ConditionMaxX() && NoneReflection())
			ReflectY();
		else
		{
			isReflect = false;
			x1 -= speed * delta;
			y1 -= speed * delta;
			x2 -= speed * delta;
			y2 -= speed * delta;
		}
	}
	void Object::Update()
	{
		switch (directNum)
		{
		case (int)Direction::Top:
			MoveTop();
			break;
		case (int)Direction::Bottom:
			MoveBottom();
			break;
		case (int)Direction::Left:
			MoveLeft();
			break;
		case (int)Direction::Right:
			MoveRight();
			break;
		case (int)Direction::TopLeft:
			MoveTopLeft();
			break;
		case (int)Direction::TopRight:
			MoveTopRight();
			break;
		case (int)Direction::BottomLeft:
			MoveBottomLeft();
			break;
		case (int)Direction::BottomRight:
			MoveBottomRight();
			break;
		default:
			__noop;
		}
	}
	void Object::Render(HDC _hdc)
	{
		Ellipse(_hdc, x1, y1, x2, y2);
	}
	bool Object::ConditionMaxX()  // 클라이언트 영역_x 안에 있으면 true
	{
		return x1 >= 0 && x1 <= maxX && x2 >= 0 && x2 <= maxX;
	}
	bool Object::ConditionMaxY()  // 클라이언트 영역_y 안에 있으면 true
	{
		return y1 >= 0 && y1 <= maxY && y2 >= 0 && y2 <= maxY;
	}
}