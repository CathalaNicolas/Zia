#include "HTTPModule.h"

std::string						HTTPModule::setDeleteHeaderResponse(const std::string &error) const
{
	auto						actualDate = std::chrono::system_clock::now();
	std::time_t					time = std::chrono::system_clock::to_time_t(actualDate);
	std::string					date = std::string("Date: ") + std::string(std::ctime(&time));
	std::string					serverSoftware = "Server: Zia/1.0\r\n";
	std::string					connection = "Connection: keep-alive\r\n";
	std::string					response = "HTTP/1.1 ";

	if (error.compare("") == 0) {
		response += "204 No Content\r\n";
	}
	else {
		response += error + "\r\n";
		connection = "Connection: Closed\r\n";
	}
	return (response + date + serverSoftware + connection + "\r\n");
}

void							HTTPModule::methodDELETE(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response) const
{
	boost::filesystem::path		workingDir = request->getHeader()->getValueFromMapField("Host");
	boost::filesystem::path		fileName = request->getHeader()->getValueFromMapField("REQUEST_RESSOURCE");
	ZiApi::HttpMessage			message;
	std::string					error("");

	if (!isValidFile(workingDir / fileName))
		error = "404 Not Found";
	else if (!isValidPath(fileName))
		error = "403 Forbidden";
	else
		deleteFile(request, fileName);
	setResponse(request, response, message, error);
}