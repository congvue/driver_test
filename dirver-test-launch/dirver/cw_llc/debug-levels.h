#ifndef __DEBUG_LEVELS_H__
#define __DEBUG_LEVELS_H__

#define D_MODULENAME LLC
#define D_MASTER 10

#include "linux/cohda/debug.h"

enum d_module
{
  D_SUBMODULE_DECLARE(LLC_Module),
  D_SUBMODULE_DECLARE(LLC_API),
  D_SUBMODULE_DECLARE(LLC_Device),
  D_SUBMODULE_DECLARE(LLC_NetDev),
  D_SUBMODULE_DECLARE(LLC_Netlink),
  D_SUBMODULE_DECLARE(LLC_Monitor),
  D_SUBMODULE_DECLARE(LLC_List),
  D_SUBMODULE_DECLARE(LLC_Msg),
  D_SUBMODULE_DECLARE(LLC_Thread),
  D_SUBMODULE_DECLARE(LLC_USB),
  D_SUBMODULE_DECLARE(LLC_Firmware),
  D_SUBMODULE_DECLARE(LLC_Debug),
};

#define D_API                   (D_NOTICE)
#define D_INTERN                (D_INFO)

#define D_TST                   (D_TEST)
#define D_DBG                   (D_VERBOSE)

#endif //__DEBUG_LEVELS_H__
