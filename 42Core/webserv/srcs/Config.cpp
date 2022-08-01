#include "Config.hpp"
#include "utils.hpp"

static const std::map<int, std::string> genRedirectStatusCodes() {
    std::map<int, std::string> map;

    map[300] = "Multiple Choice";
    map[301] = "Moved Permanently";
    map[302] = "Found";
    map[303] = "See Other";
    map[304] = "Not Modified";
    map[307] = "Temporary Redirect";
    map[308] = "Permanent Redirect";
    return (map);
}

const std::string Config::_server_directives[SERVER_CONTEXT_DIRECTIVES] = {"root", "listen", "server_name", "error_page", "client_max_body_size", "location", "index", "autoindex"};
const std::string Config::_location_directives[LOCATION_CONTEXT_DIRECTIVES] = {"root", "index", "limit_methods", "autoindex", "error_page", "client_max_body_size", "cgi", "cgi-bin", "upload", "redirect"};
const std::string Config::ServerConfig::Methods::_valid_methods[4] = {"GET", "POST", "DELETE", "PUT"};
const int Config::ServerConfig::ErrorCodePage::_allErrorCodes[ALL_ERROR_CODES] = {
	400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418,
	421, 422, 423, 424, 425, 426, 428, 429, 431, 451, 500, 501, 502, 503, 504, 505, 506, 507, 508, 510, 511
};

std::map<int, std::string> Config::ServerConfig::Redirect::_redirect_status_codes = genRedirectStatusCodes();

const char * Config::InvalidConfigurationFileException::what() const throw() {return ("Invalid File, make sure you have permissions, that the file exists and the extension is .conf");}
const char * Config::InvalidDirectiveException::what() const throw() {return ("Directive is invalid");}
const char * Config::WrongSyntaxException::what() const throw() {return ("Wrong Directive Syntax");}
/*
 * @brief Construct a new Config
 *  The constructor takes a string which should be the configuration file.
 *  It will parse all the directives inside the configuration file.
 *  The parsing is done line by line, one directive per line.
 *  Throw exception in case of error (file, directive, syntax)
 *  There are 3 valid contexts
 *    - No context (Outside any scope)
 *    - Server context (Inside the server Scope) [server {...}]
 *    - Location context (Inside the server Scope, inside Location scope) [server {... location {...}}]
 * @param content 
 *  First check if the file is valid and create a file input stream to read from.
 *  Create a line tmp variable and an integer to keep track of the context [0: No Context, 1: Server context, 2:  Location context].
 *  Read the file stream line by line with a while loop.
 *  For every line:
 *   1 - Trim both sides of the string with the SEPARATORS string constant MACRO
 *   2.a - If line is empty or starts with '#' then continue to the next line. (ignore this line)
 *   2.b - Split the string in 2 pieces. [1:directive, 2:directive_content]
 *         The first piece is the line from the begining to the first separator.
 *         The second piece is the line from the first separator to the end. (empty if there is no separator)
 *   3.a  - Check for '}' to close the context. And read next line
 *   3.b  - Based on the context, check for valid directives and create them.
            After creating the directive, setUp the Directive context.  
 */
Config::Config(std::string const & file_str) throw(std::exception) {
    std::ifstream   file;
    std::string     tmp;
    std::string     line;
    std::string     directive;
    std::string     directive_content;
    short int       context(0);

    file.open(file_str.c_str(), std::ios::in);
	if (!file.is_open())
		throw InvalidConfigurationFileException();
    while (std::getline(file, line)) {
        line = strtrim(line);
        if (!line.length() || line[0] == '#')
            continue;
        directive = line.substr(0, line.find_first_of(SEPARATORS));
        directive = strtrim(directive);
        if (line.find_first_of(SEPARATORS) == std::string::npos) {
            directive_content = "";
        } else {
            tmp = line.substr(line.find_first_of(SEPARATORS) + 1);
            directive_content = strtrim(tmp);
        }
        if (directive == "}" && directive_content.empty()) {
            if (--context < 0)
                throw InvalidDirectiveException();
            continue ;
        }
        if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
            std::cout << BLUE << std::left << (context == LOCATION_CONTEXT ? "Location" : "Server") << " context: " << YELLOW << "[" << directive << "]" << " - [" << directive_content << "]";
        switch (context) {
            case 0:
                if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
                    std::cout << ENDC << std::endl;
                if (directive == "server" && directive_content == "{") {
                    ServerConfig s;
                    _servers.push_back(s);
                    context++;
                }
                else
                    throw InvalidDirectiveException();
                break ;
            case SERVER_CONTEXT:
                if (validDirective(directive, _server_directives, SERVER_CONTEXT_DIRECTIVES)) {
                    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
                        std::cout << GREEN << "[OK]" << ENDC << std::endl;
                    ServerConfig::Directive * _directive;
                    _directive = createDirective(directive, directive_content);
                    if (_directive != 0) {
                        _directive->setDirective(_servers.back(), context);
                        if (directive == "location") {
                            _servers.back().setDefaults();
                            context++;
                        }
                        delete (_directive);
                    }
                    else
                        throw WrongSyntaxException();
                }
                else {
                    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
                        std::cout << RED << "[INVALID DIRECTIVE]" << ENDC << std::endl;
                    throw InvalidDirectiveException();
                }
                break;
            case LOCATION_CONTEXT:
                if (validDirective(directive, _location_directives, LOCATION_CONTEXT_DIRECTIVES)) {
                    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
                        std::cout << GREEN << "[OK]" << ENDC << std::endl;
                    ServerConfig::Directive * _directive;
                    _directive = createDirective(directive, directive_content);
                    if (_directive != 0) {
                        _directive->setDirective(_servers.back(), context);
                        delete (_directive);
                    }
                    else
                        throw WrongSyntaxException();
                }
                else {
                    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
                        std::cout << RED << "[INVALID DIRECTIVE]" << ENDC << std::endl;
                    throw InvalidDirectiveException();
                }
                break;
            default:
                std::cout << ENDC << std::endl;
                throw InvalidDirectiveException();
        }
    }
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << WHITE << "Config created" << ENDC << std::endl;
}
Config::~Config() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << RED << "Config destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct a Server Configuration
 *  Represents a server configuration from the configuration file.
 *  Set's all default values on construction.
 *  It can be setted up accordingly with the directives.
 */
Config::ServerConfig::ServerConfig() : _autoindex(false), _max_body_size(0), _ip("127.0.0.1"), _port(80), _root_path("www/") {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << WHITE << "ServerConfig created" << ENDC << std::endl;
}
Config::ServerConfig::~ServerConfig() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << RED << "ServerConfig destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct Directive Abstract Object 
 *  virtual setDirective will take one Directive implementation and set it up.
 *  If directives are repeated the last configuration is overwritten,
 *  except directives that can be repeated.
 * @param id
 *  Directive identifier, all macros listed on the Config.hpp. 
 */
Config::ServerConfig::Directive::Directive(int id): _id(id) {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Directive(" << id << ") created" << ENDC << std::endl;
}
Config::ServerConfig::Directive::~Directive() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << RED << "Directive destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct an Autoindex
 *  This directive will determine whether to show a directory listing from the route/location.
 * @param content 
 *  The only two valid options are: [on, off]. Anything else is wrong.  
 */
Config::ServerConfig::AutoIndex::AutoIndex(const std::string & content) throw (std::exception):
    Directive(AUTOINDEX) {
    if (content == "on")
        _option = true;
    else if (content == "off")
        _option = false;
    else
        throw InvalidDirectiveException();
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "AutoIndex created [" << (_option == true ? "on" : "off") << "]" << ENDC << std::endl;
}
Config::ServerConfig::AutoIndex::~AutoIndex() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "AutoIndex Directive destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct a Client Max Body Size
 *  Set up a maximum amount bytes to accept from a request. 
 *  If the size in a request exceeds the configured value, 
 *  the 413 (Request Entity Too Large) error is returned to the client.
 *  Please be aware that browsers cannot correctly display this error. 
 *  Setting size to 0 disables checking of client request body size.
 * @param content 
 *   Should be a number without any spaces or letters, just a number.
 */
Config::ServerConfig::ClientMaxBodySize::ClientMaxBodySize(const std::string & content) throw (std::exception):
    Directive(CLIENTMAXBODYSIZE), _max_size(0) {
    if (content.find(SEPARATORS) != std::string::npos)
        throw WrongSyntaxException();
    std::stringstream intValue(content);
    intValue >> _max_size;
    if (_max_size < 0)
        throw InvalidDirectiveException();
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "ClientMaxBodySize created [" << _max_size << "]" << ENDC << std::endl;
}
Config::ServerConfig::ClientMaxBodySize::~ClientMaxBodySize() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "ClientMaxBodySize Directive destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct a Cgi object
 *  This constructor takes a string as argument that will be splited to get two arguments: "program extension" and "list of program paths".
 *  Then both will be parsed and pushed to the object. If one of the arguments is missing or one of them is invalid, throws an exception.
 * @param content 
 *  The string after directive "cgi" is found by the parser.
 */
Config::ServerConfig::Cgi::Cgi(const std::string& content) throw (std::exception):
    Directive(CGI) {
        if (content.empty())
            throw WrongSyntaxException();
        _parseCgiContent(_cgi, content);
        if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
            std::cout << WHITE << "Cgi created" << ENDC << std::endl;
}
Config::ServerConfig::Cgi::~Cgi() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Cgi Directive destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct a CgiBin object
 *  This constructor takes a string as argument that will be the path of cgi-bin.
 *  If path is empty, throws an exception.
 * @param content
 *  The string after directive "cgi" is found by the parser.
 */
Config::ServerConfig::CgiBin::CgiBin(const std::string& content) throw (std::exception):
    Directive(CGIBIN), _path(content) {
        if (_path.empty())
            throw WrongSyntaxException();
        if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
            std::cout << WHITE << "CgiBin created!" << ENDC << std::endl;
}
Config::ServerConfig::CgiBin::~CgiBin() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "CgiBin destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct an Error Page
 *  This directive configures a custom error page.
 *  If the same error is in more than one directive only the first one is used.
 * @param content
 *  String with a list of codes followed by the path.
 *  The path will be used to search for the code.
 *  The file will be constructed with the error code and .html extension.
 */
Config::ServerConfig::ErrorCodePage::ErrorCodePage(const std::string & content) throw(std::exception):
    Directive(ERRORPAGE) {
    size_t found = content.find_last_of(SEPARATORS);
    if (content.empty() || found == std::string::npos)
        throw WrongSyntaxException();
    if (!loadErrorCodes(content.substr(0, found)))
        throw InvalidDirectiveException();
    _error_path = content.substr(found + 1);
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "ErrorCode created [" << _error_path << "]" << std::endl;
}
Config::ServerConfig::ErrorCodePage::~ErrorCodePage() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "ErrorPage Directive destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct an Index
 *  The index is used to set up a default page to return.
 *  If the resource is founded it ignores the autoindex if it was on.
 * @param content
 *  A string with a list of files to look for in the resource requested.
 */
Config::ServerConfig::Index::Index(const std::string & content) throw (std::exception):
    Directive(INDEX) {
    if (content.empty())
        throw WrongSyntaxException();
    std::string tmp(content);
    char *token = strtok(const_cast<char*>(tmp.c_str()), SEPARATORS);
    while (token != NULL) {
        _indexes.push_back(token);
        token = strtok(NULL, SEPARATORS);
    }
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Index created [" << _indexes.size() << " indexes]" << ENDC << std::endl;
}
Config::ServerConfig::Index::~Index() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Index Directive destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct a Method
 *  A list of the accepted methods for a specific resource.
 * @param content
 *  A string with a list of valid methods in CAPITAL letters.
 */
Config::ServerConfig::Methods::Methods(const std::string & content) throw (std::exception):
	Directive(LIMITMETHODS) {
	char * str = std::strtok(const_cast<char*>(content.c_str()), " ");
	while (str) {
		if (!_validMethod(std::string(str)))
            throw InvalidDirectiveException();
		std::vector<std::string>::iterator last = _methods.end();
		std::vector<std::string>::iterator tmp = _methods.begin();
		for (; tmp != last; ++tmp)
			if (*tmp == str)
                throw InvalidDirectiveException();
		_methods.push_back(std::string(str));
		str = std::strtok(NULL, " ");	
	}
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << WHITE << "Limit Methods created [" << _methods.size() << " methods]" << ENDC << std::endl;
}
Config::ServerConfig::Methods::~Methods() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Methods destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct a Listen
 *  Configures the ip and port which the server looks for.
 * @param content
 *  String with either a number which indicates the port,
 *  or a valid ip [*.*.*.*], or both separated by a colon (':').
 */
Config::ServerConfig::Listen::Listen(const std::string & content) throw (std::exception):
    Directive(LISTEN), _ip("127.0.0.1"), _port(80) {
    if (content.empty() || content.find_first_of(SEPARATORS) != std::string::npos)
        throw WrongSyntaxException();
    std::string temp;
    std::stringstream stoi_converter;
    if (content.find(':') != std::string::npos) {
        std::string ip_str = content.substr(0, content.find(':'));
        std::string port_str = content.substr(content.find(':') + 1);
        if (!isIpValid(ip_str) || port_str.empty())
                throw InvalidDirectiveException();
        else if (ip_str == "*")
            _ip = "0.0.0.0";
        else if (ip_str == "localhost")
            _ip = "127.0.0.1";
        else
            _ip = ip_str;
        stoi_converter << port_str;
        stoi_converter >> _port;
    }
    else if (isIpValid(content)) {
        temp = content;
        if (!temp.compare("localhost"))
            _ip = "127.0.0.1";
        else if (!temp.compare("*"))
            _ip = "0.0.0.0";
        else
            _ip = temp;
    }
    else {
        stoi_converter << content;
        stoi_converter >> _port;
        if (_port > PORT_MAX || _port <= PORT_MIN)
            throw InvalidDirectiveException();
    }
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Listen created [" << _ip << ":" << _port << "]" << ENDC << std::endl;
}
Config::ServerConfig::Listen::~Listen() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Listen destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct a Location
 *  This directive enanbles a new route to the ServerConfig.
 * @param content
 *  Takes a string terminated by '{', the previous content of the string is the route of the location.
 */
Config::ServerConfig::Location::Location(std::string const & content) throw (std::exception):
    Directive(LOCATION), _target(content), _max_body_size(0), _redirect_status(0), _autoindex(false) {
    if (content.empty() || content[content.length() - 1] != '{')
        throw WrongSyntaxException();
    _target = _target.substr(0, content.length() - 1);
    _target = strtrim(_target);
    if (_target.empty() || _target.find(SEPARATORS) != std::string::npos)
        throw WrongSyntaxException();
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Location created [" << _target << "]" << ENDC << std::endl;
}
Config::ServerConfig::Location::~Location() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Location Directive destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct a Server Name
 * @param content
 *
 */
Config::ServerConfig::Redirect::Redirect(const std::string & content) throw (std::exception):
    Directive(REDIRECT) {
    std::string tmp;
    std::stringstream ss;

    ss << content.substr(0, 3);
    ss >> _status_code;
    if (_redirect_status_codes.find(_status_code) == _redirect_status_codes.end())
        throw WrongSyntaxException();
    tmp = content.substr(3);
    tmp = strtrim(tmp);
    if (tmp.empty() || tmp.find(SEPARATORS) != std::string::npos)
        throw WrongSyntaxException();
    _redirect_uri = tmp;
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Redirect created [" << _redirect_uri<< "]" << ENDC << std::endl;
}
Config::ServerConfig::Redirect::~Redirect() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Redirect Directive destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct a Root
 *  The root configures the path where the server will look for the resources.
 * @param content
 *  String without separators indicating a path where to look. Must finish with '/'.
 */
Config::ServerConfig::Root::Root(const std::string & content) throw (std::exception):
	Directive(ROOT), _path(content) {
	if (content.empty() || content.find(SEPARATORS) != std::string::npos || content[content.length() - 1] != '/')
		throw WrongSyntaxException();
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << WHITE << "Root created [" << _path << "]" << ENDC << std::endl;
}
Config::ServerConfig::Root::~Root() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << RED << "Root destroyed" << ENDC << std::endl;
}
/*
 * @brief Construct a Server Name
 *  The server name will be connfigured to set up the response Server-Name Header.
 *  The server name can be determined in the following request processing stage:
 *  after processing the request line, after processing the Host header field.
 *  If the server name was not determined after processing the request line or from the Host header field,
 *  will use the first server configuration which matches the Host.
 * @param content
 *  String with a list of server names to use for the Server-Name header.
 */
Config::ServerConfig::ServerName::ServerName(const std::string & content) throw (std::exception):
    Directive(SERVERNAME) {
    std::string tmp(content);
    char *token = strtok(const_cast<char*>(tmp.c_str()), SEPARATORS);
    while (token != NULL) {
        _server_names.push_back(token);
        token = strtok(NULL, SEPARATORS);
    }
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "ServerName created [" << _server_names.size() << " names]" << ENDC << std::endl;
}
Config::ServerConfig::ServerName::~ServerName() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "ServerName Directive destroyed" << ENDC << std::endl;
}
Config::ServerConfig::Upload::Upload(const std::string & content) throw (std::exception):
    Directive(ROOT), _upload_path(content) {
    if (content.empty() || content.find(SEPARATORS) != std::string::npos || content[content.length() - 1] != '/')
        throw WrongSyntaxException();
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
       std::cout << WHITE << "Upload created [" << _upload_path << "]" << ENDC << std::endl;
}
Config::ServerConfig::Upload::~Upload() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
       std::cout << RED << "Upload destroyed" << ENDC << std::endl;
}

bool Config::validDirective(const std::string & str, const std::string * list, int len) const {
	int i(0);
	while (i < len)
		if (list[i++] == str)
			return (true);
	return (false);
}

Config::ServerConfig::Directive * Config::createDirective(std::string const & name, std::string const & content) throw(std::exception) {
    if (name == "listen")
        return (new ServerConfig::Listen(content));
    else if (name == "error_page")
        return (new ServerConfig::ErrorCodePage(content));
    else if (name == "root")
        return (new ServerConfig::Root(content));
    else if (name == "limit_methods")
        return (new ServerConfig::Methods(content));
    else if (name == "location")
        return (new ServerConfig::Location(content));
    else if (name == "server_name")
        return (new ServerConfig::ServerName(content));
    else if (name == "client_max_body_size")
        return (new ServerConfig::ClientMaxBodySize(content));
    else if (name == "index")
        return (new ServerConfig::Index(content));
    else if (name == "autoindex")
        return (new ServerConfig::AutoIndex(content));
    else if (name == "cgi")
        return (new ServerConfig::Cgi(content));
    else if (name == "cgi-bin")
        return (new ServerConfig::CgiBin(content));
    else if (name == "redirect")
        return (new ServerConfig::Redirect(content));
    else if (name == "upload")
        return (new ServerConfig::Upload(content));
    return (0);
}

int Config::ServerConfig::getPort() const {return (_port);}

void Config::ServerConfig::setDefaults() {
    Config::ServerConfig::Location & loc = _locations.back();

    loc._autoindex = _autoindex;
    loc._max_body_size = _max_body_size;
    loc._root_path = _root_path;
    loc._indexes = _indexes;
    //loc._location_errors_map = _server_errors_map;
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << "Setted Defaults" << std::endl;
}

std::string const & Config::ServerConfig::getIp() const {return (_ip);}

Config::ServerConfig::Location * Config::ServerConfig::findLocation(std::string const & target) const {
    size_t matches;
    std::vector<Location>::iterator l_it;
    std::vector<Location>::iterator tmp_it;
    std::vector<Location> tmp_locs(_locations);

    for (l_it = tmp_locs.begin(), matches = 0; l_it != tmp_locs.end(); ++l_it) {
        int tmp = target.compare(0, l_it->_target.length(), l_it->_target);
        if (tmp == 0 && l_it->_target.length() > matches) {
            matches = l_it->_target.length();
            tmp_it = l_it;
        }
    }
    if (matches == 0)
        return (0);
    return (new Location(*tmp_it));
}

bool Config::ServerConfig::checkMaxBody(int len) const {
    if (len > _max_body_size && _max_body_size > 0)
        return (false);
    return (true);
}

bool Config::ServerConfig::Location::checkMaxBody(int len) const {
    if (len > _max_body_size && _max_body_size > 0)
        return (false);
    return (true);
}

int Config::ServerConfig::Directive::getId() const {return (_id);}

bool Config::ServerConfig::ErrorCodePage::loadErrorCodes(const std::string &content) {
    std::stringstream stoi_converter;
    int converted_number;
    int loop_counter;

    loop_counter = 0;
    char *token = std::strtok(const_cast<char*>(content.c_str()), SEPARATORS);
    while (token != 0) {
        loop_counter++;
        stoi_converter.clear();
        stoi_converter << token;
        stoi_converter >> converted_number;
        for (int i = 0; i < 40; i++) {
            if (converted_number == _allErrorCodes[i]) {
                _error_codes.push_back(converted_number);
                loop_counter--;
            }
        }
        token = std::strtok(NULL, SEPARATORS);
    }
    if (!loop_counter)
        return true;
    return false;
}

void Config::ServerConfig::Cgi::_parseCgiContent(std::vector<std::string>& _target, const std::string& content) {
    char * token = std::strtok(const_cast<char*>(content.c_str()), " ");
    while (token) {
        _target.push_back(std::string(token));
        token = std::strtok(NULL, SEPARATORS);
    }
    if (_target.size() != 2)
        throw InvalidDirectiveException();
}

bool Config::ServerConfig::Methods::_validMethod(const std::string & method) {
    for(size_t i = 0; i < 4; i++)
        if (method == _valid_methods[i])
            return true;
    return false;
}

bool Config::ServerConfig::Location::findMethod(const std::string& method) const {
    std::vector<std::string>::const_iterator it = _methods.begin();
    for(; it != _methods.end() ; it++)
        if (method == *it)
            return true;
    return false;
}

bool Config::ServerConfig::Listen::isIpValid(const std::string &ip) {
    int counter, part1, rest;
    char ch;

    counter = 0;
    if (!ip.compare("0.0.0.0") || !ip.compare("localhost") || !ip.compare("*"))
        return true;
    for (u_int16_t i = 0; i <= ip.length() ; i++)
        if (ip[i] == '.')
            counter++;
    if (counter != 3)
        return false;
    std::stringstream stoi_converter(ip);
    stoi_converter >> part1 >> ch;
    if (part1 < 1 || part1 > 255 || ch != '.')
        return false;
    while (stoi_converter.rdbuf()->in_avail() != 0) {
        stoi_converter >> rest >> ch;
        if (rest < 0 || rest > 255)
            return false;
        if (ch != '.')
            return false;
    }
    return true;
}

void Config::ServerConfig::AutoIndex::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == SERVER_CONTEXT)
        serv_conf._autoindex = _option;
    else if (context == LOCATION_CONTEXT)
        serv_conf._locations.back()._autoindex = _option;
}

void Config::ServerConfig::Cgi::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == LOCATION_CONTEXT) {
        serv_conf._locations.back()._cgi_map[_cgi[0]].push_back(_cgi[1]);
    }
}

void Config::ServerConfig::CgiBin::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == LOCATION_CONTEXT)
        serv_conf._locations.back()._cgi_bin = _path;
}

void Config::ServerConfig::ClientMaxBodySize::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == SERVER_CONTEXT)
        serv_conf._max_body_size = _max_size;
    else if (context == LOCATION_CONTEXT)
        serv_conf._locations.back()._max_body_size = _max_size;
}

void Config::ServerConfig::ErrorCodePage::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == SERVER_CONTEXT)
        serv_conf._server_errors_map[_error_path] = _error_codes;
    else if (context == LOCATION_CONTEXT) {
        std::vector<int>::const_iterator it;
        for (it = _error_codes.begin(); it != _error_codes.end(); ++it)
            serv_conf._locations.back()._location_errors_map[_error_path].push_back(*it);
    }
}

void Config::ServerConfig::Index::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == SERVER_CONTEXT)
        serv_conf._indexes = _indexes;
    else if (context == LOCATION_CONTEXT)
        serv_conf._locations.back()._indexes = _indexes;
}

void Config::ServerConfig::Methods::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == LOCATION_CONTEXT)
        serv_conf._locations.back()._methods = _methods;
}

void Config::ServerConfig::Listen::setDirective(ServerConfig & serv_conf, int context) const {
    if (context != SERVER_CONTEXT)
        return ;
    serv_conf._port = _port;
    serv_conf._ip = _ip;
}

void Config::ServerConfig::Location::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == SERVER_CONTEXT) {
        serv_conf._locations.push_back(*this);
    }
}

void Config::ServerConfig::Redirect::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == LOCATION_CONTEXT) {
        serv_conf._locations.back()._redirect_uri = _redirect_uri;
        serv_conf._locations.back()._redirect_status = _status_code;
    }
}

void Config::ServerConfig::Root::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == SERVER_CONTEXT)
        serv_conf._root_path = _path;
    else if (context == LOCATION_CONTEXT)
        serv_conf._locations.back()._root_path = _path;
}

void Config::ServerConfig::ServerName::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == SERVER_CONTEXT)
        serv_conf._names = _server_names;
}

void Config::ServerConfig::Upload::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == LOCATION_CONTEXT)
        serv_conf._locations.back()._upload_path = _upload_path;
}

// std::string const &Config::ServerConfig::Location::getLocation() const{return _location;}
// bool &Config::ServerConfig::Location::l_getAutoindex() {return _autoindex;}
// std::string Config::ServerConfig::Location::l_getRoot() const {return _l_root;}
// std::vector<int> &Config::ServerConfig::Location::l_getErrorCodes() {return _l_errorCodes;}
// std::string &Config::ServerConfig::Location::l_getErrorPath() {return _l_errorPath;}
// std::vector<std::string> &Config::ServerConfig::Location::l_getMethods() {return _l_methods;}

std::ostream& operator<<(std::ostream & s, const Config & param) {
	s << "Some configuration Text";
	(void)param;
	return (s);
}

std::ostream& operator<<(std::ostream & s, const Config::ServerConfig & param) {
    s << "|**********************************************|"<< std::endl;
    s << "| Server " << param.getIp() << ":" << param.getPort() << std::endl;
    s << "| - root path " << param._root_path << std::endl;
    s << "| - errors map [" << std::endl;
    std::map<std::string, std::vector<int> >::const_iterator it_m = param._server_errors_map.begin();
    std::vector<int>::const_iterator it_v;
	for (; it_m != param._server_errors_map.end() ; it_m++) {
        s << "|     " << it_m->first << ": [";
		for (it_v = (it_m->second).begin() ; it_v != it_m->second.end() ; it_v++)
            s << *it_v << " ";
        s << "]" << std::endl;
    }
    std::cout << "|   ]" << std::endl;
    s << "| - server names [";
    std::vector<std::string> s_list(param._names);
    std::vector<std::string>::iterator s_it(s_list.begin());
    for (; s_it != s_list.end() ; ++s_it)
        s << *s_it << " ";
    s << "]" << std::endl;
    s << "| - autoindex " << (param._autoindex == true ? "on" : "off") << std::endl;
    s << "| - max_body_size " << param._max_body_size << std::endl;
    s << "| - [" << param._locations.size() << "] locations " << std::endl;
    std::vector<Config::ServerConfig::Location> list = param._locations;
    std::vector<Config::ServerConfig::Location>::iterator it(list.begin());
    for (; it != list.end() ; ++it) {
        s << "|  + target " << it->_target << std::endl;
        s << "|     root path " << it->_root_path << std::endl;
        s << "|     errors map [" << std::endl;
        std::map<std::string, std::vector<int> >::const_iterator it_m = it->_location_errors_map.begin();
        std::vector<int>::const_iterator it_v;
        for (; it_m !=  it->_location_errors_map.end() ; it_m++) {
            s << "|       " << it_m->first << ": [";
            for (it_v = (it_m->second).begin() ; it_v != it_m->second.end() ; it_v++)
                s << *it_v << " ";
            s << "]" << std::endl;
        }
        std::cout << "|     ]" << std::endl;
        s << "|     autoindex " << (it->_autoindex == true ? "on" : "off") << std::endl;
        s << "|     max_body_size " << it->_max_body_size << std::endl;
        s << "|     Index [";
        std::vector<std::string>::iterator i_it(it->_indexes.begin());
        for (; i_it != it->_indexes.end() ; ++i_it)
            s << *i_it << " ";
        s << "]" << std::endl;
        s << "|     Methods [";
        std::vector<std::string>::iterator m_it(it->_methods.begin());
        for (; m_it != it->_methods.end() ; ++m_it)
            s << *m_it << " ";
        s << "]" << std::endl;
        s << "|     cgi_bin " << it->_cgi_bin << std::endl;
        s << "|     CGI [" << std::endl;
        std::map<std::string, std::vector<std::string> >::iterator cgi_it;
        for (cgi_it = it->_cgi_map.begin(); cgi_it != it->_cgi_map.end() ; ++cgi_it) {
            s << "|      " << cgi_it->first << ": ";
            std::vector<std::string>::iterator cgi_bin_it;
            for(cgi_bin_it = cgi_it->second.begin(); cgi_bin_it != cgi_it->second.end(); ++cgi_bin_it) 
                s << *cgi_bin_it << " ";
            std::cout << std::endl;
        }
        s << "|     ]" << std::endl;
    }
    s << "|**********************************************|"<< std::endl;
    return (s);
}
