#include "Handler.h"

ZiApi::Handler::Handler()
	: request(std::make_shared<HttpMessage>()), response(std::make_shared<HttpMessage>())
{
}

ZiApi::Handler::~Handler()
{
}

void											ZiApi::Handler::setRequest(const std::string &request)
{
	this->request->setMessage(request);
}

void											ZiApi::Handler::setResponse(const std::string &response)
{
	this->response->setMessage(response);
}

std::shared_ptr<ZiApi::HttpMessage>				&ZiApi::Handler::getRequest()
{
	return (request);
}

std::shared_ptr<ZiApi::HttpMessage>				&ZiApi::Handler::getResponse()
{
	return (response);
}

std::list<std::shared_ptr<ZiApi::IModule>>		ZiApi::Handler::getModules() const
{
	return (listModule);
}

void											ZiApi::Handler::registerModule(const std::shared_ptr<IModule> &module)
{
	listModule.push_back(module);
}

void											ZiApi::Handler::removeModule(const std::shared_ptr<IModule> &module)
{
	listModule.remove(module);
}

std::list<ZiApi::Event>							ZiApi::Handler::getAllModuleEvent(const std::list<std::shared_ptr<IModule>> &listModule, const ZiApi::Event::AnchorPoint &anchorPoint)
{
	std::list<ZiApi::Event>						allModuleEvent;

	for (auto &&it = listModule.begin(); it != listModule.end(); it++)
	{
		std::list<ZiApi::Event> tmp = (*it)->createListForAnchorPoint(anchorPoint);
		for (auto &&it = tmp.begin(); it != tmp.end(); it++)
		{
			allModuleEvent.push_back(*it);
		}
	}
	allModuleEvent.sort(ZiApi::compareEvent);
	return (allModuleEvent);
}

std::list<ZiApi::EventFunction>					ZiApi::Handler::getListFunction(const std::list<std::shared_ptr<IModule>> &listModule, const ZiApi::Event::AnchorPoint &anchorPoint)
{
	std::list<ZiApi::Event>						allModuleEvent = getAllModuleEvent(listModule, anchorPoint);
	std::list<ZiApi::EventFunction>				allModuleFunction;

	for (auto &&it = allModuleEvent.begin(); it != allModuleEvent.end(); it++)
	{
		allModuleFunction.push_front(it->getFunction());
	}
	return (allModuleFunction);
}

void											ZiApi::Handler::launchTypeModule(const Event::AnchorPoint &anchorPoint)
{
	std::list<ZiApi::EventFunction>				allModuleFunction = getListFunction(listModule, anchorPoint);

	for (auto &&it = allModuleFunction.begin(); it != allModuleFunction.end(); it++)
	{
		(*it)(this->request, this->response);
	}
}

void											ZiApi::Handler::resetRequestResponse()
{
	this->request.reset(new HttpMessage());

	this->response.reset(new HttpMessage());
}