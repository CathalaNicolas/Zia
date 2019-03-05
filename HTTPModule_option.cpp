#include "HTTPModule.h"

std::string					HTTPModule::setOptionHeaderResponse() const
{
	auto						actualDate = std::chrono::system_clock::now();
	std::time_t					time = std::chrono::system_clock::to_time_t(actualDate);
	std::string					date = std::string("Date: ") + std::string(std::ctime(&time));
	std::string					serverSoftware = "Server: Zia/1.0\r\n";
	std::string					connection = "Connection: Closed\r\n";
	std::string					response = "HTTP/1.1 200 OK\r\n";

	return (response + date + serverSoftware  + connection + "\r\n");
}

void						HTTPModule::methodOPTION(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response) const
{
	ZiApi::HttpMessage		message;

	setResponse(request, response, message, "");
}
