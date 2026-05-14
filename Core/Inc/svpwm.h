/*
 * svpwm.h
 *
 *  Created on: May 14, 2026
 *      Author: Shantanu
 */

#ifndef INC_SVPWM_H_
#define INC_SVPWM_H_

#include "stdint.h"

#define ONE_BY_SQRT3   0.57735027f
#define TWO_BY_SQRT3   1.15470054f
#define PWM_PERIOD     9000U
#define VBUS           1.0f
#define SIGNAL_FREQ    100.0f
#define SAMPLE_FREQ    10000.0f

void svpwm(float valpha, float vbeta, float vbus, uint32_t pwm_period,
           uint32_t *pwm_u, uint32_t *pwm_v, uint32_t *pwm_w);


#endif /* INC_SVPWM_H_ */
