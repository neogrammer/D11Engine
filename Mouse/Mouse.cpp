#include "Mouse.h"

void Mouse::OnLeftPressed(int x, int y)
{
	leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	eventBuffer.push(me);
}

void Mouse::OnRightPressed(int x, int y)
{
	rightIsDown = true;
	eventBuffer.push({ MouseEvent::EventType::RPress, x, y });
}

void Mouse::OnRightReleased(int x, int y)
{
	rightIsDown = false;
	eventBuffer.push({ MouseEvent::EventType::RRelease, x, y });
}

void Mouse::OnLeftReleased(int x, int y)
{
	leftIsDown = false;
	eventBuffer.push({ MouseEvent::EventType::LRelease, x, y });
}

void Mouse::OnMiddlePressed(int x, int y)
{
	mbuttonDown = true;
	eventBuffer.push({ MouseEvent::EventType::MPress, x, y });
}

void Mouse::OnMiddleReleased(int x, int y)
{
	mbuttonDown = false;
	eventBuffer.push({ MouseEvent::EventType::MPress, x, y });
}
 
void Mouse::OnWheelUp(int x, int y)
{

	eventBuffer.push({ MouseEvent::EventType::WheelUp, x, y });
}

void Mouse::OnWheelDown(int x, int y)
{
	eventBuffer.push({ MouseEvent::EventType::WheelDown, x, y });
}

void Mouse::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	eventBuffer.push({ MouseEvent::EventType::Move, x, y });
}

void Mouse::OnMouseMoveRaw(int x, int y)
{
	this->x = x;
	this->y = y;
	eventBuffer.push({ MouseEvent::EventType::RAW_MOVE, x, y });
}

bool Mouse::IsLeftDown()
{
	return leftIsDown;
}

bool Mouse::IsRightDown()
{
	return rightIsDown;
}

bool Mouse::IsMiddleDown()
{
	return mbuttonDown;
}

int Mouse::GetPosX()
{
	return x;
}

int Mouse::GetPosY()
{
	return y;
}

MousePoint Mouse::GetPos()
{
	
		return { x,y };
}

bool Mouse::EventBufferIsEmpty()
{
	if (eventBuffer.empty())
		return true;
	else
		return false;
}

MouseEvent Mouse::ReadEvent()
{
	if (eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = this->eventBuffer.front();
		eventBuffer.pop();
		return e;
	}
}
