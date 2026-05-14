/*
 * svpwm.c
 *
 *  Created on: May 14, 2026
 *      Author: Shantanu
 */

#include "svpwm.h"
#include "stdint.h"

void svpwm(float valpha, float vbeta, float vbus, uint32_t pwm_period,
           uint32_t *pwm_u, uint32_t *pwm_v, uint32_t *pwm_w)
{
    // 1. Normalize
    float alpha = valpha / vbus;
    float beta  = vbeta  / vbus;

    // 2. Sector determination
    uint8_t sector;
    if (beta >= 0.0f) {
        if (alpha >= 0.0f)
            sector = (ONE_BY_SQRT3 * beta > alpha) ? 2 : 1;
        else
            sector = (-ONE_BY_SQRT3 * beta > alpha) ? 3 : 2;
    } else {
        if (alpha >= 0.0f)
            sector = (-ONE_BY_SQRT3 * beta > alpha) ? 5 : 6;
        else
            sector = (ONE_BY_SQRT3 * beta > alpha) ? 4 : 5;
    }

    // 3. T1, T2 and PWM compare values per sector
    int32_t t1, t2;
    switch (sector)
    {
        case 1:
            t1 = (int32_t)((alpha - ONE_BY_SQRT3 * beta) * pwm_period);
            t2 = (int32_t)((TWO_BY_SQRT3 * beta)         * pwm_period);
            *pwm_u = (pwm_period + t1 + t2) / 2;
            *pwm_v = *pwm_u - t1;
            *pwm_w = *pwm_v - t2;
            break;

        case 2:
            t1 = (int32_t)((alpha + ONE_BY_SQRT3 * beta)  * pwm_period);
            t2 = (int32_t)((-alpha + ONE_BY_SQRT3 * beta) * pwm_period);
            *pwm_v = (pwm_period + t1 + t2) / 2;
            *pwm_u = *pwm_v - t2;
            *pwm_w = *pwm_u - t1;
            break;

        case 3:
            t1 = (int32_t)((TWO_BY_SQRT3 * beta)          * pwm_period);
            t2 = (int32_t)((-alpha - ONE_BY_SQRT3 * beta) * pwm_period);
            *pwm_v = (pwm_period + t1 + t2) / 2;
            *pwm_w = *pwm_v - t1;
            *pwm_u = *pwm_w - t2;
            break;

        case 4:
            t1 = (int32_t)((-alpha + ONE_BY_SQRT3 * beta) * pwm_period);
            t2 = (int32_t)((-TWO_BY_SQRT3 * beta)         * pwm_period);
            *pwm_w = (pwm_period + t1 + t2) / 2;
            *pwm_v = *pwm_w - t2;
            *pwm_u = *pwm_v - t1;
            break;

        case 5:
            t1 = (int32_t)((-alpha - ONE_BY_SQRT3 * beta) * pwm_period);
            t2 = (int32_t)((alpha - ONE_BY_SQRT3 * beta)  * pwm_period);
            *pwm_w = (pwm_period + t1 + t2) / 2;
            *pwm_u = *pwm_w - t1;
            *pwm_v = *pwm_u - t2;
            break;

        case 6:
            t1 = (int32_t)((-TWO_BY_SQRT3 * beta)         * pwm_period);
            t2 = (int32_t)((alpha + ONE_BY_SQRT3 * beta)  * pwm_period);
            *pwm_u = (pwm_period + t1 + t2) / 2;
            *pwm_w = *pwm_u - t2;
            *pwm_v = *pwm_w - t1;
            break;
    }

    // 4. Clamp to valid range [0, pwm_period]
    *pwm_u = (*pwm_u > pwm_period) ? pwm_period : *pwm_u;
    *pwm_v = (*pwm_v > pwm_period) ? pwm_period : *pwm_v;
    *pwm_w = (*pwm_w > pwm_period) ? pwm_period : *pwm_w;
}
