#include "utils.hpp"


std::string & strtrim(std::string & str, const char * separators) {
    str.erase(str.find_last_not_of(separators) + 1);
    str.erase(0, str.find_first_not_of(separators));
    return (str);
}

static uint64_t    gettimeofday_ms(void) {
    static struct timeval   tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

uint64_t    timestamp_in_ms(void) {
    static uint64_t start = 0;

    if (start == 0)
        start = gettimeofday_ms();
    return (gettimeofday_ms() - start);
}

int ft_tolower (int c) {
    if (c >= 'A' && c <= 'Z')
        return (c + 32);
    return (c);
}

int ft_toupper (int c) {
    if (c >= 'a' && c <= 'z')
        return (c - 32);
    return (c);
}

char * get_local_time (void) {
  time_t rawtime;
  struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);
  return (asctime(timeinfo));
}

const std::string & readFileStream(std::ifstream const & file, std::string & buffer) {
  if(file.is_open()) {
    std::ostringstream ss;
    ss << file.rdbuf();
    if (ss.str().empty())
      buffer = "This resource is empty...";
    else
      buffer = ss.str();
  }
  return (buffer);
}

const std::string & readFileString(std::string const & file_name, std::string & buffer) {
  std::ifstream file_stream;

  file_stream.open(file_name.c_str());
  readFileStream(file_stream, buffer);
  file_stream.close();
  return (buffer);
}

bool isDirectory(const std::string & path) {
  struct stat s;

  if (lstat(path.c_str(), &s) == 0)
    if (S_ISDIR(s.st_mode))
      return (true);
  return (false);
}
