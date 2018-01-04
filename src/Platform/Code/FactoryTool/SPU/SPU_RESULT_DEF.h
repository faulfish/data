#ifndef SPU_RESULT_DEF
#define SPU_RESULT_DEF

#include <string>

enum SPU_RESULT
{
	SPU_SUCCESS,
	SPU_FAIL,
	SPU_BREAK
};

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#else
#define TSTRING std::string
#endif
#endif

#endif //#define SPU_RESULT_DEF