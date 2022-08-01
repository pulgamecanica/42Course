# webserver
Epoll HTTP Web Server with C++98
***

### Introduction
    ARPANET: Advanced Research Projects Agency Network
    HTTP: Hyper Text Transfer Protocole
    HTML: Hyper Text Markup Language
    CSS: Cascading Style Sheets
    RFC: Read For Comment
    
    "Development of HTTP was initiated by Tim Berners-Lee at CERN in 1989 and summarized in a simple document describing the behavior of a client and a server using the first HTTP protocol version that was named 0.9" (Wikipedia)
    The RFC system was invented by Steve Crocker in 1969 to help record unofficial notes on the development of ARPANET. Which later was used to define how the HTTP should work among other things.
    In the RFC it's defined the semantics used to best create HTTP Requests and Responses.
    
    What is a Client and a Server?
    A Server is a computer program which stores, manages, sends and recieves data. In particular an HTTP Server recieves an HTTP Request, and creates an HTTP Response.
    An HTTP server is usefull to navigate through resources in a given enviroment. For example, a website is a set of files which togeather make a functional webpage.
    Usually websites use HTML to define their content, a CSS stylesheet to modify the properties of the content (color, position, size; etc) and a script which can dinamicly modify the website (usually javascript).
    A Client is a program which can create an HTTP Request, and receive an HTTP Response.
    Web Browsers are the most common clients, which also can display the resources Requested in a readable way, using an HTML interpreter.
    Browsers will interpret the Request content, and will identify all the resources that are needed (images, videos, embeded links; etc) and will create a Request to show all resources needed by the webpage.
    What is a Socket
    .... explain sockets, host: ports ips etc etc
***

    For example
    The HTML document bellow, on the third line is requesting a CSS stylesheet, on line 4 it's requesting a JS script and on the last lines it's requesting an image.
    This HTML document needs 3 resouces and each will be requested individually, it's up to the Client to request it, and how to do it.
```html
<!DOCTYPE html>
<html lang="en">
<link rel="stylesheet" href="index.css">
<script src="index.js"></script>
<body>
 <h1>Hello, this is Home</h1>
 <h3>Click on the screen.</h3>
 <p>Coding is fun!</p>
 <img src="/random/content/mexico.jpg", width="500px" height="auto">
</body>
</html>
```
    Assuming that this HTML document is named index.html and the following configuration:
    The Route(location) "/" accepts the GET method, and has the indexes: "index.html & index.htm" and the root is "www/"
<details>
  <summary><h3>Configuration images</h3></summary>
    
![Screenshot from 2022-07-17 11-56-37](https://user-images.githubusercontent.com/28810331/179395051-a0dae928-8076-4023-8156-bc5f3b29b4ab.png)
![Screenshot from 2022-07-17 11-56-58](https://user-images.githubusercontent.com/28810331/179395054-bb179041-2f4e-4f29-9526-6c4f03d0741a.png)
</details>

<details>
  <summary><h3>Requests and Responses Walkthrough</h3></summary>
  
    The Following image shows the whole process of retrieving a webpage:
![image(3)](https://user-images.githubusercontent.com/28810331/179395199-32fdc267-f9d4-4e44-9878-2a7f332a9055.png)

    You get this as a result:
![Screenshot from 2022-07-17 12-04-26](https://user-images.githubusercontent.com/28810331/179395293-258feddb-3801-480a-8d39-9ac901b08f30.png)
</details>

### Configuration
    The webserver can be configured with directives. Each Directive serves one purpose although some Directives are obsolete when used in a certain context.
    This websserver a configuration file based on the NGINX configuration file.
    All Featured Directives are listed bellow on the table.
    The Configuration File has 2 contexts, server & location context. The Location context must belong to a server context. CGI is suported.
<details>
  <summary><h3>Directives</h3></summary>
Example:

```python
server {
  # INSIDE SERVER CONTEXT
  # (SP) Space
  DIRECTIVE (SP) VALUE
  location / {
    #INSIDE LOCATION CONTEXT
    DIRECTIVE (SP) VALUE
  }
}
```

| Directive | Description | Example |
| --------- | ----------- | ------- |
| autoindex | directory listing on & off | autoindex off \| on  |
| cgi | cgi set up a new cgi support extension. Each cgi directive will add an executable to the location. The cgi URI should always make a request to the executable including the extension. When the URI target is an executable which is not defined, the first executable for the extension is used. | cgi \<extension\> \<path_to_executable\> |
| cgi-bin | cgi-bin will set up the path to the executable where it should be executed also, it's setted up as an enviromental variable. | cgi-bin \<path_to_bin\> |
| client_max_body_size | Limit client body size, if the request exceeds the number of bytes, the error 414 is returned | client_max_body_size #bytes|
| error_page | Setup default error pages. You can define one or more error codes. The path where the server searches for the errors should be the last parameter of the directive. Webserv only supports custom HTML error pages. Webserv will look for "error path + error code + .html". The error pages are inherited from the server context to the location context.| error_page code1 [code2] [codeN] ... \<path\> |
| limit_methods | Define a list of accepted HTTP methods for the route (inside location scope), if not defined any, the default is to accept GET only. | limit_methods METHODS |
| listen | Choose the port and host of each ’server', localhost and "*" is accepted. | listen host:port |
| location | Setup routes with one or multiple rules/configuration | location route { ... }|
| root | Set up a root path to  | root \<path\> |
| redirect | Redirect to another location.  | redirect [full URI or relative URI] |
| server_name | Setup the server_names or not | server_name name1 [name2] ...|
| upload | upload | upload |

</details>

### TODO
  - File Upload
  - Limit Body Size
  - PUT & DELETE actions
  - Coockies

### BUGS
  - Autoindex is on
  - Location root doesn't exist
  - Should Return 404 not found -> returns 403 Forbidden
  
### RFC's

[HTTP Semantics RFC](https://www.rfc-editor.org/rfc/rfc9110.pdf)

[HTTP/1.1 RFC](https://www.rfc-editor.org/rfc/rfc9112.pdf)

[URI RFC](https://www.rfc-editor.org/rfc/pdfrfc/rfc3986.txt.pdf)

[ABNF RFC](https://www.rfc-editor.org/rfc/pdfrfc/rfc5234.txt.pdf)

[MIME RFC](https://www.rfc-editor.org/rfc/pdfrfc/rfc2045.txt.pdf)

### Guides

- Simple Webserver
    + [Simple-webserver-from-scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
- NGINX (Directives)
    + [NGINX webserver configuration file tutorial](https://www.plesk.com/blog/various/nginx-configuration-guide/)
- GET / POST / DELETE
    + [https://www.youtube.com/watch?v=nMGlaiNBbNU&list=PL6n9fhu94yhW7yoUOGNOfHurUE6bpOO2b&index=4](https://www.youtube.com/watch?v=nMGlaiNBbNU&list=PL6n9fhu94yhW7yoUOGNOfHurUE6bpOO2b&index=4)
- MORE...
    + [https://www.youtube.com/watch?v=eesqK59rhGA](https://www.youtube.com/watch?v=eesqK59rhGA)
    + [https://www.youtube.com/watch?v=FBkZ2TJZZUY](https://www.youtube.com/watch?v=FBkZ2TJZZUY)

- epoll
    + [https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642](https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642)
    + [https://www.youtube.com/watch?v=O-yMs3T0APU](https://www.youtube.com/watch?v=O-yMs3T0APU)
    + [https://www.youtube.com/watch?v=dEHZb9JsmOU](https://www.youtube.com/watch?v=dEHZb9JsmOU)

<details>
  <summary><h3>Tony Extra Info</h3></summary>

* SEARCH FOR A FILE:
 * if found -> return a file in a body message
 * else -> return a file with error code (what code? 404 - file not found)
  * find response codes and messages corresponding to them, create std::map<errorCode, response>
* BOTH CASES:
 * content length = size of the buffer of the file that we look for / or response html
 * content type = CHECK HOW TO GET FILE METADATA; can also set encoding
 * DATE: TODAY, NOW (when the request was made or when we sent response?)
   * ! Last-Modified - last modified date(only for encountered file)



// PSEUDO CODE:

```c
std::map<errorCode, response>;

buffer = read(expected_file)
if buffer == -1
    return 404.html;

if (expected_file.has_value() != error)
    .create file "response"; // actually it can be stringstream where we save

    response << "Status-Line = HTTP/1.1 << map.at(position_of_status_code).first << map[status_code]; // HTTP/1.1 404 Not Found << std::endl;
    response << "Server: PulgaBrenoPrzemek/1.0 (Ubuntu 20.04 LTS)" << std::endl;
    response << "Content Type: " << split the file name, take the part after "." eg: castle.jpg -> so if jpg -> content type: (find it in the internet)
        *Content-type: text/plain; charset=us-ascii <- default (when cliend does not specify nothing). type/subtype ; parametre // parametre := attribute(charset) = value(us-ascii)
    We will have to check if the content type we sent is accepted by the browser (mime types has to be parsed in request). if not it throws 415)


if (buffer.compare("connection")
    if value after = keep-alive(is now by default)

    response << "Connection: Open/Keep-Alive" << std::endl;

    else
    response << "Connection: Closed" << std::endl;
        CLOSE SOCKET AT THE END - dont! client close connection. We can set time after we gonna close it but not depends on it.


Client Content Negotiation:
If there is multiple resource, server may sent back list of avaliable representations of the resource (code 300), then client sent another get request with a
link that he wants reagarding to the headders (language he accept, encoding etc)


{
  // Content-Type:

std::string mimetype(std::string const & file_name) {
  file_name = split('.') -> take part after '.' eg jpg/html/css etc;


//TEXT
  if (txt)
    return "text/plain"
  else if (html)
    return "text/html";
  else if (css)
    return "text/css";
  else if (js) //javascript // "; parametre" - charset=/anything/ makes it invalid!
    return "text/javascript"
  else if (json)
    return "application/json"
  else if (jsonld)
    return "application/ld+json";
  else if (xml)
    return "application/xml";
  else if (pdf)
    return "application/pdf"

//DOCUMENTS
  else if(doc)
    return "application/msword";
  else if(docx)
    return "application/vnd.openxmlformats-officedocument.wordprocessingml.document"
  else if (ppt)
    return "application/vnd.ms-powerpoint"
  else if (pptx)
    return "application/vnd.openxmlformats-officedocument.presentationml.presentation"
  else if (odt)
    retrn "application/vnd.oasis.opendocument.text"
  else if (xls)
    return "application/vnd.ms-excel"
  else if (xlsx)
    return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
  else if (odp)
    return "application/vnd.oasis.opendocument.presentation"
  else if (ods)
    return "application/vnd.oasis.opendocument.spreadsheet"
  
  
//IMAGE
  else if (jpeg)
    return "image/jpeg"
  else if (png)
    return "image/png"
  else if (apng)
    return "image/apng"
  else if (avif)
    return "image/avif"
  else if (gif)
    return "image/gif"
  else if (svg)
    return "image/svg+xml"
  else if (webp)
    return "image/webp"
  else if (bmp)
    return "image/bmp"
  else if (ico || cur)
    return "image/x-icon
  else if (.tif || .tiff)
    return "image/tiff"

//SOUND
  else if (mp3)
    return "audio/mpeg"
  else if (aac)
    return "audio/aac"
  else if (wav)
    return "audio/wave"

//VIDEO
  else if (flac)
    return "audio/flac"
  else if (mpeg)
    return "audio/mpeg"
  else if (mp4)
    return "video/mp4"
  else if (avi)
    return "video/x-msvideo"
  
//AUDIO-VIDEO
  else if (3gp)
    return "video/3gpp; audio/3gpp" - audio if file does not contain video
  
//ARCHIVES
  else if (bz)
    return "application/x-bzip"
  else if (bz2)
    return "application/x-bzip2"
  else if (gz)
    return "application/gzip"
  else if (zip)
    return "application/zip"
  else if (7z)
    return "application/x-7z-compressed"
  else if (tar)
    return "application/x-tar"

//DEFAULT    h
  else // else if (bin) - any kind of data
    return "application/octet-stream" // default for binary files. It means unknown binary file
}

} //CONTENT TYPE


autoindex on:
  fork
    std::cout << execv(ls -l / -R) << std::endl;



RESPONSE(string path)
std::map<int, std::string> _codeMessage;
split host from direction
split file from direction

//CHECK IF METHOD IS SUPPORTED
if (server._methods.compare(method) == false) {
      std::string buffer;
      int contentlength = read(WHATSERVER?/error/404.html)
      std::stringstream error;

      error << "HTTP/1.1" << 414 (not sure whats the code) << _codeMessage[404] << "\r\n";
      error << "Content-Type: text/html\r\n"
      error << "Content-Length: " << contentlength << "\r\n";
      error << "\r\n"
      error << buffer;

      write(client_fd, error, strlen(buffer));
}

std::vector<std::string>::iterator it = _location.begin()
for (; it != _location.end() ; it++){
  if ((direction == *it)) // ============= IF LOCATION IT REQUEST FOUND
    if (file_open(_l_root/file) == 0) { // ==== IF REQUESTED FILE NOT FOUND
      std::string buffer;
      int contentlength = read(WHATSERVER?/error/404.html)
      std::stringstream error;

      error << "HTTP/1.1" << 404 << _codeMessage[404] << "\r\n";
      error << "Content-Type: text/html\r\n"
      error << "Content-Length: " << contentlength << "\r\n";
      error << "\r\n"
      error << buffer;

      write(client_fd, error, strlen(buffer));
    }
    else if (file_not_specified) ======== IF FILE NOT SPECIFIED check WHAT HAPPEN?
      @same as if, but with index.html

    else ================================= IF REQUESTED FILE FOUND {
      std::string buffer;
      int contentlength = read(WHATSERVER?/content/split_file.extension)
      std::stringstream response;

      response << "HTTP/1.1" << 200 << _codeMessage[200] << "\r\n";
      response << "Content-Type: " << mimetype(file_name) << "\r\n";
      response << "Content-Length: " << contentlength << "\r\n";
      response << "\r\n";
      response << buffer;
    }
}
```

</details>
