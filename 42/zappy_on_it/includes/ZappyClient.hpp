//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ZAPPYCLIENT_HPP__
# define __ZAPPYCLIENT_HPP__

#include <iostream>
#include <vector>

namespace Zappy {
	enum ClientRole
	{
		CLIENT,
		EXPECTATOR,
		PLAYER,
		DISCONNECTED,
		REJECTED
	};

	class ZappyClient {
		public: 
			ZappyClient(int fd);
			~ZappyClient();
			void read_msg();
			int get_fd() const;
			ClientRole get_role() const;
			std::string get_role_str() const;
			std::string get_cmd() const;
			std::string get_cmd_opts() const;
			bool client_should_disconnect() const;
			void reset_cmd();
			bool msg_is_ready() const;
		private:
			static constexpr int BUF_SIZE = 1024;
			const std::string client_role_str_map[5] = {
			    "Client",
			    "Expectator",
			    "Player",
			    "Disconnected",
			    "Rejected"
			};
			const int fd;
			enum ClientRole role;
			std::string msg;
			std::vector<std::string> cmd_history;
	};
	std::ostream&	operator<<(std::ostream&, const ZappyClient&);
}

#endif

