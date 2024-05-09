#include "LEDFilter.h"
#include "logging.h"
#include "../drivers/ws2812b.h"

LOG_MODULE(LEDFilter.cpp)


/*When you declare a static member variable in a class,
 * you also need to define it outside the class, typically
 * in a source file (.cpp) to allocate memory for it.
 * Otherwise, you'll get an "undefined reference" error
 * because the compiler can't find the memory allocation
 * for that variable.*/




LEDFilter::LEDFilter(uint8_t accel_values[3]) : accel_values(accel_values) {
 {
}








