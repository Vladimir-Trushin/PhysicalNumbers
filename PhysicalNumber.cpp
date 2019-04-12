#include <iostream>
#include <stdexcept>
#include <regex>
#include <cctype>

#include "Unit.h"
#include "PhysicalNumber.h"

namespace ariel
{
////////
// The static data members
double PhysicalNumber::_epsilon = EPSILON;

////--------------------------------------------------------------------------------------------
// Print "digit[unit]"
std::ostream &operator<<(std::ostream &os, const PhysicalNumber &ob)
{
    os << ob._num << ob.print_unit();
    return os;
}

////--------------------------------------------------------------------------------------------
// Assignment unit
std::istream &operator>>(std::istream &os, PhysicalNumber &ob)
{
    std::string str;
    os >> str;

    // The regular expression which checks the right syntax --digit[unit]--
    std::regex match_regex("^(((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?(\\[([[:alpha:]]+)\\]))$", std::regex_constants::ECMAScript);

    // The regular expression which match the unit --[unit]--
    std::regex digit_regex("(((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?)", std::regex_constants::ECMAScript);

    // The regular expression which match the digit --digit-- ( (+-)xx || (+-)xx. || (+-)xx.xx )
    std::regex unit_regex("(\\[([[:alpha:]]+)\\])", std::regex_constants::ECMAScript);

    std::smatch s_match;         // The match string
    std::string unit_str_l = ""; // unit lowercase
    std::string unit_str;
    std::string digit_str;

    if (!std::regex_match(str, s_match, match_regex)) // checks the right syntax
        throw std::logic_error("syntax error, not match --digit[unit]--");

    if (!std::regex_search(str, s_match, unit_regex)) // match the unit
        throw std::logic_error("syntax error, not match --unit--");
    unit_str = s_match[0];

    if (!std::regex_search(str, s_match, digit_regex)) // match the digit
        throw std::logic_error("syntax error, not match --digit--");
    digit_str = s_match[0];

    for (size_t i = 0; i < unit_str.size(); i++) // convert to lowercase
        unit_str_l += tolower(unit_str[i]);

    bool not_ok = true;
    for (size_t i = 0; i < LEN; i++) // check if unit exist ane assignment
    {
        if (units[i] == unit_str_l)
        {
            ob._unit = ob.int_to_unit(i);
            not_ok = false;
            break;
        }
    }

    if (not_ok) // unit not exist
        throw std::logic_error("syntax error, not match one of the units");

    ob._num = stod(digit_str, nullptr); // convert string to double
                                        //throw invalid_argument

    return os;
}

////--------------------------------------------------------------------------------------------
// Check if units match, absolute difference value
// one unit to another unit in the same field can't be bigger 2
int PhysicalNumber::check(const PhysicalNumber &ob) const
{
    int temp = (size_t)ob._unit - (size_t)this->_unit;

    if ((temp < -2) || (2 < temp))
        throw std::logic_error("Units do not match - " + ob.print_unit() + " cannot be converted to " + this->print_unit());

    return temp;
}

////--------------------------------------------------------------------------------------------
// Convert unit to integer
size_t PhysicalNumber::unit_to_int() const
{
    switch (this->_unit)
    {
    case Unit::CM:
    case Unit::M:
    case Unit::KM:
        return (size_t)this->_unit - 1;
    case Unit::SEC:
    case Unit::MIN:
    case Unit::HOUR:
        return (size_t)this->_unit - 7;
    case Unit::G:
    case Unit::KG:
    case Unit::TON:
        return (size_t)this->_unit - 14;
    }
}

////--------------------------------------------------------------------------------------------
// Convert integer to units
Unit PhysicalNumber::int_to_unit(const size_t n) const
{
    Unit ret;

    switch (n)
    {
    case 0:
    case 1:
    case 2:
        ret = (Unit)(n + 1);
        break;
    case 3:
    case 4:
    case 5:
        ret = (Unit)(n + 7);
        break;
    case 6:
    case 7:
    case 8:
        ret = (Unit)(n + 14);
        break;
    }

    return ret;
}

////--------------------------------------------------------------------------------------------
// Convert one unit to another unit in the same field
double PhysicalNumber::convert(int shift) const
{
    double ret = 0;

    switch (this->_unit)
    {
    case Unit::CM: //------------------------------------CM
        switch (shift)
        {
        case -2:
            ret = (this->_num / 100000); // cm to km
            break;
        case -1:
            ret = (this->_num / 100); // cm to m
            break;
        case 0:
            ret = this->_num;
            break;
        }
        break;
    case Unit::M: //-------------------------------------M
        switch (shift)
        {
        case -1:
            ret = (this->_num / 1000); // m to km
            break;
        case 1:
            ret = (this->_num * 100); // m to cm
            break;
        case 0:
            ret = this->_num;
            break;
        }
        break;
    case Unit::KM: //------------------------------------KM
        switch (shift)
        {
        case 1:
            ret = (this->_num * 1000); // km to m
            break;
        case 2:
            ret = (this->_num * 100000); // km to cm
            break;
        case 0:
            ret = this->_num;
            break;
        }
        break;
    case Unit::SEC: //-----------------------------------SEC
        switch (shift)
        {
        case -2:
            ret = (this->_num / 3600); // sec to hour
            break;
        case -1:
            ret = (this->_num / 60); // sec to min
            break;
        case 0:
            ret = this->_num;
            break;
        }
        break;
    case Unit::MIN: //-----------------------------------MIN
        switch (shift)
        {
        case -1:
            ret = (this->_num / 60); // min to hour
            break;
        case 1:
            ret = (this->_num * 60); // min to sec
            break;
        case 0:
            ret = this->_num;
            break;
        }
        break;
    case Unit::HOUR: //-----------------------------------hour
        switch (shift)
        {
        case 1:
            ret = (this->_num * 60); // hour to min
            break;
        case 2:
            ret = (this->_num * 3600); // hour to sec
            break;
        case 0:
            ret = this->_num;
            break;
        }
        break;
    case Unit::G: //-----------------------------------G
        switch (shift)
        {
        case -2:
            ret = (this->_num / 1000000); // g to ton
            break;
        case -1:
            ret = (this->_num / 1000); // g to kg
            break;
        case 0:
            ret = this->_num;
            break;
        }
        break;
    case Unit::KG: //-----------------------------------KG
        switch (shift)
        {
        case -1:
            ret = (this->_num / 1000); // kg to ton
            break;
        case 1:
            ret = (this->_num * 1000); // kg to g
            break;
        case 0:
            ret = this->_num;
            break;
        }
        break;
    case Unit::TON: //-----------------------------------TON
        switch (shift)
        {
        case 1:
            ret = (this->_num * 1000); // ton to kg
            break;
        case 2:
            ret = (this->_num * 1000000); // ton to g
            break;
        case 0:
            ret = this->_num;
            break;
        }
        break;
    }

    return ret;
}

////--------------------------------------------------------------------------------------------
// Module
double PhysicalNumber::abs(double diff) const
{
    return (diff > 0) ? diff : -diff; // module
}

////--------------------------------------------------------------------------------------------
// Print "[unit]"
std::string PhysicalNumber::print_unit() const
{
    return units[this->unit_to_int()];
}

//
//--------------------------------------------INTERFACE-------------------------------------------
//

////--------------------------------------------------------------------------------------------
// Constructor
PhysicalNumber::PhysicalNumber(double num, Unit unit) : _num(num), _unit(unit)
{
}

////--------------------------------------------------------------------------------------------
// Get epsilon, static function
double PhysicalNumber::get_epsilon()
{
    return _epsilon;
}

////--------------------------------------------------------------------------------------------
// Set epsilon, static function
void PhysicalNumber::set_epsilon(double epsilon)
{
    _epsilon = epsilon;
}

////--------------------------------------------------------------------------------------------
// Operator+ (a + b)
const PhysicalNumber PhysicalNumber::operator+(const PhysicalNumber &ob) const
{
    return PhysicalNumber(this->_num + ob.convert(this->check(ob)), this->_unit);
}

////--------------------------------------------------------------------------------------------
// Operator+= (a += b)
const PhysicalNumber &PhysicalNumber::operator+=(const PhysicalNumber &ob)
{
    this->_num += ob.convert(this->check(ob));

    return *this;
}

////--------------------------------------------------------------------------------------------
// Operator+ (+a)
const PhysicalNumber PhysicalNumber::operator+() const
{
    return PhysicalNumber(this->_num, this->_unit);
}

////--------------------------------------------------------------------------------------------
// Operator++ (++a)
const PhysicalNumber &PhysicalNumber::operator++()
{
    this->_num++;

    return *this;
}

////--------------------------------------------------------------------------------------------
// Operator++ (a++)
const PhysicalNumber PhysicalNumber::operator++(int postfix)
{
    PhysicalNumber ret(*this);
    this->_num++;

    return ret;
}

////--------------------------------------------------------------------------------------------
// Operator- (a - b)
const PhysicalNumber PhysicalNumber::operator-(const PhysicalNumber &ob) const
{
    return PhysicalNumber(this->_num - ob.convert(this->check(ob)), this->_unit);
}

////--------------------------------------------------------------------------------------------
// Operator-= (a -= b)
const PhysicalNumber &PhysicalNumber::operator-=(const PhysicalNumber &ob)
{
    this->_num -= ob.convert(this->check(ob));

    return *this;
}

////--------------------------------------------------------------------------------------------
// Operator- (-a)
const PhysicalNumber PhysicalNumber::operator-() const
{
    return PhysicalNumber(-(this->_num), this->_unit);
}

////--------------------------------------------------------------------------------------------
// Operator-- (--a)
const PhysicalNumber &PhysicalNumber::operator--()
{
    this->_num--;

    return *this;
}

////--------------------------------------------------------------------------------------------
// Operator-- (a--)
const PhysicalNumber PhysicalNumber::operator--(int postfix)
{
    PhysicalNumber ret(*this);
    this->_num--;

    return ret;
}

////--------------------------------------------------------------------------------------------
// Operator== (a == b)
bool PhysicalNumber::operator==(const PhysicalNumber &ob) const
{
    PhysicalNumber temp(ob.convert(this->check(ob)), this->_unit);
    double diff = abs(temp._num - this->_num);

    if (diff <= _epsilon)
        return true;
    else
        return false;
}

////--------------------------------------------------------------------------------------------
// Operator!= (a != b)
bool PhysicalNumber::operator!=(const PhysicalNumber &ob) const
{
    if (*this == ob)
        return false;
    else
        return true;
}

////--------------------------------------------------------------------------------------------
// Operator< (a < b)
bool PhysicalNumber::operator<(const PhysicalNumber &ob) const
{
    PhysicalNumber temp(ob.convert(this->check(ob)), this->_unit);

    if ((temp._num - this->_num) > _epsilon)
        return true;
    else
        return false;
}

////--------------------------------------------------------------------------------------------
// Operator> (a > b)
bool PhysicalNumber::operator>(const PhysicalNumber &ob) const
{
    PhysicalNumber temp(ob.convert(this->check(ob)), this->_unit);

    if ((this->_num - temp._num) > _epsilon)
        return true;
    else
        return false;
}

////--------------------------------------------------------------------------------------------
// Operator<= (a <= b)
bool PhysicalNumber::operator<=(const PhysicalNumber &ob) const
{
    PhysicalNumber temp(ob.convert(this->check(ob)), this->_unit);

    if ((*this) < ob)
        return true;
    else if ((*this) == ob)
        return true;
    else
        return false;
}

////--------------------------------------------------------------------------------------------
// Operator>= (a >= b)
bool PhysicalNumber::operator>=(const PhysicalNumber &ob) const
{
    PhysicalNumber temp(ob.convert(this->check(ob)), this->_unit);

    if ((*this) > ob)
        return true;
    else if ((*this) == ob)
        return true;
    else
        return false;
}

} // namespace ariel