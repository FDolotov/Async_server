#include <memory>
#include <atomic>

#include <boost/asio.hpp>

namespace echo
{
	using socket_type = boost::asio::ip::tcp::socket;

	class connection_handler
	{
		public:
  			void handle(std::shared_ptr<socket_type> socket, boost::system::error_code& ec);
	};

	class server
	{
		public:
  			explicit server(boost::asio::io_service& ios, const uint16_t port);
  			~server();
  			void start();
  			void stop();

		private:
  			void run();
  			std::unique_ptr<std::thread> main_loop_thread_;

  			boost::asio::io_service& ios_;
  			boost::asio::ip::tcp::endpoint endpoint_;
  			boost::asio::ip::tcp::acceptor acceptor_;
  			connection_handler handler_;
  			std::atomic<bool> is_running{false};
	};

} // namespace tcp_echo
