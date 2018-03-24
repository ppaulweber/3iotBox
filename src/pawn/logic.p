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

#pragma amxram 1310720
#pragma dynamic  10000


#include "filetransfer"
#include "sdk/iotbox"
#include "rM2M"

native getapilevel();
native CRC32(data{}, len, initial=0);

forward public Timer1s();
forward public ReadConfig(cfg);
forward public KeyChanged(iKeyState);

const TXMODE = RM2M_TXMODE_TRIG;

const CFG_BASIC_INDEX = 0;
const CFG_BASIC_SIZE = 9;
const HISTDATA_SIZE = 3 * 2 + 1;

static iRecItv;
static iTxItv;
static iTxMode; // (0 = interval, 1 = wakeup, 2 = online)

static iRecTimer;
static iTxTimer;

forward public Handle_SMS( const SmsTel[], const SmsText[] );


static receipt_uuid{ 48 };
static receipt_sync{ 16 };
static receipt_data[ 60 ]{ 60 };

forward public Display_Init();
forward public Display_Powerup();
forward public Display_Setup();
forward public Display_Refresh();
forward public Display_Sleep();
forward public Display_Deinit();
forward public Display_Reset();
forward public Display_Sync();
forward public Display_IsBusy();
forward public Display_Task();

static currentLine = -30;


main()
{
    Led_Init( LED_MODE_SCRIPT );
    Switch_Init( SWITCH_MODE_SCRIPT, funcidx( "KeyChanged" ) );
    
    new timeHour;
    new timeMinute;
    new timeSecond;
    new timeStamp;
    timeStamp = rM2M_GetTime( timeHour, timeMinute, timeSecond );
    printf( "time: %d:%d:%d | %d\r\n", timeHour, timeMinute, timeSecond, timeStamp );
    
    Display_Init();
    
    ReadConfig( CFG_BASIC_INDEX );
    ReadConfig( 2 );

    iTxTimer  = 0;
    iRecTimer = 0;
    rM2M_TxSetMode( iTxMode );
    rM2M_CfgOnChg( funcidx( "ReadConfig" ) );
    rM2M_SmsInit( funcidx( "Handle_SMS" ), 0 );
    rM2M_TimerAdd( funcidx( "Timer1s" ) );
    rM2M_TimerAddExt( funcidx( "Display_Task" ), true, 1 );
}



public Timer1s()
{
    Handle_Led();
    Handle_Transmission();
    Handle_Record();

    printf( "api %i, levelsignal strength %i\r\n", getapilevel(), rM2M_GSMGetRSSI() );
}


public Handle_SMS( const SmsTel[], const SmsText[] )
{
    printf( "received SMS from %s with text: %s\r\n", SmsTel, SmsText );    
}




const COLOR_RED     = 0x00ff0000;
const COLOR_GREEN   = 0x0000ff00;
const COLOR_BLUE    = 0x000000ff;
const COLOR_YELLOW  = 0x00ffff00;

Handle_Led()
{
    new connectionStatus = rM2M_TxGetStatus();

    Led_Off();
    if( connectionStatus & RM2M_TX_ACTIVE )
    {
	Led_On( COLOR_GREEN );
    }
    else if( connectionStatus & RM2M_TX_STARTED )
    {
	Led_Flicker( 0, COLOR_YELLOW );
    }
    else if( connectionStatus & RM2M_TX_RETRY ) 
    {
	Led_Flicker( 0, COLOR_BLUE );
    }
    else if( connectionStatus & RM2M_TX_FAILED )
    {
	Led_Blink( 0, COLOR_RED );
    }
}

Handle_Transmission()
{
    rM2M_TxStart();
}

Handle_Record()
{
    new aRecData{ HISTDATA_SIZE };

    iRecTimer--;
    if( iRecTimer <= 0 )
    {
	new aSysValues[ TIoTbox_SysValue ];
	IoTbox_GetSysValues(aSysValues);

	aRecData{ 0 } = 0;
	rM2M_Pack( aRecData, 1, aSysValues.VBat, RM2M_PACK_BE + RM2M_PACK_S16 );
	rM2M_Pack( aRecData, 3, aSysValues.VUsb, RM2M_PACK_BE + RM2M_PACK_S16 );
	rM2M_Pack( aRecData, 5, aSysValues.Temp, RM2M_PACK_BE + RM2M_PACK_S16 );

	rM2M_RecData( 0, aRecData, HISTDATA_SIZE );

	iRecTimer = iRecItv;
	// printf( "Vb:%d Vu:%d Ti:%d\r\n",aSysValues.VBat, aSysValues.VUsb, aSysValues.Temp );
    }
}

public KeyChanged( iKeyState )
{
    // iKeyState (0=release, 1=press)
    printf("K:%d\r\n", iKeyState);
    
    if( iKeyState == 1 )
    {
	if( currentLine >= 400 )
	{
	    currentLine = 199;
	}
    }
}

const ITV_RECORD        = 1 * 60;               // Record interval [sec.], default 1 min
const ITV_TRANSMISSION  = 1 * 60 * 60;          // Transmission interval [sec.], default 60 min

public ReadConfig( cfg )
{
    if( cfg == CFG_BASIC_INDEX )
    {
	new aData{ CFG_BASIC_SIZE };
	new iSize;
	new iTmp;

	iSize = rM2M_CfgRead
	( cfg
	, 0
	, aData
	, CFG_BASIC_SIZE
	);

	printf( "Cfg %d size = %d\r\n", cfg, iSize );

	if( iSize < CFG_BASIC_SIZE )
	{
	    iTmp = ITV_RECORD;
	    rM2M_Pack(aData, 0, iTmp,  RM2M_PACK_BE + RM2M_PACK_U32);
	    iTmp = ITV_TRANSMISSION;
	    rM2M_Pack(aData, 4, iTmp,   RM2M_PACK_BE + RM2M_PACK_U32);
	    iTmp = TXMODE;
	    rM2M_Pack(aData, 8, iTmp,  RM2M_PACK_BE + RM2M_PACK_U8);
	    iSize = CFG_BASIC_SIZE;
	    print("created new Config #0\r\n");
	}
	else
	{
	    rM2M_Pack( aData, 0, iTmp,  RM2M_PACK_BE + RM2M_PACK_U32 + RM2M_PACK_GET);
	    if( iTmp != iRecItv )
	    {
		printf( "iRecItv changed to %d s\r\n", iTmp );
		iRecItv = iTmp;
	    }

	    rM2M_Pack( aData, 4, iTmp,  RM2M_PACK_BE + RM2M_PACK_U32 + RM2M_PACK_GET);
	    if( iTmp != iTxItv )
	    {
		printf( "iTxItv changed to %d s\r\n", iTmp );
		iTxItv = iTmp;
	    }

	    rM2M_Pack( aData, 8, iTmp,  RM2M_PACK_BE + RM2M_PACK_U8 + RM2M_PACK_GET );
	    if( iTmp != iTxMode )
	    {
		printf( "iTxMode changed to %d\r\n", iTmp );
		iTxMode = iTmp;
		rM2M_TxSetMode( iTxMode );
	    }
	}
    }
    else if( cfg == 2 )
    {
	rM2M_CfgRead( cfg,  0, receipt_uuid, 48 );
	rM2M_CfgRead( cfg, 48, receipt_sync, 16 );	    

	new index;
	new offset;
	for( index = 0; index < 60; index++ )
	{
	    offset = (index * 60) + 64;
	    rM2M_CfgRead( cfg, offset, receipt_data[ index ], 60 );
	}

	receipt_uuid{ 47 } = '\0';
	receipt_sync{ 15 } = '\0';

	printf
	( "uuid = %s\r\nsync = %s\r\n"
	, receipt_uuid
	, receipt_sync
	);

	if( currentLine > 400 )
	{
	    currentLine = -1;
	}
    }
}




const DISPLAY_SPI_INTERFACE = 0;

const DISPLAY_SPI_CHIP_SELECT = 0;
const DISPLAY_SPI_DATA_COMMAND = 1;
const DISPLAY_SPI_RESET = 2;
const DISPLAY_SPI_BUSY_FLAG = 3;


spi_init()
{
    new status;
    status = rM2M_SpiInit( DISPLAY_SPI_INTERFACE, 12000000, RM2M_SPI_CLKPOL );
    if( status != OK )
    {
	printf( "spi: init (status %d)\r\n", status );
    }
}

spi_send( buffer{}, length )
{
    new status;
    new index;
    for( index = 0; index < length; index++ )
    {
	status = rM2M_SpiCom( DISPLAY_SPI_INTERFACE, buffer, length, 0 );
	if( status != OK )
	{
	    printf( "spi: send data length %d (status %d)\r\n", length, status );
	    break;
	}
    }
}



const EPD_WIDTH = 640;
const EPD_HEIGHT = 384;
const EDP_DATA_LENGTH = (640 / 2) * 384; // := 122880

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


/* const Color : { */
/*     BLACK = 0, */
/*     GRAY, */
/*     WHITE, */
/* } */

epd_init()
{
    spi_init();
    
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
    
    epd_framebuffer_clear();
    /* epd_framebuffer_swap(); */
    /* epd_framebuffer_clear(); */
}

/* epd_framebuffer_swap() */
/* { */
/*     edp_current = ( edp_current + 1 ) % 2; */
/* } */

epd_framebuffer_clear()
{
    /* new index; */
    /* for( index = 0; index < 1024; index++ ) */
    /* { */
    /* 	epd_framebuffer[ index ] = 0; */
    /* } */
}


Display_Enable()
{
    
    rM2M_GpioSet( DISPLAY_SPI_CHIP_SELECT, 0 ); // low (enable)
}

Display_Disable()
{
    rM2M_GpioSet( DISPLAY_SPI_CHIP_SELECT, 1 ); // high (disable)
}


Display_SendCommand( const command )
{
    // printf( "display: c = %d\r\n", command );
    rM2M_GpioSet( DISPLAY_SPI_DATA_COMMAND, 0 ); // low (0) command
    new buffer{1};
    buffer{0} = command;
    spi_send( buffer, 1 );
}

Display_SendData( data{}, const length )
{
    // printf( "display: d = %d\r\n", data );
    rM2M_GpioSet( DISPLAY_SPI_DATA_COMMAND, 1 ); // high (1) data
    spi_send( data, length );
}

Display_Command( const command, dataBuffer{} = { 0x00 }, const dataLength = 0 )
{
    Display_SendCommand( command );
    Display_SendData( dataBuffer, dataLength );
}


public Display_Init()
{
    epd_init();
}

public Display_Powerup()
{
    Display_Enable();
    Display_Command( BOOSTER_SOFT_START, { 0xc7, 0xcc, 0x28 }, 3 );
    Display_Command( POWER_SETTING, { 0x37, 0x00, 0x08, 0x08 }, 4 );
    Display_Command( POWER_ON );
}

public Display_Setup()
{
    Display_Command( PANEL_SETTING, { 0xcf, 0x00 }, 2 );    
    Display_Command( PLL_CONTROL, { 0x3a }, 1 );
    Display_Command( TCON_RESOLUTION, { 0x02, 0x80, 0x01, 0x80 }, 4 );
    Display_Command( VCM_DC_SETTING, { 0x1e }, 1 ); //decide by LUT file
    Display_Command( VCOM_AND_DATA_INTERVAL_SETTING, { 0x07 }, 1 );
    Display_Command( FLASH_MODE, { 0x03 }, 1 ); //FLASH MODE

    Display_SendCommand( DATA_START_TRANSMISSION_1 );
}

public Display_Refresh()
{
    Display_SendCommand( DATA_STOP );    
    Display_SendCommand( DISPLAY_REFRESH );
    
    Display_SendCommand( DATA_START_TRANSMISSION_1 );
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

public Display_Task()
{
    if( currentLine < 500 )
    {
	currentLine = currentLine + 1;
    }
    
    if( currentLine == -20 )
    {
	Display_Powerup();
    }
    if( currentLine == -10 )
    {
	Display_Setup();
    }
    
    if( currentLine >= 0 && currentLine < 60 )
    {
    	Display_RenderLineData( currentLine );
    }
    if( currentLine == 60 )
    {
    	Display_Refresh();
	currentLine = 400;
    }

    if( currentLine >= 200 && currentLine < 230 )
    {
    	Display_RenderLineColor( 0x00 );
    }
    if( currentLine == 230 )
    {
    	Display_Refresh();
    }    
}

Display_RenderLineData( line )
{
    new index;
    new buffer{ 1 };
    for( index = 0; index < 640; index += 2 )
    {
	if( index >= 60 )
	{
	    buffer{ 0 } = 0x00;
	    Display_SendData( buffer, 1 );
	    continue;
	}

	new pixel = 0x00;
	new pixel0 = receipt_data[ line ]{ index };
	new pixel1 = receipt_data[ line ]{ index+1 };
	    
	if( pixel0 == 'w' )
	{
	    pixel = 0x00 | (0x0f & pixel);
	}
	if( pixel0 == 'b' )
	{
	    pixel = 0x30 | (0x0f & pixel);
	}
	if( pixel1 == 'w' )
	{
	    pixel = 0x00 | (0xf0 & pixel);
	}
	if( pixel1 == 'b' )
	{
	    pixel = 0x03 | (0xf0 & pixel);
	}
	    
	buffer{ 0 } = pixel;	
	Display_SendData( buffer, 1 );
    }
}

Display_RenderLineColor( color )
{
    new index;
    new buffer{ 1 };
    for( index = 0; index < EPD_WIDTH; index++ )
    {
	buffer{ 0 } = color;	
	Display_SendData( buffer, 1 );
    }
}


//
//  Local variables:
//  mode: c-mode
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
