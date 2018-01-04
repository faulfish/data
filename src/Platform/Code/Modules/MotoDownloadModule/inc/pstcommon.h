//--------------------------------------------------------------------------------------------------
//
//   Module Name: pstcommon.h
//
//   General Description: A list of PST core APIs, Error messages, #defines for the core, usb
//                        descriptors and interfaces, and registry entries.
//
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2005 - 2010, All Rights Reserved
//
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Steve Scherer                10/30/01                    Added header
//
// Steve Scherer                04/24/02        LIBbb16228  Added interfaces and descriptors for Paragon Flash.
//
// Dan Murphy                   6/4/02          LIBbb34506  Added command to setalternate setting
//                                              LIBbb34518  Added command to switch the USB configuration
//                                                          and added FindInterfaceHandle
// Dan Murphy                   07/08/02        LIBbb41574  Added carrier commands and exposed security calls
//
// Dan Murphy                   08/27/02        LIBbb34506  Added 2 new USB configurations and USB configuration strings
//
// J Kotecki                    07/12/02        LIBbb19081  Added checking for returned faults
//                                                          in the UnInitialize function.
// Marc Risop                   10/30/2002      LIBbb58064  Adding string descriptors for EZX
//
//
// Dan Murphy                   11/26/02        LIBbb68970  Added Reg values for Multi flash tool
//
// Dan Murphy                   11/29/02        LIBbb58855  Added command to get the product descriptor
//
//
// Wen Yi                       10/31/2002      libbb61422  libbb61422_il93e_wenyi1_serial_scan
//                                                          Expose method to control serial device.                                                
// Wen Yi                       01/16/2003      libbb80098  libbb80098_il93e_wenyi1_post_serial_msg
//                                                          Post windows message when serial device
//                                                          is connected or removed.
// Wen Yi                       02/21/2003      libbb82987  libbb82987_il93e_wenyi1_no_wibu_for_ng_pst
//                                                          Add new registry base for SWDL
// Marc Risop                   04/04/03        LIBbb77562  Adding Configuration string descriptors for EZX
//                                                          configurations 16,17,and 18
// Mike Zarletti                07/31/2003      LIBdd25936  corrected tci error values
// Marc Risop                   09/24/03        LIBdd41733  Adding interface descriptors for multi-processor products
// Dan Murphy                   10-02-2003      LIBdd26017  Added default params for initialization and added Logger commands
//
// Marc Risop                   10/21/03        LIBdd20210  Fix Flash dialogue doesn't load all the time
// George Standish-WLGS03       04/20/04        LIBdd77795  Add PST message WM_PST_USB_BAD_DEVICE
// Dan Murphy                   2/25/04         LIBdd80837  Added string for Intel Bulverde Flash
// Dindo Jao                    07-28-04        LIBee39390  Added path for RSD Lite
// George Standish-WLGS03       6/30/04         LIBee10716  Added work modes for Socket
// Dan Murphy                   2/26/04                     Added SDK preprocessor definition
// Dan Murphy                   5-5-04          LIBee08063  Added Bulk endpoint support
// George Standish-WLGS03       05/28/04        LIBee10790  Remove PST API PSTCloseSerialDevice
// George Standish              4/20/04         LIBdd59890  Add PSTSetMDMDetect API
// Dindo Jao                    09/13/04        LIBff14170  Removed PSTGetSuperFileInfo

// Dindo Jao                    8/25/04         LIBdd66282  Added support for Bulverde
// Dindo Jao                    9/29/2004       LIBff00480  Added error code PST_CRC_READ_ERROR
//
// Zhongwei Qiu                 9/20/04         LIBee32601  Add JUIX_TCMD to ETCIProtocol and pszTCIJUIXCommandSets
// Zhongwei Qiu                 3/7/2005        LIBff59889  Expose Modem device
// Daniel Imamura               03/29/2005      LIBff59902  OBEX protocol support
// Dindo Jao                    04/15/05        LIBff35922  Added three functions for retrieving device interfaces. Added an array of flash names.
// Zhongwei Qiu                 05/19/05        LIBgg14435  Added support for Franklin test cmd
// Zhongwei Qiu                 05/23/05        LIBgg15473  Added support for application defined tcmd data file location.
// Zhongwei Qiu                 05/23/05        LIBgg16186  Added function for retrieving device id from hubname and port number.
// Zhongwei Qiu                 6/2/2005        Libgg22919  bug fix for the v2000 AUTO tcmd support and AP Data Logger
// Zhongwei Qiu                 06/13/05        LIBgg28703  Added function for releasing the interface handle.
// Daniel Imamura               06/15/05        LIBgg11057  Added new EPSTStatus and modified exposed OBEX APIs
// Wen Yi                       08/29/05        LIBgg25969  Make change for compiling pst_library.lib
// Dan Murphy                   07/11/2005      LIBgg47733  Added enum entry for no switching and no modem handle
// Dan Murphy                   08/10/05        LIBgg61023  Added V2000 support
// Dan Murphy                   08/16/05        LIBgg71757  Added Barbados MDM interface
// Daniel Imamura               07/22/05        LIBgg50195  Changed interface handle parameter from LPHANDLE to HANDLE in PSTOBEXInitialize/Ex
// Prashanth Ramachandran       08/30/05        LIBgg82025  Added enum entry and function for obtaining Interface Descriptor values 
// Prashanth Ramachandran       09/21/05        LIBhh01938  Added enum entry for transport type 
// Karthik Ramachandran         09/26/05        LIBgg95540  Added WM_PST_BLAN_DEVICE_ENUMERATION message.
// Karthik Ramachandran         09/27/05        LIBgg83548  Added an entry PST_BLAN_NOT_SUPPORTED to EPSTStatus .
// Tanya Radeva                 10/07/05        LIBhh12873  Added string entry for MCCI - AT-EN interface 
// Prashanth Ramachandran       10/17/05        LIBhh18227  Added overloaded GetProductStringDescriptor function for .NET users 
// Jagat Shah                   10/15/05        LIBhh26896  Added flash interface for AP file system
// Mike Zarletti                10/21/05        LIBhh17839  Added api PSTSetSwitchMode
// Prashanth Ramachandran       10/27/05        LIBhh25356  Added string entry for Motorola Data Interface (Net Mon ) phone  
// Prashanth Ramachandran       11/15/05        LIBhh32751  Added 2 Windows messages for detecting Found new hardware window
// Dindo Jao                    11/21/05        LIBgg22924  Added function definition to retrieve phone TCI protocol
// Prashanth Ramachandran       12/21/05        LIBhh33635  Changes for Alladin 
// Prashanth Ramachandran       01/31/06        LIBhh74790  workaround for MCPC dual mode 
// Dan Murphy                   09/01/05        LIBgg77374  Added support for TI ROM Flash
// Samir Khial                  02/15/06        LIBgg77383  Rework ODM Naked Flash support
// Prashanth Ramachandran       02/15/06        LIBhh92197  enum added for reading PST Settings     
// Prashanth Ramachandran       03/01/06        LIBhh89985  Configuration string for ACM Blan Added  
// Prashanth Ramachandran       03/24/06        LIBii24017  Added Motorola vendor id string     
// Prashanth Ramachandran       03/20/06        LIBii09921  Support for not switching MSD + Composite Configuration 
// Karthik Ramachandran         04/03/06        LIBii26505  Added support for MTP dual mode     
// Prashanth Ramachandran       04/07/06        LIBii07495  Modified WaitForDevice function     
// Daniel Imamura               03/09/06        LIBii14868  Including SML.dll registry value
// Samir Khial                  05/17/06        LIBii41706  Add Serial Flash API support
// Satya Calloji                05/21/06        LIBhh40320  Electra Changes  
// Prashanth Ramachandran       05/23/06        LIBgg56720  Clean up PST.dll configurations
// Samir Khial                  06/07/06        LIBii47509  Added support for Kenai dll
// Prashanth Ramachandran       06/19/06        LIBii60772  Added Intf enum for AP DataLogger 
// Prashanth Ramachandran       06/29/06        LIBjj08370  Added Intel Rom Vendor Name
// Samir Khial                  07/03/06        LIBii76588  Add ETM test command interface support
// Karthik Ramachandran         07/16/06        LIBjj07938  Added support for 3G test commands
// Karthik Ramachandran         07/24/06        LIBjj37554  Added TCI protocol max enum value

// Karthik Ramachandran         06/18/06        LIBii50247  Added new enum for device type 
// Samir Khial                  08/11/06        LIBjj58012  Decouple detection from PST 
// Karthik Ramachandran         08/23/06        LIBjj75992  Added new core API to get USB device descriptor
// Prashanth Ramachandran       08/22/06        LIBjj07708  Added support for Electra Recovery
// Samir Khial                  08/08/06        LIBii76569  Add D-program multi-interface support
// Karthik Ramachandran         09/06/06        LIBjj61109  Added support to specify ini file path
// Prashanth Ramachandran       09/05/06        LIBjj80965  Added enum PST_GENERIC_MEMORY_ALLOCATE_ERROR
// Karthik Ramachandran         08/02/06        LIBjj43453  Added new EPSTtatus for BLAN
// Samir Khial                  09/07/06        LIBjj47529  Changes for D-CDMA
// Karthik   Ramachandran       09/19/06        LIBkk01002  Added support to handle / ignore specific devices
// Guilherme Cincoetti          09/28/06        LIBkk10272  Added "ven_docomo" for Scorpius and Izar MSD.
// Prashanth Ramachandran       10/05/06        LIBjj74441  Added Maximum for USBConfigType
// Prashanth Ramachandran       11/07/06        LIBkk46333  Added PST_TCI_TDB_FILE_NOT_FOUND error status  
// Karthik Ramachandran         11/08/06        LIBkk34956  Added new API to get COM port name.
// Stefan Jez                   12/18/06        LIBkk83083  Changes needed for TI OMAP naked flash
// Samir Khial                  12/19/06        LIBkk61289  Fixed CDMA-D multi-up issue
// Kai Zhao                     01/26/07        LIBhh58330  Comment about PSTTestCmdInit is disabled.
// Fathi Ben Amar               01/26/07        LIBkk50194  FW: Handle memory overflow/orun
// Karthik Ramachandran         02/14/07        LIBll13914  Added test command options parameter to PSTInitStruct
// Fathi Ben Amar               02/14/07        LIBkk16139  FW: Keep only external usage details in PSTCommon.h
// Stefan Jez                   02/21/07        LIBkk39192  Renamed EMultiSessionType to ESessionType, added values to TRANSPORT_TYPE and ESessionType, added new Session type  
// Prashanth Ramachandran       02/22/07        LIBll04999  Added support for LJ test cmd 
// Daniel Lauzon                02/27/07        LIBkk18252  FW file unicode support
// Fathi Ben Amar               02/28/07        LIBjj61073  FW: Support ODM TCMDs (ETM / DTest)
// Karthik Ramachandran         03/22/07        libll60901  Added detection option to PSTInitialize struct, process device and ignore device APIs
// Prashanth Ramachandran       03/22/07        LIBll60901  Detection PST merge 
// Charles Ndikuryayo           05/04/07        LIBmm12437  FW: TI ROM driver setconfiguration issue with Lipari and Salina 
// Prashanth Ramachandran       04/24/07        LIBmm09715  PSTNotifyOnDeviceChange added parameters  
// Prashanth Ramachandran       05/23/07        LIBmm01293  Added PSTGetInterfaceMapping function
// Prashanth Ramachandran       06/04/07        LIBll91348  Added  ESessionType for USB Printing Mode

// Miguel Moretti               06/11/07        LIBmm51124  Replaced MODE_SOCKET to MODE_SIMULATOR
// Charles Ndikuryayo           06/14/07        LIBmm42987  FW: New API to find out serial device enumeration details in a port 
// Miguel Moretti               08/01/07        LIBmm65784  Removed non-secure APIs.
// Miguel Moretti               08/01/07        LIBmm65784  Removed non-secure APIs
// Miguel Moretti               08/14/07        LIBnn07016  Removed SDK_CALLER.
// Miguel Moretti               08/21/07        LIBnn10330  Add EPSTStatus value: PST_INSUFFICIENT_USER_BUFFER.
// Prashanth Ramachandran       09/06/07        LIBmm78281  Added new API's and structures for feature 18387   
// Xiang Ye                     10/31/07        LIBnn54585  Add EPSTStatus value: PST_TCI_POLL_DEVICE_TIMEOUT
// Xishang Ji                   01/02/08        LIBoo39139  Notify GUI of Ajar Locosto MSD device arrival
// Satya Calloji                02/13/08        LIBoo64343  Port #11008 support
// Xishang Ji                   02/20/08        LIBoo89242  Option for closing down other ports
// Xiang Ye                     04/29/08        LIBpp43149  Add support for TCMD Error String Response
// Lei Wu                       07/07/08        LIBqq24111  FW: Add support to switch mode for PST in detection mode
// Song Bao                     07/02/08        LIBpp22639  Add new devcie id option
// Xishang Ji                   05/26/08        LIBpp81949  DIAG command support
// Song Bao                     08/18/08        LIBqq68408  FW: User configurable TCMD timeout
// Song Bao                     08/15/08        LIBqq68408  FW: Add device type field in MotDeviceNotificationStruct
// Zhiyong Yang                 08/27/08        LIBqq62200  Flash: Add workaround for Ischia Zero-Length packet issue
// Song Bao                     09/04/08        LIBqq82196  Enable sending TCMD via opcode 
// Xiang Ye                     09/08/08        LIBpp43163  Handle new port-11009
// ZhiMing Leng                 10/17/08        LIBrr11791  Realese socket port per as device ID and handle
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------

#ifndef __PSTCommon_h__
#define __PSTCommon_h__

   #include <afxtempl.h>
   #include <PSTLogger.h>
   #include "PSTOBEXCommon.h"

   // defines
   #ifndef IN
      #define IN
   #endif

   #ifndef OUT
      #define OUT
   #endif


   // type definition
   typedef void (*DLLPROC)();
   typedef WORD DEVICEID;
   typedef CArray<DEVICEID, DEVICEID> CDeviceArray;
   typedef BYTE CONFIG;
   typedef CArray<CString, LPCTSTR> CPSTStringArray;

//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

    #define SWAP_WORD_ENDIANESS(wWord) wWord=((wWord & 0x00FF) << 8) | ((wWord & 0xFF00) >> 8) 
    #define SWAP_DWORD_ENDIANESS(dw) dw=((dw & 0xFF) << 24) | ((dw & 0xFF00) << 8) | ((dw & 0xFF0000) >> 8) | ((dw & 0xFF000000) >> 24)

    #define  DEVICE_NOTIFICATION_MIN_TIMEOUT 30000
//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------
   
    enum TEST_COMMAND_OPTIONS
    {
	    TCMD_OPT_NONE	      = 0x00,	
	    TCMD_OPT_TDB_INTERNAL = 0x01,	// use internal test command database (tdb)
	    TCMD_OPT_TDB_EXTERNAL = 0x02,	// use external test command database (tdb)
	    TCMD_OPT_TDB_INI	  = 0x03	// RECOMMENDED: refers to the pst.ini file to decide on internal or external. 
    };



   
   enum eDetectionNotificationType 
   {
      DETECTION_ARRIVAL ,
	  DETECTION_REMOVAL 
   };

   enum eDetectionTransportType
   {
      TRANSPORT_UNDEFINED=-1,
	  USB = 0,
	  USBLAN,
	  WUSB,
	  SERIAL,
	  BLUETOOTH,
	  ETHERNET,
	  IRDA,
	  PCMCIA,
	  FIRE_1394,
	  ALL = 0xFF
   };

   enum MOTAUTOSWITCHMODE {
      MOT_AUTOSWITCH_NO = 0,
      MOT_AUTOSWITCH_CONFIG8,
	  MOT_AUTOSWITCH_NO_SW_NO_MDM,
	  MOT_AUTOSWITCH_NEGLECT_MCPC_CFG11_MODEM,
	  MOT_AUTOSWITCH_DETECTION,
      MOT_AUTOSWITCH_DIAG
   };

   enum USBConfigType {
      USB_CFG_UNKNOWN = -1,
      USB_CFG_FLASH = 0,
      USB_CFG_CDC   = 1,
      USB_CFG_FULLCONFIG = 2,     //All possible interface exposed
      USB_CFG_DSP_LOGGER = 3,     //DSP Logger
      USB_CFG_CDC_DSP_DEBUG = 4,
      USB_CFG_MCU_DSP = 5,
      USB_CFG_TEST_CMD = 6,
      USB_CFG_DSP_DEBUG = 7,
      USB_CFG_TEST_CMD_ACC_MCU = 8, //Standard PST Configuration
      USB_CFG_MCU = 9,
      USB_CFG_PLUGFEST = 10,
        USB_CFG_CDC_USB = 11,       //CDC Communications and USB data logging (MCCI driver required)
      USB_CFG_TCMD_MCU_DSP = 12,   //MCU and DSP Data Logger + Test Commands
	  USB_CFG_USB_NDI = 13,
	  USB_CFG_USB_MSD = 14,
	  // LIBjj74441 - MTP responds to 18 in Vendor Command ( Using MoSwch)
	  USB_CFG_USB_MTP = 18,

	  
	  // Add Values Above this 
	  USB_CFG_MAX
   };

   enum REGISTERATION_MODE {
      PST_REG_NONE  = 0x00,
      PST_REG_LOG   = 0x01,
      PST_REG_NOTIF = 0x02,
      PST_REG_ALL   = 0x03
   };

   enum eSerialPhoneState
   {
        UNKNOWN_CMD_STATE = -1,
	    AT_CMD_STATE = 1,
	    TC_SWITCH_STATE = 2,
	    TEST_CMD_STATE = 3,
	    FLASH_STATE = 4,
	    TI_FLASH_STATE = 5
    };


	//interfaces supported by PST
	// please do not modify the order of interface.
	// if you need to add new interface/s please add it
	// before INTF_ALL
    // Important --> Has dependency with strMotInterfaceNames
    // Update strMotInterfaceNames if adding / deleting new interfaces 
	enum PST_INTERFACE
	{
        INTF_UNDEFINED,		//Unrecognized interface
        INTF_ACCESSORY,		//Motorola Accessory
        INTF_TCI,			//Motorola Test Command
        INTF_MCU_LOGGER,	//Motorola MCU Data Logger
        INTF_DSP_LOGGER,	//Motorola DSP Logger
		// Start of Flash Interface 
		INTF_FLASH_AP,		//Motorola AP Flash 
        INTF_FLASH,			//Motorola Flash
        INTF_BOOT,			//Motorola Boot
        INTF_FLASH_OS,		//Motorola Paragon OS
        INTF_FLASH_FS,		//Motorola Paragon Filesystem
        INTF_FLASH_BP,		//Motorola Paragon Baseband
        INTF_EZX_OS,		//Motorola EZX OS
        INTF_BLVRD_BOOT,	//Motorola Bulverde Naked
		INTF_FLASH_AP_FS,	//Motorola AP Filesyste
		INTF_SECURE_FLASH_AP,	//Motorola AP Filesyste
		// Add new Flash Interface here 
		
		// End of Flash interface Names 

		// Add new Non Flash Interface here 
		INTF_AP_DATA_LOGGER,//Motorola AP Data Logger				
		INTF_MODEM,			//Modem Interface
		INTF_MSD,           //Mass Storage
		INTF_MTP,           //Media Sync
		INTF_PTP,           //USB Priting, Image transfer etc.
        INTF_QC,            // QC interface 
		INTF_GKI_DATA_LOGGER,	// GKI Data Logger
        INTF_DIAG,          //Diagnostic Command interface

		INTF_SWITCH_UNSUPPORTED,   //interface that do not support auto mode switching 
		INTF_ALL = 255			//All interfaces available

   };

	
    // Interface enum to string mapping 
	typedef struct _INTF_STRING_MAP
	{
		enum PST_INTERFACE eInterface ;
		CString  strInterfaceName ;
		
		_INTF_STRING_MAP()
		{
			eInterface = INTF_UNDEFINED;			
		} ; 

		_INTF_STRING_MAP(const enum PST_INTERFACE &eIntf , const CString &strIntf)
		{
			eInterface = eIntf ; 
			strInterfaceName=strIntf;
		};


	} INTF_STRING_MAP ;



	// Class that provides status / Error information in the form of an out object 
	class CStatusError
	{
	public :
		CString strMessage ; 
		UINT uErrorNumber ; 
		UINT8 uDirection; 
		CStatusError():uErrorNumber(0),uDirection(0)
		{
		} ; 
		void Clear()
		{ 
			strMessage.Empty();
			uErrorNumber = 0;
			uDirection =0;
		}; 	
	};  


    const LPCTSTR pszInterfaceAccessory = _T("Motorola Accessory");
    const LPCTSTR pszInterfaceTCI = _T("Motorola Test Command");
    const LPCTSTR pszInterfaceMCUDataLogger = _T("Motorola MCU Data Logger");
    const LPCTSTR pszInterfaceDSPLogger = _T("Motorola DSP Logger");
    const LPCTSTR pszInterfaceFlash = _T("Motorola Flash");
    const LPCTSTR pszInterfaceBoot = _T("Motorola Boot");
    const LPCTSTR pszInterfaceA920_OS = _T("Motorola Paragon OS");
    const LPCTSTR pszInterfaceA920_FS = _T("Motorola Paragon Filesystem");
    const LPCTSTR pszInterfaceA920_BP = _T("Motorola Paragon Baseband");
    const LPCTSTR pszInterfaceEZX_OS = _T("Motorola Ezx AP Flash");
    const LPCTSTR pszInterfaceSecureBP = _T("Secure Motorola Flash");
    const LPCTSTR pszInterfaceSecureBootBP = _T("Secure Motorola Boot");
    const LPCTSTR pszInterfaceIntelFlashBulverde = _T("FLASHBVY"); // intel bulverde naked boot flash
    const LPCTSTR pszInterfaceAPFlash = _T("Motorola AP Flash");
	const LPCTSTR pszInterfaceAPDataLogger = _T("Motorola AP Data Logger");
	const LPCTSTR pszInterfaceComInterface = _T("Motorola Communication Interface");
	const LPCTSTR pszInterfaceMCPCATCom    = _T("Motorola AT Comm Interface");
	const LPCTSTR pszInterfaceMCPCATCom2   = _T("Motorola EN-AT Communication Interface");
	const LPCTSTR pszInterfaceMCPCOBEX     = _T("Motorola OBEX Communication Interface");
	const LPCTSTR pszInterfaceCom          = _T("Comm Intf");
	const LPCTSTR pszInterfaceAPFS = _T("Motorola AP Filesystem");
	const LPCTSTR pszInterfaceMotDataIntf  = _T("Motorola Data Interface");
    const LPCTSTR pszInterfaceFOMAComlMdm = _T("FOMA Commerical Modem"); 
    const LPCTSTR pszInterfaceTIMDM = _T("TI Modem Interface");
	const LPCTSTR pszInterfaceSecureFlash = _T("Secure Motorola AP Flash");
	const LPCTSTR pszInterfaceQC = _T("QC Interface");
	const LPCTSTR pszInterfaceGKIDataLogger = _T("GKI Data Logger");

    const DWORD dwUseDefaultTcmdTimeout = 0xfffffff;

   enum TRANSPORT_TYPE
   {
      PST_USB         = 0x0001,
      PST_SERIAL      = 0x0002, 
	  PST_USB_TCP     = 0x0004,
	  TRANS_UNDEFINED = 0x0000
   };

   // USB packet types
   enum PCKTYPE
   {
      PCK_DATA_CONTROL     = 0,
      PCK_DATA_ISOCHRONOUS = 1,
      PCK_DATA_BULK        = 2,
      PCK_DATA_INTERRUPT   = 3,
      PCK_COMMAND
   };

   // USB descriptor type
   enum DESCRTYPE
   {
      // Device String Descriptor TYPE | SUBTYPE
      DESCR_DEVICE_MANUFACTURER  =0x0000, // Device String Descriptor: Manufacturer
      DESCR_DEVICE_PRODUCT    =0x0001, // Device String Descriptor: Product
      DESCR_DEVICE_SERIAL_NUMBER =0x0002, // Device String Descriptor: Serial Number

      DESCR_CONFIGURATION        =0x0100, // Configuration String Descriptor
      DESCR_INTERFACE            =0x0200, // Interface String Descriptor
      DESCR_DIRECT_STRING        =0x0300, // Direct String
   };

   //TCI protocol type
   enum ETCIProtocol
   {
      P2K_USB,
      P2K_SCI,
      LEGACY_CDMA_SCI,
      LEGACY_TDMA_SCI,
      JUIX_TCMD,
	  P2K_TCP,
      ETM_TCMD,
	  AUTO ,
      P2K_3G_USB,
	  LJ_TCMD,
      VALID_MAX_VALUE,  //add new entries above this one. This is used for validation
	  TCI_UNDEFINED = 0xff
   };

   //Diagnostic protocol type
   enum EDiagProtocol
   {
       DIAG_PROT_LEGACY,

       DIAG_PROT_MAX_VALUE, //add new entries above this one
       DIAG_PROT_UNDEFINED = 0XFF 
   };

   // Serial communication
   enum BaudRateEnum
   {
      Baud_None = 0,
      Baud_110 = CBR_110,
      Baud_300 = CBR_300,
      Baud_600 = CBR_600,
      Baud_1200 = CBR_1200,
      Baud_2400 = CBR_2400,
      Baud_4800 = CBR_4800,
      Baud_9600 = CBR_9600,
      Baud_14400 = CBR_14400,
      Baud_19200 = CBR_19200,
      Baud_28800 = 28800,
      Baud_38400 = CBR_38400,
      Baud_56000 = CBR_56000,
      Baud_57600 = CBR_57600,
      //Baud_76800 = CBR_76800, // Microsoft does not support 76800 Baudrate
      Baud_115200 = CBR_115200,
      Baud_128000 = CBR_128000,
      Baud_203125 = 203125,
      Baud_230400 = 230400,
      Baud_256000 = CBR_256000,
      Baud_406250 = 406250,
      Baud_460800 = 460800,
	  Baud_812500 = 812500,
	  Baud_921600 = 921600
   };

   typedef struct SerialDeviceEnumDetails
   {
	eSerialPhoneState CurrentDeviceEnumState;
	BaudRateEnum  CurrentDeviceBaudRate;
	
	SerialDeviceEnumDetails()
	{
        CurrentDeviceEnumState= UNKNOWN_CMD_STATE;
        CurrentDeviceBaudRate= Baud_None;	
	}

   } SERIAL_DEV_ENUM_DETAILS ;

/* ******************************************************************************************************************************* */
// PST Work Modes
enum PST_WORK_MODE
{
    MODE_NONE                       =0x00,          // No mode is selected
    MODE_USB       =0x01,         // Enable work on USB only
    MODE_SERIAL      =0x02,         // Enable work over Serial only
    MODE_SIMULATOR     =0x04,          // Enable work on Socket for simulator
    MODE_NETWORK         =0x08,         // Enable work on network only
    MODE_DETECTION_OFF              =0x10,          // Disable driver notifications

    // Mode combinations
    MODE_USB_SERIAL                 =0x03,          // Enable work on USB + Serial
    MODE_USB_SERIAL_SOCKET          =0x07,          // Enable work on USB + Serial + Socket
    MODE_USB_NETWORK                =0x09,          // Enable work on NETWORK over USB
    MODE_USB_SERIAL_SOCKET_NETWORK  =0x0F,          // Enable work on USB + Serial + Socket + Network over USB
    MODE_ALL_NO_SERIAL              =0x0D,          // Enable work for all modes except Serial
    MODE_ALL                        =0x0F ,          // Enable work for all modes 



};


// option for Device Id Mode 
enum PST_DEVICEID_OPTION
{
    // Configuration Modes
    MODE_DEVICEID_CONFIGURATION     = 0x01,     // Enable work in Device ID Configuration Mode only 

    // Operational Modes 
    MODE_DEVICEID_OPERATION_AUTO    = 0x02,     // Automatic Configuration of Device ID 
    MODE_DEVICEID_OPERATION_MANUAL  = 0x04,     // Use Config Data to decide device id 
    MODE_DEVICEID_OPERATION_LEGACY  = 0x08,     // Legacy mechanism for device id allocation     
    MODE_DEVICEID_OPERATION_PASSIVE = 0x10,     // User module is responsible for the device id allocation
                                                // PST just relies on the MotDevNotificationStruct.eDeviceId 
                                                // passed in by PSTNotifyOnDeviceChange
    
    // Set this as the last value ( 1 greater than the previous )
    MODE_DEVICEID_MAX_VALUE
};


//Different detection options available
enum PST_DETECTION_MODE
{
    DETECT_ALL,                 // by default will detect all devices based on the work mode specified
    DETECT_NONE,                // by default will ignore all devices.
    //add new memebers here
	DETECT_DETECTION_MODE ,     // mode to use PST Dll in Detection Mode 

    DETECT_UNDEFINED
};


// Used to turn off serial
const int MODE_SERIAL_OFF           = 0xFD;


// Enum type for Interface descriptor values
enum USBInterfaceDescEnumType 
{
	bInterfaceNumber,
	bAlternateSetting,
	bNumEndpoints,
	bInterfaceClass,
	bInterfaceSubClass,
	bInterfaceProtocol,
	iInterface
};

// Device ID specific 
const UINT MIN_DEVICE_ID        = 0;
const UINT USB_MIN_DEVICE_ID    = 0;
const UINT USB_MAX_DEVICE_ID    = 999;
const UINT COM_MIN_DEVICE_ID    = 1000;
const UINT COM_MAX_DEVICE_ID    = 1999;
const UINT MAX_DEVICE_ID        = 1999;
const UINT NO_DEVICE_ID         = 0xFFFF;

// return codes EPSTStatus
enum EPSTStatus
{
   PST_OK                  =0,
   PST_INTERNAL_ERROR,                                // PST encountered an internal error
   PST_NOT_SUPPORTED,                                 // command is not supported
   PST_WRONG_PARAM_NUMBER,                            // function was called with the wrong number of parameters
   PST_INVALID_PARAM,                                 // function was called incorrect value of parameter/s
   PST_MEM_ALLOC_FAILURE,                              // memory allocation failed
   PST_REGISTRY_READ_ERROR,                           // could not open keys or read values in the registry
   PST_DEVICE_NOT_FOUND,                              // could not find the specified device
   PST_DEVICE_READ_ERROR,                             // SERIAL only - could not read data from the device
   PST_DEVICE_WRITE_ERROR,                            // could not write data to the device
   PST_DEVICE_ABORT_ERROR,                            // could not abort the device
   PST_DLL_LOAD_ERROR,                                // could not load DLL
   PST_DLL_NOT_PST,                                // loaded DLL is not PST core (does not implement standard PST interface)
   PST_DLL_NOT_TCI,                                // loaded DLL is not a TCI (does not implement standard TCI interface)
   PST_DLL_NOT_UT,                                    // loaded DLL is not a User Tool (does not implement standard UT interface)
   PST_TCI_INTERNAL_ERROR,                            // TCI encountered an internal error
   PST_UT_INTERNAL_ERROR,                             // User Tool encountered an internal error
   PST_UT_RUNNING,                                    // User Tool is running and can not be stopped
   PST_UT_NOT_RUNNING,                                // User Tool is not running and cannot process the command
   PST_SECURITY_INVALID,                              // Security validation failed
   PST_30DAYS_TIMEOUT,                                // For the Alpha version, PST Phase 1PST_TCI_SCI_CHECKSUM_ERROR,
   PST_TCI_SCI_CHECKSUM_ERROR,
   PST_TCI_SCI_BYTERECEIVED_TIMEOUT_ERROR,
   PST_TCI_COMMUNICATION_RECEIVE_ERROR,
   PST_TCI_SCI_FRAMING_TIMEOUT_ERROR,
   PST_TCI_NOISE_ONLINE_ERROR,
   PST_TCI_OVERRUN_ERROR,
   PST_TCI_INVALID_DATA_LENGTH_FOR_COMMAND,
   PST_TCI_INADEQATE_SECURITY_LEVEL,
   PST_TCI_COMMAND_PARAMETER_NOT_SUPPORT_BY_PROTOCOL,
   PST_TCI_COMMAND_NOT_SUPPORT_BY_MODE,               
   PST_TCI_COMMAND_UNSUPPORT_OPCODE,        
   PST_TCI_COMMAND_UNSUPPORT_PARAMETER_FOR_OPCODE,
   PST_TCI_GENERIC_RESPONSE,	
   PST_TCI_GENERIC_FAILURE,
   PST_TCI_ALLOCATE_MEMORY_FAILURE,
   PST_TCI_INTERNAL_TASK_ERROR,
   PST_TCI_TEST_COMMAND_TASK_TIMEOUT,
   PST_TCI_MODE_ERROR,
   PST_TCI_CMD_HEADER_LNGTH,
   PST_TCI_PHONE_LOST,
   PST_TCI_ERROR_STRING_RETURNED,
   PST_TCI_PACKET_BYTE_MISMATCH_FAILURE,
   PST_TCI_INTERFACE_NOT_INITIALIZE,
   PST_TCI_QUEUED_DATA_REQUEST_FAILURE,
   PST_TCI_PULL_DEVICE_UNSUCCESSFUL,
   PST_TCI_TECHNOLOGY_SELECTED_COMMUNICATION_ERROR,
   PST_TCI_SELECTED_PROTOCOL_NOT_FOUND,
   PST_TCI_TEST_COMMAND_NOT_FOUND_IN_TABLE,
   PST_TCI_SUPPORT_DATA_AMOUNT_EXCEED,
   PST_TCI_COMMAND_NEED_MORE_SUPPORT_DATA,
   PST_TCI_COMMAND_HISTORY_BUFFER_SIZE_EXCEEDED,
   PST_TCI_INSTANCE_NOT_FOUND,
   PST_TCI_MAXIUM_NUMBER_INSTANCE_REACHED,
   PST_TCI_NOT_RUNNING,                            // TCI is not running and cannot process the command
   PST_TCI_TDB_FILE_NOT_FOUND,                     // TCI Database file ( .tdb) file not found  
   PST_FLASH_FAIL,
   PST_COM_INIT_FAILED,                            // PST is not able to initialize COM enviroment
   PST_WIBU_INIT_FAILED,                              // PST is not able to initialize WIBU component
   PST_INVALID_WIBU_VER,                              // Invalid WIBU driver, version 3.0 is required
   PST_INVALID_WIBU_KEY,                              // Invalid WIBU key/ WIBU key is not present
   PST_UNSPECIFIED_SECURITY_ERROR,                       // Unspecified PST security error
   PST_DEVICE_READ_WIN32_ERROR,                       // USB only - could not read data from the device - problems with OVERLAPPED structure
   PST_DEVICE_READ_TIMEOUT_ERROR,                        // USB only - did not receive any data from the device in the specified interval
   PST_DEVICE_READ_COMMUNICATIONS_ERROR,                 // USB only - could not correctly retrieve data read from the device
   PST_DEVICE_READ_INVALID_HANDLE_ERROR,                 // USB only - device disconnected and eventually reconnected on the same port
   PST_DEVICE_READ_SERIAL_NULL_INPUT_ERROR,              // SERIAL only - one of the input pointers specified when called ReadData is NULL
   PST_DEVICE_READ_SERIAL_MAX_INPUT_ERROR,                  // SERIAL only - the input buffer specified when called ReadData exceeds max. length
   PST_DEVICE_READ_SERIAL_INVALID_HANDLE_ERROR,          // SERIAL only - the handle to the device is invalid
   PST_DEVICE_READ_SERIAL_PARITY_ERROR,                  // SERIAL only - the hardware detected a parity error
   PST_DEVICE_READ_SERIAL_FRAME_ERROR,                   // SERIAL only - the hardware detected a framing error
   PST_DEVICE_READ_SERIAL_OVERRUN_ERROR,                 // SERIAL only - a character-buffer overrun has occurred
   PST_DEVICE_READ_SERIAL_OVERFLOW_ERROR,                // SERIAL only - an input buffer overflow has occurred; there is either no room in the input buffer, or a character was received after the end-of-file (EOF) character
   PST_DEVICE_READ_SERIAL_OTHER_ERROR,                   // SERIAL only - any other error than those already defined occured
   PST_DEVICE_READ_SERIAL_INVALID_PORT_ERROR,            // DATALOGGER on SERIAL only - the name of COM port provided is wrong
   PST_DEVICE_INVALID_INTERFACE_HANDLE_ERROR,            // PST could not retrieve a valid handle for specified device/interface
   PST_DEVICE_INTERFACE_NOT_FOUND,                       // Specified interface is not found in the given device

   //the following error codes were defined as result of the new architecture
   PST_COM_NOT_ENOUGH_MEMORY,                         // not enough memory
   PST_COM_LOCKED_DEVICE,                             // the device requested is locked by other UT
   PST_COM_UT_NOT_REGISTERED,                         // the indicated UT is not registered
   PST_COM_CANNOT_CREATE_GUID,                           // cannot create a new GUID
   PST_COM_INVALID_ENUMERATION_VALUE,                    // invalid value for enumeration
   PST_COM_CANNOT_CREATE_PST_SERVER,                     // cannot create PST COM server
   PST_COM_UNEXPECTED_ERROR,                          // unexpected error

       //The following errors are a result of USB issues
       PST_INVAILD_CONFIGURATION,                                  //USB Configuration is not supported by the core
       PST_CONFIGURATION_FAILED,                                    //If the command failed as a result of a USB error
       PST_ALTERNATE_SETTING_FAILED,

   PST_STOP_THREAD_ERROR,                             // failed to stop the thread with SetEvent
   PST_WAIT_ABANDONED_ERROR,                          // WaitForSingleObject timer abandoned
   PST_WAIT_TIMEDOUT_ERROR,                           // WaitForSingleObject timer timed out
   PST_WAIT_FAILED_ERROR,                             // WaitForSingleObject timer failed
    PST_CRC_READ_ERROR,                                // CRC read error
   PST_BLAN_GETIPADDRESS_ERROR,                       // Blan Get IP address failed
   PST_BLAN_SOCKET_CREATION_ERROR,                       // Blan Socket Creation failed
   PST_BLAN_SOCKET_CONNECTION_ERROR,                     // Blan Socket Connection failed
   PST_BLAN_SOCKET_WRITE_ERROR,                       // Blan Socket Write failed
   PST_BLAN_SOCKET_READ_ERROR,                         // Blan Socket Read failed.
   PST_BLAN_NOT_SUPPORTED,                               // The device does not support BLAN
   PST_BLAN_GET_ADAPTER_INFO_FAILED,
   PST_BLAN_DHCP_SERVER_NOT_REACHABLE

   // OBEX related status
   , PST_OBEX_CONTINUE                 // Indicates that the operation still continues
   , PST_OBEX_PRECONDITION_FAILED      // Pre-conditions required for execution are not satisfied
   , PST_OBEX_INVALID_PARAMETER        // One or more parameters passed are invalid
   , PST_OBEX_INVALID_DEVICE           // The device Id passed is invalid
   , PST_OBEX_INVALID_INTERFACE_HANDLE // The interface handle passed is invalid
   , PST_OBEX_INTERNAL_ERROR           // An unespecified error occured during the processing
   , PST_OBEX_TIMEOUT                  // Read/Write timeout occured in transfer operation
   , PST_OBEX_CREATED                  // Object created
   , PST_OBEX_ACCEPTED                 // Operation accepted
   , PST_OBEX_UNAUTHORIZED             // Operation unauthorized
   , PST_OBEX_REFUSED                  // Operation understood but refused
   , PST_OBEX_NOT_IMPLEMENTED          // Operation not implemented
   , PST_OBEX_INTERNAL_SERVER_ERROR    // OBEX internal server error

   , PST_INVALID_BUFFER_ERROR          // error when NULL pointer passed to GetProductStringDescriptorChar function 
   , PST_MANAGER_NOT_INITIALIZED       // If instance of CPSTSessionMgr is requested and the class is not initialized
    // INI related 
   , PST_SETTINGS_INITIALIZED 
   , PST_SETTINGS_NOT_INITIALIZED
   , PST_SETTINGS_INIT_FAILURE 
   , PST_SETTINGS_READ_FAILED 
   , PST_SETTINGS_ENTRY_NOT_FOUND 
   , PST_SETTINGS_INI_FILE_NOT_FOUND
   , PST_SETTINGS_WRITE_FAILURE

   , PST_SUPERVISOR_NOT_INITIALIZED   // If instance of CPSTSupervisor is requested and the class is not initialized
   , PST_NOT_INITIALIZED              // If PST is not initialized yet or was PST is queried after been unitialized
   , PST_DEVICE_IN_CHARGING_MODE      // If device is in charging mode and that is the cause of failure.
   , PST_DETECTION_DISABLED_ERROR     // If driver notification and device detection is not enabled
   , PST_CHILD_DEVICE_NOT_FOUND	      // if a specified child device( Interface) is not present 
   , PST_DEVICE_IN_FLASH_MODE         // Device is in flash mode , use in conjunction with PSTSwitchToRecoveryMode 
   , PST_FAILED_HCD_DETECTION		  // Unable to detect Host Controller Device , use in conjunction with Electra devices (PSTSwitchToRecoveryMode)
   , PST_GENERIC_MEMORY_ALLOCATE_ERROR // Heap memory allocation failure 
   , PST_FLASH_SWITCH_ERROR
   , PST_ETM_FLASH_ERROR
   , PST_ETM_RESET_ERROR
   , PST_TCI_INVALID_BUFFER_SIZE
   , PST_TCI_POLL_DEVICE_TIMEOUT
   , PST_TCI_INVALID_ERROR_STRING_BUFFER_SIZE
   , PST_TCI_ETM_FINISHED
   , PST_TCI_ETM_OK_MORE
   , PST_TCI_ETM_PACKET
   , PST_TCI_ETM_NOSYS
   , PST_TCI_ETM_FATAL
   , PST_TCI_ETM_BADOP
   , PST_TCI_ETM_AGAIN
   , PST_TCI_ETM_INVAL
   , PST_TCI_ETM_NOMEM
   , PST_TCI_ETM_MESSAGE
   , PST_TCI_ETM_RV_FATAL
   , PST_TCI_ETM_RV_NOT_SUPPORTED
   , PST_TCI_ETM_RV_MEMORY
   , PST_DEVICE_NOTIFICATION_FAILURE	// Device notification message could not be processed.
   , PST_UNRECOGNIZED_INTERFACE
   , PST_INSUFFICIENT_USER_BUFFER       // When user supplied buffer is small 
   , PST_API_DEPRECATED                 // Returned by API which aren't maintained anymore. 
	
   // Detection Specific 
   ,PST_DETECTION_INVALID_MODE 
   ,PST_DETECTION_FAILURE 
   ,PST_DETECTION_SCANNING_IN_PROGRESS
   ,PST_DETECTION_SCANNING_STARTED
   ,PST_DETECTION_SCANNING_COMPLETE
   ,PST_DETECTION_INTERNAL_ERROR
   ,PST_DETECTION_CALLBACK_ADDED
   ,PST_DETECTION_CALLBACK_EXISTS
   ,PST_DETECTION_CALLBACK_ADDITION_FAILED
   ,PST_DETECTION_CALLBACK_REMOVAL_FAILED
   

   // Device ID Specfic 
   , PST_DEVICEID_NOT_SPECIFIED
   , PST_DEVICEID_ID_EXISTS  
   , PST_DEVICEID_ID_NOT_FOUND

   //DIAG command specific
   , PST_DIAG_BAD_COMMAND
   , PST_DIAG_BAD_PARAMETERS
   , PST_DIAG_BAD_LENGTH
   , PST_DIAG_BAD_MODE
   , PST_DIAG_COMMAND_NOT_IN_TABLE

   , PST_WIN32_ERROR
   
};

    typedef struct MotDevNotificationStruct
    {
	    LPTSTR pStrSymLink ;
	    LPTSTR pStrProductString;
        GUID guid;
	    eDetectionNotificationType eNotificationType;
	    eDetectionTransportType eTransportType; 
        DEVICEID eDeviceId;
		bool bWaitTillFinished;
		DWORD dwTimeout;
		UINT iDeviceType;
        
        MotDevNotificationStruct()
        {
            eDeviceId = NO_DEVICE_ID;
			bWaitTillFinished = false ;
			dwTimeout = DEVICE_NOTIFICATION_MIN_TIMEOUT;
			iDeviceType = 0;
        }
               
    } MotDeviceNotificationStruct ; 


enum ESessionType 
{
	SESSION_INVALID     = 0x0000,
    SESSION_MULTI       = 0x0001, // only an indicator of multi session, not really a type
        
    SESSION_MODEM       = 0x0002,
	SESSION_ALLADIN     = 0x0004,
	SESSION_NDI         = 0x0008,
	SESSION_P2K         = 0x0010,
	SESSION_FLASH       = 0x0020,
    SESSION_HC_MODEM    = 0x0040,
	SESSION_MSD         = 0x0080,
	SESSION_MTP         = 0x0100,
    SESSION_SERIAL      = 0x0200,
	SESSION_USB_PRINTING = 0x0400
};

    
typedef struct DeviceConnectivityInfoStruct
{
    unsigned int iSessionType; // used to OR values of type ESessionType    
    unsigned int iTransType;   // used to OR values of type TRANSPORT_TYPE
      
    DeviceConnectivityInfoStruct()
    {
        iSessionType = SESSION_INVALID;
        iTransType   = TRANS_UNDEFINED;
    }  
} DeviceConnectivityInfoStruct;

enum BLAN_PORT_STATUS
{
	PORT_STATUS_IGNORE  = 0,   //do not open this port
	PORT_STATUS_OPEN    = 1,   //open this port
	PORT_STATUS_EXCLUSIVE = 2  //open this port ONLY
};

typedef struct tagBlanPortStatusStruct
{
	PST_INTERFACE eIntfType;
	UINT iPort;
	BLAN_PORT_STATUS eStatus;
}BlanPortStatusStruct;

typedef CList<BlanPortStatusStruct, BlanPortStatusStruct&> CBlanPortStatusList;

// Typedef's 
typedef void (CALLBACK *DETECTION_CALLBACK ) (void * pCallBackObject , const MotDeviceNotificationStruct *pDeviceNotification );

typedef struct _MotCallBackParams
{
	void * pCallBackObject ;
	DETECTION_CALLBACK pCallBackFunc ; 

	_MotCallBackParams()
	{
		pCallBackObject = NULL ; 
		pCallBackFunc = NULL ; 
	};

	bool IsValid() const 
	{ 
		return ( pCallBackObject!=NULL && pCallBackFunc!=NULL) ;
	}; 
	
} MotCallBackParams ;


typedef struct  _DetectionParams 
{
	MotCallBackParams motCallBackParams ; 

}DetectionParams ; 




/************************************************************************/
/* PST_USB_DEVICE_DESCRIPTOR is meant to handle USB device descriptors.
/* This may not be supported for all device types.
/* String arrays in this structure are of size 255 per the USB spec.
/************************************************************************/  
   typedef struct _PST_USB_DEVICE_DESCRIPTOR{
    USHORT  bcdUSB;             //Identifies the version of the USB specification that this descriptor structure complies with. This value is a binary-coded decimal number
    UCHAR   bDeviceClass;       //Specifies the class code of the device as assigned by the USB specification group
    UCHAR   bDeviceSubClass;    //Specifies the subclass code of the device as assigned by the USB specification group
    UCHAR   bDeviceProtocol;    //Specifies the protocol code of the device as assigned by the USB specification group
    UCHAR   bMaxPacketSize0;    //Specifies the maximum packet size, in bytes, for endpoint zero of the device. The value will be 8, 16, 32, or 64
    USHORT  idVendor;           //Specifies the vendor identifier for the device as assigned by the USB specification committee
    USHORT  idProduct;          //Specifies the product identifier. This value is assigned by the manufacturer and is device-specific
    USHORT  bcdDevice;          //Identifies the version of the device. This value is a binary-coded decimal number
    char    szManufacturer[255];//Provides a string containing the name of the manufacturer of this device
    char    szProduct[255] ;    //Provides a string that contains a description of the device      
    char    szSerialNumber[255];//Provides a string that contains a manufacturer-determined serial number for the device
    UCHAR   bNumConfigurations; //Specifies the total number of possible configurations for the device
    }PST_USB_DEVICE_DESCRIPTOR; 


//*****************************************************************************
//  PSTInitializeStruct is used to pass input to PSTInitialize 
//  All the members will be initialized in the constructor.
//  Users should set the structure members they intend to use to desired value
//  and pass the structure to the PSTInitialie API.
//*****************************************************************************    
    typedef struct PSTInitStruct
    {
     PST_WORK_MODE      eWorkMode;              // Specifies the work mode
     HWND               hAppHandle;             // Handle to the application
     REGISTERATION_MODE eRegMode;               // Registration mode. 
     MOTAUTOSWITCHMODE  eSwitch;                //Switching option
     wchar_t            szPstIniPath[SHRT_MAX]; //Absolute path of the pst ini file to use.           
     TEST_COMMAND_OPTIONS eTcmdOption;		    //  Options for test command data base usage, etc
     PST_DETECTION_MODE   eDetectionOption;      //  Detection mode
	 DetectionParams     detectionParams ; 
	 PST_DEVICEID_OPTION eDeviceIdOption ;       // PST Device ID Option    
	 

     PSTInitStruct()
     {
        eWorkMode   = MODE_NONE;
        hAppHandle = NULL;
        eRegMode = PST_REG_NONE; 
        eSwitch = MOT_AUTOSWITCH_CONFIG8;
        memset(szPstIniPath, 0, SHRT_MAX * sizeof(wchar_t));
	    eTcmdOption = TCMD_OPT_TDB_INI;
        eDetectionOption = DETECT_ALL;
     }

    }PSTInitializeStruct;


    // Filter for type of device 
	enum USB_DEVICE_MASK 
	{
		USB_NONE =0x01,
		USB_HCD  =0x02,
		USB_HUB  =0x04,
		USB_DEVICE=0x08,

		USB_ALL = 0xFF 		 
	};


	// HCD type - universal or enhanced 
	enum HCD_TYPE 
	{
		HCD_INVALID = 0,
		HCD_USB1=1,
		HCD_USB2=2,
		HCD_ELECTRA=3 
	} ; 

	// Hub type ( root or generic hub)
	enum HUB_TYPE 
	{
		HUB_INVALID = 0,
		HUB_ROOT_HUB=1,
		HUB_GENERIC_HUB=2,
		HUB_ELECTRA=3

	}; 

	
	// structure that contains the topology id for a device 
	// str_tid - represents the topology id for the device ( hub.port) eg 3.2 
	typedef struct _USB_PORT_INFO
	{
		CString  str_tid;
		CString  str_mappedTid ; 
		CString strProductDescriptor;
		CString strSerialNum ; 
		DEVICEID deviceId ; 
		USHORT  idVendor;
		USHORT  idProduct;


		_USB_PORT_INFO()
		{
			deviceId = -1 ; 
			idVendor =  0xFFFF ; 
			idProduct = 0xFFFF ; 
		}

		void Clear()
		{
			str_tid.Empty();
			str_mappedTid.Empty();
			strProductDescriptor.Empty();
			strSerialNum.Empty();
			deviceId = -1;
			idVendor = 0xFFFF ; 
			idProduct = 0xFFFF; 

		}


		// Function name	: IsSameDevice
		// Description	    : 
		// Return type		: bool 
		// Argument         : const _USB_PORT_INFO &rhs
		bool IsSameDevice(const _USB_PORT_INFO &rhs)
		{
			if(idVendor==0xFFFF   || idProduct == 0xFFFF)
				return false ; 

			if((idVendor == rhs.idVendor) && (idProduct == rhs.idProduct)
				&& (strSerialNum.CompareNoCase(rhs.strSerialNum)==0)
				&& (strProductDescriptor.CompareNoCase(rhs.strProductDescriptor)==0))
			{
				return true ; 
				
			}
			return false ; 
		} ; 


		// Function name	: operator=
		// Description	    : Assignment operator 
		// Return type		: _USB_PORT_INFO& 
		// Argument         : const _USB_PORT_INFO &rhs
		_USB_PORT_INFO& operator=(const _USB_PORT_INFO &rhs)
		{
			if(this == &rhs) 
				return *this ; 			
			if(rhs.deviceId!=-1)
				deviceId = rhs.deviceId;			
			if(rhs.idProduct!=0xFFFF)
				idProduct = rhs.idProduct ; 
			if(rhs.idVendor!=0xFFFF)
				idVendor = rhs.idVendor ; 
			if(!rhs.str_tid.IsEmpty())
				str_tid = rhs.str_tid ; 
			if(!rhs.strProductDescriptor.IsEmpty())
				strProductDescriptor = rhs.strProductDescriptor ; 
			if(!rhs.strSerialNum.IsEmpty())
				strSerialNum = rhs.strSerialNum;			
			if(!rhs.str_mappedTid.IsEmpty())
				str_mappedTid = rhs.str_mappedTid;			
			return *this ; 
			
		}; 



	} USB_PORT_INFO;

	
	// represents a single USB device connected 
	typedef struct _USB_TOPOLOGY_INFO
	{
		USB_PORT_INFO  portInfo ; 
		HUB_TYPE hubType ; 
		HCD_TYPE hcdType ;
		CString strHubSymbolicLink ; 
		CString strHcdName ; 
		short portIndex ; 
		USB_DEVICE_MASK  eDeviceType ; 	
		bool bHcdFunctioning ; 

		_USB_TOPOLOGY_INFO()
		{
			hubType = HUB_INVALID ; 
			hcdType = HCD_INVALID ; 
			portIndex = -1 ; 
			eDeviceType = USB_NONE ; 
			bHcdFunctioning  = false ; 
		}	

		_USB_TOPOLOGY_INFO(const _USB_TOPOLOGY_INFO &rhs)
		{
			portInfo = rhs.portInfo ; 
			hubType = rhs.hubType ; 
			hcdType = rhs.hcdType ; 
			strHubSymbolicLink = rhs.strHubSymbolicLink ; 
			strHcdName = rhs.strHcdName ; 
			portIndex = rhs.portIndex;
			eDeviceType = rhs.eDeviceType ; 
			bHcdFunctioning  = rhs.bHcdFunctioning ; 
		

		};

		_USB_TOPOLOGY_INFO& operator = (const _USB_TOPOLOGY_INFO &rhs)
		{
			if(this == &rhs)
				return *this ; 

			portInfo = rhs.portInfo ; 
			hubType = rhs.hubType ; 
			hcdType = rhs.hcdType ; 
			strHubSymbolicLink = rhs.strHubSymbolicLink ; 
			strHcdName = rhs.strHcdName ; 
			portIndex = rhs.portIndex;
			eDeviceType = rhs.eDeviceType ; 			
			bHcdFunctioning  = rhs.bHcdFunctioning ; 
			return *this ; 
		}

		// Function name	: IsValidDevice
		// Description	    : 
		// Return type		: bool 
		// Argument         : const USB_TOPOLOGY_INFO &tp
		bool IsValidDevice()
		{

			if(portInfo.deviceId!=NO_DEVICE_ID )
				return true ; 
			if(hubType ==  HUB_ROOT_HUB)
			{
				if((portInfo.idVendor!=0xFFFF && portInfo.idProduct!=0xFFFF)
					&& ((!portInfo.strProductDescriptor.IsEmpty()) || (!portInfo.strSerialNum.IsEmpty())))
					return true ; 

			}
			else 
			{
				return true ;
			}

			return false ; 

		}
	}USB_TOPOLOGY_INFO  ; 	
	typedef CArray<USB_TOPOLOGY_INFO  , USB_TOPOLOGY_INFO &> USB_DEVICE_ARRAY ;

/* ******************************************************************************************************************************* */

   inline bool PSTSuccess(EPSTStatus Status)    { return (Status == PST_OK); }
   inline bool PSTFailure(EPSTStatus Status)    { return (Status != PST_OK); }

   // registered Windows messages
   const UINT WM_PST_DISPLAY_STATUS       = ::RegisterWindowMessage(_T("WM_PST_DISPLAY_STATUS"));
   const UINT WM_PST_DISPLAY_ERROR           = ::RegisterWindowMessage(_T("WM_PST_DISPLAY_ERROR"));
   const UINT WM_PST_DISPLAY_PROGRESS        = ::RegisterWindowMessage(_T("WM_PST_DISPLAY_PROGRESS"));
   const UINT WM_PST_DISPLAY_CORE_ERROR      = ::RegisterWindowMessage(_T("WM_PST_DISPLAY_CORE_ERROR"));
   const UINT WM_PST_DEVICE_ARRIVAL       = ::RegisterWindowMessage(_T("WM_PST_DEVICE_ARRIVAL"));
   const UINT WM_PST_MDMDEVICE_ARRIVAL       = ::RegisterWindowMessage(_T("WM_PST_MDMDEVICE_ARRIVAL"));
   const UINT WM_PST_DEVICE_REMOVAL       = ::RegisterWindowMessage(_T("WM_PST_DEVICE_REMOVAL"));
   const UINT WM_PST_SER_DEVICE_ARRIVAL      = ::RegisterWindowMessage(_T("WM_PST_SER_DEVICE_ARRIVAL"));
   const UINT WM_PST_SER_DEVICE_REMOVAL      = ::RegisterWindowMessage(_T("WM_PST_SER_DEVICE_REMOVAL"));
   const UINT WM_PST_LOG_CORE_MESSAGE        = ::RegisterWindowMessage(_T("WM_PST_LOG_CORE_MESSAGE"));
   const UINT WM_PST_USB_BAD_DEVICE       = ::RegisterWindowMessage(_T("WM_PST_BAD_DEVICE"));
   const UINT WM_PST_BLAN_DEVICE_ENUMERATION = ::RegisterWindowMessage(_T("WM_PST_BLAN_DEVICE_ENUMERATION"));
   const UINT WM_PST_DEVICE_IGNORED               = ::RegisterWindowMessage(_T("WM_PST_DEVICE_IGNORED"));
   
   // Registry entries
   const HKEY    hRootKey                 =  HKEY_LOCAL_MACHINE;
   const LPCTSTR pszDefaultApp				 = _T("PSTSDK");
   const LPCTSTR pszPSTValFilename          = _T("Filename");
   /////////////////////////////////////////////////////////////////////////////
   // PST_CORE Exports
   //

#ifndef _LIB
   #ifdef PST_CORE_IMPLEMENT
   #define PST_CORE_API extern "C" __declspec(dllexport)
   #else
   #define PST_CORE_API extern "C" __declspec(dllimport)
   #endif
#else
    #define PST_CORE_API
#endif

   // interface used by PST GUI and external applications
   PST_CORE_API EPSTStatus PSTInitialize        (HANDLE hAppHandle= NULL);
   PST_CORE_API EPSTStatus PSTInitializeEx         (PST_WORK_MODE workmode,HANDLE hAppHandle = NULL,MOTAUTOSWITCHMODE iSwitch = MOT_AUTOSWITCH_CONFIG8);
   PST_CORE_API EPSTStatus PSTInitializeAndRegister (PST_WORK_MODE workmode, HWND hWnd = NULL, REGISTERATION_MODE eRegMode = PST_REG_NOTIF, MOTAUTOSWITCHMODE iSwitch = MOT_AUTOSWITCH_CONFIG8);
   PST_CORE_API EPSTStatus PSTInitializeAndRegisterEx (const PSTInitializeStruct& structPSTInitializeInput); 
   PST_CORE_API EPSTStatus PSTUninitialize         ();
   PST_CORE_API EPSTStatus PSTSetSwitchMode		(MOTAUTOSWITCHMODE iSwitch, DEVICEID DeviceID = NO_DEVICE_ID);
   PST_CORE_API EPSTStatus PSTGetDeviceList     (CDeviceArray& DeviceArray);
   PST_CORE_API EPSTStatus PSTRefreshConnections   ();
   PST_CORE_API EPSTStatus PSTSwitchUSBConfig(DEVICEID DeviceID, USBConfigType eConfigNum, CString sInterface,CString sArriveInterface);
   PST_CORE_API EPSTStatus PSTFindInterfaceHandle(DEVICEID DeviceID, CString sInterface, LPHANDLE phInterface);

   PST_CORE_API EPSTStatus PSTReleaseDevice(DEVICEID DeviceID);
   PST_CORE_API EPSTStatus PSTOpenDevice(DEVICEID DeviceID);
   PST_CORE_API EPSTStatus PSTSuspendScanningDevice(DEVICEID DeviceID);
   PST_CORE_API EPSTStatus PSTResumeScanningDevice(DEVICEID DeviceID);

   PST_CORE_API EPSTStatus PSTProcessDevice(DEVICEID DeviceID, MotDeviceNotificationStruct *pNotifyStruct=NULL, DWORD dwTimeout=DEVICE_NOTIFICATION_MIN_TIMEOUT);
   PST_CORE_API EPSTStatus PSTIgnoreDevice(DEVICEID DeviceID, MotDeviceNotificationStruct *pNotifyStruct=NULL, DWORD dwTimeout=DEVICE_NOTIFICATION_MIN_TIMEOUT);
  
   PST_CORE_API EPSTStatus PSTWaitforDevice(DEVICEID DeviceID, DWORD dwHint, DWORD dwLength );


   PST_CORE_API EPSTStatus PSTSwitchToRecoveryMode();

   // -- ODM support
   PST_CORE_API EPSTStatus PSTODMConfigureSerial(BOOL bEnabled);
   PST_CORE_API EPSTStatus PSTEnableBlankFlashTIROM(BOOL bEnabled);
   PST_CORE_API EPSTStatus PSTGetSerialEnumDetails(const CString& strPortName, SerialDeviceEnumDetails& CurrentDeviceEnumDetails);

   // New APIs used when driver notification is disabled
   PST_CORE_API EPSTStatus PSTNotifyOnDeviceChange(MotDeviceNotificationStruct *pNotifyStruct);
   PST_CORE_API EPSTStatus PSTRegisterForNotifications(HWND hWnd);
   PST_CORE_API EPSTStatus PSTUnregisterForNotifications(HWND hWnd);

   // -- OBEX support
   // OBEX exported APIs
   PST_CORE_API EPSTStatus PSTOBEXInitialize(DEVICEID deviceID, HANDLE hInfac);
   PST_CORE_API EPSTStatus PSTOBEXInitializeEx(DEVICEID deviceID, HANDLE hInfac, char *strOBEXInitCmd);
   PST_CORE_API EPSTStatus PSTOBEXConnect(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SCONNECT *pConnOptions, OBEXCONNECTIONID *OBEXConnID);
   PST_CORE_API EPSTStatus PSTOBEXDisconnect(DEVICEID deviceID, HANDLE hInfac, OBEXCONNECTIONID iOBEXConnID);
   PST_CORE_API EPSTStatus PSTOBEXAbort(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SABORT *pOptions);
   PST_CORE_API EPSTStatus PSTOBEXSetPath(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SSETPATH *pSetpathOptions);
   PST_CORE_API EPSTStatus PSTOBEXGet(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SGET *pGetOptions);
   PST_CORE_API EPSTStatus PSTOBEXGetInit(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SGET *pGetOptions);
   PST_CORE_API EPSTStatus PSTOBEXGetDataBlock(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SGET *pGetOptions);
   PST_CORE_API EPSTStatus PSTOBEXPut(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SPUT *pPutOptions);
   PST_CORE_API EPSTStatus PSTOBEXPutInit(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SPUT *pPutOptions, LPDWORD pdwSentData);
   PST_CORE_API EPSTStatus PSTOBEXPutDataBlock(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SPUT *pPutOptions, LPDWORD pdwSentData);
   PST_CORE_API EPSTStatus PSTOBEXPutFinish(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SPUT *pPutOptions, LPDWORD pdwSentData);
   PST_CORE_API EPSTStatus PSTOBEXDelFile(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SDEL *pDelOptions);
   PST_CORE_API EPSTStatus PSTOBEXDirList(DEVICEID deviceID, HANDLE hInfac, PSTOBEX_SDIRLIST *pDirOptions, CStringList *pStrListDirXML);

   // Structures initializations API
   PST_CORE_API EPSTStatus PSTOBEXInitStructConnect(PSTOBEX_SCONNECT *pConnOptions);
   PST_CORE_API EPSTStatus PSTOBEXInitStructAbort(PSTOBEX_SABORT *pOptions);
   PST_CORE_API EPSTStatus PSTOBEXInitStructSetPath(PSTOBEX_SSETPATH *pSetpathOptions);
   PST_CORE_API EPSTStatus PSTOBEXInitStructGet(PSTOBEX_SGET *pGetOptions);
   PST_CORE_API EPSTStatus PSTOBEXInitStructPut(PSTOBEX_SPUT *pPutOptions);
   PST_CORE_API EPSTStatus PSTOBEXInitStructDel(PSTOBEX_SDEL *pDelOptions);
   PST_CORE_API EPSTStatus PSTOBEXInitStructDir(PSTOBEX_SDIRLIST *pDirOptions);

   PST_CORE_API EPSTStatus PSTReadData (DEVICEID DeviceID,
                               HANDLE hInface,
                               PCKTYPE Type,
                               LPVOID pBuffer,
                               LPDWORD pdwLen,
                               DWORD timeout);

   PST_CORE_API EPSTStatus PSTReadDataEx(DEVICEID DeviceID,
                               HANDLE hInface,
                               PCKTYPE Type,
                               LPVOID pBuffer,
                               LPDWORD pdwLen,
                               DWORD totalTimeout,
                               DWORD intervalTimeout);


   PST_CORE_API EPSTStatus PSTWriteData (DEVICEID DeviceID,
                                HANDLE hInface,
                                PCKTYPE Type,
                                LPCVOID pBuffer,
                                LPDWORD pdwLen,
                                DWORD timeout);

   PST_CORE_API EPSTStatus PSTGetNumberofEndpoints(DEVICEID DeviceID, CString interfaceName,int& iNumofEndpoints);
   PST_CORE_API EPSTStatus PSTGetInterfaceNumber(DEVICEID DeviceID, CString interfaceName,int& iPhoneInterfaceNumber);
   PST_CORE_API EPSTStatus PSTGetInterfaceHandle   (DEVICEID DeviceId, CString interfaceName, LPHANDLE phInface);
   PST_CORE_API EPSTStatus PSTGetStringDescriptor  (DEVICEID DeviceId, HANDLE hInface, DESCRTYPE DescriptorType, BYTE bNumber, CString& sDescriptor);
    PST_CORE_API EPSTStatus PSTGetProductStringDescriptor   (DEVICEID DeviceId, CString& sDescriptor);
	PST_CORE_API EPSTStatus PSTGetProductStringDescriptorChar(DEVICEID deviceId, char* sDescriptor , int &numBytes);
   PST_CORE_API EPSTStatus PSTSetBaudRate(BaudRateEnum eBaudRate, DEVICEID deviceID);
   PST_CORE_API EPSTStatus PSTGetBaudRate(DEVICEID deviceID, BaudRateEnum &eBaudRate);
   PST_CORE_API EPSTStatus PSTSetSerialState(DEVICEID deviceID,  eSerialPhoneState eState);
   PST_CORE_API EPSTStatus PSTStartSerialDevice(CString& strComPort, DEVICEID DeviceID, BOOL bODMPhone);
   PST_CORE_API EPSTStatus PSTStopSerialDevice(CString& strComPort);
   
	PST_CORE_API EPSTStatus PSTGetTCIProtocol(DEVICEID DeviceID, ETCIProtocol* peProtocol);

	PST_CORE_API EPSTStatus PSTGetFlashInterface(DEVICEID DeviceId, HANDLE *phInterface, CString* pstrInterface, int* pnSize);
	PST_CORE_API EPSTStatus PSTFindFlashInterface(DEVICEID DeviceId, HANDLE *phInterface, CString* pstrInterface, int* pnSize);
	PST_CORE_API EPSTStatus PSTGetAllInterface(DEVICEID DeviceId, HANDLE *phInterface, CString* pstrInterface, int* pnSize);

	PST_CORE_API EPSTStatus PSTReleaseInterfaceHandle(DEVICEID DeviceId, CString& strInterface);


   


   //interface to the wrapper functions of TCI
   PST_CORE_API EPSTStatus PSTSendCommand       (DEVICEID   DeviceID,
                                        HANDLE      hInface,
                                        LPCSTR      pszCmd,
                                        LPBYTE      pbSupportData,
                                        DWORD       dwSupportDataArraySize,
                                        LPBYTE      pbReturnData,
                                        LPDWORD     pdwReturnDataArraySize,
                                        HINSTANCE   hUTDll,
                                        DWORD       dwTimeOut = dwUseDefaultTcmdTimeout);
                                        
   PST_CORE_API EPSTStatus PSTSendCommandEx     (DEVICEID   DeviceID,
                                        HANDLE      hInface,
                                        LPCSTR      pszCmd,
                                        LPBYTE      pbSupportData,
                                        DWORD       dwSupportDataArraySize,
                                        LPBYTE      pbReturnData,
                                        LPDWORD     pdwReturnDataArraySize,
                                        HINSTANCE   hUTDll,
                                        DWORD       dwTimeOut = dwUseDefaultTcmdTimeout);
                                        
   PST_CORE_API EPSTStatus PSTSendCommandByOpcode  (DEVICEID   DeviceID,
                                        HANDLE      hInface,
                                        DWORD       dwOpcode,
                                        LPBYTE      pbSupportData,
                                        DWORD       dwSupportDataArraySize,
                                        LPBYTE      pbReturnData,
                                        LPDWORD     pdwReturnDataArraySize,
                                        HINSTANCE   hUTDll,
                                        DWORD       dwTimeOut = dwUseDefaultTcmdTimeout);

   PST_CORE_API EPSTStatus PSTSendCommandByOpcodeEx (DEVICEID   DeviceID,
                                        HANDLE      hInface,
                                        DWORD       dwOpcode,
                                        LPBYTE      pbSupportData,
                                        DWORD       dwSupportDataArraySize,
                                        LPBYTE      pbReturnData,
                                        LPDWORD     pdwReturnDataArraySize,
                                        HINSTANCE   hUTDll,
                                        DWORD       dwTimeOut = dwUseDefaultTcmdTimeout);

   PST_CORE_API EPSTStatus PSTSendRawCommand    (DEVICEID   DeviceID,
                                        HANDLE      hInface,
                                        LPBYTE      pbData,
                                        DWORD       dwDataArraySize,
                                        LPBYTE      pbReturnData,
                                        LPDWORD     pdwReturnDataArraySize,
                                        HINSTANCE   hUTDll,
                                        DWORD       dwTimeOut = dwUseDefaultTcmdTimeout);
                                        
   PST_CORE_API EPSTStatus PSTSendRawCommandEx  (DEVICEID   DeviceID,
                                        HANDLE      hInface,
                                        LPBYTE      pbData,
                                        DWORD       dwDataArraySize,
                                        LPBYTE      pbReturnData,
                                        LPDWORD     pdwReturnDataArraySize,
                                        HINSTANCE   hUTDll,
                                        DWORD       dwTimeOut = dwUseDefaultTcmdTimeout);                                        

   PST_CORE_API EPSTStatus PSTGetTCITableName      (HINSTANCE hUTDll,CString& sTableName);
   PST_CORE_API void PSTSetLoggerHandle   (HANDLE hAppHandle);
   PST_CORE_API EPSTStatus PSTGetInterfaceDescriptorValue(DEVICEID DeviceID,HANDLE hInterface, USBInterfaceDescEnumType vType,BYTE &iDescValue);
   PST_CORE_API EPSTStatus PSTGetBLANDeviceIPAddress(DEVICEID DeviceID, CString &strIPAddress);
   PST_CORE_API EPSTStatus PSTGetBLANDevicePortNumber(DEVICEID DeviceID, CString strInterface, UINT &iPort);
   PST_CORE_API EPSTStatus PSTSetBLANPortStaus(DEVICEID DeviceID, CBlanPortStatusList& listPortStatus);
   PST_CORE_API EPSTStatus PSTGetComPortName(DEVICEID DeviceID, CString& strPortName, CString strInterface = pszInterfaceComInterface); 
   PST_CORE_API EPSTStatus PSTGetDevConnectivityInfo(const DEVICEID &DeviceID, DeviceConnectivityInfoStruct &ConnectivityInfoStruct);

	PST_CORE_API EPSTStatus PSTGetDeviceIDFromHubandPort(CString& strHubName, unsigned char port, DEVICEID& devID);
    PST_CORE_API EPSTStatus PSTGetUSBDeviceDescriptor(DEVICEID DeviceID, PST_USB_DEVICE_DESCRIPTOR& structPSTUSBDeviceDesciptor);

	PST_CORE_API EPSTStatus PSTGetInterfaceMapping(INTF_STRING_MAP &intfMapDetail, bool bEnumToString = true);

	// Detection Specific 
	PST_CORE_API EPSTStatus PSTDetectionRegisterCallback(MotCallBackParams callBackParams) ; 
	PST_CORE_API EPSTStatus PSTDetectionUnRegisterCallback(MotCallBackParams callBackParams) ; 


	// Device id configuration specific 
	PST_CORE_API EPSTStatus PSTGetUSBTopology(USB_DEVICE_ARRAY &deviceArray , const USB_DEVICE_MASK &eFilter=(USB_DEVICE_MASK)(USB_DEVICE|USB_NONE));
	PST_CORE_API EPSTStatus PSTSetDeviceID(const USB_TOPOLOGY_INFO &device  , const DEVICEID deviceID);
	PST_CORE_API EPSTStatus PSTClearConfiguration(const DEVICEID deviceID = NO_DEVICE_ID , bool bClearAll=true ) ; 


/*---------------------------------------------------------------------------
                     DEPRECATED FUNCTIONS
-------------------------------------------------------------------------*/
PST_CORE_API EPSTStatus PSTSecurityInit();
PST_CORE_API EPSTStatus PSTSecurityCheck();
PST_CORE_API EPSTStatus PSTSecurityUninit();
PST_CORE_API EPSTStatus PSTSecurityLevel(ULONG SecurityVal);

//interface used only by Datalogger
PST_CORE_API EPSTStatus PSTInitializeComDirect(CString szPortName, BaudRateEnum eBaudRate);
PST_CORE_API EPSTStatus PSTDirectSerialReadData(CString szPortName, LPVOID pBuffer, LPDWORD pdwLen, DWORD timeout);
PST_CORE_API EPSTStatus PSTUninitializeComDirect(CString szPortName);

PST_CORE_API EPSTStatus PSTGetCmdHistory     (HINSTANCE hUTDll,CString& sCmdHistory);
PST_CORE_API EPSTStatus PSTTogglePower(LPCTSTR pszCOMPort);
PST_CORE_API EPSTStatus PSTGetPhonePowerState(BOOL *pbState, LPCTSTR pszCOMPort);
PST_CORE_API EPSTStatus PSTSetMDMDetect(MOTAUTOSWITCHMODE bMDMDetect);

// PSTTestCmdInit() API is actually deprecated. The TestCmd engine is now initialized automatically.
// Its functionality is handled by CPSTSession::CheckAndSetTCIAutoInitStatus()
PST_CORE_API EPSTStatus PSTTestCmdInit(HINSTANCE hUTDll, ETCIProtocol eProtocol,CString str = (CString)pszDefaultApp);

PST_CORE_API EPSTStatus PSTGetUTHandle(HINSTANCE hUTDll);
PST_CORE_API EPSTStatus PSTGetUTHandleEx(LPCTSTR pszUTFriendlyName, HINSTANCE& rhUTDll);

PST_CORE_API EPSTStatus PSTGetSelectedDeviceList(CDeviceArray& DeviceArray);
PST_CORE_API EPSTStatus PSTAbort          (DEVICEID DeviceID, HANDLE hInface);
PST_CORE_API EPSTStatus PSTCloseUTC          (HANDLE hThread);

// interface used by UTs
PST_CORE_API EPSTStatus PSTDisplayStatus     (HANDLE hThread, LPCTSTR pszMessage);
PST_CORE_API EPSTStatus PSTDisplayError         (HANDLE hThread, LPCTSTR pszMessage);
PST_CORE_API EPSTStatus PSTDisplayProgress      (HANDLE hThread, UINT nPercentValue);

PST_CORE_API EPSTStatus PSTDisplayStatusEx   (DEVICEID DeviceID, HANDLE hThread, LPCTSTR pszMessage);
PST_CORE_API EPSTStatus PSTDisplayErrorEx  (DEVICEID DeviceID, HANDLE hThread, LPCTSTR pszMessage);
PST_CORE_API EPSTStatus PSTDisplayProgressEx (DEVICEID DeviceID, HANDLE hThread, UINT nPercentValue);

PST_CORE_API EPSTStatus PSTRefreshConnectionsEx ();
PST_CORE_API EPSTStatus PSTSetAlternateUSB(DEVICEID DeviceID,HANDLE hInterface, UCHAR ualtsetting);

PST_CORE_API EPSTStatus PSTSetSelectedDeviceList(const CDeviceArray& DeviceArray);
PST_CORE_API EPSTStatus PSTStartUT           (LPCTSTR pszUTFriendlyName);
PST_CORE_API EPSTStatus PSTStopUT            (LPCTSTR pszUTFriendlyName);
PST_CORE_API EPSTStatus PSTPerformRequest    (LPCTSTR pszUTFriendlyName, LPCTSTR pszFuncName, UINT nNumParam, ...);

PST_CORE_API EPSTStatus PSTStartUTGUI           (LPCTSTR pszUTFriendlyName, HWND hParent, const CRect &PosRect);
PST_CORE_API EPSTStatus PSTResizeUTGUI          (LPCTSTR pszUTFriendlyName, const CRect &PosRect);
PST_CORE_API EPSTStatus PSTGetUTVersion            (LPCTSTR pszUTFriendlyName, CString& sVersion);
PST_CORE_API EPSTStatus PSTGetTCIVersion        (CString& sVersion);
PST_CORE_API EPSTStatus PSTGetUSBDriverVersion     (CString& sVersion);
PST_CORE_API EPSTStatus PSTCloseInterfaceHandle(DEVICEID DeviceID,HANDLE hInterface);

/*---------------------------------------------------------------------------
                         END DEPRECATED FUNCTIONS
-------------------------------------------------------------------------*/


#endif /* __PSTCommon_h__ */
