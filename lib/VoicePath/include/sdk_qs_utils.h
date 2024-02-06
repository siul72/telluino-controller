#ifndef SDK_QS_UTILS_H
#define SDK_QS_UTILS_H

#include "vp_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QS_DEBUG printf

extern char* MapStatus(VpStatusType status);

extern void UtilPrintEvent(VpEventType *pEvent);

#ifdef __cplusplus
}
#endif

#endif /* SDK_QS_UTILS_H */
