#ifndef BASICTYPE_H
#define BASICTYPE_H

#include <assert.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <functional>
#include <iostream>

#pragma warning(disable:4996)

// Utils:
#define DISABLE_COPY_AND_ASSIGN(name) \
	name(const name&); \
	void operator = (const name&)

#define DISABLE_DEFAULT_CONSTRUCTOR(name) \
	name(); \
	name(const name&); \
	void operator = (const name&)

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#define CCH(array) (sizeof(ArraySizeHelper(array)))

#define OFFSET_OF(base, member) \
	(reinterpret_cast<ptrdiff_t>(&reinterpret_cast<base*>(16)->member) - 16)

#define UNUSED(arg) ((void)(arg))

namespace grav {

template<class T>
inline void ignore_result(T) {}

template<class T>
inline void delete_and_clear_container(T *array)
{
	for(typename T::iterator iter = array->begin();
	    iter != array->end(); ignore_result(++iter))
		delete (*iter);
	array->clear();
}

struct Linker {
	enum Value { INITIALIZER };
};

} // namespace grav

#endif // BASICTYPE_H
