#include "HTTPModule.h"

HTTPModule::HTTPModule()
{
	createInitHTTP();
}

HTTPModule::~HTTPModule()
{
}

void							HTTPModule::createInitHTTP()
{
	std::string					name("initHTTP");
	ZiApi::Event::EventPriority t = ZiApi::Event::EventPriority::LOW;
	ZiApi::Event::AnchorPoint	point = ZiApi::Event::AnchorPoint::REQUEST;

	ZiApi::EventFunction function = std::bind(&HTTPModule::initHTTP, this, std::placeholders::_1, std::placeholders::_2);

	ZiApi::Event event(name, function, t, point);

	this->addEvent(event);
}

void							HTTPModule::createExecHTTP()
{
	std::string					name("execHTTP");
	ZiApi::Event::EventPriority t = ZiApi::Event::EventPriority::HIGH;
	ZiApi::Event::AnchorPoint	point = ZiApi::Event::AnchorPoint::MODULE;

	ZiApi::EventFunction function = std::bind(&HTTPModule::execHTTP, this, std::placeholders::_1, std::placeholders::_2);

	ZiApi::Event event(name, function, t, point);

	this->addEvent(event);
}

void							HTTPModule::execHTTP(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response)
{
	std::string					method = request->getHeader()->getValueFromMapField("REQUEST_METHOD");
	ZiApi::HttpMessage			message;
	std::string					error;

	if ((error = isValidHTTPHeader(request)).compare("") != 0)
		methodERROR(request, response, error);
	else if (method.compare("GET") == 0 || method.compare("HEAD") == 0)
		methodGET(request, response);
	else if (method.compare("POST") == 0 || method.compare("PUT") == 0)
		methodPOST(request, response);
	else if (method.compare("TRACE") == 0)
		methodTRACE(request, response);
	else if (method.compare("DELETE") == 0)
		methodDELETE(request, response);
	else if (method.compare("OPTIONS") == 0)
		methodOPTION(request, response);
}

std::string						HTTPModule::isValidHTTPHeader(std::shared_ptr<ZiApi::HttpMessage> &request) const
{
	std::string					httpVersion = request->getHeader()->getValueFromMapField("REQUEST_HTTP_VERSION");

	if (request->getHeader()->getValueFromMapField("Host").compare("") == 0)
		return ("400 Bad Request");

	if (httpVersion.compare("HTTP/1.1") == 0 && httpVersion.compare("HTTP/1.0") == 0
		&& httpVersion.compare("HTTP/0.9") == 0 && httpVersion.compare("HTTP/2.0") == 0)
		return ("400 Bad Request");

	return ("");
}

void							HTTPModule::setResponse(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response, const ZiApi::HttpMessage &message, const std::string &error) const
{
	if (error.compare("") == 0) {
		response->setHeader(setHeaderResponse(request, message, ""));
		response->setBody(message.getBody()->getStringField());
	}
	else {
		response->setHeader(setHeaderResponse(request, message, error));
		response->setBody("");
	}
}

std::string						HTTPModule::setHeaderResponse(std::shared_ptr<ZiApi::HttpMessage> &request, const ZiApi::HttpMessage &message, const std::string &error) const
{
	std::string					method = request->getHeader()->getValueFromMapField("REQUEST_METHOD");

	if (method.compare("GET") == 0 || method.compare("HEAD") == 0)
		return setGetHeaderResponse(message, error);
	else if (method.compare("POST") == 0 || method.compare("PUT") == 0)
		return setPostHeaderResponse(message, error);
	else if (method.compare("TRACE") == 0)
		return setTraceHeaderResponse(message);
	else if (method.compare("DELETE") == 0)
		return setDeleteHeaderResponse(error);
	else if (method.compare("OPTIONS") == 0)
		return setOptionHeaderResponse();
	return ("");
}

void							HTTPModule::initHTTP(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response)
{
	if (!isForHttpModule(request))
		return;
	if (!init)
	{
		init = true;
		createExecHTTP();
	}
}

void							HTTPModule::setFileContent(std::shared_ptr<ZiApi::HttpMessage> &request, ZiApi::HttpMessage &message, const std::string &fileName) const
{
	std::istringstream			content;
	boost::filesystem::path		workingDir = request->getHeader()->getValueFromMapField("Host");
	boost::filesystem::path		actualPath = boost::filesystem::current_path();

	if (!boost::filesystem::exists(workingDir))
		boost::filesystem::create_directory(workingDir);

	if (!boost::filesystem::exists(workingDir / fileName)) {
		message.setMessage("Location: " + (workingDir / fileName).string());
	}

	std::ofstream				file((workingDir / fileName).string(), std::ofstream::out | std::ofstream::trunc);

	file << request->getBody()->getStringField();
}

std::string						HTTPModule::getFileContent(std::shared_ptr<ZiApi::HttpMessage> &request, const std::string &fileName) const
{
	std::ostringstream			result;
	boost::filesystem::path		workingDir = request->getHeader()->getValueFromMapField("Host");
	boost::filesystem::path		actualPath = boost::filesystem::current_path();
	boost::filesystem::ifstream	file(actualPath / workingDir / fileName);

	result << file.rdbuf();

	return (result.str());
}

void							HTTPModule::deleteFile(std::shared_ptr<ZiApi::HttpMessage> &request, const boost::filesystem::path &fileName) const
{
	boost::filesystem::path		workingDir = request->getHeader()->getValueFromMapField("Host");

	boost::filesystem::remove(workingDir / fileName);
}

bool								HTTPModule::isForHttpModule(std::shared_ptr<ZiApi::HttpMessage> &request) const
{
	if (request->getHeader()->getValueFromMapField("REQUEST_RESSOURCE").find("/cgi-bin/") != std::string::npos)
		return (false);
	return (true);
}

bool								HTTPModule::isValidFile(const boost::filesystem::path &path) const
{
	if (!boost::filesystem::exists(path))
		return (false);
	return (true);
}

bool								HTTPModule::isValidPath(const boost::filesystem::path &path) const
{
	if (path.string().find("..") != std::string::npos)
		return (false);
	return (true);
}

std::string							HTTPModule::isValidFileRead(std::shared_ptr<ZiApi::HttpMessage> &request, const boost::filesystem::path &path) const
{
	boost::filesystem::path			workingDir = request->getHeader()->getValueFromMapField("Host");

	std::cout << "Directory : " << workingDir / path << std::endl;
	if (!isValidFile(workingDir / path))
		return ("404 Not Found");
	if (!isValidPath(path))
		return ("403 Forbidden");

	boost::filesystem::perms		rights = boost::filesystem::status(path).permissions();

	if (!(rights | boost::filesystem::perms::others_read) && !(rights | boost::filesystem::perms::group_read)
		&& !(rights | boost::filesystem::perms::owner_read))
		return ("403 Forbidden");
	return ("");
}

std::string							HTTPModule::isValidFileWrite(std::shared_ptr<ZiApi::HttpMessage> &request, const boost::filesystem::path &path) const
{
	boost::filesystem::path			workingDir = request->getHeader()->getValueFromMapField("Host");

	if (!isValidFile(workingDir / path))
		return ("404 Not Found");
	if (!isValidPath(path))
		return ("403 Forbidden");

	boost::filesystem::perms		rights = boost::filesystem::status(path).permissions();

	if (!(rights | boost::filesystem::perms::others_write) && !(rights | boost::filesystem::perms::group_write)
		&& !(rights | boost::filesystem::perms::owner_write))
		return ("403 Forbidden");
	return ("");
}