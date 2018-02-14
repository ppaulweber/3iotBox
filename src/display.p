//
//  Copyright (C) 2018 Philipp Paulweber, Simon Tragatschnig, and Patrick Gaubatz
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                <https://github.com/ppaulweber/3iotBox>
//
//  This file is part of 3iotBox.
//
//  3iotBox is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  3iotBox is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with 3iotBox. If not, see <http://www.gnu.org/licenses/>.
//

#include "display"

#include "rM2M"

const DISPLAY_SPI_INTERFACE = 0;

const DISPLAY_SPI_CHIP_SELECT = 0;
const DISPLAY_SPI_DATA_COMMAND = 1;
const DISPLAY_SPI_RESET = 2;
const DISPLAY_SPI_BUSY_FLAG = 3;

const EPD_WIDTH = 640;
const EPD_HEIGHT = 384;

const PANEL_SETTING                             = 0x00;
const POWER_SETTING                             = 0x01;
const POWER_OFF                                 = 0x02;
const POWER_OFF_SEQUENCE_SETTING                = 0x03;
const POWER_ON                                  = 0x04;
const POWER_ON_MEASURE                          = 0x05;
const BOOSTER_SOFT_START                        = 0x06;
const DEEP_SLEEP                                = 0x07;
const DATA_START_TRANSMISSION_1                 = 0x10;
const DATA_STOP                                 = 0x11;
const DISPLAY_REFRESH                           = 0x12;
const IMAGE_PROCESS                             = 0x13;
const LUT_FOR_VCOM                              = 0x20; 
const LUT_BLUE                                  = 0x21;
const LUT_WHITE                                 = 0x22;
const LUT_GRAY_1                                = 0x23;
const LUT_GRAY_2                                = 0x24;
const LUT_RED_0                                 = 0x25;
const LUT_RED_1                                 = 0x26;
const LUT_RED_2                                 = 0x27;
const LUT_RED_3                                 = 0x28;
const LUT_XON                                   = 0x29;
const PLL_CONTROL                               = 0x30;
const TEMPERATURE_SENSOR_COMMAND                = 0x40;
const TEMPERATURE_CALIBRATION                   = 0x41;
const TEMPERATURE_SENSOR_WRITE                  = 0x42;
const TEMPERATURE_SENSOR_READ                   = 0x43;
const VCOM_AND_DATA_INTERVAL_SETTING            = 0x50;
const LOW_POWER_DETECTION                       = 0x51;
const TCON_SETTING                              = 0x60;
const TCON_RESOLUTION                           = 0x61;
const SPI_FLASH_CONTROL                         = 0x65;
const REVISION                                  = 0x70;
const GET_STATUS                                = 0x71;
const AUTO_MEASUREMENT_VCOM                     = 0x80;
const READ_VCOM_VALUE                           = 0x81;
const VCM_DC_SETTING                            = 0x82;
const FLASH_MODE                                = 0xe5;


Display_Enable()
{
    rM2M_GpioSet( DISPLAY_SPI_CHIP_SELECT, 0 ); // low (enable)
}

Display_Disable()
{
    rM2M_GpioSet( DISPLAY_SPI_CHIP_SELECT, 1 ); // high (disable)
}

SPI_init()
{
    new status;
    status = rM2M_SpiInit( DISPLAY_SPI_INTERFACE, 12000000, RM2M_SPI_CLKPOL );
    if( status != OK )
    {
	printf( "spi: init (status %d)\r\n", status );
    }
}

SPI_send( const data )
{
    new status;
    new buffer{1};
    buffer{0} = data;
    status = rM2M_SpiCom( DISPLAY_SPI_INTERFACE, buffer, 1, 0 );
    if( status != OK )
    {
	printf( "spi: send %x (status %d)\r\n", data, status );
    }
}

Display_SendCommand( const command )
{
    // printf( "display: c = %d\r\n", command );
    rM2M_GpioSet( DISPLAY_SPI_DATA_COMMAND, 0 ); // low (0) command
    SPI_send( command );
}

Display_SendData( const data )
{
    // printf( "display: d = %d\r\n", data );
    rM2M_GpioSet( DISPLAY_SPI_DATA_COMMAND, 1 ); // high (1) data
    SPI_send( data );
}

Display_Command( const command, dataBuffer{} = { 0x00 }, const dataLength = 0 )
{
    Display_SendCommand( command );

    new index;
    for( index = 0; index < dataLength; index++ )
    {
	Display_SendData( dataBuffer{index} );
    }
}


public Display_Init()
{
    SPI_init();
    
    // inputs
    rM2M_GpioDir( DISPLAY_SPI_BUSY_FLAG, RM2M_GPIO_INPUT );    
    
    // outputs
    rM2M_GpioDir( DISPLAY_SPI_CHIP_SELECT, RM2M_GPIO_OUTPUT );
    rM2M_GpioSet( DISPLAY_SPI_CHIP_SELECT, 1 ); // low-active
    
    rM2M_GpioDir( DISPLAY_SPI_DATA_COMMAND, RM2M_GPIO_OUTPUT );
    rM2M_GpioSet( DISPLAY_SPI_DATA_COMMAND, 0 ); // low (0) command, high (1) data

    rM2M_GpioDir( DISPLAY_SPI_RESET, RM2M_GPIO_OUTPUT );
    rM2M_GpioSet( DISPLAY_SPI_RESET, 1 ); // low-active

    Display_Reset();
}

public Display_Powerup()
{
    Display_Enable();
    // Display_Command( POWER_SETTING, { 0x37, 0x00, 0x08, 0x08 }, 4 );
    // Display_Command( PANEL_SETTING, { 0xcf, 0x08 }, 2 );
    // Display_Command( BOOSTER_SOFT_START, { 0xc7, 0xcc, 0x28 }, 3 );
    // Display_Command( POWER_ON );

    Display_Command( BOOSTER_SOFT_START, { 0xc7, 0xcc, 0x28 }, 3 );
    Display_Command( POWER_SETTING, { 0x37, 0x00, 0x08, 0x08 }, 4 );
    Display_Command( POWER_ON );
    // Display_Sync();
}

public Display_Setup()
{
    // Display_Command( PLL_CONTROL, { 0x3c }, 1 );
    // Display_Command( TEMPERATURE_CALIBRATION, { 0x00 }, 1 );
    // Display_Command( VCOM_AND_DATA_INTERVAL_SETTING, { 0x77 }, 1 );
    // Display_Command( TCON_SETTING, { 0x22 }, 1 );
    // Display_Command( TCON_RESOLUTION, { 0x02, 0x80, 0x01, 0x80 }, 4 );
    // Display_Command( VCM_DC_SETTING, { 0x1e }, 1 ); //decide by LUT file
    // Display_Command( FLASH_MODE, { 0x03 }, 1 ); //FLASH MODE

    Display_Command( PANEL_SETTING, { 0xcf, 0x00 }, 2 );    
    Display_Command( PLL_CONTROL, { 0x3a }, 1 );
    Display_Command( TCON_RESOLUTION, { 0x02, 0x80, 0x01, 0x80 }, 4 );
    Display_Command( VCM_DC_SETTING, { 0x1e }, 1 ); //decide by LUT file
    Display_Command( VCOM_AND_DATA_INTERVAL_SETTING, { 0x77 }, 1 );
    Display_Command( FLASH_MODE, { 0x03 }, 1 ); //FLASH MODE
    // Display_Sync();
    // Display_Refresh();
    Display_SendCommand( DATA_START_TRANSMISSION_1 );
}

public Display_TestImage()
{
    

    new index;
    /* for( index = 0; index < 30720; index++ ) */
    for( index = 0; index < EPD_WIDTH; index++ )
    {
    	Display_SendData( 0x03 );
    }
    // Display_Sync();
}

public Display_Refresh()
{
    /* Display_SendCommand( DATA_STOP ); */
    
    Display_SendCommand( DISPLAY_REFRESH );
    //Display_Sync();    
}

public Display_Sleep()
{
    printf( "display: sleep: starting\r\n" );

    Display_Command( POWER_OFF );
    Display_Sync();
    Display_Command( DEEP_SLEEP, { 0xa5 }, 1 );

    printf( "display: sleep: done\r\n" );
}

public Display_Deinit()
{
    new res;
    res = rM2M_SpiClose( DISPLAY_SPI_INTERFACE );
    Display_Disable();
    printf( "display: spi: close = %d\r\n", res );
}

public Display_Reset()
{
    rM2M_GpioSet( DISPLAY_SPI_RESET, 0 ); // low-active
    // Display_Wait();
    rM2M_GpioSet( DISPLAY_SPI_RESET, 1 );
    // Display_Wait();
}

public Display_Sync()
{
    new isBusyPrevious = true;
    for( ;; )
    {
	new isBusyCurrent = Display_IsBusy();
	if( isBusyCurrent == isBusyPrevious )
	{
	    break;
	}
	isBusyPrevious = isBusyCurrent;
    }
}

public Display_IsBusy()
{
    new busy;
    busy = rM2M_GpioGet( DISPLAY_SPI_BUSY_FLAG ); // low (0) busy, high (1) idle
    if( busy == 0 )
    {
	return true;
    }
    else
    {
	return false;
    }
}
