#include "CGIModule.h"
#include <boost/process.hpp>

CGI::CGIModule::CGIModule()
{
	createInitCGI();
}

CGI::CGIModule::~CGIModule()
{
}

void							CGI::CGIModule::createInitCGI()
{
	std::string					name("initCGI");
	ZiApi::Event::EventPriority t = ZiApi::Event::EventPriority::LOW;
	ZiApi::Event::AnchorPoint	point = ZiApi::Event::AnchorPoint::AFTER_REQUEST;

	ZiApi::EventFunction function = std::bind(&CGI::CGIModule::initCGI, this, std::placeholders::_1, std::placeholders::_2);

	ZiApi::Event event(name, function, t, point);

	this->addEvent(event);
}

void							CGI::CGIModule::createExecCGI()
{
	std::string					name("execCGI");
	ZiApi::Event::EventPriority t = ZiApi::Event::EventPriority::LOW;
	ZiApi::Event::AnchorPoint	point = ZiApi::Event::AnchorPoint::MODULE;

	ZiApi::EventFunction		function = std::bind(&CGI::CGIModule::execCGI, this, std::placeholders::_1, std::placeholders::_2);

	ZiApi::Event event(name, function, t, point);

	this->addEvent(event);
}


void							CGI::CGIModule::initCGI(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response)
{
	initEnvValue(request);
	if (init == false) {
		init = true;
		createExecCGI();
	}
}

void							CGI::CGIModule::execCGI(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response)
{
	boost::process::environment ziaEnv = boost::this_process::environment();
	boost::process::environment childEnv = ziaEnv;
	std::string					requestResponse;

	initChildEnv(childEnv);
	if (isValidCGIRequest() == true)
	{
		ZiApi::HttpMessage message;

		requestResponse = startChild(childEnv);
		message.setMessage(requestResponse);
		setResponse(response, requestResponse, message);
	}
	envValue.clear();
}


std::string						CGI::CGIModule::setResponseHeader(const ZiApi::HttpMessage &message, const std::string error)
{
	auto						actualDate = std::chrono::system_clock::now();
	std::time_t					time = std::chrono::system_clock::to_time_t(actualDate);
	std::string					date = std::string("Date: ") + std::string(std::ctime(&time));
	std::string					serverSoftware = "Server: " + envValue["SERVER_SOFTWARE"] + "\r\n";
	std::string					contentLength = "Content-Length: " + std::to_string(message.getBody()->getStringField().size()) + "\r\n";
	std::string					contentType = "";
	std::string					connection = "Connection: keep-alive\r\n";
	std::string					response = envValue["SERVER_PROTOCOL"];
	std::string					status = message.getHeader()->getValueFromMapField("Status");
	std::string					allow = "Allow: GET, POST, HEAD\r\n";
	std::string					location = "";

	if (status.compare("") == 0 && error.compare("") == 0)
	{
		response += std::string(" ") + "200 OK\r\n";
		status = "Status: 200 OK\r\n";
	}
	else if (status.compare("") != 0)
	{
		response += std::string(" ") + message.getHeader()->getValueFromMapField("Status") + "\r\n";
		status = "Status: " + message.getHeader()->getValueFromMapField("Status") + "\r\n";
	}
	else {
		response += std::string(" ") + error + "\r\n";
		status = "Status: " + error + "\r\n";
	}
	if (message.getHeader()->getValueFromMapField("Location").compare("") != 0)
		location = message.getHeader()->getValueFromMapField("Location") + "\r\n";
	if (error.compare("500 Internal Server Error") == 0)
		connection = "Connection: close\r\n";
	return (response + date + serverSoftware + contentLength + contentType + connection + allow + location + status + "\r\n");
}

void						CGI::CGIModule::setResponse(std::shared_ptr<ZiApi::HttpMessage> &response, std::string &requestResponse, const ZiApi::HttpMessage &message)
{

	response->setHeader(setResponseHeader(message, requestResponse));
	response->setBody(message.getBody()->getStringField());
	if (envValue["REQUEST_METHOD"].compare("HEAD") == 0)
		response->setBody("");
}
