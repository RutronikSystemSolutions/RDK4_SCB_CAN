/*
 * can.h
 *
 *  Created on: 06.05.2024
 *      Author: FAE-NODE4813
 */

#ifndef CAN_CAN_H_
#define CAN_CAN_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#ifdef COMPONENT_CAT1
#define CANFD_INTERRUPT         canfd_0_interrupts0_0_IRQn
#else
#define CANFD_INTERRUPT         canfd_interrupts0_0_IRQn
#endif

#define CAN_FD_HW_CHANNEL        0
#define CAN_FD_BUFFER_INDEX      0
#define CAN_FD_DLC		 8

/* init_Can */
void init_can(void);

#endif /* CAN_CAN_H_ */
