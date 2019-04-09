/**
 * Demo file for the exercise on physical numbers
 *
 * @author Erel Segal-Halevi
 * @since 2019-02
 */

#include <iostream>
#include <sstream>
#include <stdexcept>
using std::cout, std::endl, std::boolalpha, std::istringstream;
#include "PhysicalNumber.h"
#include "Unit.h"
using ariel::PhysicalNumber, ariel::Unit;

int main()
{
  try
  {
    PhysicalNumber a(2, Unit::KM);  // 2 kilometers
    PhysicalNumber b(300, Unit::M); // 300 meters
    cout << a << endl;              // Prints "2[km]"
    cout << (a + b) << endl;        // Prints "2.3[km]"

    cout << (b - a) << endl; // Prints "-1700[m]"
    cout << (-a) << endl;    // Prints "-2[km]"

    cout << boolalpha;                                    // print booleans as strings from now on:
    cout << (a > b) << endl;                              // Prints "true"
    cout << (a <= b) << endl;                             // Prints "false"
    cout << (a == PhysicalNumber(2000, Unit::M)) << endl; // Prints "true"

    istringstream input("700[kg]");
    input >> a;
    cout << endl;
    cout << a << endl;                                   // Prints "700[kg]"
    cout << (a += PhysicalNumber(1, Unit::TON)) << endl; // prints "1700[kg]"
    cout << a << endl;                                   // Prints "1700[kg]" since a has changed.

    //---------------------------------test exception---------------------------------------------------------------
    try
    {
      cout << (a + b) << endl; // throw exception
    }
    catch (const std::exception &ex)
    {
      cout << "Exception: " << ex.what() << endl; // Prints "Units do not match - [m] cannot be converted to [kg]"
    }
    //---------------------------------end test exception-----------------------------------------------------------

    ////////
    // My test
    PhysicalNumber x(200, Unit::KG);  // 200 KG
    PhysicalNumber y(0.2, Unit::TON); // 0.2 TON

    cout << endl;
    cout << "-------------------------1" << endl;

    cout << (x == y) << endl;  // Prints true
    cout << (++x > y) << endl; // Prints true
    y++;
    cout << (x < y) << endl;  // Prints true
    cout << (x != y) << endl; // Prints true

    cout << endl;
    cout << "-------------------------2" << endl;

    istringstream input1("20[Sec]"); // 20 SEC
    istringstream input2("1[HouR]"); // 1 HOUR

    input1 >> x;
    cout << x << endl; // 20 SEC

    input2 >> y;
    cout << y << endl; // 1 HOUR

    x += y;
    cout << x << endl; // Prints 3620[sec]
    y += x;
    cout << y << endl; // Prints 2.00556[hour]

    cout << endl;
    cout << "-------------------------3" << endl;

    istringstream input3("-200[M]"); // 20 SEC
    istringstream input4("1[kM]");   // 1 HOUR

    input3 >> x;
    cout << x << endl; // 20 SEC

    input4 >> y;
    cout << y << endl; // 1 HOUR

    cout << (x + y) << endl; // Prints 800[m]
    cout << (y - x) << endl; // Prints 1.2[km]
    cout << (+x) << endl;    // Prints -200[m]
    cout << (-y) << endl;    // Prints -1[km]
  }
  catch (...)
  {
    cout << "Unexpected exception!" << endl;
  }

  return 100;
}
