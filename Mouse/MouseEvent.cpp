#include "MouseEvent.h"

MouseEvent::MouseEvent()
	: type{MouseEvent::EventType::Invalid}
	, x{ 0 }
	, y{ 0 }
{
}

MouseEvent::MouseEvent(const EventType type, const int x, const int y)
	: type{ type }
	, x{x}
	, y{y}
{

}

bool MouseEvent::IsValid()
{
	return type != EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const
{
	return this->type;
}

MousePoint MouseEvent::GetPos() const
{
	return { x,y };
}

int MouseEvent::GetPosX() const
{
	return x;
}

int MouseEvent::GetPosY() const
{
	return y;
}
