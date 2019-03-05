#pragma once

#include "IModule.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>
#include <boost/filesystem.hpp>

class HTTPModule : public ZiApi::IModule
{
public:
	HTTPModule();
	~HTTPModule();

	void			execHTTP(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response);
	void			initHTTP(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response);

private:
	bool			init = false;

	void			createInitHTTP();
	void			createExecHTTP();
	
	bool			isValidPath(const boost::filesystem::path &path)const;
	bool			isValidFile(const boost::filesystem::path &path)const;
	bool			isForHttpModule(std::shared_ptr<ZiApi::HttpMessage> &request)const;
	std::string		isValidFileRead(std::shared_ptr<ZiApi::HttpMessage> &request, const boost::filesystem::path &path)const;
	std::string		isValidFileWrite(std::shared_ptr<ZiApi::HttpMessage> &request, const boost::filesystem::path &path)const;
	std::string		isValidHTTPHeader(std::shared_ptr<ZiApi::HttpMessage> &request)const;

	void			setResponse(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response, const ZiApi::HttpMessage &message, const std::string &error)const;
	std::string		setHeaderResponse(std::shared_ptr<ZiApi::HttpMessage> &request, const ZiApi::HttpMessage &message, const std::string &error)const;
	
	std::string		getFileContent(std::shared_ptr<ZiApi::HttpMessage> &request, const std::string &fileName)const;
	void			setFileContent(std::shared_ptr<ZiApi::HttpMessage> &request, ZiApi::HttpMessage &message, const std::string &fileName)const;
	void			deleteFile(std::shared_ptr<ZiApi::HttpMessage> &request, const boost::filesystem::path &fileName)const;

	void			methodGET(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response)const;
	std::string		setGetHeaderResponse(const ZiApi::HttpMessage &message, const std::string &error)const;

	void			methodPOST(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response)const;
	std::string		setPostHeaderResponse(const ZiApi::HttpMessage &message, const std::string &error)const;

	void			methodERROR(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response, const std::string &error)const;
	std::string		setERRORHeaderResponse(const std::string &error)const;

	void			methodTRACE(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response)const;
	std::string		setTraceHeaderResponse(const ZiApi::HttpMessage &message)const;

	std::string		setOptionHeaderResponse()const;
	void			methodOPTION(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response)const;

	std::string		setDeleteHeaderResponse(const std::string &error)const;
	void			methodDELETE(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response)const;

};
