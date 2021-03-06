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

#ifndef NODI_SPIM_H
#define NODI_SPIM_H

#include "nodi_common.h"

#if (NODI_SPIM_ENABLED == 1) || defined(__DOXYGEN__)
#include "nodi_spim_const.h"

#if !defined(NODI_SPIM_USE_SPIM0) || defined(__DOXYGEN__)
#define NODI_SPIM_USE_SPIM0 0
#endif

#if !defined(NODI_SPIM_USE_SPIM1) || defined(__DOXYGEN__)
#define NODI_SPIM_USE_SPIM1 0
#endif

#if !defined(NODI_SPIM_USE_SPIM2) || defined(__DOXYGEN__)
#define NODI_SPIM_USE_SPIM2 0
#endif

#if !defined(NODI_SPIM_USE_SPIM3) || defined(__DOXYGEN__)
#define NODI_SPIM_USE_SPIM3 0
#endif

typedef struct nodi_spim_drv nodi_spim_drv_t;

/**
 * @brief   SPIM notification callback type.
 *
 * @param[in] p_spim_drv      pointer to the nodi_spim_drv_t object triggering the callback
 */
typedef void (*nodi_spim_irq_callback_t)(nodi_spim_drv_t *p_spim_drv);

typedef struct {
    nodi_spim_irq_callback_t  end_cb;    ///< Operation complete callback NULL.
    nodi_gpio_pin_t           sck_pin;   ///< SCK pin config structure
    nodi_gpio_pin_t           cs_pin;    ///< CS pin config structure
    nodi_gpio_pin_t           miso_pin;  ///< MISO pin config structure
    nodi_gpio_pin_t           mosi_pin;  ///< MOSI pin config structure
    uint32_t                  frequency; ///< SPIM frequency
    uint32_t                  mode;      ///< SPI mode (0,1,2,3)
    uint32_t                  bit_order; ///< Bit order (MSB, LSB)
    uint8_t                   orc;       ///< Overrun character sending
} nodi_spim_config_s;

/**
 * @brief   SPIM Driver state machine possible states.
 */
typedef enum {
    NODI_SPIM_DRV_STATE_UNINIT, ///< Driver is uninitialized.
    NODI_SPIM_DRV_STATE_READY,  ///< Driver is ready to start an operation.
    NODI_SPIM_DRV_STATE_BUSY,   ///< Driver is busy, executing operation.
    NODI_SPIM_DRV_STATE_FINISH, ///< Driver finished operation.
} nodi_spim_state_t;

/**
 * @brief   Structure representing a SPIM driver.
 */
struct nodi_spim_drv {
    const nodi_spim_config_s  *config;       ///< Current configuration data.
    volatile nodi_spim_state_t spim_state;   ///< SPIM driver current state.
    NRF_SPIM_Type             *p_spim_reg;   ///< Pointer to the SPIM registers block.
    IRQn_Type                  irq;          ///< SPIM peripheral instance IRQ number.
    uint8_t                    irq_priority; ///< Interrupt priority.
};

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (NODI_SPIM_USE_SPIM0 == 1) && !defined(__DOXYGEN__)
extern nodi_spim_drv_t NODI_SPIM0;
#endif

#if (NODI_SPIM_USE_SPIM1 == 1) && !defined(__DOXYGEN__)
extern nodi_spim_drv_t NODI_SPIM1;
#endif

#if (NODI_SPIM_USE_SPIM2 == 1) && !defined(__DOXYGEN__)
extern nodi_spim_drv_t NODI_SPIM2;
#endif

#if (NODI_SPIM_USE_SPIM3 == 1) && !defined(__DOXYGEN__)
extern nodi_spim_drv_t NODI_SPIM3;
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Initializes structures of active drivers
 */
void nodi_spim_prepare();

/**
 * @brief Initializes selected peripheral.
 *
 * @param[in] p_spim_drv        Pointer to structure representing SPIM driver.
 */
void nodi_spim_init(nodi_spim_drv_t *p_spim_drv);

/**
 * @brief Selects chip select pin.
 *
 * @param[in] p_spim_drv        Pointer to structure representing SPIM driver.
 */
void nodi_spim_select(nodi_spim_drv_t *p_spim_drv);

/**
 * @brief Deselects chip select pin.
 *
 * @param[in] p_spim_drv        Pointer to structure representing SPIM driver.
 */
void nodi_spim_unselect(nodi_spim_drv_t *p_spim_drv);

/**
 * @brief   Exchanges data using SPIM peripheral.
 *
 * @param[in]  p_spim_drv       Pointer to structure representing SPIM driver.
 * @param[in]  n_tx             Output data length.
 * @param[out] p_txbuf          Output data buffer.
 * @param[in]  n_rx             Input data length.
 * @param[in]  p_rxbuf          Input data buffer.
 */
void nodi_spim_exchange(nodi_spim_drv_t *p_spim_drv,
                       uint32_t n_tx,
                       const void *p_txbuf,
                       uint32_t n_rx,
                       void *p_rxbuf);

/**
 * @brief Sends data using SPIM peripheral.
 *
 * @param[in]  p_spim_drv       Pointer to structure representing SPIM driver.
 * @param[in]  n                Output data length.
 * @param[out] p_txbuf          Output data buffer.
 */
void nodi_spim_send(nodi_spim_drv_t *p_spim_drv, uint32_t n, const void *p_txbuf);

/**
 * @brief Receives data using SPIM peripheral.
 *
 * @param[in] p_spim_drv        Pointer to structure representing SPIM driver.
 * @param[in] n                 Input data length.
 * @param[in] p_rxbuf           Input data buffer.
 */
void nodi_spim_receive(nodi_spim_drv_t *p_spim_drv, uint32_t n, void *p_rxbuf);

/**
 * @brief Deinitializes SPIM peripheral.
 *
 * @param[in] p_spim_drv        Pointer to structure representing SPIM driver.
 */
void nodi_spim_deinit(nodi_spim_drv_t *p_spim_drv);

/*===========================================================================*/
/* PPI related functions.                                                    */
/*===========================================================================*/

/**
 * @brief Configures registers in SPIM peripheral.
 *
 * Function for configuring buffer registers in SPIM peripheral. This function disables interrupts
 * because assumes that developer will use it with PPI without interrupts. For mixing interrupts
 * and events please read Nordic's IC product specification.
 *
 * @param[in]  p_spim_drv       Pointer to structure representing SPIM driver.
 * @param[in]  n_tx             Output data length.
 * @param[out] p_txbuf          Output data buffer.
 * @param[in]  n_rx             Input data length.
 * @param[in]  p_rxbuf          Input data buffer.
 */
void nodi_spim_xfer_configure(nodi_spim_drv_t *p_spim_drv,
                              uint32_t n_tx,
                              const void *p_txbuf,
                              uint32_t n_rx,
                              void *p_rxbuf);

/**
 * @brief Returns start task address to connect some events with this task using PPI.
 *
 * @param[in]  p_spim_drv       Pointer to structure representing SPIM driver.
 *
 * @return Address to start task of the SPIM peripheral.
 */
uint32_t nodi_spim_task_addr_get(nodi_spim_drv_t *p_spim_drv);

/**
 * @brief Returns end event address to connect some tasks with this event using PPI.
 *
 * @param[in]  p_spim_drv       Pointer to structure representing SPIM driver.
 *
 * @return Address to end event of the SPIM peripheral.
 */
uint32_t nodi_spim_evt_addr_get(nodi_spim_drv_t *p_spim_drv);


#ifdef NODI_SPIM_DISABLE_IRQ_CONNECT

/**
 * @brief SPIM interrupt service routine.
 *
 * @details This interrupt routine should be connect to interrupt system used in specific
 *          environment.To use direct connection between IRQ and this function, undefine
 *          NODI_SPIM_DISABLE_IRQ_CONNECT define.
 *
 * @param[in] p_ctx             Pointer to context internally casted to structure representing SPIM driver.
 */
void nodi_spim_irq_routine(void *p_ctx);

#endif

#ifdef __cplusplus
}
#endif


#endif /* NODI_SPIM_ENABLED */

#endif /* NODI_SPIM_H */
