#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include "utils.hpp"

void check_error(boost::system::error_code ec)
{
 	if (!ec) 
	{
    		return;
  	};
	
  	std::cout << ec.message() << std::endl;
  	exit(-1);
};

int main(int argc, char* argv[])
{
  	if (argc != 4)
	{
    		std::cout << "invalid number of arguments\n";
    		return -1;
  	};

  	boost::asio::io_service ios;
  
  	const std::string ip{argv[1]};
  	const auto port{utils::to_int<uint16_t>(argv[2])};
  	const std::string request{argv[3]};
  
  
  	boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::address::from_string(ip), port};

  	boost::asio::ip::tcp::socket socket(ios);
  
  	boost::system::error_code ec;
  	socket.open(endpoint.protocol(), ec);

  	check_error(ec);

  	socket.connect(endpoint, ec);

  	check_error(ec);

  	std::cout << "sending request\n";
  	boost::asio::write(socket, boost::asio::buffer(request + "\n"), ec);

  	check_error(ec);

  	std::cout << "waiting for response\n";
  	boost::asio::streambuf buf;
  	boost::asio::read_until(socket, buf, '\n');

  	check_error(ec);

  	std::istream input{&buf};
  	std::string response;
  	std::getline(input, response);

  	std::cout << "response: " << response << "\n";
}
