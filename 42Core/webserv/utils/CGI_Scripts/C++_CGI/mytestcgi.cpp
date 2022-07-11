#include <iostream>

#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <cgicc/CgiDefs.h> 
#include <cgicc/Cgicc.h> 
#include <cgicc/HTTPHTMLHeader.h> 
#include <cgicc/HTMLClasses.h>

using namespace cgicc;

// const std::string ENV[24] = {
//    "COMSPEC", "DOCUMENT_ROOT", "GATEWAY_INTERFACE",   
//    "HTTP_ACCEPT", "HTTP_ACCEPT_ENCODING",             
//    "HTTP_ACCEPT_LANGUAGE", "HTTP_CONNECTION",         
//    "HTTP_HOST", "HTTP_USER_AGENT", "PATH",            
//    "QUERY_STRING", "REMOTE_ADDR", "REMOTE_PORT",      
//    "REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME",
//    "SCRIPT_NAME", "SERVER_ADDR", "SERVER_ADMIN",      
//    "SERVER_NAME","SERVER_PORT","SERVER_PROTOCOL",     
//    "SERVER_SIGNATURE","SERVER_SOFTWARE" };


int main () {
   Cgicc formData;
  
   std::cout << "Content-type:text/html\r\n\r\n";
   std::cout << "<html>\n";
   std::cout << "<head>\n";
   std::cout << "<title>Drop Down Box Data to CGI</title>\n";
   std::cout << "</head>\n";
   std::cout << "<body>\n";

   form_iterator fi = formData.getElement("country");
   if( !fi->isEmpty() && fi != (*formData).end()) {  
      std::cout << "Value Selected: " << **fi << std::endl;  
   }
  
   std::cout << "<br/>\n";
   std::cout << "</body>\n";
   std::cout << "</html>\n";
   
   return 0;
}


// int main(int argc, char *argv[], char *env[]) {
//     int i = -1;
//     std::cout << argc << std::endl;
//     std::cout << "ARGUMENTS" << std::endl;
//     while (++i < argc)
//         std::cout << argv[i] << std::endl;
//     i = -1;
//     std::cout << "ENVIRONMENTALS" << std::endl;
//     while (env[++i])
//         std::cout << env[i] << std::endl;

//     char buff[20];
//     int valread = read(0, buff, 20);

//     std::cout << "buffer; valread; " <<  buff << valread << std::endl;

//     <form action = "/cgi-bin/cpp_dropdown.cgi" method = "post" target = "_blank">
//     <select name = "dropdown">
//         <option value = "Maths" selected>Maths</option>
//         <option value = "Physics">Physics</option>
//     </select>

//     <input type = "submit" value = "Submit"/>
//     </form>


//     std::cout << std::string('*', 50) << std::endl;





//     // for ( int i = 0; i < 24; i++ ) {
//     //     std::cout << "\n" << ENV[ i ] << "\n";
    
//     //     char *value = getenv(ENV[i].c_str());
//     //     if (value)
//     //         std::cout << value << std::endl;
//     //     else
//     //         std::cout << "Value is empty" << std::endl;
//     // }
// }