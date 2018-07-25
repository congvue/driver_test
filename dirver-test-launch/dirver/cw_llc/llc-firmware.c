/**
 * @addtogroup cohda_llc LLC module
 * @{
 *
 * @addtogroup cohda_llc_intern LLC internals
 * @{
 *
 * @file
 * LLC: SAF5100 firmware programing (GPIO + SPI or DFU) functions
 *
 * @todo The firmware binary is currnetly is stored 'in memory': Store it
 *       in a compressed format and uncompress when loading.
 */

//------------------------------------------------------------------------------
// Copyright (c) 2013 Cohda Wireless Pty Ltd
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Included headers
//------------------------------------------------------------------------------
#include "llc-internal.h"
#include "llc-firmware.h"

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>

#include <linux/sched.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/spi/spi.h>


#define D_SUBMODULE LLC_Firmware
#include "debug-levels.h"

//------------------------------------------------------------------------------
// Local Macros & Constants
//------------------------------------------------------------------------------
#if 0
#undef D_TST
#define D_TST D_WARN
#endif

#if (defined(BOARD_V2X) || defined(BOARD_MK4) || defined(BOARD_I686))
  #define LLC_BOOTLOADER_BINARY_PTR DFUBootLoader_bin
  #define LLC_BOOTLOADER_BINARY_LEN DFUBootLoader_bin_len
  #define LLC_FIRMWARE_BINARY_PTR SDRMK4Dual_bin
  #define LLC_FIRMWARE_BINARY_LEN SDRMK4Dual_bin_len
#else
  #define LLC_FIRMWARE_BINARY_PTR NULL
  #define LLC_FIRMWARE_BINARY_LEN 0
  #define LLC_BOOTLOADER_BINARY_PTR NULL 
  #define LLC_BOOTLOADER_BINARY_LEN 0
#endif

#if (defined(BOARD_MK4))
#define M_RST_N_SMARC	IMX_GPIO_NR(5, 15)
#endif

//------------------------------------------------------------------------------
// Local Type definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Local (static) Function Prototypes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Local Variables
//------------------------------------------------------------------------------
#if (defined(BOARD_V2X) || defined(BOARD_MK4) || defined(BOARD_I686))
  // Firmware binary
  #include "llc-firmware.inc"
  #include "llc-bootloader.inc"
#endif

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

/*
 * @brief Request device tree gpio and reset
 */
static int SAF5100_Reset(struct device_node *np)
{
  unsigned int GPIO_Reset;
  int Ret;

  Ret = of_device_is_available(np);
  if (!Ret)
  {
    d_error(D_ERR, NULL, "device tree node not available\n");
    goto Error;
  }

  GPIO_Reset = of_get_named_gpio(np, "modem-reset-gpios", 0);
  if (!gpio_is_valid(GPIO_Reset))
  {
    d_error(D_ERR, NULL, "modem-reset-gpios doesn't specify a valid GPIO\n");
    goto Error;
  }

  Ret = gpio_request(GPIO_Reset, "modem-reset");
  if (Ret)
  {
    d_error(D_ERR, NULL, "Unable to request modem-reset GPIO %u\n", GPIO_Reset);
    goto Error;
  }

  gpio_direction_output(GPIO_Reset, 1);
  schedule_timeout_interruptible(1);
  gpio_direction_output(GPIO_Reset, 0);
#if defined(BOARD_CONDOR)
  schedule_timeout_interruptible(1);
  gpio_direction_input(GPIO_Reset);
  schedule_timeout_interruptible(1);
#endif
  gpio_free(GPIO_Reset);
  schedule_timeout_interruptible(1);
Error:
  return Ret;
}

/*
 * @brief Reset the chip and program
 */
static int SAF5100_Probe(struct spi_device *spi)
{
  int Ret;
  Ret = SAF5100_Reset(spi->dev.of_node);
  if (Ret)
  {
    d_printf(D_ERR, NULL, "SAF5100 reset failed.\n");
    goto Error;
  }
/*
  # SDR Image load performed using DFU
  Ret = spi_write(spi, LLC_FIRMWARE_BINARY_PTR, LLC_FIRMWARE_BINARY_LEN);
  if (Ret) d_printf(D_ERR, NULL, "SPI programming failed.\n");
*/
Error:
  return Ret;
}

static int SAF5100_Remove(struct spi_device *spi) { return 0; }

static const struct of_device_id SAF5100_DTIDs[] = {
  { .compatible = "saf5100" },
  {},
};

MODULE_DEVICE_TABLE(of, SAF5100_DTIDs);

static struct spi_driver SAF5100_SPIDriver = {
  .driver = {
    .name =		"saf5100",
    .owner =	THIS_MODULE,
    .of_match_table = of_match_ptr(SAF5100_DTIDs),
  },
  .probe =	SAF5100_Probe,
  .remove =	SAF5100_Remove,
};

/**
 * @brief Setup all resources used by the firmware programming interface
 * @param pDev The LLC device structure
 * @return Zero for success or a negative errno on failure
 *
 */
int LLC_FirmwareSetup (struct LLCDev *pDev)
{
    printk("\n############### LIne:%d  Fun:%s\n",__LINE__, __func__);

  return spi_register_driver(&SAF5100_SPIDriver);
}

/**
 * @brief Release any resources used by the firmware programming interface
 * @param pDev The LLC device structure
 * @return Zero for success or a negative errno on failure
 *
 */
int LLC_FirmwareRelease (struct LLCDev *pDev)
{
  spi_unregister_driver(&SAF5100_SPIDriver);
  return 0;
}
// Close the Doxygen group
/**
 * @}
 * @}
 */
