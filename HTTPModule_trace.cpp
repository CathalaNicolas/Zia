#include "HTTPModule.h"

void						HTTPModule::methodTRACE(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response) const
{
	ZiApi::HttpMessage		message;

	message.setBody(request->getHeader()->getStringField());
	setResponse(request, response, message, "");
}

std::string					HTTPModule::setTraceHeaderResponse(const ZiApi::HttpMessage &message) const
{
	auto						actualDate = std::chrono::system_clock::now();
	std::time_t					time = std::chrono::system_clock::to_time_t(actualDate);
	std::string					date = std::string("Date: ") + std::string(std::ctime(&time));
	std::string					serverSoftware = "Server: Zia/1.0\r\n";
	std::string					contentLength = "Content-Length: " + std::to_string(message.getBody()->getStringField().size()) + "\r\n";
	std::string					contentType = "Content-Type: message/http\r\n";
	std::string					connection = "Connection: keep-alive\r\n";
	std::string					response = "HTTP/1.1 200 OK\r\n";

	return (response + date + serverSoftware + contentLength + contentType + connection + "\r\n");
}
