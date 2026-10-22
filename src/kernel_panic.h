#include "printf.h"
#include <stdarg.h>

void cli();

void infloop();

void vkernel_panic(const char* fmt, va_list args);

void kernel_panic(const char* fmt, ...);