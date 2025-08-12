#ifndef FT_EXCEPTION_HPP
#define FT_EXCEPTION_HPP

namespace ft {

class exception {
public:
    exception() : _msg("ft::exception") {}
    explicit exception(const char* msg) : _msg(msg) {}
    virtual ~exception() throw() {}

    virtual const char* what() const throw() {
        return _msg;
    }

protected:
    const char* _msg;
};

class out_of_range : public exception {
public:
    explicit out_of_range(const char* msg) : exception(msg) {}
    virtual ~out_of_range() throw() {}
};

} // namespace ft

#endif // FT_EXCEPTION_HPP
