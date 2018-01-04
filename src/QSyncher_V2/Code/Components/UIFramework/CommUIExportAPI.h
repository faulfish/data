#ifdef QSYNC_LIB__COMMONUI_EXPORTS
#define COMMONUI_API __declspec(dllexport)
#else
#define COMMONUI_API __declspec(dllimport)
#endif