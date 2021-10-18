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
	std::string user_inp = request; 
  
  
  	boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::address::from_string(ip), port};

  	boost::asio::ip::tcp::socket socket(ios);
  
  	boost::system::error_code ec;
  	socket.open(endpoint.protocol(), ec);

  	check_error(ec);

  	socket.connect(endpoint, ec);

  	check_error(ec);

  	std::cout << "sending request\n";
	std::string l = "\"70:\"\"CONTENT_LENGTH\" <00> \"";
	int length = request.length();
	l.append(std::to_string(length));
	l.append("\" <00>");
	l.append("\n\"SCGI\" <00> \"1\" <00>\n\"REQUEST_METHOD\" <00> \"POST\" <00>\n\"REQUEST_URI\" <00> \"/talk\" <00>\n\",\"");
	
  	boost::asio::write(socket, boost::asio::buffer(l + "\n\"" + request + "\"\n\n"), ec);

  	check_error(ec);

  	std::cout << "waiting for response\n";
  	boost::asio::streambuf buf;
  	boost::asio::read_until(socket, buf, '\n');

  	check_error(ec);

  	std::istream input{&buf};
  	std::string response;
	std::string full2;
  	while(std::getline(input, response))
	{
		response += "\n";
		full2 += response;
	};


  	std::cout << "response: " << "\n\n" << full2  << "\"" << user_inp << "\"\n";
}
