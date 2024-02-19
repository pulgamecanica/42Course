//***************************//
//*Template by pulgamecanica*//
//***************************//

extern "C" {
  #include <signal.h>
  #include <string.h>
}

#include "Zappy.hpp"
#include "ZappyServer.hpp"


// Client is Blue
// Server is Green

// Sys Signal
// -42 means the server should keep going
// 42 means the server should stop
int g_sys_signal = -42;

static void sighandler(int signal)
{
  std::cout << "Signal Received: [" << RED << signal << ENDC << ":" << BLUE << strsignal(signal) << ENDC << "]" << std::endl;
  g_sys_signal = 42; 
}

int main(void) {
  // Handle arguments

  { // Setup signal
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = sighandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGTERM, &sigIntHandler, NULL);
    sigaction(SIGINT, &sigIntHandler, NULL);
    sigaction(SIGKILL, &sigIntHandler, NULL);
    sigaction(SIGQUIT, &sigIntHandler, NULL);
  }
  // Setup & start the zappy server
  Zappy::ZappyServer sz(42);
  sz.run(&g_sys_signal);

  // Handle anything that should be freed or deleted...
	std::cout << "Thanks for running the Zappy Game\nPulgamecanica greets you :D We hope to see you back soon\n";
	return (0);
}
