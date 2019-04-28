#include "extcode.h"
#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * CmdSet
 */
void __cdecl CmdSet(char COM[], int32_t ReleyNum, LVBoolean on, 
	LVBoolean *relay, LVBoolean *serial);

MgErr __cdecl LVDLLStatus(char *errStr, int errStrLen, void *module);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma pack(pop)

