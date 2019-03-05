#include "HTTPModule.h"

std::string						HTTPModule::setPostHeaderResponse(const ZiApi::HttpMessage &message, const std::string &error) const
{
	auto						actualDate = std::chrono::system_clock::now();
	std::time_t					time = std::chrono::system_clock::to_time_t(actualDate);
	std::string					date = std::string("Date: ") + std::string(std::ctime(&time));
	std::string					serverSoftware = "Server: Zia/1.0\r\n";
	std::string					contentLength = "Content-Length: " + std::to_string(message.getBody()->getStringField().size()) + "\r\n";
	std::string					contentType = message.getHeader()->getValueFromMapField("Content-Type");
	std::string					connection = "Connection: keep-alive\r\n";
	std::string					response = "HTTP/1.1 ";
	std::string					location = "";

	if (error.compare("") == 0) {
		if (message.getHeader()->getValueFromMapField("Location").compare("") != 0) {
			location = "Location: " + message.getHeader()->getValueFromMapField("Location") + "\r\n";
			response += "201 Created\r\n";
		}
		else
			response += "200 OK\r\n";
	}
	else {
		response += error + "\r\n";
		connection = "Connection: Closed\r\n";
	}
	return (response + date + serverSoftware + contentLength + contentType + location + connection + "\r\n");
}

void			HTTPModule::methodPOST(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response) const
{
	std::string					fileName = request->getHeader()->getValueFromMapField("REQUEST_RESSOURCE");
	ZiApi::HttpMessage			message;
	std::string					error("");

	if ((error = isValidFileWrite(request, fileName)).compare("") == 0) {
		setFileContent(request, message, fileName);
		setResponse(request, response, message, "");
	}
	else if (error.compare("404 Not Found") == 0) {
		setFileContent(request, message, fileName);
		setResponse(request, response, message, "");
	}
	else
		setResponse(request, response, message, error);
}
