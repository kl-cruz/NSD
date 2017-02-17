/*
 * MIT License
 *
 * Copyright (c) 2017 Karol Lasonczyk (kl-cruz - https://github.com/kl-cruz)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef NSD_SPIM_H
#define NSD_SPIM_H

#include "nsd_device.h"

#if (NSD_SPIM_ENABLED == TRUE) || defined(__DOXYGEN__)

#if !defined(NSD_SPIM_USE_SPIM0) || defined(__DOXYGEN__)
#define NSD_SPIM_USE_SPIM0 FALSE
#endif

#if !defined(NSD_SPIM_USE_SPIM1) || defined(__DOXYGEN__)
#define NSD_SPIM_USE_SPIM1 FALSE
#endif

#if !defined(NSD_SPIM_USE_SPIM2) || defined(__DOXYGEN__)
#define NSD_SPIM_USE_SPIM2 FALSE
#endif

#if !defined(NSD_SPIM_USE_SPIM3) || defined(__DOXYGEN__)
#define NSD_SPIM_USE_SPIM3 FALSE
#endif

typedef struct nsd_spim_drv nsd_spim_drv_t;


/**
 * @brief   SPI notification callback type.
 *
 * @param[in] p_spim_drv      pointer to the @p NSD_SPI_Driver_t object triggering the
 *                            callback
 */
typedef void (*nsd_spim_irq_callback_t)(nsd_spim_drv_t *p_spim_drv);


typedef struct {
    nsd_spim_irq_callback_t  end_cb; ///< Operation complete callback or @p NULL.
    uint8_t                  sck_pin;
    uint8_t                  cs_pin;
    uint8_t                  miso_pin;
    uint8_t                  mosi_pin;
    nrf_spim_frequency_t     frequency;
    nrf_spim_mode_t          mode;
    nrf_spim_bit_order_t     bit_order;
    uint8_t                  orc;

    /* End of the mandatory fields.*/
} nsd_spim_config_s;

/**
 * @brief   SPIM Driver state machine possible states.
 */
typedef enum {
    NSD_SPIM_DRV_STATE_UNINIT, ///< Driver is uninitialized.
    NSD_SPIM_DRV_STATE_READY,  ///< Driver is ready to start an operation.
    NSD_SPIM_DRV_STATE_BUSY,   ///< Driver is busy, executing operation.
    NSD_SPIM_DRV_STATE_ENDTRX, ///< State signalizing that last transmission was finished.
} nsd_spim_state_t;

/**
 * @brief   Structure representing a SPIM driver.
 */
struct nsd_spim_drv {
    const nsd_spim_config_s   *config;       ///< Current configuration data.
    volatile nsd_spim_state_t  spim_state;   ///< SPIM driver current state.
    NRF_SPIM_Type             *spi;          ///< Pointer to the SPIx registers block.
    IRQn_Type                  irq;          ///< SPI/SPIM peripheral instance IRQ number.
    uint8_t                    irq_priority; ///< Interrupt priority.
};

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (NSD_SPIM_USE_SPIM0 == TRUE) && !defined(__DOXYGEN__)
extern nsd_spim_drv_t NSD_SPIM0;
#endif

#if (NSD_SPIM_USE_SPIM1 == TRUE) && !defined(__DOXYGEN__)
extern nsd_spim_drv_t NSD_SPIM1;
#endif

#if (NSD_SPIM_USE_SPIM2 == TRUE) && !defined(__DOXYGEN__)
extern nsd_spim_drv_t NSD_SPIM2;
#endif

#if (NSD_SPIM_USE_SPIM3 == TRUE) && !defined(__DOXYGEN__)
extern nsd_spim_drv_t NSD_SPIM3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  /**
   * @brief Initializes structures of active drivers
   */
  void nsd_spim_prepare();

  /**
   * @brief Initializes selected peripheral.
   *
   * @param[in] p_spim_drv        Pointer to structure representing SPIM driver.
   */
  void nsd_spim_init(nsd_spim_drv_t *p_spim_drv);

  /**
   * @brief Selects chip select pin.
   *
   * @param[in] p_spim_drv        Pointer to structure representing SPIM driver.
   */
  void nsd_spim_select(nsd_spim_drv_t *p_spim_drv);

  /**
   * @brief Deselects chip select pin.
   *
   * @param[in] p_spim_drv        Pointer to structure representing SPIM driver.
   */
  void nsd_spim_unselect(nsd_spim_drv_t *p_spim_drv);

  /**
   * @brief   Exchanges data using SPIM peripheral.
   *
   * @details When using nRF52 Family remember about 255 bytes hardware limitation!
   *
   * @param[in]  p_spim_drv       Pointer to structure representing SPIM driver.
   * @param[out] n_tx             Output data length.
   * @param[out] p_txbuf          Output data buffer.
   * @param[in]  n_rx             Input data length.
   * @param[in]  p_rxbuf          Input data buffer.
   */
  void nsd_spim_exchange(nsd_spim_drv_t *p_spim_drv,
                         size_t n_tx,
                         const void *p_txbuf,
                         size_t n_rx,
                         void *p_rxbuf);

  /**
   * @brief Sends data using SPIM peripheral.
   *
   * @param[in]  p_spim_drv       Pointer to structure representing SPIM driver.
   * @param[out] n                Output data length.
   * @param[out] p_txbuf          Output data buffer.
   */
  void nsd_spim_send(nsd_spim_drv_t *p_spim_drv, size_t n, const void *p_txbuf);

  /**
   * @brief Receives data using SPIM peripheral.
   *
   * @param[in] p_spim_drv        Pointer to structure representing SPIM driver.
   * @param[in] n                 Input data length.
   * @param[in] p_rxbuf           Input data buffer.
   */
  void nsd_spim_receive(nsd_spim_drv_t *p_spim_drv, size_t n, void *p_rxbuf);

  /**
   * @brief Deinitializes SPIM peripheral.
   *
   * @param[in] p_spim_drv        Pointer to structure representing SPIM driver.
   */
  void nsd_spim_deinit(nsd_spim_drv_t *p_spim_drv);

#ifdef NSD_SPIM_DISABLE_IRQ_CONNECT

  /**
   * @brief SPIM interrupt service routine.
   *
   * @details This interrupt routine should be connect to interrupt system used in specific
   *          environment.To use direct connection between IRQ and this function, undefine
   *          NSD_SPIM_DISABLE_IRQ_CONNECT define.
   *
   * @param[in] p_ctx             Pointer to context internally casted to structure representing SPIM driver.
   */
  void nsd_spim_irq_routine(void *p_ctx);

#endif

#ifdef __cplusplus
}
#endif


#endif /* NSD_SPIM_ENABLED */

#endif /* NSD_SPI_H */
