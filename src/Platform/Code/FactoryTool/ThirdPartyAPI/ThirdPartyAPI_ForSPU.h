
#ifdef THIRDPARTYAPI_EXPORTS
#define THIRDPARTYAPI_API extern "C" __declspec(dllexport)
#else
#define THIRDPARTYAPI_API extern "C" __declspec(dllimport)
#endif

THIRDPARTYAPI_API bool BTW_BondReply(char * szPin=NULL);
THIRDPARTYAPI_API bool BTW_UnBond(char * szBTAddr);
