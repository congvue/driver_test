#ifndef __LINUX__COHDA__DEBUG_H_
#define __LINUX__COHDA__DEBUG_H_


#ifdef __KERNEL__
#include "debug-kernel.h" //<linux/cohda/debug-kernel.h>
#else
#include "linux/cohda/debug-app.h" //<linux/cohda/debug-app.h>
#endif

#endif //__LINUX__COHDA__DEBUG_H
