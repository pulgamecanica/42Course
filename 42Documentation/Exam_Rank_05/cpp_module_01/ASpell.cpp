#include "ASpell.hpp"

ASpell::ASpell() {};
ASpell::ASpell(const std::string & name, const std::string & effects): _name(name), _effects(effects) {};
ASpell::ASpell(const ASpell & a): _name(a._name), _effects(a._effects) {};
ASpell & ASpell::operator = (const ASpell & a) {_name = a._name;_effects = a._effects;return (*this);};
ASpell::~ASpell() {};
const std::string & ASpell::getName() const {return (_name);};
const std::string & ASpell::getEffects() const {return (_effects);};

