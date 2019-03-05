#include "Event.h"
#include "Handler.h"
#include "CGIModule.h"
#include "HTTPModule.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include <boost/dll/import.hpp>
#include <cstring>

template<typename T>
boost::shared_ptr<T> make_shared_ptr(std::shared_ptr<T>& ptr)
{
	return boost::shared_ptr<T>(ptr.get(), [ptr](T*) mutable {ptr.reset(); });
}

template<typename T>
std::shared_ptr<T> make_shared_ptr(boost::shared_ptr<T> &ptr)
{
	return std::shared_ptr<T>(ptr.get(), [ptr](T*) mutable {ptr.reset(); });
}

void								loadModule(std::shared_ptr<ZiApi::Handler> &handler, std::string &path)
{

	//Init Boost to load dll or .so
	boost::filesystem::path			lib_path(path);				//Path to the library
	std::shared_ptr<ZiApi::IModule>	plugin;						//Smart ptr that will contain a pointer to our new module

	std::cout << lib_path << std::endl;
	auto/*boost::shared_ptr<ZiApi::IModule>*/ tmp = boost::dll::import<ZiApi::IModule>(lib_path,		//Path to the library / "name of the dll"
		"module");			//Name of the symbol to import, see HelloWorldModule.h lign:23

	plugin = make_shared_ptr(tmp);					//Convert the temporary pointer 

	//We got our new module now we can add it to the handler list
	handler->registerModule(plugin);			//Register the module in the handler's list of module.
	return;
}

int	testFunc()
{
	while (1);
	return (0);
}

int	miniServer()
{
	int server_fd, new_socket; long valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	//Init the Handler and its request / response
	std::shared_ptr<ZiApi::Handler> handler = std::make_shared<ZiApi::Handler>();

	//loadModule(handler, argv[1]);

	std::shared_ptr<ZiApi::IModule> plugin = std::make_shared<HTTPModule>();

	handler->registerModule(plugin);

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("In socket");
		exit(EXIT_FAILURE);
	}


	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);

	memset(address.sin_zero, '\0', sizeof address.sin_zero);


	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("In bind");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 10) < 0)
	{
		perror("In listen");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n" << std::endl;
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
		{
			perror("In accept");
			exit(EXIT_FAILURE);
		}

		char buffer[30000] = { 0 };
		valread = read(new_socket, buffer, 30000);
		std::cout << "Request:" << buffer << std::endl;
		handler->setRequest(buffer);
		//Launch all module for the anchorPoint Request
		handler->launchTypeModule(ZiApi::Event::AnchorPoint::REQUEST);
		handler->launchTypeModule(ZiApi::Event::AnchorPoint::MODULE);

		std::cout << "Response: " << std::endl << handler->getResponse()->getMessage() << std::endl;
		write(new_socket, handler->getResponse()->getMessage().c_str(), handler->getResponse()->getMessage().length());
		std::cout << "------------------Hello message sent-------------------" << std::endl;
		close(new_socket);
		memset(buffer, 0, 30000);
		handler->resetRequestResponse();
	}
	return 0;
}
int main(int argc, char *argv[])
{

	miniServer();
	return (0);
}