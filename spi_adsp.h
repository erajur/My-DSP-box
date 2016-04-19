/*
* Filename:	spi_adsp.h
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
* Lowest level common ADSP-21489 SPI interface driver
*
*/

#pragma once

/*************************** Include files ***********************************/
#include "AppSpec.h"

/*************************** Defines *****************************************/

/*************************** Globals *****************************************/

/*************************** Classes/Functions *******************************/

class Cspi
{
public:
    Cspi(int buff_lenght, int chip_select);
	Cspi(void);
	~Cspi(void);
    
    static int m_count;

	virtual int viReadSpi(s32 & data, u32 address = 0) {return 0;}
    virtual int viWriteSpi(s32 data, u32 address = 0) {return 0;}

    char cSpiGetc(u8 data);
    void vSpiGets(u8 *str, u8 length, u8 port);

private:
protected:
    unsigned char* m_buff;
    int m_chip_select;
    int m_buff_lenght;

};
/* EOF */
