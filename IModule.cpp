#include "IModule.h"

ZiApi::IModule::IModule(const std::string &name, const std::string &description, const std::string &version)
	: name(name), description(description), version(version)
{
}

ZiApi::IModule::~IModule()
{

}

std::string ZiApi::IModule::getName() const
{
	return (name);
}

std::string						ZiApi::IModule::getDescription() const
{
	return (description);
}

std::string						ZiApi::IModule::getVersion() const
{
	return (version);
}

void							ZiApi::IModule::addEvent(const Event &event)
{
	listEvent.push_back(event);
}

std::list<ZiApi::Event>			ZiApi::IModule::createListForAnchorPoint(const ZiApi::Event::AnchorPoint &anchorPoint) const
{
	std::list<Event>			list;
	
	for (auto &&it = listEvent.begin(); it != listEvent.end(); it++)
	{
		if (it->getAnchorPoint() == anchorPoint)
			list.push_back(*it);
	}
	return (list);
}
