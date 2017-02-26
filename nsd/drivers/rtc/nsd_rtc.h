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

#ifndef NSD_RTC_H
#define NSD_RTC_H

#include "nsd_device.h"

#if 1 || (NSD_RTC_ENABLED == 1) || defined(__DOXYGEN__)
#include "nsd_rtc_const.h"

#if !defined(NSD_RTC_USE_RTC0) || defined(__DOXYGEN__)
#define NSD_RTC_USE_RTC0 0
#endif

#if !defined(NSD_RTC_USE_RTC1) || defined(__DOXYGEN__)
#define NSD_RTC_USE_RTC1 0
#endif

#if !defined(NSD_RTC_USE_RTC2) || defined(__DOXYGEN__)
#define NSD_RTC_USE_RTC2 0
#endif

/**
 * @brief   RTC callback events.
 */
typedef enum {
    NSD_RTC_DRV_CB_EVT_TICK,     ///< Driver is uninitialized.
    NSD_RTC_DRV_CB_EVT_OVERFLOW, ///< Driver is ready to start an operation.
    NSD_RTC_DRV_CB_EVT_COMP0,    ///< Driver is busy, executing operation.
    NSD_RTC_DRV_CB_EVT_COMP1,    ///< Driver finished operation.
    NSD_RTC_DRV_CB_EVT_COMP2,    ///< Driver finished operation.
    NSD_RTC_DRV_CB_EVT_COMP3,    ///< Driver finished operation.
} nsd_rtc_cb_evt_t;

typedef struct nsd_rtc_drv nsd_rtc_drv_t;

/**
 * @brief   RTC notification callback type.
 *
 * @param[in] p_rtc_drv      pointer to the nsd_rtc_drv_t object triggering the callback
 */
typedef void (*nsd_rtc_irq_callback_t)(nsd_rtc_drv_t *p_rtc_drv, nsd_rtc_cb_evt_t evt);


typedef struct {
    nsd_rtc_irq_callback_t evt_cb;    ///< Transmit operation complete callback or NULL.
    uint32_t               prescaler; ///< RTC prescaler to gain period 32768/(PRESCALER+1).
} nsd_rtc_config_s;

/**
 * @brief   RTC Driver state machine possible states.
 */
typedef enum {
    NSD_RTC_DRV_STATE_UNINIT, ///< Driver is uninitialized.
    NSD_RTC_DRV_STATE_READY,  ///< Driver is ready to start an operation.
    NSD_RTC_DRV_STATE_BUSY,   ///< Driver is busy, executing operation.
    NSD_RTC_DRV_STATE_FINISH, ///< Driver finished operation.
} nsd_rtc_state_t;

/**
 * @brief   Structure representing a RTC driver.
 */
struct nsd_rtc_drv {
    const nsd_rtc_config_s   *config;       ///< Current configuration data.
    volatile nsd_rtc_state_t  state;        ///< RTC driver current state.
    NRF_RTC_Type             *p_rtc_reg;    ///< Pointer to the RTC registers block.
    IRQn_Type                 irq;          ///< RTC peripheral instance IRQ number.
    uint8_t                   irq_priority; ///< Interrupt priority.
};

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (NSD_RTC_USE_RTC0 == 1) && !defined(__DOXYGEN__)
extern nsd_rtc_drv_t NSD_RTC0;
#endif

#if (NSD_RTC_USE_RTC1 == 1) && !defined(__DOXYGEN__)
extern nsd_rtc_drv_t NSD_RTC1;
#endif

#if (NSD_RTC_USE_RTC2 == 1) && !defined(__DOXYGEN__)
extern nsd_rtc_drv_t NSD_RTC2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Initializes structures of active drivers
 */
void nsd_rtc_prepare();

/**
 * @brief Initializes selected peripheral.
 *
 * @param[in] p_rtc_drv       Pointer to structure representing RTC driver.
 */
void nsd_rtc_init(nsd_rtc_drv_t *p_rtc_drv);

/**
 * @brief nsd_rtc_send_stop
 * @param p_rtc_drv
 */
void nsd_rtc_evt_enable(nsd_rtc_drv_t *p_rtc_drv, nsd_rtc_cb_evt_t evt);

/**
 * @brief nsd_rtc_send_stop
 * @param p_rtc_drv
 */
void nsd_rtc_evt_disable(nsd_rtc_drv_t *p_rtc_drv, nsd_rtc_cb_evt_t evt);

/**
 * @brief Sends data using RTC peripheral.
 *
 * @param[in]  p_rtc_drv      Pointer to structure representing RTC driver.
 */
void nsd_rtc_start(nsd_rtc_drv_t *p_rtc_drv);

/**
 * @brief nsd_rtc_send_stop
 * @param p_rtc_drv
 */
void nsd_rtc_stop(nsd_rtc_drv_t *p_rtc_drv);


#ifdef NSD_RTC_DISABLE_IRQ_CONNECT

/**
 * @brief RTC interrupt service routine.
 *
 * @details This interrupt routine should be connect to interrupt system used in specific
 *          environment.To use direct connection between IRQ and this function, undefine
 *          NSD_RTC_DISABLE_IRQ_CONNECT define.
 *
 * @param[in] p_ctx             Pointer context internally casted to structure representing RTC driver.
 */
void nsd_rtc_irq_routine(void *p_ctx);

#endif

#ifdef __cplusplus
}
#endif


#endif /* NSD_RTC_ENABLED */

#endif /* NSD_RTC_H */