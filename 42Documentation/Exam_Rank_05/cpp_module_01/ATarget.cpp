#include "ATarget.hpp"

ATarget::ATarget() {};
ATarget::ATarget(const std::string & type): _type(type) {};
ATarget::ATarget(const ATarget & a): _type(a._type) {};
ATarget & ATarget::operator = (const ATarget & a) {_type = a._type;return (*this);};
ATarget::~ATarget() {};
const std::string & ATarget::getType() const {return (_type);};
void ATarget::getHitBySpell(const ASpell &s) {std::cout << _type << " has been " << s.getEffects() << "!" << std::endl;}
