#include "Event.h"

ZiApi::Event::Event(const std::string &name, const EventFunction &function, const ZiApi::Event::EventPriority &priority, const ZiApi::Event::AnchorPoint &anchorPoint)
	: name(name), function(function), priority(priority), anchorPoint(anchorPoint)
{
}

ZiApi::Event::~Event()
{
	
}

ZiApi::Event::EventPriority		ZiApi::Event::getPriority()const
{
	return (priority);
}

ZiApi::Event::AnchorPoint		ZiApi::Event::getAnchorPoint()const
{
	return (anchorPoint);
}

ZiApi::EventFunction			ZiApi::Event::getFunction() const
{
	return (function);
}

bool							ZiApi::compareEvent(const Event &event1, const Event &event2)
{
	if (event1.getPriority() > event2.getPriority())
		return (true);
	return (false);
}
