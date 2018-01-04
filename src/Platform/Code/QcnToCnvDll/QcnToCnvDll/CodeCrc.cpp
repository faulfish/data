#include "stdafx.h"

#include "CodeCrc.h"                /* CRC typedefs and definitions         */


/* The CRC table size is based on how many bits at a time we are going
** to process through the table.  Given that we are processing the data
** 8 bits at a time, this gives us 2^8 (256) entries.
*/
#define CRC_TAB_SIZE    256             /* 2^CRC_TAB_BITS      */

/* CRC table for 30 bit CRC, with generator polynomial 0x6030b9c7,
** calculated 8 bits at a time, MSB first.
*/
const dword crc30_table[ CRC_TAB_SIZE ] = {
  0x00000000, 0x2030b9c7, 0x2051ca49, 0x0061738e,
  0x20932d55, 0x00a39492, 0x00c2e71c, 0x20f25edb,
  0x2116e36d, 0x01265aaa, 0x01472924, 0x217790e3,
  0x0185ce38, 0x21b577ff, 0x21d40471, 0x01e4bdb6,
  0x221d7f1d, 0x022dc6da, 0x024cb554, 0x227c0c93,
  0x028e5248, 0x22beeb8f, 0x22df9801, 0x02ef21c6,
  0x030b9c70, 0x233b25b7, 0x235a5639, 0x036aeffe,
  0x2398b125, 0x03a808e2, 0x03c97b6c, 0x23f9c2ab,
  0x240a47fd, 0x043afe3a, 0x045b8db4, 0x246b3473,
  0x04996aa8, 0x24a9d36f, 0x24c8a0e1, 0x04f81926,
  0x051ca490, 0x252c1d57, 0x254d6ed9, 0x057dd71e,
  0x258f89c5, 0x05bf3002, 0x05de438c, 0x25eefa4b,
  0x061738e0, 0x26278127, 0x2646f2a9, 0x06764b6e,
  0x268415b5, 0x06b4ac72, 0x06d5dffc, 0x26e5663b,
  0x2701db8d, 0x0731624a, 0x075011c4, 0x2760a803,
  0x0792f6d8, 0x27a24f1f, 0x27c33c91, 0x07f38556,
  0x2824363d, 0x08148ffa, 0x0875fc74, 0x284545b3,
  0x08b71b68, 0x2887a2af, 0x28e6d121, 0x08d668e6,
  0x0932d550, 0x29026c97, 0x29631f19, 0x0953a6de,
  0x29a1f805, 0x099141c2, 0x09f0324c, 0x29c08b8b,
  0x0a394920, 0x2a09f0e7, 0x2a688369, 0x0a583aae,
  0x2aaa6475, 0x0a9addb2, 0x0afbae3c, 0x2acb17fb,
  0x2b2faa4d, 0x0b1f138a, 0x0b7e6004, 0x2b4ed9c3,
  0x0bbc8718, 0x2b8c3edf, 0x2bed4d51, 0x0bddf496,
  0x0c2e71c0, 0x2c1ec807, 0x2c7fbb89, 0x0c4f024e,
  0x2cbd5c95, 0x0c8de552, 0x0cec96dc, 0x2cdc2f1b,
  0x2d3892ad, 0x0d082b6a, 0x0d6958e4, 0x2d59e123,
  0x0dabbff8, 0x2d9b063f, 0x2dfa75b1, 0x0dcacc76,
  0x2e330edd, 0x0e03b71a, 0x0e62c494, 0x2e527d53,
  0x0ea02388, 0x2e909a4f, 0x2ef1e9c1, 0x0ec15006,
  0x0f25edb0, 0x2f155477, 0x2f7427f9, 0x0f449e3e,
  0x2fb6c0e5, 0x0f867922, 0x0fe70aac, 0x2fd7b36b,
  0x3078d5bd, 0x10486c7a, 0x10291ff4, 0x3019a633,
  0x10ebf8e8, 0x30db412f, 0x30ba32a1, 0x108a8b66,
  0x116e36d0, 0x315e8f17, 0x313ffc99, 0x110f455e,
  0x31fd1b85, 0x11cda242, 0x11acd1cc, 0x319c680b,
  0x1265aaa0, 0x32551367, 0x323460e9, 0x1204d92e,
  0x32f687f5, 0x12c63e32, 0x12a74dbc, 0x3297f47b,
  0x337349cd, 0x1343f00a, 0x13228384, 0x33123a43,
  0x13e06498, 0x33d0dd5f, 0x33b1aed1, 0x13811716,
  0x14729240, 0x34422b87, 0x34235809, 0x1413e1ce,
  0x34e1bf15, 0x14d106d2, 0x14b0755c, 0x3480cc9b,
  0x3564712d, 0x1554c8ea, 0x1535bb64, 0x350502a3,
  0x15f75c78, 0x35c7e5bf, 0x35a69631, 0x15962ff6,
  0x366fed5d, 0x165f549a, 0x163e2714, 0x360e9ed3,
  0x16fcc008, 0x36cc79cf, 0x36ad0a41, 0x169db386,
  0x17790e30, 0x3749b7f7, 0x3728c479, 0x17187dbe,
  0x37ea2365, 0x17da9aa2, 0x17bbe92c, 0x378b50eb,
  0x185ce380, 0x386c5a47, 0x380d29c9, 0x183d900e,
  0x38cfced5, 0x18ff7712, 0x189e049c, 0x38aebd5b,
  0x394a00ed, 0x197ab92a, 0x191bcaa4, 0x392b7363,
  0x19d92db8, 0x39e9947f, 0x3988e7f1, 0x19b85e36,
  0x3a419c9d, 0x1a71255a, 0x1a1056d4, 0x3a20ef13,
  0x1ad2b1c8, 0x3ae2080f, 0x3a837b81, 0x1ab3c246,
  0x1b577ff0, 0x3b67c637, 0x3b06b5b9, 0x1b360c7e,
  0x3bc452a5, 0x1bf4eb62, 0x1b9598ec, 0x3ba5212b,
  0x3c56a47d, 0x1c661dba, 0x1c076e34, 0x3c37d7f3,
  0x1cc58928, 0x3cf530ef, 0x3c944361, 0x1ca4faa6,
  0x1d404710, 0x3d70fed7, 0x3d118d59, 0x1d21349e,
  0x3dd36a45, 0x1de3d382, 0x1d82a00c, 0x3db219cb,
  0x1e4bdb60, 0x3e7b62a7, 0x3e1a1129, 0x1e2aa8ee,
  0x3ed8f635, 0x1ee84ff2, 0x1e893c7c, 0x3eb985bb,
  0x3f5d380d, 0x1f6d81ca, 0x1f0cf244, 0x3f3c4b83,
  0x1fce1558, 0x3ffeac9f, 0x3f9fdf11, 0x1faf66d6
};

/* CRC table for 16 bit CRC, with generator polynomial 0x1021,
** calculated 8 bits at a time, MSB first.
*/
const word crc16_table[ CRC_TAB_SIZE ] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
  0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
  0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
  0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
  0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
  0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
  0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
  0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
  0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
  0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
  0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
  0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
  0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
  0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
  0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

/* CRC table for 16 bit CRC, with generator polynomial 0x8408,
** calculated 8 bits at a time, LSB first.  This table is used
** from a macro in sio.c.
*/
const word crc_16_l_table[ CRC_TAB_SIZE ] = {
  0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
  0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
  0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
  0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
  0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
  0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
  0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
  0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
  0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
  0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
  0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
  0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
  0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
  0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
  0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
  0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
  0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
  0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
  0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
  0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
  0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
  0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
  0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
  0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
  0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
  0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
  0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
  0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
  0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
  0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
  0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
  0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};


/*===========================================================================

FUNCTION CRC_16_CALC

DESCRIPTION
  This function calculates a 16-bit CRC over a specified number of data
  bits.  It can be used to produce a CRC and to check a CRC.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

word crc_16_calc
(
  byte *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the MS bit of the first byte.
    */

  word len
    /* Number of data bits to calculate the CRC over */
)
{
  word data, crc_16;

  /* Generate a CRC-16 by looking up the transformation in a table and
  ** XOR-ing it into the CRC, one byte at a time.
  */
  for (crc_16 = CRC_16_SEED ; len >= 8; len -= 8, buf_ptr++) {
    crc_16 = (word)(crc16_table[ (crc_16 >> (16 - 8)) ^ *buf_ptr ] ^ (crc_16 << 8));
  }

  /* Finish calculating the CRC over the trailing data bits
  **
  ** XOR the MS bit of data with the MS bit of the CRC.
  ** Shift the CRC and data left 1 bit.
  ** If the XOR result is 1, XOR the generating polynomial in with the CRC.
  */
  if (len != 0) {

    data = ((word) (*buf_ptr)) << (16-8); /* Align data MSB with CRC MSB */

    while (len-- != 0) {
      if ( ((crc_16 ^ data) & ((word) 1 << 15)) != 0 ){ /* Is MSB of XOR a 1 */

        crc_16 <<= 1;                   /* Left shift CRC          */
        crc_16 ^= CRC_16_POLYNOMIAL;    /* XOR polynomial into CRC */

      } else {

        crc_16 <<= 1;                   /* Left shift CRC          */

      }

      data <<= 1;                       /* Left shift data         */
    }
  }

  return( ~crc_16 );            /* return the 1's complement of the CRC */

} /* end of crc_16_calc */


/*===========================================================================

FUNCTION CRC_16_L_CALC

DESCRIPTION
  This function calculates an LSB-first 16-bit CRC over a specified number
  of data bits.  It can be used to produce a CRC and to check a CRC.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_L_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

word crc_16_l_calc
(
  byte *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the LS bit of the first byte.
    */

  word len
    /* Number of data bits to calculate the CRC over */
)
{
  word data, crc_16;

  /* Generate a CRC-16 by looking up the transformation in a table and
  ** XOR-ing it into the CRC, one byte at a time.
  */
  for (crc_16 = CRC_16_L_SEED ; len >= 8; len -= 8, buf_ptr++) {
    crc_16 = crc_16_l_table[ (crc_16 ^ *buf_ptr) & 0x00ff ] ^ (crc_16 >> 8);
  }

  /* Finish calculating the CRC over the trailing data bits
  **
  ** XOR the MS bit of data with the MS bit of the CRC.
  ** Shift the CRC and data left 1 bit.
  ** If the XOR result is 1, XOR the generating polynomial in with the CRC.
  */
  if (len != 0) {

    data = ((word) (*buf_ptr)) << (16-8); /* Align data MSB with CRC MSB */

    while (len-- != 0) {
      if ( ((crc_16 ^ data) & 0x01) != 0 ){   /* Is LSB of XOR a 1 */

        crc_16 >>= 1;                   /* Right shift CRC         */
        crc_16 ^= CRC_16_L_POLYNOMIAL;  /* XOR polynomial into CRC */

      } else {

        crc_16 >>= 1;                   /* Right shift CRC         */

      }

      data >>= 1;                       /* Right shift data        */
    }
  }

  return( ~crc_16 );            /* return the 1's complement of the CRC */

} /* end of crc_16_l_calc */


/*===========================================================================

FUNCTION CRC_30_CALC

DESCRIPTION
  This function calculates a 30-bit CRC over a specified number of data
  bits.  It can be used to produce a CRC and to check a CRC.

DEPENDENCIES
  None

RETURN VALUE
  Returns a double word holding 30 bits which are the contents of the
  CRC register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_30_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

dword crc_30_calc
(
  byte *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the MS bit of the first byte.
    */

  word len
    /* Number of data bits to calculate the CRC over */
)
{
  dword data, crc_30;

  /* Generate a CRC-30 by looking up the transformation in a table and
  ** XOR-ing it into the CRC, one byte at a time.
  */
  for (crc_30 = CRC_30_SEED ; len >= 8; len -= 8, buf_ptr++) {
    crc_30 = crc30_table[ ((crc_30 >> (30 - 8)) ^ *buf_ptr) & 0xff ]
                ^ (crc_30 << 8);
  }

  /* Finish calculating the CRC over the trailing data bits
  **
  ** XOR the MS bit of data with the MS bit of the CRC.
  ** Shift the CRC and data left 1 bit.
  ** If the XOR result is 1, XOR the generating polynomial in with the CRC.
  */
  if (len != 0) {

    data = ((dword) (*buf_ptr)) << (30-8); /* Align data MSB with CRC MSB */

    while (len-- != 0) {
      if ( ((crc_30 ^ data) & (1L << 29)) != 0 ) {       /* Is MSB of XOR a 1 */

        crc_30 <<= 1;                   /* Left shift CRC          */
        crc_30 ^= CRC_30_POLYNOMIAL;    /* XOR polynomial into CRC */

      } else {

        crc_30 <<= 1;                   /* Left shift CRC          */

      }

      data <<= 1;                       /* Left shift data         */
    }
  }

  crc_30 = ~crc_30;                     /* 1's complement the CRC     */

  return( crc_30 & 0x3FFFFFFF );        /* mask the result to 30 bits */

} /* end of crc_30_calc */


/*===========================================================================

FUNCTION CRC_30_STEP

DESCRIPTION
  This function calculates a 30-bit CRC over a specified number of data
  bits.  It can be used to produce a CRC and to check a CRC.  The CRC value
  passed in is used to continue the CRC calculation from a previous call,
  this allows this routine to be used to CRC discontinuous data.

DEPENDENCIES
  None

RETURN VALUE
  Returns a double word holding 30 bits which are the contents of the
  CRC register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_30_OK (defined in crc.h) if the CRC checks correctly.
  
  NOTE: The caller is expected to mask the bottom 30 bits from the value, 

SIDE EFFECTS
  None

===========================================================================*/

dword crc_30_step
(
  dword seed,
    /* Either the result of a previous crc_30_step() or CRC_16_STEP_SEED
    ** the first time the routine is called.  Note that the input is
    ** inverted prior to use, to counteract the inversion applied when
    ** it was returned as a result of the previous step.
    */

  byte *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the MS bit of the first byte.
    */

  word len
    /* Number of data bits to calculate the CRC over */
)
{
  dword data, crc_30;

  /* Generate a CRC-30 by looking up the transformation in a table and
  ** XOR-ing it into the CRC, one byte at a time.
  */
  for (crc_30 = ~seed & 0x3FFFFFFF; len >= 8; len -= 8, buf_ptr++) {
    crc_30 = crc30_table[ ((crc_30 >> (30 - 8)) ^ *buf_ptr) & 0xff ]
                ^ (crc_30 << 8);
  }

  /* Finish calculating the CRC over the trailing data bits
  **
  ** XOR the MS bit of data with the MS bit of the CRC.
  ** Shift the CRC and data left 1 bit.
  ** If the XOR result is 1, XOR the generating polynomial in with the CRC.
  */
  if (len != 0) {

    data = ((dword) (*buf_ptr)) << (30-8); /* Align data MSB with CRC MSB */

    while (len-- != 0) {
      if ( ((crc_30 ^ data) & (1L << 29)) != 0 ) {       /* Is MSB of XOR a 1 */

        crc_30 <<= 1;                   /* Left shift CRC          */
        crc_30 ^= CRC_30_POLYNOMIAL;    /* XOR polynomial into CRC */

      } else {

        crc_30 <<= 1;                   /* Left shift CRC          */

      }

      data <<= 1;                       /* Left shift data         */
    }
  }

  crc_30 = ~crc_30;                     /* 1's complement the CRC     */

  return( crc_30 & 0x3FFFFFFF );        /* mask the result to 30 bits */

} /* end of crc_30_calc */


/*===========================================================================

FUNCTION CRC_16_STEP

DESCRIPTION
  This function calculates a 16-bit CRC over a specified number of data
  bytes.  It can be used to produce a CRC and to check a CRC.  The CRC value
  passed in is used to continue the CRC calculation from a previous call;
  this allows this routine to be used to CRC discontinuous data.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

word crc_16_step
(
  word seed,
    /* Either the result of a previous crc_16_step() or CRC_16_STEP_SEED
    ** the first time the routine is called.  Note that the input is
    ** inverted prior to use, to counteract the inversion applied when
    ** it was returned as a result of the previous step.
    */

  byte *buf_ptr,
    /* Pointer to bytes containing the data to CRC.  The bit stream starts
    ** in the MS bit of the first byte.
    */

  word byte_len
    /* Number of data bytes to calculate the CRC over */
)
{
  word crc_16;

  /* Generate a CRC-16 by looking up the transformation in a table and
  ** XOR-ing it into the CRC, one byte at a time.
  **
  ** Note: complement the seed to account for it having been complemented
  ** in a previous call to crc_16_step().
  */
  for (crc_16 = ~seed ; byte_len > 0; byte_len--, buf_ptr++) {
    crc_16 = (word)(crc16_table[ (crc_16 >> (16 - 8)) ^ *buf_ptr ] ^ (crc_16 << 8));
  }

  return( ~crc_16 );            /* return the 1's complement of the CRC */

} /* end of crc_16_step */



/*===========================================================================

FUNCTION CRC_16_L_STEP

DESCRIPTION
  This function calculates a LSB-first 16-bit CRC over a specified number 
  of data bytes.  It can be used to produce a CRC and to check a CRC.  
  The CRC value passed in is used to continue the CRC calculation from a
  previous call; this allows this routine to be used to CRC discontinuous data.

DEPENDENCIES
  None

RETURN VALUE
  Returns a word holding 16 bits which are the contents of the CRC
  register as calculated over the specified data bits.  If this
  function is being used to check a CRC, then the return value will be
  equal to CRC_16_OK (defined in crc.h) if the CRC checks correctly.

SIDE EFFECTS
  None

===========================================================================*/

word crc_16_l_step
(
  word seed,

  const void *buf_ptr,
    /* Either the result of a previous crc_16_l_step() or CRC_16_L_STEP_SEED
    ** the first time the routine is called.  Note that the input is
    ** inverted prior to use, to counteract the inversion applied when
    ** it was returned as a result of the previous step.
    */

  unsigned int byte_len
    /* Number of data bytes to calculate the CRC over */
)
{
  word crc_16;
  byte *byte_ptr = (byte *) buf_ptr;

  /* Generate a CRC-16 by looking up the transformation in a table and
  ** XOR-ing it into the CRC, one byte at a time.
  */
  for (crc_16 = ~seed ; byte_len > 0; byte_len--, byte_ptr++) {
    crc_16 = crc_16_l_table[ (crc_16 ^ *byte_ptr) & 0x00ff ] ^ (crc_16 >> 8);
  }

  return( ~crc_16 );            /* return the 1's complement of the CRC */

} /* end of crc_16_l_step */

