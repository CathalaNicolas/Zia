#include "HTTPModule.h"

void						HTTPModule::methodERROR(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response, const std::string &error) const
{
	response->setHeader(setERRORHeaderResponse(error));
	response->setBody("");
}

std::string					HTTPModule::setERRORHeaderResponse(const std::string &error) const
{
	auto						actualDate = std::chrono::system_clock::now();
	std::time_t					time = std::chrono::system_clock::to_time_t(actualDate);
	std::string					date = std::string("Date: ") + std::string(std::ctime(&time));
	std::string					serverSoftware = "Server: Zia/1.0\r\n";
	std::string					contentLength = "Content-Length: 0\r\n";
	std::string					connection = "Connection: keep-alive\r\n";
	std::string					response = "HTTP/1.1 ";
	std::string					allow = "Allow: GET, POST, HEAD, PUT, DELETE, OPTIONS, TRACE\r\n";

	response += error + "\r\n";
	return (response + date + serverSoftware + contentLength  + allow + connection + "\r\n");
}
