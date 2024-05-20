//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __FORM_HPP__
# define __FORM_HPP__

#include <iostream>
#include <vector>

#include "ex02.inc"

enum class FormType
{
  CourseFinished,
  NeedMoreClassRoom,
  NeedCourseCreation,
  SubscriptionToCourse
};

class Form {
  public:
  Form(FormType type): type_(type), signed_(false) {
    if (DEBUG)
      std::cout << GREEN << "[NEW]" << YELLOW << "[Form]\t" << ENDC << std::endl;
  }

  virtual ~Form() {
    if (DEBUG)
      std::cout << RED << "[DESTROY]" << YELLOW << "[Form]\t" << ENDC << std::endl;
  }

  Form(const Form&) = default;
  
  Form& operator= (const Form&) = default;
  
  void Signature(const std::string& signature) {
    if (Errors() == 0) {
      signature_ = signature;
      signed_ = true;
    } else {
      std::cout << "Warning, errors on the form" << std::endl;
    }
  }

  bool IsValid() {
      return Errors() == 0;
  }

  bool IsSigned() {
    return signed_;
  }

  virtual void Execute() = 0;

  virtual unsigned Errors() = 0;

  virtual const std::vector<std::string> ErrorsList() = 0;

  friend std::ostream& operator<<(std::ostream& s, const Form& f) {
    (void)f;
    s << BLUE << "[Form]" << ENDC;
    return (s);
  }

  private:
  FormType type_;
  std::string signature_;
  bool signed_;
};
#endif
