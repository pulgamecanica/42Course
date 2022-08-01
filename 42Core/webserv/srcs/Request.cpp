#include "Request.hpp"

// Can we concider that the whole request is sent at once?
Request::Request(std::string const & request, Config::ServerConfig const & sc):
	_error_code(0), _server_config(sc), _loc(NULL)
	{
	std::stringstream ss(request);
	std::string line;
	
	std::getline(ss, line);
	line = strtrim(line, " \r\t");
	_method = line.substr(0, line.find_first_of(" \r\t"));
	_uri_target = line.substr(_method.length(), line.find_last_of(" \r\t") - _method.length());
	_http_version = line.substr(_method.length() + _uri_target.length());
	std::transform(_method.begin(), _method.end(), _method.begin(), ::ft_toupper);
	std::transform(_http_version.begin(), _http_version.end(), _http_version.begin(), ::ft_tolower);
	_uri_target = strtrim(_uri_target, " \r\t");
	_http_version = strtrim(_http_version, " \r\t");
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << BLUE << "Method => [" << _method << "], Target => [" << _uri_target << "], HTTP Version => [" << _http_version << "]" << ENDC << std::endl;
	while (std::getline(ss, line) && line != "\r\n") {
		if (line.find(':') != std::string::npos) {
			std::string name(line.substr(0, line.find(':')));
			std::string content(line.substr(line.find(':')));
    		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			_headers[name] = strtrim(content, ": \t");
    		if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << BLUE << name << " => " << _headers[name] << ENDC << std::endl;
		} else {
			break;
		}
	}
	if (_headers.find("content-length") != _headers.end()) {
		int valread(0);
		std::stringstream num_ss(_headers["content-length"]);
		num_ss >> valread;
		while (std::getline(ss, line)) {
			_content += line;
		}
		_content = _content.substr(0, valread);
		if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
			std::cout << WHITE << "Content [" << valread << "]" << std::endl << PURPLE << _content << ENDC << std::endl;
	}
	if (_headers.find("coockies") != _headers.end()) {
		; // TODO
	}
	if (_uri_target.length() > 1024)
		_error_code = 414;
	else {
		size_t q_start = _uri_target.find("?");
		size_t q_end = _uri_target.find("#"); // Concider the fragment later....
		if (q_start != std::string::npos) {
			_query = _uri_target.substr(q_start, q_end - q_start);
			_uri_target = _uri_target.substr(0, q_start);
			if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << WHITE << "Query detected [" << _query << "]" << ENDC << std::endl;
		}
		_loc = _server_config.findLocation(_uri_target);
		if (!_loc) {
			_error_code = 404;
			std::cout << RED << "Wrong target [" << _uri_target << "], couldn't find any configuration" << ENDC << std::endl;
		}
		else {
			_final_path = _loc->_root_path + _uri_target.substr(_loc->_target.length());
			if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << WHITE << "Final Target Path " << CYAN << "[" << _final_path << "]" << ENDC << std::endl;
			else{
				std::string tmp("Started " + _method + " \"" + _uri_target + "\" ");
				std::cout << WHITE; std::cout.width(35);
				std::cout << std::left << tmp; std::cout << WHITE << "=>" << CYAN << " Target Path [" << _final_path << "] "; 
			}
			if (!_loc->findMethod(_method))
				_error_code = 405;
			else if (_http_version == "http/1.0")
				_error_code = 505;
			else if (_http_version != "http/1.1")
				_error_code = 400;
		}
	}
	//it's a bit stuped to process all the request and in the end check the lenght.... but why not... XD
	if (!_server_config.checkMaxBody(request.length()) &&
		(_loc && !_loc->checkMaxBody(request.length())))
		_error_code = 413;
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Request Created " << _error_code << ENDC << std::endl;
}
Request::~Request() {
	if (_loc)
		delete (_loc);
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << RED << "Request destroyed" << std::endl;
}

int Request::getErrorCode() const {
	return (_error_code);
}

bool Request::isTargetDir() const {
	if (_loc)
		if (_loc->_target == _uri_target || (*(_uri_target.end() - 1)) == '/' )
			return (true);
	return (false);
}

bool Request::isTargetRedirect() const {
	if (_loc)
		if (_loc->_target == _uri_target && _loc->_redirect_status != 0 && !_loc->_redirect_uri.empty())
			return (true);
	return (false);
}

bool Request::isTargetCGI() const {
	size_t pos(_uri_target.find('.'));
	
	if (_loc && !isTargetDir() && pos != std::string::npos) {
		std::string ext(_uri_target.substr(pos + 1));
		if (!(_loc->_cgi_bin.empty()) && (_loc->_cgi_map.find(ext) != _loc->_cgi_map.end()))
			return (true);
	}
	return (false);
}

std::string const & Request::getContent() const {
	return (_content);
}

std::string const & Request::getQuery() const {
	return (_query);
}

std::string const & Request::getMethod() const {
	return (_method);
}

std::string const & Request::getUriTarget() const {
	return (_uri_target);
}

std::string const & Request::getFinalPath() const {
	return (_final_path);
}

const std::string Request::getIndex() const {
	std::string									location;
	std::vector<std::string>::iterator			i_it;
	std::vector<std::string>::const_iterator	c_it;

	if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Try Indexes: ";
	for(i_it = _loc->_indexes.begin(); i_it != _loc->_indexes.end(); ++i_it) {
		location = getFinalPath() + *i_it;
		if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
			std::cout << *i_it << " ";
		if (isFile(location))
			return (location);
	}
	for(c_it = _server_config._indexes.begin(); c_it != _server_config._indexes.end(); ++c_it) {
		location = getFinalPath() + *c_it;
		if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
			std::cout << *c_it << " ";
		if (isFile(location))
			return (location);
	}
	return ("");
}

const std::string Request::getCGIBinPath() const {
	if (_loc)
		if (!_loc->_cgi_bin.empty())
			return (_loc->_cgi_bin);
	return("");
}

const std::string Request::getCGIFile() const {
	std::vector<std::string>::iterator	v_it;
	size_t pos(_uri_target.find_last_of('.'));

	if (_loc && isTargetCGI() && !_loc->_cgi_map.empty() && pos != std::string::npos) {
		std::string ext(_uri_target.substr(pos + 1));
		std::string file_no_ext(_uri_target.substr(0, pos));
		if (_loc->_cgi_map.find(ext) != _loc->_cgi_map.end()) {
			for (v_it = _loc->_cgi_map[ext].begin(); v_it != _loc->_cgi_map[ext].end(); v_it++) {
				size_t v_pos(v_it->find_last_of('.'));
				std::string v_it_no_ext(*v_it);
				if (pos != std::string::npos)
					v_it_no_ext = v_it_no_ext.substr(0, v_pos);
				if (v_it_no_ext.find(file_no_ext) != std::string::npos 
					|| file_no_ext.find(v_it_no_ext) != std::string::npos)
					return (*v_it);
			}
			return (_loc->_cgi_map.find(ext)->second.front());
		}
	}
	return ("");
} 

const std::string Request::getAcceptedEncoding() const {
	if (_headers.find("accept-encoding") != _headers.end())
		return (_headers.find("accept-encoding")->second);
	return ("");
}

const std::string Request::getUserAgent() const {
	if (_headers.find("user-agent") != _headers.end())
		return(_headers.find("user-agent")->second);
	return ("");
}

const std::string Request::getContentType() const {
	if (_headers.find("content-type") != _headers.end())
		return(_headers.find("content-type")->second);
	return ("");
}

const std::string Request::getCookies() const {
	if (_headers.find("cookie") != _headers.end())
		return(_headers.find("cookie")->second);
	return ("");
}

std::ostream &	operator<<(std::ostream & ss, const Request & req) {
	ss << "Status: " << req.getErrorCode() << ", Resource: " << req.getFinalPath() << (req.getCGIFile().empty() ? "" : (", CGI File Target: " + req.getCGIFile())); 
	return (ss);
}