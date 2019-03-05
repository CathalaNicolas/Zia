#include "CGIModule.h"


void												CGI::CGIModule::initChildEnv(boost::process::environment &childEnv)
{
	for (auto const &it : envValue)
	{
		childEnv[it.first] += it.second;
	}
}

void												CGI::CGIModule::initEnvValueServer(std::shared_ptr<ZiApi::HttpMessage> &request)
{
	envValue.insert(std::make_pair<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
	envValue.insert(std::make_pair<std::string, std::string>("SERVER_SOFTWARE", "ZiA/1.0"));
	envValue.insert(std::make_pair<std::string, std::string>("SERVER_NAME", "127.0.0.1"));
	envValue.insert(std::make_pair<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
	envValue.insert(std::make_pair<std::string, std::string>("SERVER_PORT", "8080"));
	envValue.insert(std::make_pair<std::string, std::string>("SERVER_NAME", "127.0.0.1"));
	envValue.insert(std::make_pair<std::string, std::string>("SERVER_ADDR", "127.0.0.1"));
	envValue.insert(std::make_pair<std::string, std::string>("SERVER_ADMIN", "ZiaTeams"));
	envValue.insert(std::make_pair<std::string, std::string>("REMOTE_ADDR", "127.0.0.1"));
}

void												CGI::CGIModule::initEnvValueHeaderRequest(std::shared_ptr<ZiApi::HttpMessage> &request)
{
	std::map<std::string, std::string>				map = request->getHeader()->getField();
	std::map<std::string, std::string>::iterator	it;

	if ((it = map.find("Accept")) != map.end())
		envValue.insert(std::make_pair<std::string, std::string>("HTTP_ACCEPT", it->second.c_str()));
	if ((it = map.find("Accept-Language")) != map.end())
		envValue.insert(std::make_pair<std::string, std::string>("HTTP_ACCEPT_LANGUAGE", it->second.c_str()));
	if ((it = map.find("Accept-Charset")) != map.end())
		envValue.insert(std::make_pair<std::string, std::string>("HTTP_ACCEPT_CHARSET", it->second.c_str()));
	if ((it = map.find("Accept-Encoding")) != map.end())
		envValue.insert(std::make_pair<std::string, std::string>("HTTP_ACCEPT_ENCODING", it->second.c_str()));
	if ((it = map.find("Accept-Datetime")) != map.end())
		envValue.insert(std::make_pair<std::string, std::string>("HTTP_ACCEPT_DATETIME", it->second.c_str()));
	if ((it = map.find("User-Agent")) != map.end())
		envValue.insert(std::make_pair<std::string, std::string>("HTTP_USER_AGENT", it->second.c_str()));
	if ((it = map.find("Cookie")) != map.end())
		envValue.insert(std::make_pair<std::string, std::string>("HTTP_COOKIE", it->second.c_str()));
	if ((it = map.find("Connection")) != map.end())
		envValue.insert(std::make_pair<std::string, std::string>("HTTP_CONNECTION", it->second.c_str()));
	if ((it = map.find("Host")) != map.end())
		envValue.insert(std::make_pair<std::string, std::string>("HTTP_HOST", it->second.c_str()));
	if ((it = map.find("Cache-Control")) != map.end())
		envValue.insert(std::make_pair<std::string, std::string>("HTTP_CACHE_CONTROL", it->second.c_str()));
}

std::string						CGI::CGIModule::getRequestURI(std::shared_ptr<ZiApi::HttpMessage> &request)
{
	std::string					requestString = request->getHeader()->getValueFromMapField("REQUEST_RESSOURCE");
	std::size_t					pos = requestString.find("/cgi-bin/");

	if (pos == std::string::npos)
		return ("error");

	return (requestString.substr(pos));
}

std::string						CGI::CGIModule::getScriptName(std::shared_ptr<ZiApi::HttpMessage> &request)
{
	std::string					requestString = request->getHeader()->getValueFromMapField("REQUEST_RESSOURCE");
	std::size_t					posStart = requestString.find("/cgi-bin/") + std::string("/cgi-bin/").length();
	std::size_t					posEnd = requestString.find(" ", posStart + 1);

	if (posStart > requestString.length())
		return ("error");
	return (requestString.substr(posStart, posEnd - posStart));
}

std::string						CGI::CGIModule::getQueryString(std::shared_ptr<ZiApi::HttpMessage> &request)
{
	std::string					requestString = request->getHeader()->getValueFromMapField("REQUEST_RESSOURCE");
	std::size_t					pos = requestString.find("?");

	if (pos == std::string::npos)
		return ("");
	if (request->getHeader()->getValueFromMapField("REQUEST_METHOD").compare("GET") == 0)
		return (requestString.substr(pos + 1));
	else if (request->getHeader()->getValueFromMapField("REQUEST_METHOD").compare("POST") == 0)
		return (request->getBody()->getStringField());
	return ("");
}

std::string						CGI::CGIModule::getRequestMethod(std::shared_ptr<ZiApi::HttpMessage> &request)
{
	std::string					requestString = request->getHeader()->getValueFromMapField("REQUEST_METHOD");

	return (requestString);
}

void							CGI::CGIModule::initEnvValueRequest(std::shared_ptr<ZiApi::HttpMessage> &request)
{
	envValue.insert(std::make_pair<std::string, std::string>("REQUEST_URI", getRequestURI(request)));
	envValue.insert(std::make_pair<std::string, std::string>("SCRIPT_NAME", getScriptName(request)));
	envValue.insert(std::make_pair<std::string, std::string>("QUERY_STRING", getQueryString(request)));
	envValue.insert(std::make_pair<std::string, std::string>("REQUEST_METHOD", getRequestMethod(request)));
}

void							CGI::CGIModule::initEnvValue(std::shared_ptr<ZiApi::HttpMessage> &request)
{
	initEnvValueHeaderRequest(request);
	initEnvValueServer(request);
	initEnvValueRequest(request);
}