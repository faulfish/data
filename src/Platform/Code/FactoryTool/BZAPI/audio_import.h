#ifndef __WRAPPER_IMPORT_H
#define __WRAPPER_IMPORT_H

#define DllImport   __declspec( dllimport )

typedef DllImport int (__stdcall *IMPORT_LABVIEWDLL2_PTR)(double*,double*);

IMPORT_LABVIEWDLL2_PTR pIMPORT_LABVIEWDLL2= NULL;

#endif
