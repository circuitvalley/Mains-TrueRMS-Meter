/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// eusci_b_spi.c - Driver for the eusci_b_spi Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup eusci_b_spi_api
//! @{
//
//*****************************************************************************

#include "inc/hw_regaccess.h"
#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_EUSCI_B0__
#include "eusci_b0_spi_5959.h"

#include <assert.h>

//*****************************************************************************
//
//! \brief Initializes the SPI Master block.
//!
//! Upon successful initialization of the SPI master block, this function will
//! have set the bus speed for the master, but the SPI Master block still
//! remains disabled and must be enabled with EUSCI_B_SPI_enable()
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI Master module.
//! \param selectClockSource selects Clock source.
//!        Valid values are:
//!        - \b EUSCI_B_SPI_CLOCKSOURCE_ACLK
//!        - \b EUSCI_B_SPI_CLOCKSOURCE_SMCLK
//! \param clockSourceFrequency is the frequency of the slected clock source
//! \param desiredSpiClock is the desired clock rate for SPI communication
//! \param msbFirst controls the direction of the receive and transmit shift
//!        register.
//!        Valid values are:
//!        - \b EUSCI_B_SPI_MSB_FIRST
//!        - \b EUSCI_B_SPI_LSB_FIRST [Default]
//! \param clockPhase is clock phase select.
//!        Valid values are:
//!        - \b EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT
//!           [Default]
//!        - \b EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
//! \param clockPolarity is clock polarity select
//!        Valid values are:
//!        - \b EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
//!        - \b EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW [Default]
//! \param spiMode is SPI mode select
//!        Valid values are:
//!        - \b EUSCI_B_SPI_3PIN
//!        - \b EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_HIGH
//!        - \b EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_LOW
//!
//! Modified bits are \b UCCKPH, \b UCCKPL, \b UC7BIT, \b UCMSB, \b UCSSELx and
//! \b UCSWRST of \b UCAxCTLW0 register.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
bool EUSCI_B_SPI_masterInit(
                            uint8_t selectClockSource,
                            uint32_t clockSourceFrequency,
                            uint32_t desiredSpiClock,
                            uint16_t msbFirst,
                            uint16_t clockPhase,
                            uint16_t clockPolarity,
                            uint16_t spiMode
                            )
{
        assert(
                (EUSCI_B_SPI_CLOCKSOURCE_ACLK == selectClockSource) ||
                (EUSCI_B_SPI_CLOCKSOURCE_SMCLK == selectClockSource)
                );

        assert(  (EUSCI_B_SPI_MSB_FIRST == msbFirst) ||
                 (EUSCI_B_SPI_LSB_FIRST == msbFirst)
                 );

        assert(  (EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT == clockPhase) ||
                 (EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT == clockPhase)
                 );

        assert(  (EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH == clockPolarity) ||
                 (EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW == clockPolarity)
                 );

        assert(
                (EUSCI_B_SPI_3PIN == spiMode) ||
                (EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_HIGH == spiMode) ||
                (EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_LOW == spiMode)
                );

        //Disable the USCI Module
       UCB0CTLW0 |= UCSWRST;

        //Reset OFS_UCBxCTLW0 values
        UCB0CTLW0 &= ~(UCCKPH + UCCKPL + UC7BIT + UCMSB +
                                                  UCMST + UCMODE_3 + UCSYNC);


        //Reset OFS_UCBxCTLW0 values
       UCB0CTLW0 &= ~(UCSSEL_3);

        //Select Clock
       UCB0CTLW0 |= selectClockSource;


        UCB0BRW =
                (uint16_t)(clockSourceFrequency / desiredSpiClock);


        /*
         * Configure as SPI master mode.
         * Clock phase select, polarity, msb
         * UCMST = Master mode
         * UCSYNC = Synchronous mode
         * UCMODE_0 = 3-pin SPI
         */
        UCB0CTLW0 |= (
                msbFirst +
                clockPhase +
                clockPolarity +
                UCMST +
                UCSYNC +
                spiMode
                );



        return STATUS_SUCCESS;
}

//*****************************************************************************
//
//! \brief Selects 4Pin Functionality
//!
//! This function should be invoked only in 4-wire mode. Invoking this function
//! has no effect in 3-wire mode.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//! \param select4PinFunctionality selects 4 pin functionality
//!        Valid values are:
//!        - \b EUSCI_B_SPI_PREVENT_CONFLICTS_WITH_OTHER_MASTERS
//!        - \b EUSCI_B_SPI_ENABLE_SIGNAL_FOR_4WIRE_SLAVE
//!
//! Modified bits are \b UCSTEM of \b UCAxCTLW0 register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_B_SPI_select4PinFunctionality(
                                         uint8_t select4PinFunctionality
                                         )
{
        assert(  (EUSCI_B_SPI_PREVENT_CONFLICTS_WITH_OTHER_MASTERS == select4PinFunctionality) ||
                 (EUSCI_B_SPI_ENABLE_SIGNAL_FOR_4WIRE_SLAVE == select4PinFunctionality)
                 );

       UCB0CTLW0 &= ~UCSTEM;
        UCB0CTLW0 |= select4PinFunctionality;
}

//*****************************************************************************
//
//! \brief Initializes the SPI Master clock. At the end of this function call,
//! SPI module is left enabled.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//! \param clockSourceFrequency is the frequency of the slected clock source
//! \param desiredSpiClock is the desired clock rate for SPI communication
//!
//! Modified bits are \b UCSWRST of \b UCAxCTLW0 register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_B_SPI_masterChangeClock(
                                   uint32_t clockSourceFrequency,
                                   uint32_t desiredSpiClock
                                   )
{
        //Disable the USCI Module
        UCB0CTLW0 |= UCSWRST;

       UCB0BRW =
                (uint16_t)(clockSourceFrequency / desiredSpiClock);

        //Reset the UCSWRST bit to enable the USCI Module
       UCB0CTLW0 &= ~(UCSWRST);
}

//*****************************************************************************
//
//! \brief Initializes the SPI Slave block.
//!
//! Upon successful initialization of the SPI slave block, this function will
//! have initialized the slave block, but the SPI Slave block still remains
//! disabled and must be enabled with EUSCI_B_SPI_enable()
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI Slave module.
//! \param msbFirst controls the direction of the receive and transmit shift
//!        register.
//!        Valid values are:
//!        - \b EUSCI_B_SPI_MSB_FIRST
//!        - \b EUSCI_B_SPI_LSB_FIRST [Default]
//! \param clockPhase is clock phase select.
//!        Valid values are:
//!        - \b EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT
//!           [Default]
//!        - \b EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
//! \param clockPolarity is clock polarity select
//!        Valid values are:
//!        - \b EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
//!        - \b EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW [Default]
//! \param spiMode is SPI mode select
//!        Valid values are:
//!        - \b EUSCI_B_SPI_3PIN
//!        - \b EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_HIGH
//!        - \b EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_LOW
//!
//! Modified bits are \b UCMSB, \b UCMST, \b UC7BIT, \b UCCKPL, \b UCCKPH, \b
//! UCMODE and \b UCSWRST of \b UCAxCTLW0 register.
//!
//! \return STATUS_SUCCESS
//
//*****************************************************************************
bool EUSCI_B_SPI_slaveInit(
                           uint16_t msbFirst,
                           uint16_t clockPhase,
                           uint16_t clockPolarity,
                           uint16_t spiMode
                           )
{
        assert(
                (EUSCI_B_SPI_MSB_FIRST == msbFirst) ||
                (EUSCI_B_SPI_LSB_FIRST == msbFirst)
                );

        assert(
                (EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT == clockPhase) ||
                (EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT == clockPhase)
                );

        assert(
                (EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH == clockPolarity) ||
                (EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW == clockPolarity)
                );

        assert(
                (EUSCI_B_SPI_3PIN == spiMode) ||
                (EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_HIGH == spiMode) ||
                (EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_LOW == spiMode)
                );

        //Disable USCI Module
        UCB0CTLW0  |= UCSWRST;

        //Reset OFS_UCBxCTLW0 register
        UCB0CTLW0 &= ~(UCMSB +
                                                  UC7BIT +
                                                  UCMST +
                                                  UCCKPL +
                                                  UCCKPH +
                                                  UCMODE_3
                                                  );


        //Clock polarity, phase select, msbFirst, SYNC, Mode0
        UCB0CTLW0 |= ( clockPhase +
                                                  clockPolarity +
                                                  msbFirst +
                                                  UCSYNC +
                                                  spiMode
                                                  );


        return STATUS_SUCCESS;
}

//*****************************************************************************
//
//! \brief Changes the SPI colock phase and polarity. At the end of this
//! function call, SPI module is left enabled.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//! \param clockPhase is clock phase select.
//!        Valid values are:
//!        - \b EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT
//!           [Default]
//!        - \b EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
//! \param clockPolarity is clock polarity select
//!        Valid values are:
//!        - \b EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
//!        - \b EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW [Default]
//!
//! Modified bits are \b UCCKPL, \b UCCKPH and \b UCSWRST of \b UCAxCTLW0
//! register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_B_SPI_changeClockPhasePolarity(
                                          uint16_t clockPhase,
                                          uint16_t clockPolarity
                                          )
{

        assert(  (EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH == clockPolarity) ||
                 (EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW == clockPolarity)
                 );

        assert(  (EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT == clockPhase) ||
                 (EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT == clockPhase)
                 );

        //Disable the USCI Module
        UCB0CTLW0 |= UCSWRST;

        UCB0CTLW0 &= ~(UCCKPH + UCCKPL);

        UCB0CTLW0 |= (
                clockPhase +
                clockPolarity
                );

        //Reset the UCSWRST bit to enable the USCI Module
        UCB0CTLW0 &= ~(UCSWRST);
}

//*****************************************************************************
//
//! \brief Transmits a byte from the SPI Module.
//!
//! This function will place the supplied data into SPI trasmit data register
//! to start transmission.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//! \param transmitData data to be transmitted from the SPI module
//!
//! \return None
//
//*****************************************************************************
void EUSCI_B_SPI_transmitData(
                               uint8_t transmitData
                               )
{
        UCB0TXBUF = transmitData;
}

//*****************************************************************************
//
//! \brief Receives a byte that has been sent to the SPI Module.
//!
//! This function reads a byte of data from the SPI receive data Register.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//!
//! \return Returns the byte received from by the SPI module, cast as an
//!         uint8_t.
//
//*****************************************************************************
uint8_t EUSCI_B_SPI_receiveData(void)
{
        return UCB0RXBUF;
}

//*****************************************************************************
//
//! \brief Enables individual SPI interrupt sources.
//!
//! Enables the indicated SPI interrupt sources.  Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor. Does not clear interrupt flags.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//! \param mask is the bit mask of the interrupt sources to be enabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b EUSCI_B_SPI_TRANSMIT_INTERRUPT
//!        - \b EUSCI_B_SPI_RECEIVE_INTERRUPT
//!
//! Modified bits of \b UCAxIFG register and bits of \b UCAxIE register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_B_SPI_enableInterrupt(
                                 uint8_t mask
                                 )
{
        assert(!(mask & ~(EUSCI_B_SPI_RECEIVE_INTERRUPT
                          | EUSCI_B_SPI_TRANSMIT_INTERRUPT)));

        UCB0IE |= mask;
}

//*****************************************************************************
//
//! \brief Disables individual SPI interrupt sources.
//!
//! Disables the indicated SPI interrupt sources. Only the sources that are
//! enabled can be reflected to the processor interrupt; disabled sources have
//! no effect on the processor.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//! \param mask is the bit mask of the interrupt sources to be disabled.
//!        Mask value is the logical OR of any of the following:
//!        - \b EUSCI_B_SPI_TRANSMIT_INTERRUPT
//!        - \b EUSCI_B_SPI_RECEIVE_INTERRUPT
//!
//! Modified bits of \b UCAxIE register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_B_SPI_disableInterrupt(
                                  uint8_t mask
                                  )
{
        assert(!(mask & ~(EUSCI_B_SPI_RECEIVE_INTERRUPT
                          | EUSCI_B_SPI_TRANSMIT_INTERRUPT)));

       UCB0IE &= ~mask;
}

//*****************************************************************************
//
//! \brief Gets the current SPI interrupt status.
//!
//! This returns the interrupt status for the SPI module based on which flag is
//! passed.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//! \param mask is the masked interrupt flag status to be returned.
//!        Mask value is the logical OR of any of the following:
//!        - \b EUSCI_B_SPI_TRANSMIT_INTERRUPT
//!        - \b EUSCI_B_SPI_RECEIVE_INTERRUPT
//!
//! \return Logical OR of any of the following:
//!         - \b EUSCI_B_SPI_TRANSMIT_INTERRUPT
//!         - \b EUSCI_B_SPI_RECEIVE_INTERRUPT
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
uint8_t EUSCI_B_SPI_getInterruptStatus(
                                       uint8_t mask
                                       )
{
        assert(!(mask & ~(EUSCI_B_SPI_RECEIVE_INTERRUPT
                          | EUSCI_B_SPI_TRANSMIT_INTERRUPT)));

        return UCB0IFG & mask;
}

//*****************************************************************************
//
//! \brief Clears the selected SPI interrupt status flag.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//! \param mask is the masked interrupt flag to be cleared.
//!        Mask value is the logical OR of any of the following:
//!        - \b EUSCI_B_SPI_TRANSMIT_INTERRUPT
//!        - \b EUSCI_B_SPI_RECEIVE_INTERRUPT
//!
//! Modified bits of \b UCAxIFG register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_B_SPI_clearInterruptFlag(
                                    uint8_t mask
                                    )
{
        assert(!(mask & ~(EUSCI_B_SPI_RECEIVE_INTERRUPT
                          | EUSCI_B_SPI_TRANSMIT_INTERRUPT)));

        UCB0IFG &=  ~mask;
}

//*****************************************************************************
//
//! \brief Enables the SPI block.
//!
//! This will enable operation of the SPI block.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//!
//! Modified bits are \b UCSWRST of \b UCAxCTLW0 register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_B_SPI_enable(void)
{
        //Reset the UCSWRST bit to enable the USCI Module
        UCB0CTLW0 &= ~(UCSWRST);
}

//*****************************************************************************
//
//! \brief Disables the SPI block.
//!
//! This will disable operation of the SPI block.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//!
//! Modified bits are \b UCSWRST of \b UCAxCTLW0 register.
//!
//! \return None
//
//*****************************************************************************
void EUSCI_B_SPI_disable(void)
{
        //Set the UCSWRST bit to disable the USCI Module
        UCB0CTLW0 |= UCSWRST;
}

//*****************************************************************************
//
//! \brief Returns the address of the RX Buffer of the SPI for the DMA module.
//!
//! Returns the address of the SPI RX Buffer. This can be used in conjunction
//! with the DMA to store the received data directly to memory.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//!
//! \return the address of the RX Buffer
//
//*****************************************************************************
uint32_t EUSCI_B_SPI_getReceiveBufferAddressForDMA(void)
{
        return UCB0RXBUF;
}

//*****************************************************************************
//
//! \brief Returns the address of the TX Buffer of the SPI for the DMA module.
//!
//! Returns the address of the SPI TX Buffer. This can be used in conjunction
//! with the DMA to obtain transmitted data directly from memory.
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//!
//! \return the address of the TX Buffer
//
//*****************************************************************************
uint32_t EUSCI_B_SPI_getTransmitBufferAddressForDMA(void)
{
        return UCB0TXBUF;
}

//*****************************************************************************
//
//! \brief Indicates whether or not the SPI bus is busy.
//!
//! This function returns an indication of whether or not the SPI bus is
//! busy.This function checks the status of the bus via UCBBUSY bit
//!
//! \param baseAddress is the base address of the EUSCI_B_SPI module.
//!
//! \return One of the following:
//!         - \b EUSCI_B_SPI_BUSY
//!         - \b EUSCI_B_SPI_NOT_BUSY
//!         \n indicating if the EUSCI_B_SPI is busy
//
//*****************************************************************************
uint16_t EUSCI_B_SPI_isBusy(void)
{
        //Return the bus busy status.
        return (UCB0STATW & UCBBUSY);
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for eusci_b_spi_api
//! @}
//
//*****************************************************************************
