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

#include "nsd_common.h"
#include "nsd_pwr_clk.h"

#if (NSD_PWR_CLK_ENABLED == 1) || defined(__DOXYGEN__)

nsd_pwr_clk_drv_t NSD_PWR_CLK;

void nsd_pwr_clk_irq_routine(void *p_ctx);

void nsd_pwr_clk_prepare(void)
{
    NSD_PWR_CLK.hfclk_state = NSD_PWR_CLK_HFCLK_STATE_RUNNING_RC;
    NSD_PWR_CLK.lfclk_state = NSD_PWR_CLK_LFCLK_STATE_NOT_RUNNING;
    NSD_PWR_CLK.hfclk_cb = 0;
    NSD_PWR_CLK.lfclk_cb = 0;
    NSD_PWR_CLK.irq_priority = NSD_POWER_CLOCK_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_pwr_clk_irq_routine, &NSD_PWR_CLK, POWER_CLOCK_IRQn);
#endif
}

void nsd_pwr_clk_init(nsd_pwr_clk_drv_t * p_pwr_clk_drv)
{
    NRF_CLOCK->INTENCLR = 0xFFFFFFFF;
    NRF_POWER->INTENCLR = 0xFFFFFFFF;
    NRF_CLOCK->INTENSET = CLOCK_INTENSET_HFCLKSTARTED_Enabled;
    nsd_common_irq_enable(POWER_CLOCK_IRQn, p_pwr_clk_drv->irq_priority);
}

void nsd_clk_hfclk_start(nsd_pwr_clk_drv_t * p_pwr_clk_drv)
{
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
}

void nsd_clk_lfclk_start(nsd_pwr_clk_drv_t * p_pwr_clk_drv)
{
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART = 1;
}

void nsd_clk_hfclk_stop(nsd_pwr_clk_drv_t * p_pwr_clk_drv)
{
    NRF_CLOCK->TASKS_HFCLKSTOP = 1;
}

void nsd_clk_lfclk_stop(nsd_pwr_clk_drv_t * p_pwr_clk_drv)
{
    NRF_CLOCK->TASKS_LFCLKSTOP = 1;
}

void nsd_pwr_clk_irq_routine(void *p_ctx)
{
    NSD_DRV_CHECK(p_ctx != NULL);
    nsd_pwr_clk_drv_t * p_pwr_clk_drv = (nsd_pwr_clk_drv_t *) p_ctx;

    /* Call callback if not null. */
    if (NRF_CLOCK->EVENTS_HFCLKSTARTED == 1)
    {
        NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
        NSD_PWR_CLK.hfclk_state = NSD_PWR_CLK_HFCLK_STATE_RUNNING_XTAL;
        if (p_pwr_clk_drv->hfclk_cb)
        {
            p_pwr_clk_drv->hfclk_cb(p_pwr_clk_drv);
        }
    }

    /* Call callback if not null. */
    if (NRF_CLOCK->EVENTS_LFCLKSTARTED == 1)
    {
        NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
        /* Set proper lfclk source and state. */
        //NSD_PWR_CLK.lfclk_state = NSD_PWR_CLK_LFCLK_STATE_RUNNING_RC;
        if (p_pwr_clk_drv->lfclk_cb)
        {
            p_pwr_clk_drv->lfclk_cb(p_pwr_clk_drv);
        }
    }
}

#endif