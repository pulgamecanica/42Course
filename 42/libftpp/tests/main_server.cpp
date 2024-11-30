#include "server.hpp"
#include "thread_safe_iostream.hpp"
#include <string>
#include <algorithm>

ThreadSafeIOStream threadSafeCout;

int main() {
    Server server;

    // Define an action for messages of type 1 (int)
    server.defineAction(1, [&server](long long& clientID, const Message& msg){
        int value;
        msg >> value;
        threadSafeCout << "Received an int " << value << " from client " << clientID << std::endl;

        // Send back a message of type 3 with double the value
        Message replyMsg(3);
        replyMsg << (double)(value * 2.0);
        server.sendTo(replyMsg, clientID);
    });

    // Define an action for messages of type 2 (size_t followed by characters)
    server.defineAction(2, [](long long& clientID, const Message& msg){
        // size_t length;
        // std::string text;
        // msg >> length;
        // text.reserve(length);
        // for (size_t i = 0; i < length; ++i) {
        //     char c;
        //     msg >> c;
        //     text.push_back(c);
        // }
        std::string text;
        msg >> text;
        threadSafeCout << "Received a string '" << text << "' of length " << text.size() << " from client " << clientID << std::endl;
    });

    // Start the server on port 8080
    server.start(8080);

   	bool quit = false;

	while (!quit)
	{
		server.update();

		threadSafeCout << "Server updated." << std::endl;
		threadSafeCout << "Available operations :" << std::endl;
		threadSafeCout << " - [Q]uit : close the program" << std::endl;
        threadSafeCout << " - [1] Send int to all clients" << std::endl;
        threadSafeCout << " - [2] Send string to all clients" << std::endl;
        threadSafeCout << " - [3] Send double to all clients" << std::endl;
		threadSafeCout << " - Any other input to continue updating the server" << std::endl;

		std::string input;
		std::getline(std::cin, input);

		std::transform(input.begin(), input.end(), input.begin(), 
		               [](unsigned char c){ return std::tolower(c); });

		if (input.length() == 1 && input[0] == '1') {
            Message msg(1);
            int val = 42;
            msg << val;
            server.sendToAll(msg);
        } else if (input.length() == 1 && input[0] == '2') {
            Message msg(2);
            std::string val = "Hello";
            msg << val;
            server.sendToAll(msg);
        } else if (input.length() == 1 && input[0] == '3') {
            Message msg(3);
            double val = 42.42f;
            msg << val;
            server.sendToAll(msg);
        } else if (input == "quit" || (input.length() == 1 && input[0] == 'q')) {
		    quit = true;
		}
	}
	
    return 0;
}

