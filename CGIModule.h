#ifndef CGIMODULE_H_
#define CGIMODULE_H_

#include <string>
#include <map>
#include <functional>
#include <boost/process.hpp>
#include <ctime>
#include <chrono>
#include <boost/chrono/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/assign/list_of.hpp>
#include "IModule.h"
#include "HttpMessage.h"
#include "Event.h"

namespace CGI
{
	class CGIModule : public ZiApi::IModule
	{
	public:
		CGIModule();
		~CGIModule();

		void			initCGI(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response);
		void			execCGI(std::shared_ptr<ZiApi::HttpMessage> &request, std::shared_ptr<ZiApi::HttpMessage> &response);
	private:
		std::map<std::string, std::string>	envValue;
		bool								init = false;

		void			createInitCGI();
		void			createExecCGI();
		std::string		setResponseHeader(const ZiApi::HttpMessage &message, const std::string error);
		void			setResponse(std::shared_ptr<ZiApi::HttpMessage> &response, std::string &requestResponse, const ZiApi::HttpMessage &message);
		void			initChildEnv(boost::process::environment &childEnv);
		std::string		execChild(const boost::process::environment &env, const std::string &cgiName);
		std::string		startChild(const boost::process::environment &env);
		std::string		getRequestMethod(std::shared_ptr<ZiApi::HttpMessage> &request);
		std::string		getQueryString(std::shared_ptr<ZiApi::HttpMessage> &request);
		std::string		getRequestURI(std::shared_ptr<ZiApi::HttpMessage> &request);
		std::string		getScriptName(std::shared_ptr<ZiApi::HttpMessage> &request);
		void			initEnvValueRequest(std::shared_ptr<ZiApi::HttpMessage> &request);
		void			initEnvValueHeaderRequest(std::shared_ptr<ZiApi::HttpMessage> &request);
		void			initEnvValueServer(std::shared_ptr<ZiApi::HttpMessage> &request);
		void			initEnvValue(std::shared_ptr<ZiApi::HttpMessage> &request);
		bool			isValidCGIRequest();
		std::string		checkValidCGIFile(const std::string &cgiName);
	};

	/*extern "C" BOOST_SYMBOL_EXPORT CGI::CGIModule CGImodule;
	CGI::CGIModule CGImodule;*/
}

#endif