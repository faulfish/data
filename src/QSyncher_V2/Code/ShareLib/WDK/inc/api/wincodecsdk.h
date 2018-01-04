

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0499 */
/* Compiler settings for wincodecsdk.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __wincodecsdk_h__
#define __wincodecsdk_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWICMetadataBlockReader_FWD_DEFINED__
#define __IWICMetadataBlockReader_FWD_DEFINED__
typedef interface IWICMetadataBlockReader IWICMetadataBlockReader;
#endif 	/* __IWICMetadataBlockReader_FWD_DEFINED__ */


#ifndef __IWICMetadataBlockWriter_FWD_DEFINED__
#define __IWICMetadataBlockWriter_FWD_DEFINED__
typedef interface IWICMetadataBlockWriter IWICMetadataBlockWriter;
#endif 	/* __IWICMetadataBlockWriter_FWD_DEFINED__ */


#ifndef __IWICMetadataReader_FWD_DEFINED__
#define __IWICMetadataReader_FWD_DEFINED__
typedef interface IWICMetadataReader IWICMetadataReader;
#endif 	/* __IWICMetadataReader_FWD_DEFINED__ */


#ifndef __IWICMetadataWriter_FWD_DEFINED__
#define __IWICMetadataWriter_FWD_DEFINED__
typedef interface IWICMetadataWriter IWICMetadataWriter;
#endif 	/* __IWICMetadataWriter_FWD_DEFINED__ */


#ifndef __IWICStreamProvider_FWD_DEFINED__
#define __IWICStreamProvider_FWD_DEFINED__
typedef interface IWICStreamProvider IWICStreamProvider;
#endif 	/* __IWICStreamProvider_FWD_DEFINED__ */


#ifndef __IWICPersistStream_FWD_DEFINED__
#define __IWICPersistStream_FWD_DEFINED__
typedef interface IWICPersistStream IWICPersistStream;
#endif 	/* __IWICPersistStream_FWD_DEFINED__ */


#ifndef __IWICMetadataHandlerInfo_FWD_DEFINED__
#define __IWICMetadataHandlerInfo_FWD_DEFINED__
typedef interface IWICMetadataHandlerInfo IWICMetadataHandlerInfo;
#endif 	/* __IWICMetadataHandlerInfo_FWD_DEFINED__ */


#ifndef __IWICMetadataReaderInfo_FWD_DEFINED__
#define __IWICMetadataReaderInfo_FWD_DEFINED__
typedef interface IWICMetadataReaderInfo IWICMetadataReaderInfo;
#endif 	/* __IWICMetadataReaderInfo_FWD_DEFINED__ */


#ifndef __IWICMetadataWriterInfo_FWD_DEFINED__
#define __IWICMetadataWriterInfo_FWD_DEFINED__
typedef interface IWICMetadataWriterInfo IWICMetadataWriterInfo;
#endif 	/* __IWICMetadataWriterInfo_FWD_DEFINED__ */


#ifndef __IWICComponentFactory_FWD_DEFINED__
#define __IWICComponentFactory_FWD_DEFINED__
typedef interface IWICComponentFactory IWICComponentFactory;
#endif 	/* __IWICComponentFactory_FWD_DEFINED__ */


/* header files for imported files */
#include "wtypes.h"
#include "wincodec.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_wincodecsdk_0000_0000 */
/* [local] */ 

DEFINE_GUID(GUID_MetadataFormatUnknown, 0xA45E592F, 0x9078, 0x4A7C, 0xAD, 0xB5, 0x4E, 0xDC, 0x4F, 0xD6, 0x1B, 0x1F);
DEFINE_GUID(GUID_MetadataFormatIfd, 0x537396C6, 0x2D8A, 0x4BB6, 0x9B, 0xF8, 0x2F, 0x0A, 0x8E, 0x2A, 0x3A, 0xDF);
DEFINE_GUID(GUID_MetadataFormatSubIfd, 0x58A2E128, 0x2DB9, 0x4E57, 0xBB, 0x14, 0x51, 0x77, 0x89, 0x1E, 0xD3, 0x31);
DEFINE_GUID(GUID_MetadataFormatExif, 0x1C3C4F9D, 0xB84A, 0x467D, 0x94, 0x93, 0x36, 0xCF, 0xBD, 0x59, 0xEA, 0x57);
DEFINE_GUID(GUID_MetadataFormatGps, 0x7134AB8A, 0x9351, 0x44AD, 0xAF, 0x62, 0x44, 0x8D, 0xB6, 0xB5, 0x02, 0xEC);
DEFINE_GUID(GUID_MetadataFormatInterop, 0xED686F8E, 0x681F, 0x4C8B, 0xBD, 0x41, 0xA8, 0xAD, 0xDB, 0xF6, 0xB3, 0xFC);
DEFINE_GUID(GUID_MetadataFormatApp0, 0x79007028, 0x268D, 0x45d6, 0xA3, 0xC2, 0x35, 0x4E, 0x6A, 0x50, 0x4B, 0xC9);
DEFINE_GUID(GUID_MetadataFormatApp1,  0x8FD3DFC3, 0xF951, 0x492B, 0x81, 0x7F, 0x69, 0xC2, 0xE6, 0xD9, 0xA5, 0xB0);
DEFINE_GUID(GUID_MetadataFormatApp13, 0x326556A2, 0xF502, 0x4354, 0x9C, 0xC0, 0x8E, 0x3F, 0x48, 0xEA, 0xF6, 0xB5);
DEFINE_GUID(GUID_MetadataFormatIPTC, 0x4FAB0914, 0xE129, 0x4087, 0xA1, 0xD1, 0xBC, 0x81, 0x2D, 0x45, 0xA7, 0xB5);
DEFINE_GUID(GUID_MetadataFormatIRB,      0x16100D66, 0x8570, 0x4BB9, 0xB9, 0x2D, 0xFD, 0xA4, 0xB2, 0x3E, 0xCE, 0x67);
DEFINE_GUID(GUID_MetadataFormat8BIMIPTC, 0x0010568c, 0x0852, 0x4e6a, 0xb1, 0x91, 0x5c, 0x33, 0xac, 0x5b, 0x04, 0x30);
DEFINE_GUID(GUID_MetadataFormatXMP, 0xBB5ACC38, 0xF216, 0x4CEC, 0xA6, 0xC5, 0x5F, 0x6E, 0x73, 0x97, 0x63, 0xA9);
DEFINE_GUID(GUID_MetadataFormatThumbnail, 0x243dcee9, 0x8703, 0x40ee, 0x8e, 0xf0, 0x22, 0xa6, 0x0, 0xb8, 0x5, 0x8c);
DEFINE_GUID(GUID_MetadataFormatChunktEXt, 0x568d8936, 0xc0a9, 0x4923, 0x90, 0x5d, 0xdf, 0x2b, 0x38, 0x23, 0x8f, 0xbc);
DEFINE_GUID(GUID_MetadataFormatXMPStruct, 0x22383CF1, 0xED17, 0x4E2E, 0xAF, 0x17, 0xD8, 0x5B, 0x8F, 0x6B, 0x30, 0xD0);
DEFINE_GUID(GUID_MetadataFormatXMPBag, 0x833CCA5F, 0xDCB7, 0x4516, 0x80, 0x6F, 0x65, 0x96, 0xAB, 0x26, 0xDC, 0xE4);
DEFINE_GUID(GUID_MetadataFormatXMPSeq, 0x63E8DF02, 0xEB6C,0x456C, 0xA2, 0x24, 0xB2, 0x5E, 0x79, 0x4F, 0xD6, 0x48);
DEFINE_GUID(GUID_MetadataFormatXMPAlt, 0x7B08A675, 0x91AA, 0x481B, 0xA7, 0x98, 0x4D, 0xA9, 0x49, 0x08, 0x61, 0x3B);
DEFINE_GUID(CLSID_WICUnknownMetadataReader, 0x699745c2, 0x5066, 0x4b82, 0xa8, 0xe3, 0xd4, 0x4, 0x78, 0xdb, 0xec, 0x8c);
DEFINE_GUID(CLSID_WICUnknownMetadataWriter, 0xa09cca86, 0x27ba, 0x4f39, 0x90, 0x53, 0x12, 0x1f, 0xa4, 0xdc, 0x8, 0xfc);
DEFINE_GUID(CLSID_WICApp0MetadataWriter, 0xF3C633A2, 0x46C8, 0x498e, 0x8F, 0xBB, 0xCC, 0x6F, 0x72, 0x1B, 0xBC, 0xDE);
DEFINE_GUID(CLSID_WICApp0MetadataReader, 0x43324B33, 0xA78F, 0x480f, 0x91, 0x11, 0x96, 0x38, 0xAA, 0xCC, 0xC8, 0x32);
DEFINE_GUID(CLSID_WICApp1MetadataWriter, 0xee366069, 0x1832, 0x420f, 0xb3, 0x81, 0x04, 0x79, 0xad, 0x06, 0x6f, 0x19);
DEFINE_GUID(CLSID_WICApp1MetadataReader, 0xdde33513, 0x774e, 0x4bcd, 0xae, 0x79, 0x02, 0xf4, 0xad, 0xfe, 0x62, 0xfc);
DEFINE_GUID(CLSID_WICApp13MetadataWriter, 0x7B19A919, 0xA9D6, 0x49E5, 0xBD, 0x45, 0x02, 0xC3, 0x4E, 0x4E, 0x4C, 0xD5);
DEFINE_GUID(CLSID_WICApp13MetadataReader, 0xAA7E3C50, 0x864C, 0x4604, 0xBC, 0x04, 0x8B, 0x0B, 0x76, 0xE6, 0x37, 0xF6);
DEFINE_GUID(CLSID_WICIfdMetadataReader, 0x8f914656, 0x9d0a, 0x4eb2, 0x90, 0x19, 0xb, 0xf9, 0x6d, 0x8a, 0x9e, 0xe6);
DEFINE_GUID(CLSID_WICIfdMetadataWriter, 0xb1ebfc28, 0xc9bd, 0x47a2, 0x8d, 0x33, 0xb9, 0x48, 0x76, 0x97, 0x77, 0xa7);
DEFINE_GUID(CLSID_WICSubIfdMetadataReader, 0x50D42F09, 0xECD1, 0x4B41, 0xB6, 0x5D, 0xDA, 0x1F, 0xDA, 0xA7, 0x56, 0x63);
DEFINE_GUID(CLSID_WICSubIfdMetadataWriter, 0x8ADE5386, 0x8E9B, 0x4F4C, 0xAC, 0xF2, 0xF0, 0x00, 0x87, 0x06, 0xB2, 0x38);
DEFINE_GUID(CLSID_WICExifMetadataReader, 0xd9403860, 0x297f, 0x4a49, 0xbf, 0x9b, 0x77, 0x89, 0x81, 0x50, 0xa4, 0x42);
DEFINE_GUID(CLSID_WICExifMetadataWriter, 0xc9a14cda, 0xc339, 0x460b, 0x90, 0x78, 0xd4, 0xde, 0xbc, 0xfa, 0xbe, 0x91);
DEFINE_GUID(CLSID_WICGpsMetadataReader, 0x3697790B, 0x223B, 0x484E, 0x99, 0x25, 0xC4, 0x86, 0x92, 0x18, 0xF1, 0x7A);
DEFINE_GUID(CLSID_WICGpsMetadataWriter, 0xCB8C13E4, 0x62B5, 0x4C96, 0xA4, 0x8B, 0x6B, 0xA6, 0xAC, 0xE3, 0x9C, 0x76);
DEFINE_GUID(CLSID_WICInteropMetadataReader, 0xB5C8B898, 0x0074, 0x459F, 0xB7, 0x00, 0x86, 0x0D, 0x46, 0x51, 0xEA, 0x14);
DEFINE_GUID(CLSID_WICInteropMetadataWriter, 0x122EC645, 0xCD7E, 0x44D8, 0xB1, 0x86, 0x2C, 0x8C, 0x20, 0xC3, 0xB5, 0x0F);
DEFINE_GUID(CLSID_WICThumbnailMetadataReader, 0xfb012959, 0xf4f6, 0x44d7, 0x9d, 0x9, 0xda, 0xa0, 0x87, 0xa9, 0xdb, 0x57);
DEFINE_GUID(CLSID_WICThumbnailMetadataWriter, 0xd049b20c, 0x5dd0, 0x44fe, 0xb0, 0xb3, 0x8f, 0x92, 0xc8, 0xe6, 0xd0, 0x80);
DEFINE_GUID(CLSID_WICIPTCMetadataReader, 0x03012959, 0xf4f6, 0x44d7, 0x9d, 0x9, 0xda, 0xa0, 0x87, 0xa9, 0xdb, 0x57);
DEFINE_GUID(CLSID_WICIPTCMetadataWriter, 0x1249b20c, 0x5dd0, 0x44fe, 0xb0, 0xb3, 0x8f, 0x92, 0xc8, 0xe6, 0xd0, 0x80);
DEFINE_GUID(CLSID_WICIRBMetadataReader, 0xD4DCD3D7, 0xB4C2, 0x47D9, 0xA6, 0xBF, 0xB8, 0x9B, 0xA3, 0x96, 0xA4, 0xA3);
DEFINE_GUID(CLSID_WICIRBMetadataWriter,      0x5C5C1935, 0x0235, 0x4434, 0x80, 0xBC, 0x25, 0x1B, 0xC1, 0xEC, 0x39, 0xC6);
DEFINE_GUID(CLSID_WIC8BIMIPTCMetadataReader, 0x0010668c, 0x0801, 0x4da6, 0xa4, 0xa4, 0x82, 0x65, 0x22, 0xb6, 0xd2, 0x8f);
DEFINE_GUID(CLSID_WIC8BIMIPTCMetadataWriter, 0x00108226, 0xee41, 0x44a2, 0x9e, 0x9c, 0x4b, 0xe4, 0xd5, 0xb1, 0xd2, 0xcd);
DEFINE_GUID(CLSID_WICPngTextMetadataReader, 0x4b59afcc, 0xb8c3, 0x408a, 0xb6, 0x70, 0x89, 0xe5, 0xfa, 0xb6, 0xfd, 0xa7); 
DEFINE_GUID(CLSID_WICPngTextMetadataWriter, 0xb5ebafb9, 0x253e, 0x4a72, 0xa7, 0x44, 0x7, 0x62, 0xd2, 0x68, 0x56, 0x83); 
DEFINE_GUID(CLSID_WICXMPMetadataReader, 0x72B624DF, 0xAE11, 0x4948, 0xA6, 0x5C, 0x35, 0x1E, 0xB0, 0x82, 0x94, 0x19);
DEFINE_GUID(CLSID_WICXMPMetadataWriter, 0x1765E14E, 0x1BD4, 0x462E, 0xB6, 0xB1, 0x59, 0x0B, 0xF1, 0x26, 0x2A, 0xC6);
DEFINE_GUID(CLSID_WICXMPStructMetadataReader, 0x01B90D9A, 0x8209, 0x47F7, 0x9C, 0x52, 0xE1, 0x24, 0x4B, 0xF5, 0x0C, 0xED);
DEFINE_GUID(CLSID_WICXMPStructMetadataWriter, 0x22C21F93, 0x7DDB, 0x411C, 0x9B, 0x17, 0xC5, 0xB7, 0xBD, 0x06, 0x4A, 0xBC);
DEFINE_GUID(CLSID_WICXMPBagMetadataReader, 0xE7E79A30, 0x4F2C, 0x4FAB, 0x8D, 0x00, 0x39, 0x4F, 0x2D, 0x6B, 0xBE, 0xBE);
DEFINE_GUID(CLSID_WICXMPBagMetadataWriter, 0xED822C8C, 0xD6BE, 0x4301, 0xA6, 0x31, 0x0E, 0x14, 0x16, 0xBA, 0xD2, 0x8F);
DEFINE_GUID(CLSID_WICXMPSeqMetadataReader, 0x7F12E753, 0xFC71, 0x43D7, 0xA5, 0x1D, 0x92, 0xF3, 0x59, 0x77, 0xAB, 0xB5);
DEFINE_GUID(CLSID_WICXMPSeqMetadataWriter, 0x6D68D1DE, 0xD432, 0x4B0F, 0x92, 0x3A, 0x09, 0x11, 0x83, 0xA9, 0xBD, 0xA7);
DEFINE_GUID(CLSID_WICXMPAltMetadataReader, 0xAA94DCC2, 0xB8B0, 0x4898, 0xB8, 0x35, 0x00, 0x0A, 0xAB, 0xD7, 0x43, 0x93);
DEFINE_GUID(CLSID_WICXMPAltMetadataWriter, 0x076C2A6C, 0xF78F, 0x4C46, 0xA7, 0x23, 0x35, 0x83, 0xE7, 0x08, 0x76, 0xEA);









typedef /* [public] */ 
enum WICMetadataCreationOptions
    {	WICMetadataCreationDefault	= 0,
	WICMetadataCreationAllowUnknown	= WICMetadataCreationDefault,
	WICMetadataCreationFailUnknown	= 0x10000,
	WICMetadataCreationMask	= 0xffff0000
    } 	WICMetadataCreationOptions;

typedef /* [public] */ 
enum WICPersistOptions
    {	WICPersistOptionDefault	= 0,
	WICPersistOptionLittleEndian	= 0,
	WICPersistOptionBigEndian	= 0x1,
	WICPersistOptionStrictFormat	= 0x2,
	WICPersistOptionNoCacheStream	= 0x4,
	WICPersistOptionPreferUTF8	= 0x8,
	WICPersistOptionMask	= 0xffff
    } 	WICPersistOptions;



extern RPC_IF_HANDLE __MIDL_itf_wincodecsdk_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wincodecsdk_0000_0000_v0_0_s_ifspec;

#ifndef __IWICMetadataBlockReader_INTERFACE_DEFINED__
#define __IWICMetadataBlockReader_INTERFACE_DEFINED__

/* interface IWICMetadataBlockReader */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWICMetadataBlockReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FEAA2A8D-B3F3-43E4-B25C-D1DE990A1AE1")
    IWICMetadataBlockReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetContainerFormat( 
            /* [out] */ __RPC__out GUID *pguidContainerFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ __RPC__out UINT *pcCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetReaderByIndex( 
            /* [in] */ UINT nIndex,
            /* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIMetadataReader) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEnumerator( 
            /* [out] */ __RPC__deref_out_opt IEnumUnknown **ppIEnumMetadata) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWICMetadataBlockReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWICMetadataBlockReader * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWICMetadataBlockReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWICMetadataBlockReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetContainerFormat )( 
            IWICMetadataBlockReader * This,
            /* [out] */ __RPC__out GUID *pguidContainerFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IWICMetadataBlockReader * This,
            /* [out] */ __RPC__out UINT *pcCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetReaderByIndex )( 
            IWICMetadataBlockReader * This,
            /* [in] */ UINT nIndex,
            /* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIMetadataReader);
        
        HRESULT ( STDMETHODCALLTYPE *GetEnumerator )( 
            IWICMetadataBlockReader * This,
            /* [out] */ __RPC__deref_out_opt IEnumUnknown **ppIEnumMetadata);
        
        END_INTERFACE
    } IWICMetadataBlockReaderVtbl;

    interface IWICMetadataBlockReader
    {
        CONST_VTBL struct IWICMetadataBlockReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWICMetadataBlockReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWICMetadataBlockReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWICMetadataBlockReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWICMetadataBlockReader_GetContainerFormat(This,pguidContainerFormat)	\
    ( (This)->lpVtbl -> GetContainerFormat(This,pguidContainerFormat) ) 

#define IWICMetadataBlockReader_GetCount(This,pcCount)	\
    ( (This)->lpVtbl -> GetCount(This,pcCount) ) 

#define IWICMetadataBlockReader_GetReaderByIndex(This,nIndex,ppIMetadataReader)	\
    ( (This)->lpVtbl -> GetReaderByIndex(This,nIndex,ppIMetadataReader) ) 

#define IWICMetadataBlockReader_GetEnumerator(This,ppIEnumMetadata)	\
    ( (This)->lpVtbl -> GetEnumerator(This,ppIEnumMetadata) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWICMetadataBlockReader_INTERFACE_DEFINED__ */


#ifndef __IWICMetadataBlockWriter_INTERFACE_DEFINED__
#define __IWICMetadataBlockWriter_INTERFACE_DEFINED__

/* interface IWICMetadataBlockWriter */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWICMetadataBlockWriter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("08FB9676-B444-41E8-8DBE-6A53A542BFF1")
    IWICMetadataBlockWriter : public IWICMetadataBlockReader
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE InitializeFromBlockReader( 
            /* [in] */ __RPC__in_opt IWICMetadataBlockReader *pIMDBlockReader) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetWriterByIndex( 
            /* [in] */ UINT nIndex,
            /* [out] */ __RPC__deref_out_opt IWICMetadataWriter **ppIMetadataWriter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddWriter( 
            /* [in] */ __RPC__in_opt IWICMetadataWriter *pIMetadataWriter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetWriterByIndex( 
            /* [in] */ UINT nIndex,
            /* [in] */ __RPC__in_opt IWICMetadataWriter *pIMetadataWriter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveWriterByIndex( 
            /* [in] */ UINT nIndex) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWICMetadataBlockWriterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWICMetadataBlockWriter * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWICMetadataBlockWriter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWICMetadataBlockWriter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetContainerFormat )( 
            IWICMetadataBlockWriter * This,
            /* [out] */ __RPC__out GUID *pguidContainerFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IWICMetadataBlockWriter * This,
            /* [out] */ __RPC__out UINT *pcCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetReaderByIndex )( 
            IWICMetadataBlockWriter * This,
            /* [in] */ UINT nIndex,
            /* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIMetadataReader);
        
        HRESULT ( STDMETHODCALLTYPE *GetEnumerator )( 
            IWICMetadataBlockWriter * This,
            /* [out] */ __RPC__deref_out_opt IEnumUnknown **ppIEnumMetadata);
        
        HRESULT ( STDMETHODCALLTYPE *InitializeFromBlockReader )( 
            IWICMetadataBlockWriter * This,
            /* [in] */ __RPC__in_opt IWICMetadataBlockReader *pIMDBlockReader);
        
        HRESULT ( STDMETHODCALLTYPE *GetWriterByIndex )( 
            IWICMetadataBlockWriter * This,
            /* [in] */ UINT nIndex,
            /* [out] */ __RPC__deref_out_opt IWICMetadataWriter **ppIMetadataWriter);
        
        HRESULT ( STDMETHODCALLTYPE *AddWriter )( 
            IWICMetadataBlockWriter * This,
            /* [in] */ __RPC__in_opt IWICMetadataWriter *pIMetadataWriter);
        
        HRESULT ( STDMETHODCALLTYPE *SetWriterByIndex )( 
            IWICMetadataBlockWriter * This,
            /* [in] */ UINT nIndex,
            /* [in] */ __RPC__in_opt IWICMetadataWriter *pIMetadataWriter);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveWriterByIndex )( 
            IWICMetadataBlockWriter * This,
            /* [in] */ UINT nIndex);
        
        END_INTERFACE
    } IWICMetadataBlockWriterVtbl;

    interface IWICMetadataBlockWriter
    {
        CONST_VTBL struct IWICMetadataBlockWriterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWICMetadataBlockWriter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWICMetadataBlockWriter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWICMetadataBlockWriter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWICMetadataBlockWriter_GetContainerFormat(This,pguidContainerFormat)	\
    ( (This)->lpVtbl -> GetContainerFormat(This,pguidContainerFormat) ) 

#define IWICMetadataBlockWriter_GetCount(This,pcCount)	\
    ( (This)->lpVtbl -> GetCount(This,pcCount) ) 

#define IWICMetadataBlockWriter_GetReaderByIndex(This,nIndex,ppIMetadataReader)	\
    ( (This)->lpVtbl -> GetReaderByIndex(This,nIndex,ppIMetadataReader) ) 

#define IWICMetadataBlockWriter_GetEnumerator(This,ppIEnumMetadata)	\
    ( (This)->lpVtbl -> GetEnumerator(This,ppIEnumMetadata) ) 


#define IWICMetadataBlockWriter_InitializeFromBlockReader(This,pIMDBlockReader)	\
    ( (This)->lpVtbl -> InitializeFromBlockReader(This,pIMDBlockReader) ) 

#define IWICMetadataBlockWriter_GetWriterByIndex(This,nIndex,ppIMetadataWriter)	\
    ( (This)->lpVtbl -> GetWriterByIndex(This,nIndex,ppIMetadataWriter) ) 

#define IWICMetadataBlockWriter_AddWriter(This,pIMetadataWriter)	\
    ( (This)->lpVtbl -> AddWriter(This,pIMetadataWriter) ) 

#define IWICMetadataBlockWriter_SetWriterByIndex(This,nIndex,pIMetadataWriter)	\
    ( (This)->lpVtbl -> SetWriterByIndex(This,nIndex,pIMetadataWriter) ) 

#define IWICMetadataBlockWriter_RemoveWriterByIndex(This,nIndex)	\
    ( (This)->lpVtbl -> RemoveWriterByIndex(This,nIndex) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWICMetadataBlockWriter_INTERFACE_DEFINED__ */


#ifndef __IWICMetadataReader_INTERFACE_DEFINED__
#define __IWICMetadataReader_INTERFACE_DEFINED__

/* interface IWICMetadataReader */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWICMetadataReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9204FE99-D8FC-4FD5-A001-9536B067A899")
    IWICMetadataReader : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetMetadataFormat( 
            /* [out] */ __RPC__out GUID *pguidMetadataFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMetadataHandlerInfo( 
            /* [out] */ __RPC__deref_out_opt IWICMetadataHandlerInfo **ppIHandler) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ __RPC__out UINT *pcCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetValueByIndex( 
            /* [in] */ UINT nIndex,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarSchema,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarId,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetValue( 
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
            /* [in] */ __RPC__in const PROPVARIANT *pvarId,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetEnumerator( 
            /* [out] */ __RPC__deref_out_opt IWICEnumMetadataItem **ppIEnumMetadata) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWICMetadataReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWICMetadataReader * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWICMetadataReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWICMetadataReader * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMetadataFormat )( 
            IWICMetadataReader * This,
            /* [out] */ __RPC__out GUID *pguidMetadataFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetMetadataHandlerInfo )( 
            IWICMetadataReader * This,
            /* [out] */ __RPC__deref_out_opt IWICMetadataHandlerInfo **ppIHandler);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IWICMetadataReader * This,
            /* [out] */ __RPC__out UINT *pcCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetValueByIndex )( 
            IWICMetadataReader * This,
            /* [in] */ UINT nIndex,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarSchema,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarId,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetValue )( 
            IWICMetadataReader * This,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
            /* [in] */ __RPC__in const PROPVARIANT *pvarId,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetEnumerator )( 
            IWICMetadataReader * This,
            /* [out] */ __RPC__deref_out_opt IWICEnumMetadataItem **ppIEnumMetadata);
        
        END_INTERFACE
    } IWICMetadataReaderVtbl;

    interface IWICMetadataReader
    {
        CONST_VTBL struct IWICMetadataReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWICMetadataReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWICMetadataReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWICMetadataReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWICMetadataReader_GetMetadataFormat(This,pguidMetadataFormat)	\
    ( (This)->lpVtbl -> GetMetadataFormat(This,pguidMetadataFormat) ) 

#define IWICMetadataReader_GetMetadataHandlerInfo(This,ppIHandler)	\
    ( (This)->lpVtbl -> GetMetadataHandlerInfo(This,ppIHandler) ) 

#define IWICMetadataReader_GetCount(This,pcCount)	\
    ( (This)->lpVtbl -> GetCount(This,pcCount) ) 

#define IWICMetadataReader_GetValueByIndex(This,nIndex,pvarSchema,pvarId,pvarValue)	\
    ( (This)->lpVtbl -> GetValueByIndex(This,nIndex,pvarSchema,pvarId,pvarValue) ) 

#define IWICMetadataReader_GetValue(This,pvarSchema,pvarId,pvarValue)	\
    ( (This)->lpVtbl -> GetValue(This,pvarSchema,pvarId,pvarValue) ) 

#define IWICMetadataReader_GetEnumerator(This,ppIEnumMetadata)	\
    ( (This)->lpVtbl -> GetEnumerator(This,ppIEnumMetadata) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWICMetadataReader_INTERFACE_DEFINED__ */


#ifndef __IWICMetadataWriter_INTERFACE_DEFINED__
#define __IWICMetadataWriter_INTERFACE_DEFINED__

/* interface IWICMetadataWriter */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWICMetadataWriter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F7836E16-3BE0-470B-86BB-160D0AECD7DE")
    IWICMetadataWriter : public IWICMetadataReader
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetValue( 
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
            /* [in] */ __RPC__in const PROPVARIANT *pvarId,
            /* [in] */ __RPC__in const PROPVARIANT *pvarValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetValueByIndex( 
            /* [in] */ UINT nIndex,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
            /* [in] */ __RPC__in const PROPVARIANT *pvarId,
            /* [in] */ __RPC__in const PROPVARIANT *pvarValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveValue( 
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
            /* [in] */ __RPC__in const PROPVARIANT *pvarId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveValueByIndex( 
            /* [in] */ UINT nIndex) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWICMetadataWriterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWICMetadataWriter * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWICMetadataWriter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWICMetadataWriter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetMetadataFormat )( 
            IWICMetadataWriter * This,
            /* [out] */ __RPC__out GUID *pguidMetadataFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetMetadataHandlerInfo )( 
            IWICMetadataWriter * This,
            /* [out] */ __RPC__deref_out_opt IWICMetadataHandlerInfo **ppIHandler);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IWICMetadataWriter * This,
            /* [out] */ __RPC__out UINT *pcCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetValueByIndex )( 
            IWICMetadataWriter * This,
            /* [in] */ UINT nIndex,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarSchema,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarId,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetValue )( 
            IWICMetadataWriter * This,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
            /* [in] */ __RPC__in const PROPVARIANT *pvarId,
            /* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetEnumerator )( 
            IWICMetadataWriter * This,
            /* [out] */ __RPC__deref_out_opt IWICEnumMetadataItem **ppIEnumMetadata);
        
        HRESULT ( STDMETHODCALLTYPE *SetValue )( 
            IWICMetadataWriter * This,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
            /* [in] */ __RPC__in const PROPVARIANT *pvarId,
            /* [in] */ __RPC__in const PROPVARIANT *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE *SetValueByIndex )( 
            IWICMetadataWriter * This,
            /* [in] */ UINT nIndex,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
            /* [in] */ __RPC__in const PROPVARIANT *pvarId,
            /* [in] */ __RPC__in const PROPVARIANT *pvarValue);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveValue )( 
            IWICMetadataWriter * This,
            /* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
            /* [in] */ __RPC__in const PROPVARIANT *pvarId);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveValueByIndex )( 
            IWICMetadataWriter * This,
            /* [in] */ UINT nIndex);
        
        END_INTERFACE
    } IWICMetadataWriterVtbl;

    interface IWICMetadataWriter
    {
        CONST_VTBL struct IWICMetadataWriterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWICMetadataWriter_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWICMetadataWriter_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWICMetadataWriter_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWICMetadataWriter_GetMetadataFormat(This,pguidMetadataFormat)	\
    ( (This)->lpVtbl -> GetMetadataFormat(This,pguidMetadataFormat) ) 

#define IWICMetadataWriter_GetMetadataHandlerInfo(This,ppIHandler)	\
    ( (This)->lpVtbl -> GetMetadataHandlerInfo(This,ppIHandler) ) 

#define IWICMetadataWriter_GetCount(This,pcCount)	\
    ( (This)->lpVtbl -> GetCount(This,pcCount) ) 

#define IWICMetadataWriter_GetValueByIndex(This,nIndex,pvarSchema,pvarId,pvarValue)	\
    ( (This)->lpVtbl -> GetValueByIndex(This,nIndex,pvarSchema,pvarId,pvarValue) ) 

#define IWICMetadataWriter_GetValue(This,pvarSchema,pvarId,pvarValue)	\
    ( (This)->lpVtbl -> GetValue(This,pvarSchema,pvarId,pvarValue) ) 

#define IWICMetadataWriter_GetEnumerator(This,ppIEnumMetadata)	\
    ( (This)->lpVtbl -> GetEnumerator(This,ppIEnumMetadata) ) 


#define IWICMetadataWriter_SetValue(This,pvarSchema,pvarId,pvarValue)	\
    ( (This)->lpVtbl -> SetValue(This,pvarSchema,pvarId,pvarValue) ) 

#define IWICMetadataWriter_SetValueByIndex(This,nIndex,pvarSchema,pvarId,pvarValue)	\
    ( (This)->lpVtbl -> SetValueByIndex(This,nIndex,pvarSchema,pvarId,pvarValue) ) 

#define IWICMetadataWriter_RemoveValue(This,pvarSchema,pvarId)	\
    ( (This)->lpVtbl -> RemoveValue(This,pvarSchema,pvarId) ) 

#define IWICMetadataWriter_RemoveValueByIndex(This,nIndex)	\
    ( (This)->lpVtbl -> RemoveValueByIndex(This,nIndex) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWICMetadataWriter_INTERFACE_DEFINED__ */


#ifndef __IWICStreamProvider_INTERFACE_DEFINED__
#define __IWICStreamProvider_INTERFACE_DEFINED__

/* interface IWICStreamProvider */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWICStreamProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("449494BC-B468-4927-96D7-BA90D31AB505")
    IWICStreamProvider : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetStream( 
            /* [out] */ __RPC__deref_out_opt IStream **ppIStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPersistOptions( 
            /* [out] */ __RPC__out DWORD *pdwPersistOptions) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPreferredVendorGUID( 
            /* [out] */ __RPC__out GUID *pguidPreferredVendor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RefreshStream( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWICStreamProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWICStreamProvider * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWICStreamProvider * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWICStreamProvider * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetStream )( 
            IWICStreamProvider * This,
            /* [out] */ __RPC__deref_out_opt IStream **ppIStream);
        
        HRESULT ( STDMETHODCALLTYPE *GetPersistOptions )( 
            IWICStreamProvider * This,
            /* [out] */ __RPC__out DWORD *pdwPersistOptions);
        
        HRESULT ( STDMETHODCALLTYPE *GetPreferredVendorGUID )( 
            IWICStreamProvider * This,
            /* [out] */ __RPC__out GUID *pguidPreferredVendor);
        
        HRESULT ( STDMETHODCALLTYPE *RefreshStream )( 
            IWICStreamProvider * This);
        
        END_INTERFACE
    } IWICStreamProviderVtbl;

    interface IWICStreamProvider
    {
        CONST_VTBL struct IWICStreamProviderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWICStreamProvider_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWICStreamProvider_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWICStreamProvider_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWICStreamProvider_GetStream(This,ppIStream)	\
    ( (This)->lpVtbl -> GetStream(This,ppIStream) ) 

#define IWICStreamProvider_GetPersistOptions(This,pdwPersistOptions)	\
    ( (This)->lpVtbl -> GetPersistOptions(This,pdwPersistOptions) ) 

#define IWICStreamProvider_GetPreferredVendorGUID(This,pguidPreferredVendor)	\
    ( (This)->lpVtbl -> GetPreferredVendorGUID(This,pguidPreferredVendor) ) 

#define IWICStreamProvider_RefreshStream(This)	\
    ( (This)->lpVtbl -> RefreshStream(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWICStreamProvider_INTERFACE_DEFINED__ */


#ifndef __IWICPersistStream_INTERFACE_DEFINED__
#define __IWICPersistStream_INTERFACE_DEFINED__

/* interface IWICPersistStream */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWICPersistStream;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("00675040-6908-45F8-86A3-49C7DFD6D9AD")
    IWICPersistStream : public IPersistStream
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE LoadEx( 
            /* [unique][in] */ __RPC__in_opt IStream *pIStream,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidPreferredVendor,
            /* [in] */ DWORD dwPersistOptions) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SaveEx( 
            /* [in] */ __RPC__in_opt IStream *pIStream,
            /* [in] */ DWORD dwPersistOptions,
            /* [in] */ BOOL fClearDirty) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWICPersistStreamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWICPersistStream * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWICPersistStream * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWICPersistStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetClassID )( 
            IWICPersistStream * This,
            /* [out] */ __RPC__out CLSID *pClassID);
        
        HRESULT ( STDMETHODCALLTYPE *IsDirty )( 
            IWICPersistStream * This);
        
        HRESULT ( STDMETHODCALLTYPE *Load )( 
            IWICPersistStream * This,
            /* [unique][in] */ __RPC__in_opt IStream *pStm);
        
        HRESULT ( STDMETHODCALLTYPE *Save )( 
            IWICPersistStream * This,
            /* [unique][in] */ __RPC__in_opt IStream *pStm,
            /* [in] */ BOOL fClearDirty);
        
        HRESULT ( STDMETHODCALLTYPE *GetSizeMax )( 
            IWICPersistStream * This,
            /* [out] */ __RPC__out ULARGE_INTEGER *pcbSize);
        
        HRESULT ( STDMETHODCALLTYPE *LoadEx )( 
            IWICPersistStream * This,
            /* [unique][in] */ __RPC__in_opt IStream *pIStream,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidPreferredVendor,
            /* [in] */ DWORD dwPersistOptions);
        
        HRESULT ( STDMETHODCALLTYPE *SaveEx )( 
            IWICPersistStream * This,
            /* [in] */ __RPC__in_opt IStream *pIStream,
            /* [in] */ DWORD dwPersistOptions,
            /* [in] */ BOOL fClearDirty);
        
        END_INTERFACE
    } IWICPersistStreamVtbl;

    interface IWICPersistStream
    {
        CONST_VTBL struct IWICPersistStreamVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWICPersistStream_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWICPersistStream_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWICPersistStream_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWICPersistStream_GetClassID(This,pClassID)	\
    ( (This)->lpVtbl -> GetClassID(This,pClassID) ) 


#define IWICPersistStream_IsDirty(This)	\
    ( (This)->lpVtbl -> IsDirty(This) ) 

#define IWICPersistStream_Load(This,pStm)	\
    ( (This)->lpVtbl -> Load(This,pStm) ) 

#define IWICPersistStream_Save(This,pStm,fClearDirty)	\
    ( (This)->lpVtbl -> Save(This,pStm,fClearDirty) ) 

#define IWICPersistStream_GetSizeMax(This,pcbSize)	\
    ( (This)->lpVtbl -> GetSizeMax(This,pcbSize) ) 


#define IWICPersistStream_LoadEx(This,pIStream,pguidPreferredVendor,dwPersistOptions)	\
    ( (This)->lpVtbl -> LoadEx(This,pIStream,pguidPreferredVendor,dwPersistOptions) ) 

#define IWICPersistStream_SaveEx(This,pIStream,dwPersistOptions,fClearDirty)	\
    ( (This)->lpVtbl -> SaveEx(This,pIStream,dwPersistOptions,fClearDirty) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWICPersistStream_INTERFACE_DEFINED__ */


#ifndef __IWICMetadataHandlerInfo_INTERFACE_DEFINED__
#define __IWICMetadataHandlerInfo_INTERFACE_DEFINED__

/* interface IWICMetadataHandlerInfo */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWICMetadataHandlerInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ABA958BF-C672-44D1-8D61-CE6DF2E682C2")
    IWICMetadataHandlerInfo : public IWICComponentInfo
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetMetadataFormat( 
            /* [out] */ __RPC__out GUID *pguidMetadataFormat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContainerFormats( 
            /* [in] */ UINT cContainerFormats,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cContainerFormats) GUID *pguidContainerFormats,
            /* [out] */ __RPC__out UINT *pcchActual) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDeviceManufacturer( 
            /* [in] */ UINT cchDeviceManufacturer,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceManufacturer) WCHAR *wzDeviceManufacturer,
            /* [out] */ __RPC__out UINT *pcchActual) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDeviceModels( 
            /* [in] */ UINT cchDeviceModels,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceModels) WCHAR *wzDeviceModels,
            /* [out] */ __RPC__out UINT *pcchActual) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DoesRequireFullStream( 
            /* [out] */ __RPC__out BOOL *pfRequiresFullStream) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DoesSupportPadding( 
            /* [out] */ __RPC__out BOOL *pfSupportsPadding) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DoesRequireFixedSize( 
            /* [out] */ __RPC__out BOOL *pfFixedSize) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWICMetadataHandlerInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWICMetadataHandlerInfo * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWICMetadataHandlerInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWICMetadataHandlerInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetComponentType )( 
            IWICMetadataHandlerInfo * This,
            /* [out] */ __RPC__out WICComponentType *pType);
        
        HRESULT ( STDMETHODCALLTYPE *GetCLSID )( 
            IWICMetadataHandlerInfo * This,
            /* [out] */ __RPC__out CLSID *pclsid);
        
        HRESULT ( STDMETHODCALLTYPE *GetSigningStatus )( 
            IWICMetadataHandlerInfo * This,
            /* [out] */ __RPC__out DWORD *pStatus);
        
        HRESULT ( STDMETHODCALLTYPE *GetAuthor )( 
            IWICMetadataHandlerInfo * This,
            /* [in] */ UINT cchAuthor,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchAuthor) WCHAR *wzAuthor,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetVendorGUID )( 
            IWICMetadataHandlerInfo * This,
            /* [out] */ __RPC__out GUID *pguidVendor);
        
        HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IWICMetadataHandlerInfo * This,
            /* [in] */ UINT cchVersion,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchVersion) WCHAR *wzVersion,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetSpecVersion )( 
            IWICMetadataHandlerInfo * This,
            /* [in] */ UINT cchSpecVersion,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchSpecVersion) WCHAR *wzSpecVersion,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetFriendlyName )( 
            IWICMetadataHandlerInfo * This,
            /* [in] */ UINT cchFriendlyName,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchFriendlyName) WCHAR *wzFriendlyName,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetMetadataFormat )( 
            IWICMetadataHandlerInfo * This,
            /* [out] */ __RPC__out GUID *pguidMetadataFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetContainerFormats )( 
            IWICMetadataHandlerInfo * This,
            /* [in] */ UINT cContainerFormats,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cContainerFormats) GUID *pguidContainerFormats,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetDeviceManufacturer )( 
            IWICMetadataHandlerInfo * This,
            /* [in] */ UINT cchDeviceManufacturer,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceManufacturer) WCHAR *wzDeviceManufacturer,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetDeviceModels )( 
            IWICMetadataHandlerInfo * This,
            /* [in] */ UINT cchDeviceModels,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceModels) WCHAR *wzDeviceModels,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *DoesRequireFullStream )( 
            IWICMetadataHandlerInfo * This,
            /* [out] */ __RPC__out BOOL *pfRequiresFullStream);
        
        HRESULT ( STDMETHODCALLTYPE *DoesSupportPadding )( 
            IWICMetadataHandlerInfo * This,
            /* [out] */ __RPC__out BOOL *pfSupportsPadding);
        
        HRESULT ( STDMETHODCALLTYPE *DoesRequireFixedSize )( 
            IWICMetadataHandlerInfo * This,
            /* [out] */ __RPC__out BOOL *pfFixedSize);
        
        END_INTERFACE
    } IWICMetadataHandlerInfoVtbl;

    interface IWICMetadataHandlerInfo
    {
        CONST_VTBL struct IWICMetadataHandlerInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWICMetadataHandlerInfo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWICMetadataHandlerInfo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWICMetadataHandlerInfo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWICMetadataHandlerInfo_GetComponentType(This,pType)	\
    ( (This)->lpVtbl -> GetComponentType(This,pType) ) 

#define IWICMetadataHandlerInfo_GetCLSID(This,pclsid)	\
    ( (This)->lpVtbl -> GetCLSID(This,pclsid) ) 

#define IWICMetadataHandlerInfo_GetSigningStatus(This,pStatus)	\
    ( (This)->lpVtbl -> GetSigningStatus(This,pStatus) ) 

#define IWICMetadataHandlerInfo_GetAuthor(This,cchAuthor,wzAuthor,pcchActual)	\
    ( (This)->lpVtbl -> GetAuthor(This,cchAuthor,wzAuthor,pcchActual) ) 

#define IWICMetadataHandlerInfo_GetVendorGUID(This,pguidVendor)	\
    ( (This)->lpVtbl -> GetVendorGUID(This,pguidVendor) ) 

#define IWICMetadataHandlerInfo_GetVersion(This,cchVersion,wzVersion,pcchActual)	\
    ( (This)->lpVtbl -> GetVersion(This,cchVersion,wzVersion,pcchActual) ) 

#define IWICMetadataHandlerInfo_GetSpecVersion(This,cchSpecVersion,wzSpecVersion,pcchActual)	\
    ( (This)->lpVtbl -> GetSpecVersion(This,cchSpecVersion,wzSpecVersion,pcchActual) ) 

#define IWICMetadataHandlerInfo_GetFriendlyName(This,cchFriendlyName,wzFriendlyName,pcchActual)	\
    ( (This)->lpVtbl -> GetFriendlyName(This,cchFriendlyName,wzFriendlyName,pcchActual) ) 


#define IWICMetadataHandlerInfo_GetMetadataFormat(This,pguidMetadataFormat)	\
    ( (This)->lpVtbl -> GetMetadataFormat(This,pguidMetadataFormat) ) 

#define IWICMetadataHandlerInfo_GetContainerFormats(This,cContainerFormats,pguidContainerFormats,pcchActual)	\
    ( (This)->lpVtbl -> GetContainerFormats(This,cContainerFormats,pguidContainerFormats,pcchActual) ) 

#define IWICMetadataHandlerInfo_GetDeviceManufacturer(This,cchDeviceManufacturer,wzDeviceManufacturer,pcchActual)	\
    ( (This)->lpVtbl -> GetDeviceManufacturer(This,cchDeviceManufacturer,wzDeviceManufacturer,pcchActual) ) 

#define IWICMetadataHandlerInfo_GetDeviceModels(This,cchDeviceModels,wzDeviceModels,pcchActual)	\
    ( (This)->lpVtbl -> GetDeviceModels(This,cchDeviceModels,wzDeviceModels,pcchActual) ) 

#define IWICMetadataHandlerInfo_DoesRequireFullStream(This,pfRequiresFullStream)	\
    ( (This)->lpVtbl -> DoesRequireFullStream(This,pfRequiresFullStream) ) 

#define IWICMetadataHandlerInfo_DoesSupportPadding(This,pfSupportsPadding)	\
    ( (This)->lpVtbl -> DoesSupportPadding(This,pfSupportsPadding) ) 

#define IWICMetadataHandlerInfo_DoesRequireFixedSize(This,pfFixedSize)	\
    ( (This)->lpVtbl -> DoesRequireFixedSize(This,pfFixedSize) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWICMetadataHandlerInfo_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wincodecsdk_0000_0007 */
/* [local] */ 

typedef struct WICMetadataPattern
    {
    ULARGE_INTEGER Position;
    ULONG Length;
    BYTE *Pattern;
    BYTE *Mask;
    ULARGE_INTEGER DataOffset;
    } 	WICMetadataPattern;



extern RPC_IF_HANDLE __MIDL_itf_wincodecsdk_0000_0007_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wincodecsdk_0000_0007_v0_0_s_ifspec;

#ifndef __IWICMetadataReaderInfo_INTERFACE_DEFINED__
#define __IWICMetadataReaderInfo_INTERFACE_DEFINED__

/* interface IWICMetadataReaderInfo */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWICMetadataReaderInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EEBF1F5B-07C1-4447-A3AB-22ACAF78A804")
    IWICMetadataReaderInfo : public IWICMetadataHandlerInfo
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetPatterns( 
            /* [in] */ __RPC__in REFGUID guidContainerFormat,
            /* [in] */ UINT cbSize,
            /* [unique][out][in] */ __RPC__inout_opt WICMetadataPattern *pPattern,
            /* [unique][out][in] */ __RPC__inout_opt UINT *pcCount,
            /* [unique][out][in] */ __RPC__inout_opt UINT *pcbActual) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MatchesPattern( 
            /* [in] */ __RPC__in REFGUID guidContainerFormat,
            /* [in] */ __RPC__in_opt IStream *pIStream,
            /* [out] */ __RPC__out BOOL *pfMatches) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIReader) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWICMetadataReaderInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWICMetadataReaderInfo * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWICMetadataReaderInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWICMetadataReaderInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetComponentType )( 
            IWICMetadataReaderInfo * This,
            /* [out] */ __RPC__out WICComponentType *pType);
        
        HRESULT ( STDMETHODCALLTYPE *GetCLSID )( 
            IWICMetadataReaderInfo * This,
            /* [out] */ __RPC__out CLSID *pclsid);
        
        HRESULT ( STDMETHODCALLTYPE *GetSigningStatus )( 
            IWICMetadataReaderInfo * This,
            /* [out] */ __RPC__out DWORD *pStatus);
        
        HRESULT ( STDMETHODCALLTYPE *GetAuthor )( 
            IWICMetadataReaderInfo * This,
            /* [in] */ UINT cchAuthor,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchAuthor) WCHAR *wzAuthor,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetVendorGUID )( 
            IWICMetadataReaderInfo * This,
            /* [out] */ __RPC__out GUID *pguidVendor);
        
        HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IWICMetadataReaderInfo * This,
            /* [in] */ UINT cchVersion,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchVersion) WCHAR *wzVersion,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetSpecVersion )( 
            IWICMetadataReaderInfo * This,
            /* [in] */ UINT cchSpecVersion,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchSpecVersion) WCHAR *wzSpecVersion,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetFriendlyName )( 
            IWICMetadataReaderInfo * This,
            /* [in] */ UINT cchFriendlyName,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchFriendlyName) WCHAR *wzFriendlyName,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetMetadataFormat )( 
            IWICMetadataReaderInfo * This,
            /* [out] */ __RPC__out GUID *pguidMetadataFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetContainerFormats )( 
            IWICMetadataReaderInfo * This,
            /* [in] */ UINT cContainerFormats,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cContainerFormats) GUID *pguidContainerFormats,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetDeviceManufacturer )( 
            IWICMetadataReaderInfo * This,
            /* [in] */ UINT cchDeviceManufacturer,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceManufacturer) WCHAR *wzDeviceManufacturer,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetDeviceModels )( 
            IWICMetadataReaderInfo * This,
            /* [in] */ UINT cchDeviceModels,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceModels) WCHAR *wzDeviceModels,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *DoesRequireFullStream )( 
            IWICMetadataReaderInfo * This,
            /* [out] */ __RPC__out BOOL *pfRequiresFullStream);
        
        HRESULT ( STDMETHODCALLTYPE *DoesSupportPadding )( 
            IWICMetadataReaderInfo * This,
            /* [out] */ __RPC__out BOOL *pfSupportsPadding);
        
        HRESULT ( STDMETHODCALLTYPE *DoesRequireFixedSize )( 
            IWICMetadataReaderInfo * This,
            /* [out] */ __RPC__out BOOL *pfFixedSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetPatterns )( 
            IWICMetadataReaderInfo * This,
            /* [in] */ __RPC__in REFGUID guidContainerFormat,
            /* [in] */ UINT cbSize,
            /* [unique][out][in] */ __RPC__inout_opt WICMetadataPattern *pPattern,
            /* [unique][out][in] */ __RPC__inout_opt UINT *pcCount,
            /* [unique][out][in] */ __RPC__inout_opt UINT *pcbActual);
        
        HRESULT ( STDMETHODCALLTYPE *MatchesPattern )( 
            IWICMetadataReaderInfo * This,
            /* [in] */ __RPC__in REFGUID guidContainerFormat,
            /* [in] */ __RPC__in_opt IStream *pIStream,
            /* [out] */ __RPC__out BOOL *pfMatches);
        
        HRESULT ( STDMETHODCALLTYPE *CreateInstance )( 
            IWICMetadataReaderInfo * This,
            /* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIReader);
        
        END_INTERFACE
    } IWICMetadataReaderInfoVtbl;

    interface IWICMetadataReaderInfo
    {
        CONST_VTBL struct IWICMetadataReaderInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWICMetadataReaderInfo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWICMetadataReaderInfo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWICMetadataReaderInfo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWICMetadataReaderInfo_GetComponentType(This,pType)	\
    ( (This)->lpVtbl -> GetComponentType(This,pType) ) 

#define IWICMetadataReaderInfo_GetCLSID(This,pclsid)	\
    ( (This)->lpVtbl -> GetCLSID(This,pclsid) ) 

#define IWICMetadataReaderInfo_GetSigningStatus(This,pStatus)	\
    ( (This)->lpVtbl -> GetSigningStatus(This,pStatus) ) 

#define IWICMetadataReaderInfo_GetAuthor(This,cchAuthor,wzAuthor,pcchActual)	\
    ( (This)->lpVtbl -> GetAuthor(This,cchAuthor,wzAuthor,pcchActual) ) 

#define IWICMetadataReaderInfo_GetVendorGUID(This,pguidVendor)	\
    ( (This)->lpVtbl -> GetVendorGUID(This,pguidVendor) ) 

#define IWICMetadataReaderInfo_GetVersion(This,cchVersion,wzVersion,pcchActual)	\
    ( (This)->lpVtbl -> GetVersion(This,cchVersion,wzVersion,pcchActual) ) 

#define IWICMetadataReaderInfo_GetSpecVersion(This,cchSpecVersion,wzSpecVersion,pcchActual)	\
    ( (This)->lpVtbl -> GetSpecVersion(This,cchSpecVersion,wzSpecVersion,pcchActual) ) 

#define IWICMetadataReaderInfo_GetFriendlyName(This,cchFriendlyName,wzFriendlyName,pcchActual)	\
    ( (This)->lpVtbl -> GetFriendlyName(This,cchFriendlyName,wzFriendlyName,pcchActual) ) 


#define IWICMetadataReaderInfo_GetMetadataFormat(This,pguidMetadataFormat)	\
    ( (This)->lpVtbl -> GetMetadataFormat(This,pguidMetadataFormat) ) 

#define IWICMetadataReaderInfo_GetContainerFormats(This,cContainerFormats,pguidContainerFormats,pcchActual)	\
    ( (This)->lpVtbl -> GetContainerFormats(This,cContainerFormats,pguidContainerFormats,pcchActual) ) 

#define IWICMetadataReaderInfo_GetDeviceManufacturer(This,cchDeviceManufacturer,wzDeviceManufacturer,pcchActual)	\
    ( (This)->lpVtbl -> GetDeviceManufacturer(This,cchDeviceManufacturer,wzDeviceManufacturer,pcchActual) ) 

#define IWICMetadataReaderInfo_GetDeviceModels(This,cchDeviceModels,wzDeviceModels,pcchActual)	\
    ( (This)->lpVtbl -> GetDeviceModels(This,cchDeviceModels,wzDeviceModels,pcchActual) ) 

#define IWICMetadataReaderInfo_DoesRequireFullStream(This,pfRequiresFullStream)	\
    ( (This)->lpVtbl -> DoesRequireFullStream(This,pfRequiresFullStream) ) 

#define IWICMetadataReaderInfo_DoesSupportPadding(This,pfSupportsPadding)	\
    ( (This)->lpVtbl -> DoesSupportPadding(This,pfSupportsPadding) ) 

#define IWICMetadataReaderInfo_DoesRequireFixedSize(This,pfFixedSize)	\
    ( (This)->lpVtbl -> DoesRequireFixedSize(This,pfFixedSize) ) 


#define IWICMetadataReaderInfo_GetPatterns(This,guidContainerFormat,cbSize,pPattern,pcCount,pcbActual)	\
    ( (This)->lpVtbl -> GetPatterns(This,guidContainerFormat,cbSize,pPattern,pcCount,pcbActual) ) 

#define IWICMetadataReaderInfo_MatchesPattern(This,guidContainerFormat,pIStream,pfMatches)	\
    ( (This)->lpVtbl -> MatchesPattern(This,guidContainerFormat,pIStream,pfMatches) ) 

#define IWICMetadataReaderInfo_CreateInstance(This,ppIReader)	\
    ( (This)->lpVtbl -> CreateInstance(This,ppIReader) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWICMetadataReaderInfo_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wincodecsdk_0000_0008 */
/* [local] */ 

typedef struct WICMetadataHeader
    {
    ULARGE_INTEGER Position;
    ULONG Length;
    BYTE *Header;
    ULARGE_INTEGER DataOffset;
    } 	WICMetadataHeader;



extern RPC_IF_HANDLE __MIDL_itf_wincodecsdk_0000_0008_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wincodecsdk_0000_0008_v0_0_s_ifspec;

#ifndef __IWICMetadataWriterInfo_INTERFACE_DEFINED__
#define __IWICMetadataWriterInfo_INTERFACE_DEFINED__

/* interface IWICMetadataWriterInfo */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWICMetadataWriterInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B22E3FBA-3925-4323-B5C1-9EBFC430F236")
    IWICMetadataWriterInfo : public IWICMetadataHandlerInfo
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetHeader( 
            /* [in] */ __RPC__in REFGUID guidContainerFormat,
            /* [in] */ UINT cbSize,
            /* [unique][out][in] */ __RPC__inout_opt WICMetadataHeader *pHeader,
            /* [unique][out][in] */ __RPC__inout_opt UINT *pcbActual) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [out] */ __RPC__deref_out_opt IWICMetadataWriter **ppIWriter) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWICMetadataWriterInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWICMetadataWriterInfo * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWICMetadataWriterInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWICMetadataWriterInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetComponentType )( 
            IWICMetadataWriterInfo * This,
            /* [out] */ __RPC__out WICComponentType *pType);
        
        HRESULT ( STDMETHODCALLTYPE *GetCLSID )( 
            IWICMetadataWriterInfo * This,
            /* [out] */ __RPC__out CLSID *pclsid);
        
        HRESULT ( STDMETHODCALLTYPE *GetSigningStatus )( 
            IWICMetadataWriterInfo * This,
            /* [out] */ __RPC__out DWORD *pStatus);
        
        HRESULT ( STDMETHODCALLTYPE *GetAuthor )( 
            IWICMetadataWriterInfo * This,
            /* [in] */ UINT cchAuthor,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchAuthor) WCHAR *wzAuthor,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetVendorGUID )( 
            IWICMetadataWriterInfo * This,
            /* [out] */ __RPC__out GUID *pguidVendor);
        
        HRESULT ( STDMETHODCALLTYPE *GetVersion )( 
            IWICMetadataWriterInfo * This,
            /* [in] */ UINT cchVersion,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchVersion) WCHAR *wzVersion,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetSpecVersion )( 
            IWICMetadataWriterInfo * This,
            /* [in] */ UINT cchSpecVersion,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchSpecVersion) WCHAR *wzSpecVersion,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetFriendlyName )( 
            IWICMetadataWriterInfo * This,
            /* [in] */ UINT cchFriendlyName,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchFriendlyName) WCHAR *wzFriendlyName,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetMetadataFormat )( 
            IWICMetadataWriterInfo * This,
            /* [out] */ __RPC__out GUID *pguidMetadataFormat);
        
        HRESULT ( STDMETHODCALLTYPE *GetContainerFormats )( 
            IWICMetadataWriterInfo * This,
            /* [in] */ UINT cContainerFormats,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cContainerFormats) GUID *pguidContainerFormats,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetDeviceManufacturer )( 
            IWICMetadataWriterInfo * This,
            /* [in] */ UINT cchDeviceManufacturer,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceManufacturer) WCHAR *wzDeviceManufacturer,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *GetDeviceModels )( 
            IWICMetadataWriterInfo * This,
            /* [in] */ UINT cchDeviceModels,
            /* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceModels) WCHAR *wzDeviceModels,
            /* [out] */ __RPC__out UINT *pcchActual);
        
        HRESULT ( STDMETHODCALLTYPE *DoesRequireFullStream )( 
            IWICMetadataWriterInfo * This,
            /* [out] */ __RPC__out BOOL *pfRequiresFullStream);
        
        HRESULT ( STDMETHODCALLTYPE *DoesSupportPadding )( 
            IWICMetadataWriterInfo * This,
            /* [out] */ __RPC__out BOOL *pfSupportsPadding);
        
        HRESULT ( STDMETHODCALLTYPE *DoesRequireFixedSize )( 
            IWICMetadataWriterInfo * This,
            /* [out] */ __RPC__out BOOL *pfFixedSize);
        
        HRESULT ( STDMETHODCALLTYPE *GetHeader )( 
            IWICMetadataWriterInfo * This,
            /* [in] */ __RPC__in REFGUID guidContainerFormat,
            /* [in] */ UINT cbSize,
            /* [unique][out][in] */ __RPC__inout_opt WICMetadataHeader *pHeader,
            /* [unique][out][in] */ __RPC__inout_opt UINT *pcbActual);
        
        HRESULT ( STDMETHODCALLTYPE *CreateInstance )( 
            IWICMetadataWriterInfo * This,
            /* [out] */ __RPC__deref_out_opt IWICMetadataWriter **ppIWriter);
        
        END_INTERFACE
    } IWICMetadataWriterInfoVtbl;

    interface IWICMetadataWriterInfo
    {
        CONST_VTBL struct IWICMetadataWriterInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWICMetadataWriterInfo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWICMetadataWriterInfo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWICMetadataWriterInfo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWICMetadataWriterInfo_GetComponentType(This,pType)	\
    ( (This)->lpVtbl -> GetComponentType(This,pType) ) 

#define IWICMetadataWriterInfo_GetCLSID(This,pclsid)	\
    ( (This)->lpVtbl -> GetCLSID(This,pclsid) ) 

#define IWICMetadataWriterInfo_GetSigningStatus(This,pStatus)	\
    ( (This)->lpVtbl -> GetSigningStatus(This,pStatus) ) 

#define IWICMetadataWriterInfo_GetAuthor(This,cchAuthor,wzAuthor,pcchActual)	\
    ( (This)->lpVtbl -> GetAuthor(This,cchAuthor,wzAuthor,pcchActual) ) 

#define IWICMetadataWriterInfo_GetVendorGUID(This,pguidVendor)	\
    ( (This)->lpVtbl -> GetVendorGUID(This,pguidVendor) ) 

#define IWICMetadataWriterInfo_GetVersion(This,cchVersion,wzVersion,pcchActual)	\
    ( (This)->lpVtbl -> GetVersion(This,cchVersion,wzVersion,pcchActual) ) 

#define IWICMetadataWriterInfo_GetSpecVersion(This,cchSpecVersion,wzSpecVersion,pcchActual)	\
    ( (This)->lpVtbl -> GetSpecVersion(This,cchSpecVersion,wzSpecVersion,pcchActual) ) 

#define IWICMetadataWriterInfo_GetFriendlyName(This,cchFriendlyName,wzFriendlyName,pcchActual)	\
    ( (This)->lpVtbl -> GetFriendlyName(This,cchFriendlyName,wzFriendlyName,pcchActual) ) 


#define IWICMetadataWriterInfo_GetMetadataFormat(This,pguidMetadataFormat)	\
    ( (This)->lpVtbl -> GetMetadataFormat(This,pguidMetadataFormat) ) 

#define IWICMetadataWriterInfo_GetContainerFormats(This,cContainerFormats,pguidContainerFormats,pcchActual)	\
    ( (This)->lpVtbl -> GetContainerFormats(This,cContainerFormats,pguidContainerFormats,pcchActual) ) 

#define IWICMetadataWriterInfo_GetDeviceManufacturer(This,cchDeviceManufacturer,wzDeviceManufacturer,pcchActual)	\
    ( (This)->lpVtbl -> GetDeviceManufacturer(This,cchDeviceManufacturer,wzDeviceManufacturer,pcchActual) ) 

#define IWICMetadataWriterInfo_GetDeviceModels(This,cchDeviceModels,wzDeviceModels,pcchActual)	\
    ( (This)->lpVtbl -> GetDeviceModels(This,cchDeviceModels,wzDeviceModels,pcchActual) ) 

#define IWICMetadataWriterInfo_DoesRequireFullStream(This,pfRequiresFullStream)	\
    ( (This)->lpVtbl -> DoesRequireFullStream(This,pfRequiresFullStream) ) 

#define IWICMetadataWriterInfo_DoesSupportPadding(This,pfSupportsPadding)	\
    ( (This)->lpVtbl -> DoesSupportPadding(This,pfSupportsPadding) ) 

#define IWICMetadataWriterInfo_DoesRequireFixedSize(This,pfFixedSize)	\
    ( (This)->lpVtbl -> DoesRequireFixedSize(This,pfFixedSize) ) 


#define IWICMetadataWriterInfo_GetHeader(This,guidContainerFormat,cbSize,pHeader,pcbActual)	\
    ( (This)->lpVtbl -> GetHeader(This,guidContainerFormat,cbSize,pHeader,pcbActual) ) 

#define IWICMetadataWriterInfo_CreateInstance(This,ppIWriter)	\
    ( (This)->lpVtbl -> CreateInstance(This,ppIWriter) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWICMetadataWriterInfo_INTERFACE_DEFINED__ */


#ifndef __IWICComponentFactory_INTERFACE_DEFINED__
#define __IWICComponentFactory_INTERFACE_DEFINED__

/* interface IWICComponentFactory */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWICComponentFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("412D0C3A-9650-44FA-AF5B-DD2A06C8E8FB")
    IWICComponentFactory : public IWICImagingFactory
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateMetadataReader( 
            /* [in] */ __RPC__in REFGUID guidMetadataFormat,
            /* [size_is][unique][in] */ __RPC__in_ecount_full_opt(1) const GUID *pguidVendor,
            /* [in] */ DWORD dwOptions,
            /* [in] */ __RPC__in_opt IStream *pIStream,
            /* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIReader) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateMetadataReaderFromContainer( 
            /* [in] */ __RPC__in REFGUID guidContainerFormat,
            /* [size_is][unique][in] */ __RPC__in_ecount_full_opt(1) const GUID *pguidVendor,
            /* [in] */ DWORD dwOptions,
            /* [in] */ __RPC__in_opt IStream *pIStream,
            /* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIReader) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateMetadataWriter( 
            /* [in] */ __RPC__in REFGUID guidMetadataFormat,
            /* [size_is][unique][in] */ __RPC__in_ecount_full_opt(1) const GUID *pguidVendor,
            /* [in] */ DWORD dwMetadataOptions,
            /* [out] */ __RPC__deref_out_opt IWICMetadataWriter **ppIWriter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateMetadataWriterFromReader( 
            /* [in] */ __RPC__in_opt IWICMetadataReader *pIReader,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidVendor,
            /* [out] */ __RPC__deref_out_opt IWICMetadataWriter **ppIWriter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateQueryReaderFromBlockReader( 
            /* [in] */ __RPC__in_opt IWICMetadataBlockReader *pIBlockReader,
            /* [out] */ __RPC__deref_out_opt IWICMetadataQueryReader **ppIQueryReader) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateQueryWriterFromBlockWriter( 
            /* [in] */ __RPC__in_opt IWICMetadataBlockWriter *pIBlockWriter,
            /* [out] */ __RPC__deref_out_opt IWICMetadataQueryWriter **ppIQueryWriter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateEncoderPropertyBag( 
            /* [size_is][unique][in] */ __RPC__in_ecount_full_opt(cCount) PROPBAG2 *ppropOptions,
            /* [in] */ UINT cCount,
            /* [out] */ __RPC__deref_out_opt IPropertyBag2 **ppIPropertyBag) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWICComponentFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWICComponentFactory * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWICComponentFactory * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDecoderFromFilename )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in LPCWSTR wzFilename,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidVendor,
            /* [in] */ DWORD dwDesiredAccess,
            /* [in] */ WICDecodeOptions metadataOptions,
            /* [retval][out] */ __RPC__deref_out_opt IWICBitmapDecoder **ppIDecoder);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDecoderFromStream )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in_opt IStream *pIStream,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidVendor,
            /* [in] */ WICDecodeOptions metadataOptions,
            /* [retval][out] */ __RPC__deref_out_opt IWICBitmapDecoder **ppIDecoder);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDecoderFromFileHandle )( 
            IWICComponentFactory * This,
            /* [in] */ ULONG_PTR hFile,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidVendor,
            /* [in] */ WICDecodeOptions metadataOptions,
            /* [retval][out] */ __RPC__deref_out_opt IWICBitmapDecoder **ppIDecoder);
        
        HRESULT ( STDMETHODCALLTYPE *CreateComponentInfo )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in REFCLSID clsidComponent,
            /* [out] */ __RPC__deref_out_opt IWICComponentInfo **ppIInfo);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDecoder )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in REFGUID guidContainerFormat,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidVendor,
            /* [retval][out] */ __RPC__deref_out_opt IWICBitmapDecoder **ppIDecoder);
        
        HRESULT ( STDMETHODCALLTYPE *CreateEncoder )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in REFGUID guidContainerFormat,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidVendor,
            /* [retval][out] */ __RPC__deref_out_opt IWICBitmapEncoder **ppIEncoder);
        
        HRESULT ( STDMETHODCALLTYPE *CreatePalette )( 
            IWICComponentFactory * This,
            /* [out] */ __RPC__deref_out_opt IWICPalette **ppIPalette);
        
        HRESULT ( STDMETHODCALLTYPE *CreateFormatConverter )( 
            IWICComponentFactory * This,
            /* [out] */ __RPC__deref_out_opt IWICFormatConverter **ppIFormatConverter);
        
        HRESULT ( STDMETHODCALLTYPE *CreateBitmapScaler )( 
            IWICComponentFactory * This,
            /* [out] */ __RPC__deref_out_opt IWICBitmapScaler **ppIBitmapScaler);
        
        HRESULT ( STDMETHODCALLTYPE *CreateBitmapClipper )( 
            IWICComponentFactory * This,
            /* [out] */ __RPC__deref_out_opt IWICBitmapClipper **ppIBitmapClipper);
        
        HRESULT ( STDMETHODCALLTYPE *CreateBitmapFlipRotator )( 
            IWICComponentFactory * This,
            /* [out] */ __RPC__deref_out_opt IWICBitmapFlipRotator **ppIBitmapFlipRotator);
        
        HRESULT ( STDMETHODCALLTYPE *CreateStream )( 
            IWICComponentFactory * This,
            /* [out] */ __RPC__deref_out_opt IWICStream **ppIWICStream);
        
        HRESULT ( STDMETHODCALLTYPE *CreateColorContext )( 
            IWICComponentFactory * This,
            /* [out] */ __RPC__deref_out_opt IWICColorContext **ppIWICColorContext);
        
        HRESULT ( STDMETHODCALLTYPE *CreateColorTransformer )( 
            IWICComponentFactory * This,
            /* [out] */ __RPC__deref_out_opt IWICColorTransform **ppIWICColorTransform);
        
        HRESULT ( STDMETHODCALLTYPE *CreateBitmap )( 
            IWICComponentFactory * This,
            /* [in] */ UINT uiWidth,
            /* [in] */ UINT uiHeight,
            /* [in] */ __RPC__in REFWICPixelFormatGUID pixelFormat,
            /* [in] */ WICBitmapCreateCacheOption option,
            /* [out] */ __RPC__deref_out_opt IWICBitmap **ppIBitmap);
        
        HRESULT ( STDMETHODCALLTYPE *CreateBitmapFromSource )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in_opt IWICBitmapSource *pIBitmapSource,
            /* [in] */ WICBitmapCreateCacheOption option,
            /* [out] */ __RPC__deref_out_opt IWICBitmap **ppIBitmap);
        
        HRESULT ( STDMETHODCALLTYPE *CreateBitmapFromSourceRect )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in_opt IWICBitmapSource *pIBitmapSource,
            /* [in] */ UINT x,
            /* [in] */ UINT y,
            /* [in] */ UINT width,
            /* [in] */ UINT height,
            /* [out] */ __RPC__deref_out_opt IWICBitmap **ppIBitmap);
        
        HRESULT ( STDMETHODCALLTYPE *CreateBitmapFromMemory )( 
            IWICComponentFactory * This,
            /* [in] */ UINT uiWidth,
            /* [in] */ UINT uiHeight,
            /* [in] */ __RPC__in REFWICPixelFormatGUID pixelFormat,
            /* [in] */ UINT cbStride,
            /* [in] */ UINT cbBufferSize,
            /* [size_is][in] */ __RPC__in_ecount_full(cbBufferSize) BYTE *pbBuffer,
            /* [out] */ __RPC__deref_out_opt IWICBitmap **ppIBitmap);
        
        HRESULT ( STDMETHODCALLTYPE *CreateBitmapFromHBITMAP )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in HBITMAP hBitmap,
            /* [in] */ __RPC__in HPALETTE hPalette,
            /* [in] */ WICBitmapAlphaChannelOption options,
            /* [out] */ __RPC__deref_out_opt IWICBitmap **ppIBitmap);
        
        HRESULT ( STDMETHODCALLTYPE *CreateBitmapFromHICON )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in HICON hIcon,
            /* [out] */ __RPC__deref_out_opt IWICBitmap **ppIBitmap);
        
        HRESULT ( STDMETHODCALLTYPE *CreateComponentEnumerator )( 
            IWICComponentFactory * This,
            /* [in] */ DWORD componentTypes,
            /* [in] */ DWORD options,
            /* [out] */ __RPC__deref_out_opt IEnumUnknown **ppIEnumUnknown);
        
        HRESULT ( STDMETHODCALLTYPE *CreateFastMetadataEncoderFromDecoder )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in_opt IWICBitmapDecoder *pIDecoder,
            /* [out] */ __RPC__deref_out_opt IWICFastMetadataEncoder **ppIFastEncoder);
        
        HRESULT ( STDMETHODCALLTYPE *CreateFastMetadataEncoderFromFrameDecode )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in_opt IWICBitmapFrameDecode *pIFrameDecoder,
            /* [out] */ __RPC__deref_out_opt IWICFastMetadataEncoder **ppIFastEncoder);
        
        HRESULT ( STDMETHODCALLTYPE *CreateQueryWriter )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in REFGUID guidMetadataFormat,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidVendor,
            /* [out] */ __RPC__deref_out_opt IWICMetadataQueryWriter **ppIQueryWriter);
        
        HRESULT ( STDMETHODCALLTYPE *CreateQueryWriterFromReader )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in_opt IWICMetadataQueryReader *pIQueryReader,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidVendor,
            /* [out] */ __RPC__deref_out_opt IWICMetadataQueryWriter **ppIQueryWriter);
        
        HRESULT ( STDMETHODCALLTYPE *CreateMetadataReader )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in REFGUID guidMetadataFormat,
            /* [size_is][unique][in] */ __RPC__in_ecount_full_opt(1) const GUID *pguidVendor,
            /* [in] */ DWORD dwOptions,
            /* [in] */ __RPC__in_opt IStream *pIStream,
            /* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIReader);
        
        HRESULT ( STDMETHODCALLTYPE *CreateMetadataReaderFromContainer )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in REFGUID guidContainerFormat,
            /* [size_is][unique][in] */ __RPC__in_ecount_full_opt(1) const GUID *pguidVendor,
            /* [in] */ DWORD dwOptions,
            /* [in] */ __RPC__in_opt IStream *pIStream,
            /* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIReader);
        
        HRESULT ( STDMETHODCALLTYPE *CreateMetadataWriter )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in REFGUID guidMetadataFormat,
            /* [size_is][unique][in] */ __RPC__in_ecount_full_opt(1) const GUID *pguidVendor,
            /* [in] */ DWORD dwMetadataOptions,
            /* [out] */ __RPC__deref_out_opt IWICMetadataWriter **ppIWriter);
        
        HRESULT ( STDMETHODCALLTYPE *CreateMetadataWriterFromReader )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in_opt IWICMetadataReader *pIReader,
            /* [unique][in] */ __RPC__in_opt const GUID *pguidVendor,
            /* [out] */ __RPC__deref_out_opt IWICMetadataWriter **ppIWriter);
        
        HRESULT ( STDMETHODCALLTYPE *CreateQueryReaderFromBlockReader )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in_opt IWICMetadataBlockReader *pIBlockReader,
            /* [out] */ __RPC__deref_out_opt IWICMetadataQueryReader **ppIQueryReader);
        
        HRESULT ( STDMETHODCALLTYPE *CreateQueryWriterFromBlockWriter )( 
            IWICComponentFactory * This,
            /* [in] */ __RPC__in_opt IWICMetadataBlockWriter *pIBlockWriter,
            /* [out] */ __RPC__deref_out_opt IWICMetadataQueryWriter **ppIQueryWriter);
        
        HRESULT ( STDMETHODCALLTYPE *CreateEncoderPropertyBag )( 
            IWICComponentFactory * This,
            /* [size_is][unique][in] */ __RPC__in_ecount_full_opt(cCount) PROPBAG2 *ppropOptions,
            /* [in] */ UINT cCount,
            /* [out] */ __RPC__deref_out_opt IPropertyBag2 **ppIPropertyBag);
        
        END_INTERFACE
    } IWICComponentFactoryVtbl;

    interface IWICComponentFactory
    {
        CONST_VTBL struct IWICComponentFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWICComponentFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWICComponentFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWICComponentFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWICComponentFactory_CreateDecoderFromFilename(This,wzFilename,pguidVendor,dwDesiredAccess,metadataOptions,ppIDecoder)	\
    ( (This)->lpVtbl -> CreateDecoderFromFilename(This,wzFilename,pguidVendor,dwDesiredAccess,metadataOptions,ppIDecoder) ) 

#define IWICComponentFactory_CreateDecoderFromStream(This,pIStream,pguidVendor,metadataOptions,ppIDecoder)	\
    ( (This)->lpVtbl -> CreateDecoderFromStream(This,pIStream,pguidVendor,metadataOptions,ppIDecoder) ) 

#define IWICComponentFactory_CreateDecoderFromFileHandle(This,hFile,pguidVendor,metadataOptions,ppIDecoder)	\
    ( (This)->lpVtbl -> CreateDecoderFromFileHandle(This,hFile,pguidVendor,metadataOptions,ppIDecoder) ) 

#define IWICComponentFactory_CreateComponentInfo(This,clsidComponent,ppIInfo)	\
    ( (This)->lpVtbl -> CreateComponentInfo(This,clsidComponent,ppIInfo) ) 

#define IWICComponentFactory_CreateDecoder(This,guidContainerFormat,pguidVendor,ppIDecoder)	\
    ( (This)->lpVtbl -> CreateDecoder(This,guidContainerFormat,pguidVendor,ppIDecoder) ) 

#define IWICComponentFactory_CreateEncoder(This,guidContainerFormat,pguidVendor,ppIEncoder)	\
    ( (This)->lpVtbl -> CreateEncoder(This,guidContainerFormat,pguidVendor,ppIEncoder) ) 

#define IWICComponentFactory_CreatePalette(This,ppIPalette)	\
    ( (This)->lpVtbl -> CreatePalette(This,ppIPalette) ) 

#define IWICComponentFactory_CreateFormatConverter(This,ppIFormatConverter)	\
    ( (This)->lpVtbl -> CreateFormatConverter(This,ppIFormatConverter) ) 

#define IWICComponentFactory_CreateBitmapScaler(This,ppIBitmapScaler)	\
    ( (This)->lpVtbl -> CreateBitmapScaler(This,ppIBitmapScaler) ) 

#define IWICComponentFactory_CreateBitmapClipper(This,ppIBitmapClipper)	\
    ( (This)->lpVtbl -> CreateBitmapClipper(This,ppIBitmapClipper) ) 

#define IWICComponentFactory_CreateBitmapFlipRotator(This,ppIBitmapFlipRotator)	\
    ( (This)->lpVtbl -> CreateBitmapFlipRotator(This,ppIBitmapFlipRotator) ) 

#define IWICComponentFactory_CreateStream(This,ppIWICStream)	\
    ( (This)->lpVtbl -> CreateStream(This,ppIWICStream) ) 

#define IWICComponentFactory_CreateColorContext(This,ppIWICColorContext)	\
    ( (This)->lpVtbl -> CreateColorContext(This,ppIWICColorContext) ) 

#define IWICComponentFactory_CreateColorTransformer(This,ppIWICColorTransform)	\
    ( (This)->lpVtbl -> CreateColorTransformer(This,ppIWICColorTransform) ) 

#define IWICComponentFactory_CreateBitmap(This,uiWidth,uiHeight,pixelFormat,option,ppIBitmap)	\
    ( (This)->lpVtbl -> CreateBitmap(This,uiWidth,uiHeight,pixelFormat,option,ppIBitmap) ) 

#define IWICComponentFactory_CreateBitmapFromSource(This,pIBitmapSource,option,ppIBitmap)	\
    ( (This)->lpVtbl -> CreateBitmapFromSource(This,pIBitmapSource,option,ppIBitmap) ) 

#define IWICComponentFactory_CreateBitmapFromSourceRect(This,pIBitmapSource,x,y,width,height,ppIBitmap)	\
    ( (This)->lpVtbl -> CreateBitmapFromSourceRect(This,pIBitmapSource,x,y,width,height,ppIBitmap) ) 

#define IWICComponentFactory_CreateBitmapFromMemory(This,uiWidth,uiHeight,pixelFormat,cbStride,cbBufferSize,pbBuffer,ppIBitmap)	\
    ( (This)->lpVtbl -> CreateBitmapFromMemory(This,uiWidth,uiHeight,pixelFormat,cbStride,cbBufferSize,pbBuffer,ppIBitmap) ) 

#define IWICComponentFactory_CreateBitmapFromHBITMAP(This,hBitmap,hPalette,options,ppIBitmap)	\
    ( (This)->lpVtbl -> CreateBitmapFromHBITMAP(This,hBitmap,hPalette,options,ppIBitmap) ) 

#define IWICComponentFactory_CreateBitmapFromHICON(This,hIcon,ppIBitmap)	\
    ( (This)->lpVtbl -> CreateBitmapFromHICON(This,hIcon,ppIBitmap) ) 

#define IWICComponentFactory_CreateComponentEnumerator(This,componentTypes,options,ppIEnumUnknown)	\
    ( (This)->lpVtbl -> CreateComponentEnumerator(This,componentTypes,options,ppIEnumUnknown) ) 

#define IWICComponentFactory_CreateFastMetadataEncoderFromDecoder(This,pIDecoder,ppIFastEncoder)	\
    ( (This)->lpVtbl -> CreateFastMetadataEncoderFromDecoder(This,pIDecoder,ppIFastEncoder) ) 

#define IWICComponentFactory_CreateFastMetadataEncoderFromFrameDecode(This,pIFrameDecoder,ppIFastEncoder)	\
    ( (This)->lpVtbl -> CreateFastMetadataEncoderFromFrameDecode(This,pIFrameDecoder,ppIFastEncoder) ) 

#define IWICComponentFactory_CreateQueryWriter(This,guidMetadataFormat,pguidVendor,ppIQueryWriter)	\
    ( (This)->lpVtbl -> CreateQueryWriter(This,guidMetadataFormat,pguidVendor,ppIQueryWriter) ) 

#define IWICComponentFactory_CreateQueryWriterFromReader(This,pIQueryReader,pguidVendor,ppIQueryWriter)	\
    ( (This)->lpVtbl -> CreateQueryWriterFromReader(This,pIQueryReader,pguidVendor,ppIQueryWriter) ) 


#define IWICComponentFactory_CreateMetadataReader(This,guidMetadataFormat,pguidVendor,dwOptions,pIStream,ppIReader)	\
    ( (This)->lpVtbl -> CreateMetadataReader(This,guidMetadataFormat,pguidVendor,dwOptions,pIStream,ppIReader) ) 

#define IWICComponentFactory_CreateMetadataReaderFromContainer(This,guidContainerFormat,pguidVendor,dwOptions,pIStream,ppIReader)	\
    ( (This)->lpVtbl -> CreateMetadataReaderFromContainer(This,guidContainerFormat,pguidVendor,dwOptions,pIStream,ppIReader) ) 

#define IWICComponentFactory_CreateMetadataWriter(This,guidMetadataFormat,pguidVendor,dwMetadataOptions,ppIWriter)	\
    ( (This)->lpVtbl -> CreateMetadataWriter(This,guidMetadataFormat,pguidVendor,dwMetadataOptions,ppIWriter) ) 

#define IWICComponentFactory_CreateMetadataWriterFromReader(This,pIReader,pguidVendor,ppIWriter)	\
    ( (This)->lpVtbl -> CreateMetadataWriterFromReader(This,pIReader,pguidVendor,ppIWriter) ) 

#define IWICComponentFactory_CreateQueryReaderFromBlockReader(This,pIBlockReader,ppIQueryReader)	\
    ( (This)->lpVtbl -> CreateQueryReaderFromBlockReader(This,pIBlockReader,ppIQueryReader) ) 

#define IWICComponentFactory_CreateQueryWriterFromBlockWriter(This,pIBlockWriter,ppIQueryWriter)	\
    ( (This)->lpVtbl -> CreateQueryWriterFromBlockWriter(This,pIBlockWriter,ppIQueryWriter) ) 

#define IWICComponentFactory_CreateEncoderPropertyBag(This,ppropOptions,cCount,ppIPropertyBag)	\
    ( (This)->lpVtbl -> CreateEncoderPropertyBag(This,ppropOptions,cCount,ppIPropertyBag) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWICComponentFactory_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_wincodecsdk_0000_0010 */
/* [local] */ 

HRESULT WICMatchMetadataContent(
    __in_ecount(1) REFGUID guidContainerFormat,
    __in_ecount_opt(1)  const GUID *pguidVendor,
    __in_ecount(1)  IStream *pIStream,
    __out_ecount(1) GUID *pguidMetadataFormat
    );
HRESULT WICSerializeMetadataContent(
    __in_ecount(1) REFGUID guidContainerFormat,
    __in_ecount(1) IWICMetadataWriter *pIWriter,
    __in DWORD dwPersistOptions,
    __in_ecount(1)  IStream *pIStream
    );
HRESULT WICGetMetadataContentSize(
    __in_ecount(1) REFGUID guidContainerFormat,
    __in_ecount(1) IWICMetadataWriter *pIWriter,
    __out_ecount(1) ULARGE_INTEGER *pcbSize
    );


extern RPC_IF_HANDLE __MIDL_itf_wincodecsdk_0000_0010_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wincodecsdk_0000_0010_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  CLIPFORMAT_UserSize(     unsigned long *, unsigned long            , CLIPFORMAT * ); 
unsigned char * __RPC_USER  CLIPFORMAT_UserMarshal(  unsigned long *, unsigned char *, CLIPFORMAT * ); 
unsigned char * __RPC_USER  CLIPFORMAT_UserUnmarshal(unsigned long *, unsigned char *, CLIPFORMAT * ); 
void                      __RPC_USER  CLIPFORMAT_UserFree(     unsigned long *, CLIPFORMAT * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



