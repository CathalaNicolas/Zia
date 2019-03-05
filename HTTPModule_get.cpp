#include "HTTPModule.h"


void							HTTPModule::methodGET(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response) const
{
	std::string					fileName = request->getHeader()->getValueFromMapField("REQUEST_RESSOURCE");
	ZiApi::HttpMessage			message;
	std::string					error("");

	if ((error = isValidFileRead(request, fileName)).compare("") == 0) {
		if (request->getHeader()->getValueFromMapField("REQUEST_METHOD").compare("HEAD") != 0)
			message.setBody(getFileContent(request, fileName));
		setResponse(request, response, message, "");
	}
	else
		setResponse(request, response, message, error);
}


std::string						HTTPModule::setGetHeaderResponse(const ZiApi::HttpMessage &message, const std::string &error) const
{
	auto						actualDate = std::chrono::system_clock::now();
	std::time_t					time = std::chrono::system_clock::to_time_t(actualDate);
	std::string					date = std::string("Date: ") + std::string(std::ctime(&time));
	std::string					serverSoftware = "Server: Zia/1.0\r\n";
	std::string					contentLength = "Content-Length: " + std::to_string(message.getBody()->getStringField().size()) + "\r\n";
	std::string					contentType = message.getHeader()->getValueFromMapField("Content-Type");
	std::string					connection = "Connection: keep-alive\r\n";
	std::string					response = "HTTP/1.1 ";
	std::string					allow = "Allow: GET, POST, HEAD, PUT, DELETE, OPTIONS, TRACE\r\n";

	if (error.compare("") == 0)
		response += "200 OK\r\n";
	else {
		response += error + "\r\n";
		connection = "Connection: Closed\r\n";
	}
	return (response + date + serverSoftware + contentLength + contentType + connection + allow + "\r\n");
}