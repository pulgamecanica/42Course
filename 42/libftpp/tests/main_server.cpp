#include "server.hpp"
#include "thread_safe_iostream.hpp"
#include <string>

int main() {
    Server server;

    // Define an action for messages of type 1 (int)
    server.defineAction(1, [&server](long long& clientID, const Message& msg){
        int value;
        msg >> value;
        threadSafeCout << "Received an int " << value << " from client " << clientID << std::endl;

        // Send back a message of type 3 with double the value
        Message replyMsg;
        replyMsg << (value * 2);
        server.sendTo(replyMsg, clientID);
    });

    // Define an action for messages of type 2 (size_t followed by characters)
    server.defineAction(2, [](long long& clientID, const Message& msg){
        size_t length;
        std::string text;
        msg >> length;
        text.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            char c;
            msg >> c;
            text.push_back(c);
        }
        threadSafeCout << "Received a string '" << text << "' of length " << length << " from client " << clientID << std::endl;
    });

    // Start the server on port 8080
    server.start(8080);

   	bool quit = false;

	while (!quit)
	{
		client.update();

		threadSafeCout << "Server updated." << std::endl;
		threadSafeCout << "Available operations :" << std::endl;
		threadSafeCout << " - [Q]uit : close the program" << std::endl;
		threadSafeCout << " - Any other input to continue updating the server" << std::endl;

		std::string input;
		std::getline(std::cin, input);

		std::transform(input.begin(), input.end(), input.begin(), 
		               [](unsigned char c){ return std::tolower(c); });

		if (input == "quit" || (input.length() == 1 && input[0] == 'q')) {
		    quit = true;
		}
	}
	
    return 0;
}

