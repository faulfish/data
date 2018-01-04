#pragma once

#ifdef SPT_EXPORTS
#define SPT_API __declspec(dllexport)
#else
#define SPT_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    SPT_API void ShowDlgServiceTool();

#ifdef __cplusplus
}
#endif
