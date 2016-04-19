/*
* Filename:	spi.c
* Project:	Harken
*
* copyright (c) EII 2013
*
* The copyright to the computer Program(s) herein is the
* property of Pera/EII. The program(s) may be
* used and or copied only with the written permission of
* Pera/EII, or in accordance with the terms and conditions
* stipulated in the agreement contract under which the
* program(s) have been supplied.
*
* File Description:
*
*
*/


/*************************** Include files ***********************************/
#include "spi_adsp.h"

/*************************** Defines *****************************************/
#define SELECT_SPI_SLAVE(cs)    (*pSPIFLG &= ~(cs<<8))
#define DESELECT_SPI_SLAVE(cs)  (*pSPIFLG |=  (cs<<8))

/*************************** Globals *****************************************/

/*************************** Classes/Functions *******************************/
int Cspi::m_count = 0;  // only one definition of a static member in a program.

/******************************************************************************
 Function:	Constructor
 Purpose:   Enable corresponding SPI port and secure needed data length buffer	
 Arguments: int buff_lenght - data communication buffer (how many bytes can be
            read out or written into SPI peripheral chip)
            int chip_select - enable individual SPI slave-select line and store
            slave-select line for automatic use.
 Return:
 Remark:
******************************************************************************/
Cspi::Cspi(int buff_lenght, int chip_select)
{
    Cspi();

    m_chip_select = chip_select;
    *pSPIFLG |= m_chip_select;              // Enable SPI operation

    m_buff_lenght = buff_lenght;
    m_buff = new unsigned char [buff_lenght+1];
    m_buff[buff_lenght] = 0;
}

/******************************************************************************
 Function:	Default constructor
 Purpose:   Only first declared class member will initialize SPI peripheral 
 Arguments:
 Return:
 Remark:
******************************************************************************/
Cspi::Cspi(void)
{
    m_buff_lenght = 0;
    m_chip_select = 0;
    m_buff = 0;

    if(m_count == 0)
    {
        m_count ++;                         // Only one occurrence 

        // Configure IO pins first
        SRU(SPI_MOSI_O, DPI_PB01_I);        //Connect MOSI to DPI PB1.
        SRU(DPI_PB02_O, SPI_MISO_I);        //Connect DPI PB2 to MISO.
        SRU(SPI_CLK_O, DPI_PB03_I);         //Connect SPI CLK to DPI PB3.
        SRU(SPI_FLG0_O, DPI_PB04_I);        //Connect SPI FLAG0 to DPI PB4.

        SRU(SPI_MOSI_PBEN_O, DPI_PBEN01_I);
        SRU(SPI_MISO_PBEN_O, DPI_PBEN02_I);
        SRU(SPI_CLK_PBEN_O, DPI_PBEN03_I);
        SRU(SPI_FLG0_PBEN_O, DPI_PBEN04_I); 

        // Configure the SPI Control registers
        // First clear a few registers
        *pSPICTL = (TXFLSH | RXFLSH) ;
        *pSPIFLG = 0;

        // Setup the baud rate to 1MHz
        *pSPIBAUD = 100;

        // Setup the SPI Flag register using the Flag specified in the call
        // Now setup the SPI Control register
        *pSPIFLG = (0x0F00);
        *pSPICTL = (SPIEN | SPIMS | WL8 | MSBF | TIMOD1 | CPHASE | CLKPL | SMLS | GM);
    }
}

/******************************************************************************
 Function:	Default destructor
 Purpose:   Clean the memory
 Arguments:
 Return:
 Remark:
******************************************************************************/
Cspi::~Cspi(void)
{
    if(m_buff != 0)
    {
        delete [] m_buff;
    }
}

/******************************************************************************
 Function:	SPI_getc
 Purpose:   get symbol
 Arguments:
 Return:
 Remark:                
******************************************************************************/
char Cspi::cSpiGetc(u8 data)
{
int j = 0;
	
    *pTXSPI = data;

    while ((*pSPISTAT & TXS)){j++;}         // Wait for Tx end
    while (!(*pSPISTAT & RXS)){j++;}        // Check for Rx done

return *pRXSPI;
}

/******************************************************************************
 Function:	ser_gets
 Purpose:   get string
 Arguments:
 Return:
 Remark:    
******************************************************************************/
void Cspi::vSpiGets (u8 *str, u8 length, u8 port)
{
int i=0;
int tmp = 0;

    SELECT_SPI_SLAVE(port);

    for(i = 0; i<length; i++)
    {
        str[i] = cSpiGetc(str[i]);
    }

    while (!(*pSPISTAT & SPIF));

    DESELECT_SPI_SLAVE(port);
}
