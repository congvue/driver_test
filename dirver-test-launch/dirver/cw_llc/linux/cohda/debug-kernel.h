/**
 * @addtogroup cohda_debug_kernel Cohda's kernel debug library
 * @{
 *
 * Don't \#include this file directly, read on!
 *
 * @section cohda_debug_kernel_overview EXECUTING DEBUGGING ACTIONS OR NOT
 *
 * The main thing this framework provides is decision power to take a
 * debug action (like printing a message) if the current debug level
 * allows it.
 *
 * The decision power is at two levels: at compile-time (what does
 * not make it is compiled out) and at run-time. The run-time
 * selection is done per-submodule (as they are declared by the user
 * of the framework).
 *
 * A call to d_test(L) (L being the target debug level) returns true
 * if the action should be taken because the current debug levels
 * allow it (both compile and run time).
 *
 * It follows that a call to d_test() that can be determined to be
 * always false at compile time will get the code depending on it
 * compiled out by optimization.
 *
 * @subsection debuglevel DEBUG LEVELS
 *
 * It is up to the caller to define how much a debugging level is.
 *
 * Convention sets 0 as "no debug" (so an action marked as debug level 0
 * will always be taken). The increasing debug levels are used for
 * increased verbosity.
 *
 *
 * @subsection usage USAGE
 *
 * Group the code in modules and submodules inside each module [which
 * in most cases maps to Linux modules and .c files that compose
 * those].
 *
 * For each module, there is:
 *
 *  - a MODULENAME (single word, legal C identifier)
 *
 *  - a debug-levels.h header file that declares the list of
 *    submodules and that is included by all .c files that use
 *    the debugging tools. The file name can be anything.
 *
 *  - some (optional) .c code to manipulate the runtime debug levels
 *    through debugfs.
 *
 * The debug-levels.h file would look like:
 * @code
 *     #ifndef __DEBUG_LEVELS_H__
 *     #define __DEBUG_LEVELS_H__
 *
 *     #define D_MODULENAME modulename
 *     #define D_MASTER 10
 *
 *     #include <linux/cohda/debug.h>
 *
 *     enum d_module {
 *             D_SUBMODULE_DECLARE(submodule_1),
 *             D_SUBMODULE_DECLARE(submodule_2),
 *             ...
 *             D_SUBMODULE_DECLARE(submodule_N)
 *     };
 *
 *     #endif
 * @endcode
 *
 * D_MASTER is the maximum compile-time debug level; any debug actions
 * above this will be out. D_MODULENAME is the module name (legal C
 * identifier), which has to be unique for each module (to avoid
 * namespace collisions during linkage). Note those \#defines need to
 * be done before \#including debug.h
 *
 * We declare N different submodules whose debug level can be
 * independently controlled during runtime.
 *
 * In a .c file of the module (and only in one of them), define the
 * following code:
 * @code
 *     struct d_level D_LEVEL[] = {
 *             D_SUBMODULE_DEFINE(submodule_1),
 *             D_SUBMODULE_DEFINE(submodule_2),
 *             ...
 *             D_SUBMODULE_DEFINE(submodule_N),
 *     };
 *     size_t D_LEVEL_SIZE = ARRAY_SIZE(D_LEVEL);
 * @endcode
 *
 * Externs for d_level_MODULENAME and d_level_size_MODULENAME are used
 * and declared in this file using the D_LEVEL and D_LEVEL_SIZE macros
 * \#defined also in this file.
 *
 * To provide runtime control of the debug level, invoke the d_init() and
 * d_exit() functions in the same .c file when the module is initialized and
 * de-initialized respectively:
 * @code
 * static void __init XYZ_ModuleInit(void)
 * {
 *    d_init();
 *    d_fnstart(...
 * }
 * module_exit(XYZ_ModuleInit);
 * @endcode
 * @code
 * static void __exit XYZ_ModuleExit(void)
 * {
 *    ...
 *    d_fnend(...
 *    d_exit();
 *    return;
 * }
 * module_exit(XYZ_ModuleExit);
 * @endcode
 *
 * On each submodule (for example, .c files), the debug infrastructure
 * should be included like this:
 * @code
 *     #define D_SUBMODULE submodule_x     // matches one in debug-levels.h
 *     #include "debug-levels.h"
 * @endcode
 * after \#including all your include files.
 *
 * Now you can use the d_*() macros below [d_test(), d_fnstart(),
 * d_fnend(), d_printf(), d_dump()].
 *
 * If their debug level is greater than D_MASTER, they will be
 * compiled out.
 *
 * If their debug level is lower or equal than D_MASTER but greater
 * than the current debug level of their submodule, they'll be
 * ignored.
 *
 * Otherwise, the action will be performed.
 *
 * @subsection control RUNTIME CONTROL
 * To view and manipulate the levels from user space, see the examples below:
 * @code
 * # Inspect the current debug levels
 * sudo cat /proc/D_modulename
 * @endcode
 * @code
 * # Disable all debug output for the entire module
 * echo "modulename 0" | sudo tee -a /proc/D_modulename
 * @endcode
 * @code
 * # Set all submodules to the same debug level
 * echo "modulename 5" | sudo tee -a /proc/D_modulename
 * @endcode
 * @code
 * # Disable all debug output for the specified submodule
 * echo "submodule_N 0" | sudo tee -a /proc/D_modulename
 * @endcode
 *
 * @note The output is subject to printk() ratelimiting. So if you're enabling
 * a lot of debug, and want to be sure that is all output to the console/log:
 * @code
 * echo "0" | sudo tee -a /proc/sys/kernel/printk_ratelimit
 * echo "0" | sudo tee -a /proc/sys/kernel/printk_ratelimit_burst
 * echo "0" | sudo tee -a /proc/sys/kernel/printk_delay
 * @endcode
  *
 *
 * @file
 * Collection of kernel space tools to manage debug operations.
 *
 */
#ifndef __INCLUDE__LINUX__COHDA__DEBUG_KERNEL_H__
#define __INCLUDE__LINUX__COHDA__DEBUG_KERNEL_H__

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

/// Some handy levels to consider using with the d_*() functions
typedef enum
{
  D_EMERG = 0,
  D_ALERT = 1,
  D_CRIT = 2,
  D_ERR = 3,
  D_WARN = 4,
  D_NOTICE = 5,
  D_INFO = 6,
  D_DEBUG = 7,
  D_TEST = 8,
  D_VERBOSE = 9,
  D_IRQ = 254,
  D_ALL = 255,
} d_levels;

// Backend stuff


/**
 * @brief Generate a message header from a 'struct device'
 * @param head: buffer where to place the header
 * @param head_size: length of @param head
 * @param dev: pointer to device used to generate a header from. If NULL,
 *     an empty ("") header is generated.
 */
static inline void __d_head(char *head, size_t head_size, void *dev)
{
  if (dev == NULL)
  {
    head[0] = 0;
  }
  else if ((unsigned long) dev < 4096)
  {
    printk(KERN_ERR "E: Corrupt dev %p\n", dev);
    WARN_ON(1);
  }
  else
  {
    snprintf(head, head_size, "[%p]: ", dev);
  }
}

/**
 * @brief Log some message if debugging is enabled
 * @param l: intended debug level
 * @param tag: tag to prefix the message with
 * @param dev: 'struct device' associated to this message
 * @param f: printf-like format
 * @param a: printf-like arguments
 *
 * Note this is optimized out if it doesn't pass the compile-time
 * check; however, it is *always* compiled. This is useful to make
 * sure the printf-like formats and variables are always checked and
 * they don't get bit rot if you have all the debugging disabled.
 */
#define _d_printf(l, tag, dev, f, a...) \
  do { \
    char head[64]; \
    if (!d_test(l)) break; \
    if (!printk_ratelimit()) break; \
    __d_head(head, sizeof(head), dev); \
    printk(KERN_ERR "%s%s%s: " f, head, __func__, tag, ##a); \
  } while (0)

// CPP syntatic sugar to generate A_B like symbol names when one of
// the arguments is a a preprocessor #define.
#define __D_PASTE__(varname, modulename) varname##_##modulename
#define __D_PASTE(varname, modulename) (__D_PASTE__(varname, modulename))
#define _D_SUBMODULE_INDEX(_name) (D_SUBMODULE_DECLARE(_name))

/**
 * @brief Store a submodule's runtime debug level and name
 */
struct d_level
{
  u8 level;
  const char *name;
};

#define D_LEVEL __D_PASTE(d_level, D_MODULENAME)
#define D_LEVEL_SIZE __D_PASTE(d_level_size, D_MODULENAME)

/**
 * List of available submodules and their debug levels
 *
 * We call them d_level_MODULENAME and d_level_size_MODULENAME; the
 * macros D_LEVEL and D_LEVEL_SIZE contain the name already for
 * convenience.
 *
 * This array and the size are defined on some .c file that is part of
 * the current module.
 */
extern struct d_level D_LEVEL[];
extern size_t D_LEVEL_SIZE;

//#define d_stringify(name) ##name
//#define d_modulename d_stringify(D_MODULENAME)

#define xstr(s) str(s)
#define str(s) #s
#define D_TMP D_MODULENAME
#define d_modulename xstr(D_TMP)

static ssize_t __d_proc_write(struct file *pFile,
                              const char *pBuffer,
                              size_t Length,
                              loff_t *pOff)
{
  int i;
  #define D_PROC_CMD_CNT (500)
  char pCmd[D_PROC_CMD_CNT];

  if (Length > D_PROC_CMD_CNT)
    Length = D_PROC_CMD_CNT;

  if (copy_from_user(pCmd, pBuffer, Length))
    goto Error;

  for (i = 0; i < D_LEVEL_SIZE; i++)
  {
    if (!strncmp(pCmd, D_LEVEL[i].name, strlen(D_LEVEL[i].name)))
    {
      unsigned long level;
      level = simple_strtoul(pCmd + strlen(D_LEVEL[i].name) + 1, NULL, 0);
      D_LEVEL[i].level = level & 0xFF;
      goto Success;
    }
  }

  if (!strncmp(pCmd, d_modulename, strlen(d_modulename)))
  {
    unsigned long level;
    level = simple_strtoul(pCmd + strlen(d_modulename) + 1, NULL, 0);
    for (i = 0; i < D_LEVEL_SIZE; i++)
    {
      D_LEVEL[i].level = level & 0xFF;
    }
    goto Success;
  }
Error:
Success:
  return Length;
}

/*
 * @brief Reads debugging items.
 */
static int __d_proc_read(struct seq_file *pSeq, void *pVoid)
{
  int i;
  seq_printf(pSeq, "%s:\n", d_modulename);
  for (i = 0; i < D_LEVEL_SIZE; i++)
    seq_printf(pSeq, "%s %u\n", D_LEVEL[i].name, D_LEVEL[i].level);
  return 0;
}

/*
 * @brief Handles debugging proc open.
 */
static int __d_proc_open(struct inode *pInode, struct  file *pFile)
{
  return single_open(pFile, __d_proc_read, NULL);
}

static const struct file_operations __d_proc_fop =
{
  .open = __d_proc_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .write = __d_proc_write,
  .release = single_release,
};

/**
 * @brief
 */
static inline void __d_proc_init(void)
{
  static struct proc_dir_entry *d_proc_entry = NULL;

  d_proc_entry = proc_create("D_" d_modulename, 0666, NULL, &__d_proc_fop);
}

/**
 * @brief
 */
static inline void __d_proc_exit(void)
{
  remove_proc_entry("D_" d_modulename, NULL);
}

// Frontend stuff

// Stuff you need to declare prior to using the actual "debug" actions
// (defined below).

#ifndef D_MODULENAME
#error D_MODULENAME is not defined in your debug-levels.h file
/**
 * @brief Name of the current module
 *
 * \#define in your module's debug-levels.h, making sure it is
 * unique. This has to be a legal C identifier.
 */
#define D_MODULENAME undefined_modulename
#endif

#ifndef D_MASTER
#warning D_MASTER not defined, but debug.h included! [see docs]
/**
 * @brief Compile time maximum debug level
 *
 * \#define in your debug-levels.h file to the maximum debug level the
 * runtime code will be allowed to have. This allows you to provide a
 * main knob.
 *
 * Anything above that level will be optimized out of the compile.
 *
 * Defaults to zero (no debug code compiled in).
 *
 * Maximum one definition per module (at the debug-levels.h file).
 */
#define D_MASTER 0
#endif

#ifndef D_DEFAULT
/**
 * @brief Compile time default debug level
 *
 * \#define in your debug-levels.h file to the default debug level the
 * runtime code will start with.
 *
 * Defaults to D_WARN
 *
 * Maximum one definition per module (at the debug-levels.h file).
 */
#define D_DEFAULT D_WARN
#endif

#ifndef D_SUBMODULE
#error D_SUBMODULE not defined, but debug.h included! [see docs]
/**
 * @brief Name of the current submodule
 *
 * \#define in your submodule .c file before \#including debug-levels.h
 * to the name of the current submodule as previously declared and
 * defined with D_SUBMODULE_DECLARE() (in your module's
 * debug-levels.h) and D_SUBMODULE_DEFINE().
 *
 * This is used to provide runtime-control over the debug levels.
 *
 * Maximum one per .c file! Can be shared among different .c files
 * (meaning they belong to the same submodule categorization).
 */
#define D_SUBMODULE undefined_module
#endif

/**
 * @brief Declare a submodule for runtime debug level control
 * @param _name Name of the submodule, restricted to the chars that make up a
 *              valid C identifier ([a-zA-Z0-9_]).
 *
 * Declare in the module's debug-levels.h header file as:
 * @code
 * enum d_module {
 *         D_SUBMODULE_DECLARE(submodule_1),
 *         D_SUBMODULE_DECLARE(submodule_2),
 *         D_SUBMODULE_DECLARE(submodule_3),
 * };
 * @endcode
 * Some corresponding .c file needs to have a matching D_SUBMODULE_DEFINE().
 */
#define D_SUBMODULE_DECLARE(_name) __D_SUBMODULE_##_name

/**
 * @brief Define a submodule for runtime debug level control
 * @param _name Name of the submodule, restricted to the chars that make up a
 *              valid C identifier ([a-zA-Z0-9_]).
 *
 * Use once per module (in some .c file) as:
 * @code
 * static
 * struct d_level d_level_SUBMODULENAME[] = {
 *         D_SUBMODULE_DEFINE(submodule_1),
 *         D_SUBMODULE_DEFINE(submodule_2),
 *         D_SUBMODULE_DEFINE(submodule_3),
 * };
 * size_t d_level_size_SUBDMODULENAME = ARRAY_SIZE(d_level_SUBDMODULENAME);
 * @endcode
 * Matching D_SUBMODULE_DECLARE()s have to be present in a debug-levels.h header file.
 */
#define D_SUBMODULE_DEFINE(_name) \
  [__D_SUBMODULE_##_name] = { \
                              .level = D_DEFAULT, \
                              .name = #_name \
                            }

// The actual "debug" operations

/**
 * @brief Initialize the debug support functionality
 */
#define d_init(arg) \
  ({ \
    __d_proc_init(); \
  })

/**
 * @brief De-initialize the debug support functionality
 */
#define d_exit(arg) \
  ({ \
    __d_proc_exit(); \
  })

/**
 * @brief Returns true if debugging should be enabled
 * @param l: intended debug level (unsigned)
 *
 * If the master debug switch is enabled and the current settings are
 * higher or equal to the requested level, then debugging
 * output/actions should be enabled.
 *
 * @note This needs to be coded so that it can be evaluated in compile
 * time; this is why the ugly BUG_ON() is placed in there, so the
 * D_MASTER evaluation compiles all out if it is compile-time false.
 */
#define d_test(l) \
  ({ \
    unsigned __l = l; \
    (D_MASTER) >= __l \
    && ({ \
      BUG_ON(_D_SUBMODULE_INDEX(D_SUBMODULE) >= D_LEVEL_SIZE); \
      D_LEVEL[_D_SUBMODULE_INDEX(D_SUBMODULE)].level >= __l; \
    }); \
  })

/**
 * @brief Log message at function start if debugging enabled
 * @param l: intended debug level
 * @param _dev: 'struct device' pointer, NULL if none (for context)
 * @param f: printf-like format
 * @param a: printf-like arguments
 */
#define d_fnstart(l, _dev, f, a...) _d_printf(l, " FNSTART", _dev, f, ## a)

/**
 * @brief Log message at function end if debugging enabled
 * @param l: intended debug level
 * @param _dev: 'struct device' pointer, NULL if none (for context)
 * @param f: printf-like format
 * @param a: printf-like arguments
 */
#define d_fnend(l, _dev, f, a...) _d_printf(l, " FNEND", _dev, f, ## a)

/**
 * @brief Log message if debugging enabled
 * @param l: intended debug level
 * @param _dev: 'struct device' pointer, NULL if none (for context)
 * @param f: printf-like format
 * @param a: printf-like arguments
 */
#define d_printf(l, _dev, f, a...) _d_printf(l, "", _dev, f, ## a)

/**
 * @brief Log error message if debugging enabled
 * @param l: intended debug level
 * @param _dev: 'struct device' pointer, NULL if none (for context)
 * @param f: printf-like format
 * @param a: printf-like arguments
 */
#define d_error(l, _dev, f, a...) _d_printf(l, " ERROR", _dev, f, ## a)

/**
 * @brief Log buffer hex dump if debugging enabled
 * @param l: intended debug level
 * @param dev: 'device' pointer, NULL if none (for context)
 * @param ptr: Pointer to memory location to dump
 * @param size: Number of bytes of memory at ptr to dump
 */
#define d_dump(l, dev, ptr, size)      \
  do { \
    char head[64]; \
    if (!d_test(l)) break; \
    if (!printk_ratelimit()) break; \
    __d_head(head, sizeof(head), dev); \
    print_hex_dump(KERN_ERR, head, 0, 16, 1, ((void *)ptr), (size), 0); \
  } while (0)

/**
 * @brief Assertion check
 * @param condition: assertion to check
 */
#define d_assert(condition) BUG_ON(!(condition))

#endif // #ifndef __INCLUDE__LINUX__COHDA__DEBUG_KERNEL_H__
// Close the doxygen group
/**
 * @}
 */
