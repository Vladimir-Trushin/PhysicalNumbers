/**
 * Examples of automatic tests for the exercise on physical numbers.
 *
 * @author Erel Segal-Halevi
 * @since 2019-02
 */

#include <iostream>
#include <sstream>
using std::cout, std::endl, std::istringstream;
#include "PhysicalNumber.h"
using ariel::PhysicalNumber, ariel::Unit;
#include "badkan.hpp"

int main()
{
  badkan::TestCase testcase;
  int grade = 0;
  int signal = setjmp(badkan::longjmp_buffer);
  if (signal == 0)
  {

    // BASIC TESTS - DO NOT CHANGE
    PhysicalNumber a(2, Unit::KM);
    PhysicalNumber b(300, Unit::M);
    PhysicalNumber c(2, Unit::HOUR);
    PhysicalNumber d(30, Unit::MIN);

    PhysicalNumber cm(100, Unit::CM);
    PhysicalNumber km(3, Unit::KM);
    PhysicalNumber g(100000, Unit::G);
    PhysicalNumber ton(0.1, Unit::TON);
    bool temp; // To remove the warnings in the CHECK_THROWS macro

    testcase
        .setname("Basic output")
        .CHECK_OUTPUT(a, "2[km]")
        .CHECK_OUTPUT(b, "300[m]")

        .setname("Compatible dimensions")
        .CHECK_OUTPUT(b + a, "2300[m]")
        .CHECK_OUTPUT((a += b), "2.3[km]")
        .CHECK_OUTPUT(a, "2.3[km]")
        .CHECK_OUTPUT(a + a, "4.6[km]")
        .CHECK_OUTPUT(b - b, "0[m]")
        .CHECK_OUTPUT(c, "2[hour]")
        .CHECK_OUTPUT(d, "30[min]")
        .CHECK_OUTPUT(d + c, "150[min]")

        .setname("Incompatible dimensions")
        .CHECK_THROWS(a + c)
        .CHECK_THROWS(a + d)
        .CHECK_THROWS(b + c)
        .CHECK_THROWS(b + d)

        .setname("Basic input")
        .CHECK_OK(istringstream("700[kg]") >> a)
        .CHECK_OUTPUT((a += PhysicalNumber(1, Unit::TON)), "1700[kg]")

        // YOUR TESTS - INSERT AS MANY AS YOU WANT

        .setname("My test1") //-----------------------------------------------<1>

        .CHECK_OUTPUT(-cm, "-100[cm]")
        .CHECK_OUTPUT(-km, "-3[km]")
        .CHECK_OUTPUT(+g, "100000[g]")
        .CHECK_OUTPUT(ton, "0.1[ton]")
        .CHECK_OUTPUT(ton - g, "0[ton]")    // 100000[g] = 0.1[ton]
        .CHECK_OUTPUT(km - cm, "2.999[km]") // 3[km] - 100[cm] = 2.999[km]

        .setname("My test2") //-----------------------------------------------<2>

        .CHECK_THROWS(cm + g)   // throw exception
        .CHECK_THROWS(ton - km) //
        .CHECK_THROWS(km += g)  //
        .CHECK_THROWS(g -= cm)  //

        .setname("My test3") //-----------------------------------------------<3>

        .CHECK_OK(istringstream("30[sec]") >> a)
        .CHECK_OUTPUT((a += PhysicalNumber(1, Unit::HOUR)), "3630[sec]")

        .CHECK_OK(istringstream("1[cm]") >> b)
        .CHECK_THROWS((b += PhysicalNumber(1, Unit::G)))

        .setname("My test4") //-----------------------------------------------<4>

        .CHECK_THROWS(temp = (a == b)) // a[sec] # b[cm] throw exception (logic_error), units do not match b[unit] cannot be converted to a[unit]
        .CHECK_THROWS(temp = (a != b)) //
        .CHECK_THROWS(temp = (a < b))  //
        .CHECK_THROWS(temp = (a <= b)) //
        .CHECK_THROWS(temp = (a > b))  //
        .CHECK_THROWS(temp = (a >= b)) //

        .CHECK_OK(istringstream("15[min]") >> a)
        .CHECK_OK(istringstream("0.25[hour]") >> b)
        .CHECK_EQUAL(a == b, true)  // true
        .CHECK_EQUAL(a != b, false) // false

        .CHECK_OK(istringstream("20[min]") >> a)
        .CHECK_OK(istringstream("0.25[hour]") >> b)
        .CHECK_EQUAL(a > b, true)  // true
        .CHECK_EQUAL(a >= b, true) //
        .CHECK_EQUAL(a != b, true) //

        .CHECK_EQUAL(a < b, false)  // fase
        .CHECK_EQUAL(a <= b, false) //
        .CHECK_EQUAL(a == b, false) //

        .setname("My test5") //-----------------------------------------------<5>

        .CHECK_THROWS(istringstream("10[unit]") >> a) // throw exception (logic_error), unit not match
        .CHECK_THROWS(istringstream("10[hor]") >> a)  // throw exception (logic_error), unit not match
        .CHECK_THROWS(istringstream("10[]") >> a)     // throw exception (logic_error), unit not match
        .CHECK_THROWS(istringstream("10[sm]") >> a)   // throw exception (logic_error), unit not match

        .CHECK_THROWS(istringstream("[kg]") >> a)         // throw exception (logic_error), digit not match
        .CHECK_THROWS(istringstream(".10[kg]") >> a)      // throw exception (logic_error), digit not match
        .CHECK_THROWS(istringstream("10.10.10[kg]") >> a) // throw exception (logic_error), digit not match

        .setname("My test6") //-----------------------------------------------<6>

        .CHECK_OK(istringstream("1[m]") >> a)
        .CHECK_OK(istringstream("2[m]") >> b)

        .CHECK_EQUAL(a != b, true) // true (a = 1, b = 2)
        .CHECK_OK(a++)
        .CHECK_EQUAL(a == b, true) // true (a = 2, b = 2)
        .CHECK_OK(++a)
        .CHECK_OK(b--)
        .CHECK_EQUAL(a > b, true) // true (a = 3, b = 1)
        .CHECK_OK(a--)
        .CHECK_OK(--a)
        .CHECK_EQUAL(a == b, true) // true (a = 1, b = 1)

        .setname("My test7") //-----------------------------------------------<7>

        .CHECK_OK(istringstream("10.000001[m]") >> a) // 1-E6
        .CHECK_OK(istringstream("10.000002[m]") >> b) // 1-E6
        .CHECK_EQUAL(a == b, false)                   // false
        .CHECK_EQUAL(a < b, true)                     // true

        .CHECK_OK(istringstream("10.0000001[m]") >> a) // 1-E7
        .CHECK_OK(istringstream("10.0000002[m]") >> b) // 1-E7
        .CHECK_EQUAL(a == b, true)                     // true - it's work because precision is: 1-E6
        .CHECK_EQUAL(a >= b, true)                     // true - it's work because precision is: 1-E6

        .CHECK_EQUAL(a != b, false) // false - it's work because precision is: 1-E6
        .CHECK_EQUAL(a < b, false)  // false - it's work because precision is: 1-E6
        // Change the EPSILON in the file Unit.h to change the precision

        .print(cout, /*show_grade=*/false);
    grade = testcase.grade();
  }
  else
  {
    testcase.print_signal(signal);
    grade = 0;
  }
  cout << "*** Grade: " << grade << " ***" << endl;
  return grade;
}
