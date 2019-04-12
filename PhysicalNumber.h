#pragma once

//=========================================================================================
// This class provide a safe conversion of physical units
//
// For more information about regular expression please follow the links:
// &: https://solarianprogrammer.com/2011/10/12/cpp-11-regex-tutorial/
// &: https://en.cppreference.com/w/cpp/regex/ecmascript
// &: http://www.cplusplus.com/reference/regex/ECMAScript/
//
// EXCEPTION:
// PhysicalNumber()      ---> NO
// ~PhysicalNumber()     ---> NO
//
// check()               ---> Throwing an exception if the units do not match (logic_error)
// operator arithmetic() ---> Throwing an exception if the units do not match (logic_error)
// operator logical()    ---> Throwing an exception if the units do not match (logic_error)
// operator>>()          ---> Throwing an exception if syntax do not match --digit[unit]-- (logic_error, invalid_argument)
// convert()             ---> NO
// unit_to_int()         ---> NO
// int_to_unit()         ---> NO
// print_unit()          ---> NO
//
//
////////

#include "Unit.h"

namespace ariel
{
class PhysicalNumber
{
  ////--------------------------------------------------------------------------------------------
  // Print "digit[unit]"
  friend std::ostream &operator<<(std::ostream &os, const PhysicalNumber &ob);

  ////--------------------------------------------------------------------------------------------
  // Assignment unit
  friend std::istream &operator>>(std::istream &os, PhysicalNumber &ob);

private:
  double _num;
  Unit _unit;
  static double _epsilon; // precision

  ////--------------------------------------------------------------------------------------------
  // Check if units match, absolute difference value
  // one unit to another unit in the same field can't be bigger 2
  int check(const PhysicalNumber &ob) const;

  ////--------------------------------------------------------------------------------------------
  // Convert unit to integer
  size_t unit_to_int() const;

  ////--------------------------------------------------------------------------------------------
  // Convert integer to units
  Unit int_to_unit(const size_t n) const;

  ////--------------------------------------------------------------------------------------------
  // Convert one unit to another unit in the same field
  double convert(int shift) const;

  ////--------------------------------------------------------------------------------------------
  // Module
  double abs(double diff) const;

  ////--------------------------------------------------------------------------------------------
  // Print "[unit]"
  std::string print_unit() const;

  //
  //--------------------------------------------INTERFACE-------------------------------------------
  //

public:
  ////--------------------------------------------------------------------------------------------
  // Constructor
  PhysicalNumber(double num, Unit unit);

  ////--------------------------------------------------------------------------------------------
  // Get epsilon, static function
  static double get_epsilon();

  ////--------------------------------------------------------------------------------------------
  // Set epsilon, static function
  static void set_epsilon(double epsilon);

  ////--------------------------------------------------------------------------------------------
  // Arithmetic operators+ overload
  const PhysicalNumber operator+(const PhysicalNumber &ob) const;
  const PhysicalNumber &operator+=(const PhysicalNumber &ob);
  const PhysicalNumber operator+() const; // unary
  const PhysicalNumber &operator++();
  const PhysicalNumber operator++(int postfix);

  ////--------------------------------------------------------------------------------------------
  // Arithmetic operators- overload
  const PhysicalNumber operator-(const PhysicalNumber &ob) const;
  const PhysicalNumber &operator-=(const PhysicalNumber &ob);
  const PhysicalNumber operator-() const; // unary
  const PhysicalNumber &operator--();
  const PhysicalNumber operator--(int postfix);

  ////--------------------------------------------------------------------------------------------
  // Logical operators overload
  bool operator==(const PhysicalNumber &ob) const;
  bool operator!=(const PhysicalNumber &ob) const;
  bool operator<(const PhysicalNumber &ob) const;
  bool operator>(const PhysicalNumber &ob) const;
  bool operator<=(const PhysicalNumber &ob) const;
  bool operator>=(const PhysicalNumber &ob) const;
};

} // namespace ariel