#pragma once
//=========================һЩģ��Ԫ��̿�==============================

template <int m, size_t n>
struct POWER
{
	enum {value = m * POWER<m, n - 1>::value};
};
template <int m>
struct POWER<m, 0>
{
	enum {value = 1};
};