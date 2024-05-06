/*
 * can.c
 *
 *  Created on: 06.05.2024
 *      Author: Andreas Heder
 */

#include "can.h"



    /* Populate the configuration structure for CANFD Interrupt */
    cy_stc_sysint_t canfd_irq_cfg =
    {
        /* Source of interrupt signal */
        .intrSrc      = CANFD_INTERRUPT,
        /* Interrupt priority */
        .intrPriority = 1U,
    };

    /* This is a shared context structure, unique for each canfd channel */
    static cy_stc_canfd_context_t canfd_context;

static void isr_canfd(void);

void init_can(void)
    {
    cy_en_canfd_status_t status;
    /* Hook the interrupt service routine and enable the interrupt */
    (void) Cy_SysInt_Init(&canfd_irq_cfg, &isr_canfd);

    NVIC_EnableIRQ(CANFD_INTERRUPT);

    /* Initialze CANFD Channel */
    status = Cy_CANFD_Init(CAN_FD_HW, CAN_FD_HW_CHANNEL, &CAN_FD_config,&canfd_context);

    if (status != CY_CANFD_SUCCESS)
    {
        CY_ASSERT(0);
    }

    }

/*******************************************************************************
* Function Name: isr_canfd
********************************************************************************
* Summary:
* This is the interrupt handler function for the canfd interrupt.
*
* Parameters:
*  none
*
*
*******************************************************************************/
static void isr_canfd(void)
    {
    /* Just call the IRQ handler with the current channel number and context */
    Cy_CANFD_IrqHandler(CAN_FD_HW, CAN_FD_HW_CHANNEL, &canfd_context); //
    }

/*******************************************************************************
* Function Name: canfd_rx_callback
********************************************************************************
* Summary:
* This is the callback function for canfd reception
*
* Parameters:
*    msg_valid                     Message received properly or not
*    msg_buf_fifo_num              RxFIFO number of the received message
*    canfd_rx_buf                  Message buffer
*
*******************************************************************************/
void canfd_rx_callback (bool                        msg_valid,
                        uint8_t                     msg_buf_fifo_num,
                        cy_stc_canfd_rx_buffer_t*   canfd_rx_buf)
{

    /* Array to hold the data bytes of the CANFD frame */
    uint8_t canfd_data_buffer[CAN_FD_DLC];
    /* Variable to hold the data length code of the CANFD frame */
    uint32_t canfd_dlc;
    /* Variable to hold the Identifier of the CANFD frame */
    uint32_t canfd_id;

    if (true == msg_valid)
    {
        /* Checking whether the frame received is a data frame */
        if(CY_CANFD_RTR_DATA_FRAME == canfd_rx_buf->r0_f->rtr)
        {
            canfd_dlc = canfd_rx_buf->r1_f->dlc;
            canfd_id  = canfd_rx_buf->r0_f->id;

            printf("%d bytes received from Node-%d with identifier 0x%03x\r\n\r\n",
                                                        (int)canfd_dlc,
                                                        (int)canfd_id,
                                                        (int)canfd_id);

            memcpy(canfd_data_buffer,canfd_rx_buf->data_area_f,canfd_dlc);

            printf("Rx Data : ");

            for (uint8_t msg_idx = 0U; msg_idx < canfd_dlc ; msg_idx++)
            {
                printf(" %d ", canfd_data_buffer[msg_idx]);
            }

            printf("\r\n\r\n");

            //replay Message with ID+1^
            /* Setting Node-1 Identifier as 0x01 */
               CAN_FD_T0RegisterBuffer_0.id = canfd_id+1;

               /* Assign the user defined data buffer to CANFD data area */
                  CAN_FD_txBuffer_0.data_area_f = canfd_rx_buf->data_area_f;
            Cy_CANFD_UpdateAndTransmitMsgBuffer(CANFD0, 0u, &CAN_FD_txBuffer_0, 0u, &canfd_context);
        }
    }

}


