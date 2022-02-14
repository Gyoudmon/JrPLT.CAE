#include "flonum.h"
#include "mathematics.h"

using namespace WarGrey::STEM;

inline static double quick_degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

/*************************************************************************************************/
double WarGrey::STEM::radians_to_degrees(double radians) {
	return (radians / pi) * 180.0;
}

double WarGrey::STEM::degrees_to_radians(double degrees) {
	return quick_degrees_to_radians(degrees);
}
