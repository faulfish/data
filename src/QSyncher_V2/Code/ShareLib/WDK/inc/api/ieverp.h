#define VER_USE_OTHER_MAJOR_MINOR_VER

//
// override the majorversion and minorversion (normall defined in ntverp.h)
//
#define VER_PRODUCTMAJORVERSION         7
#define VER_PRODUCTMINORVERSION         00

#define VER_PRODUCTVERSION_W            (0x0700)
#define VER_PRODUCTVERSION_DW           (0x07000000 | VER_PRODUCTBUILD)


//
// override product name (normally defined in ntverp.h / common.ver)
//
#ifdef RC_INVOKED
#define VER_PRODUCTNAME_STR L"Windows\256 Internet Explorer"
#else
#define VER_PRODUCTNAME_STR "Windows (R) Internet Explorer"
#endif


#include <ntverp.h>

