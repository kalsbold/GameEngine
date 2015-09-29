#include "stdafx.h"
#include "dll_exporting.h"
#include <iostream>

using namespace std;

MATH_API void FunctionInDll()
{
	cout << "Hello. I`m from dll_exporting.dll" << endl;
}