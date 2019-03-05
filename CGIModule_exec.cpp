#include "CGIModule.h"


std::string						CGI::CGIModule::execChild(const boost::process::environment &env, const std::string &cgiName)
{
	int							timeOut = 10;
	int							latency = 10;
	int							maxTimeOut = latency * (1000 / latency) * timeOut;
	int							counter = 0;
	boost::process::ipstream	is;
	boost::process::child		c(boost::filesystem::path("./cgi-bin/" + cgiName),
		boost::process::std_out > is, boost::process::std_err > boost::process::null, env);
	std::string					line("");
	std::string					data("");

	while (c.running() || std::getline(is, line)) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(latency));
		if (c.running() && counter >= maxTimeOut) {
			c.terminate();
			return ("500 Internal Server Error");
		}
		counter += latency;
		if (!line.empty())
			data += line + "\n";
		else if (counter != latency) {
			data += "\n";
		}
	}
	c.wait();
	if (c.exit_code() != 0)
		return ("500 Internal Server Error");
	return data;
}

std::string							CGI::CGIModule::checkValidCGIFile(const std::string &cgiName)
{

	boost::filesystem::path					path = boost::filesystem::current_path().string() + "/cgi-bin/" + cgiName;

	if (!boost::filesystem::exists(path))
		return ("404 Not Found");

	boost::filesystem::perms rights = boost::filesystem::status(path).permissions();

	if (!(rights | boost::filesystem::perms::others_exe) && !(rights | boost::filesystem::perms::group_exe)
		&& !(rights | boost::filesystem::perms::owner_exe))
		return ("403 Forbidden");
	return ("");
}

std::string						CGI::CGIModule::startChild(const boost::process::environment &env)
{
	std::string					cgiName = envValue["SCRIPT_NAME"];
	std::string					data("");

	data = checkValidCGIFile(cgiName);
	if (data.compare("") != 0)
		return data;
	try {
		data = execChild(env, cgiName);
	}
	catch (boost::process::process_error error) {
		return ("403 Forbidden");
	}
	catch (std::logic_error error) {
		return ("500 Internal Server Error");
	}
	return data;
}


bool							CGI::CGIModule::isValidCGIRequest()
{
	std::map<std::string, std::string>::iterator it;

	if ((it = envValue.find("REQUEST_URI")) == envValue.end())
		return (false);
	if (envValue["REQUEST_URI"].compare("error") == 0)
		return (false);
	if ((it = envValue.find("SCRIPT_NAME")) == envValue.end())
		return (false);
	if ((it = envValue.find("QUERY_STRING")) == envValue.end())
		return (false);
	if ((it = envValue.find("REQUEST_METHOD")) == envValue.end())
		return (false);
	if (envValue["REQUEST_METHOD"].compare("GET") != 0 && envValue["REQUEST_METHOD"].compare("POST") != 0
		&& envValue["REQUEST_METHOD"].compare("HEAD") != 0)
		return (false);
	return (true);
}