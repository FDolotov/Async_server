#include <iostream>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>

#include "server.hpp"
#include "utils.hpp"

namespace echo
{
	void connection_handler::handle(std::shared_ptr<socket_type> socket, boost::system::error_code& ec)
	{
  		std::cout << "start handling connection\n";

  		std::cout << "reading request\n";
  		boost::asio::streambuf buf;
  		boost::asio::read_until(*socket.get(), buf, "\n\n");

  		std::istream input{&buf};
  		std::string request;
		std::string full;
  		while(std::getline(input, request))
		{
			request += "\n";			
			full += request;
		};

  		std::cout << "request: \n\n" << full << "\n\n";

  		std::cout << "sending response\n";
		
		std::string full2 = "\"Status: 200 OK\"\n\"Content-Type: text/plain\"\n\"\"\n";
		
  		boost::asio::write(*socket.get(), boost::asio::buffer(full2 + "\n\n"));

  		std::cout << "closing connection\n";
  		socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);

  		std::cout << "\n\n";
	};

	server::server(boost::asio::io_service& ios, const uint16_t port) : 
  		ios_(ios), 
  		endpoint_(boost::asio::ip::address_v4::any(), port), 
  		acceptor_(ios_, endpoint_) 
		{
  			std::cout << "server ctor: " << port << "\n";
		};

	server::~server()
	{
  		std::cout << "server dtor\n";
  		stop();
	};

	void server::start()
	{
  		std::cout << "starting server\n";
  		main_loop_thread_.reset(new std::thread([this]() {run();}));
	};

	void server::run()
	{
  		std::cout << "start listening\n";
  		acceptor_.listen();

  		is_running.store(true);

  		while (is_running.load())
		{
    			std::cout << "creating new socket\n";
    			auto socket = std::make_shared<socket_type>(ios_);

    			std::cout << "accepting connection\n"; 
    			acceptor_.accept(*socket.get());

    			//create a thread
    			boost::system::error_code ec;
    			handler_.handle(socket, ec);
  		};

  		std::cout << "finish listenning\n";
	};

	void server::stop()
	{
  		std::cout << "stopping server\n";
  		is_running.store(false);
  		
		if (main_loop_thread_)
		{
    			main_loop_thread_->join();
  		};
	};
};


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
    		std::cout << "invalid number of arguments\n";
    		return -1;
  	};

  	boost::asio::io_service ios;

  	const auto port = utils::to_int<uint16_t>(std::string(argv[1]));
  	echo::server srv{ios, port};
  	srv.start();
};
