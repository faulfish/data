#ifndef COMDEF_H
#define COMDEF_H

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE   1   /* Boolean true value. */
#define FALSE  0   /* Boolean false value. */

#define  ON   1    /* On value. */
#define  OFF  0    /* Off value. */

#ifdef _lint
  #define NULL 0   
#endif

#ifndef NULL
  #define NULL  0
#endif

typedef  unsigned char      boolean;     /* Boolean value type. */
#define BOOLEAN boolean

typedef  unsigned long	uint32;      /* Unsigned 32 bit value */
typedef  unsigned short	uint16;      /* Unsigned 16 bit value */
typedef  unsigned char	uint8;       /* Unsigned 8  bit value */
#define BYTE uint8
#define WORD uint16
#define DWORD uint32

typedef  signed long int    int32;       /* Signed 32 bit value */
typedef  signed short       int16;       /* Signed 16 bit value */
typedef  signed char        int8;        /* Signed 8  bit value */

typedef  unsigned char     byte;         /* Unsigned 8  bit value type. */
typedef  unsigned short    word;         /* Unsinged 16 bit value type. */
typedef  unsigned long     dword;        /* Unsigned 32 bit value type. */

typedef  unsigned char     uint1;        /* Unsigned 8  bit value type. */
typedef  unsigned short    uint2;        /* Unsigned 16 bit value type. */
typedef  unsigned long     uint4;        /* Unsigned 32 bit value type. */

typedef  signed char       int1;         /* Signed 8  bit value type. */
typedef  signed short      int2;         /* Signed 16 bit value type. */
typedef  long int          int4;         /* Signed 32 bit value type. */

typedef  signed long       sint31;       /* Signed 32 bit value */
typedef  signed short      sint15;       /* Signed 16 bit value */
typedef  signed char       sint7;        /* Signed 8  bit value */


#pragma pack(1)

typedef  signed _int64   int64;       /* Signed 64 bit value */
typedef  unsigned _int64 uint64;      /* Unsigned 64 bit value */
#define  __packed    
#define  __global_reg(x)
#define  PACKED
#define  INLINE __inline
#define  _interrupt


 
/* ---------------------------------------------------------------------- 
** Lint does not understand __packed, so we define it away here.  In the
** past we did this:
**   This helps us catch non-packed pointers accessing packed structures,
**   for example, (although lint thinks it is catching non-volatile pointers
**   accessing volatile structures).
**   This does assume that volatile is not being used with __packed anywhere
**   because that would make Lint see volatile volatile (grrr).
** but found it to be more trouble than it was worth as it would emit bogus
** errors 
** ---------------------------------------------------------------------- */
#ifdef _lint
  #define __packed
#endif


#define  MEM_B( x )  ( *( (byte *) (x) ) )
#define  MEM_W( x )  ( *( (word *) (x) ) )

#ifndef MAX
   #define  MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#endif

#ifndef MIN
   #define  MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#endif


#define FPOS( type, field ) \
    /*lint -e545 */ ( (dword) &(( type *) 0)-> field ) /*lint +e545 */
 

#define FSIZ( type, field ) sizeof( ((type *) 0)->field )

#define  FLIPW( ray ) ( (((word) (ray)[0]) * 256) + (ray)[1] )

#define  FLOPW( ray, val ) \
  (ray)[0] = ((val) / 256); \
  (ray)[1] = ((val) & 0xFF)


#define  B_PTR( var )  ( (byte *) (void *) &(var) )
#define  W_PTR( var )  ( (word *) (void *) &(var) )


#define  WORD_LO(xxx)  ((byte) ((word)(xxx) & 255))
#define  WORD_HI(xxx) \
  /*lint -e572 */  ((byte) ((word)(xxx) >> 8))  /*lint +e572 */


#define RND8( x )       ((((x) + 7) / 8 ) * 8 )

   dword enter_csection(void);
   void leave_csection(void);
   #define INTLOCK() enter_csection()
   #define INTFREE() leave_csection()

   #define  INTLOCK_SAV( sav_var ) sav_var = enter_csection()
   #define  INTFREE_SAV( sav_var ) if(!sav_var) leave_csection()


#define  UPCASE( c ) ( ((c) >= 'a' && (c) <= 'z') ? ((c) - 0x20) : (c) )

#define  DECCHK( c ) ((c) >= '0' && (c) <= '9')

#define  HEXCHK( c ) ( ((c) >= '0' && (c) <= '9') ||\
                       ((c) >= 'A' && (c) <= 'F') ||\
                       ((c) >= 'a' && (c) <= 'f') )



#define  INC_SAT( val )  (val = ((val)+1 > (val)) ? (val)+1 : (val))

#define  ARR_SIZE( a )  ( sizeof( (a) ) / sizeof( (a[0]) ) )

#ifdef LOCAL
#undef LOCAL
#endif

//#ifdef SHOW_STAT
  #define LOCAL
//#else
//  #define LOCAL static
//#endif

byte _inp(word *port);
word _inpw(word *port);
dword _inpdw(dword *port);
void _outp(word *port, byte val);
void _outpw(word *port, word val);
void _outpdw(dword *port, dword val);

#define inp(port)			_inp((word *) (port))
#define inpw(port)		_inpw((word *) (port))
#define inpdw(port)		_inpdw((dword *) (port))
#define outp(port, val)	_outp((word *) (port), (byte) (val))
#define outpw(port, val)	_outpw((word *) (port), (word) (val))
#define outpdw(port, val)	_outpdw((dword *) (port), (dword) (val))

#define NOTICE_TYPE       const char
#define NOTICE_SEG_TYPE
/*
#define __align(x)

typedef unsigned int size_t;

#include "memory.h"
#include "rex.h"

#ifndef _FPOS_T_DEFINED
typedef struct fpos_t {
        unsigned int __lo;
        int          __hi;
        } fpos_t;
#define _FPOS_T_DEFINED
#endif
#include <stdio.h>
#undef feof
*/
#endif  /* COMDEF_H */

