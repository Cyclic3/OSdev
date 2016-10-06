#include <string.h>
#ifndef STRLEN_H
#define STRLEN_H
size_t strlen(const char* string)
{
	size_t result = 0;
	while ( string[result] )
		result++;
	return result;
}
#endif
