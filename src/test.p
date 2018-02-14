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

/* #pragma amxram 1310720 */
/* #pragma dynamic  10000 */


#include "filetransfer"
#include "sdk/iotbox"
#include "rM2M"

#include "display"

native getapilevel();
native CRC32(data{}, len, initial=0);

// native W5200_Socket(s, iProtocol, iPort, iFlag);


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


/* forward public FileCmd( id, cmd, const data{}, len, ofs ); */

/* /\* static cFileId; *\/ */
/* static cFileLen; */
/* /\* static cFileBufferTx{4096}; *\/ */
/* /\* static cUartLen; *\/ */

/* static srM2MFile[ 3 ][ TFT_Info ] = */
/*     [ [ "stdin" */
/*       , 0 */
/*       , 0 */
/*       , 0 */
/*       , 0 */
/*       , FT_FLAG_READ */
/*       ] */
/*     , [ "stdout" */
/*       , 0 */
/*       , 0 */
/*       , 0 */
/*       , 0 */
/*       , FT_FLAG_WRITE */
/*       ] */
/*     , [ "stderr" */
/*       , 0 */
/*       , 0 */
/*       , 0 */
/*       , 0 */
/*       , FT_FLAG_WRITE */
/*       ] */
/*     ]; */


/* static srM2MData[3]{10240} = */
/*     [ */
/* 	{}, */
/* 	{}, */
/* 	{} */
/* 	]; */

/* static aFileCmd[]{} = */
/*     [ */
/* 	{"NONE"}, */
/* 	{"UNLOCK"}, */
/* 	{"LIST"}, */
/* 	{"READ"}, */
/* 	{"STORE"}, */
/* 	{"WRITE"}, */
/* 	{"DELETE"} */
/* 	]; */


/* public FileCmd( id, cmd, const data{}, len, ofs ) */
/* { */
/*     new cFileBuffer{4096}; */
 
/*     printf( "FileCmd(%s, %s, data{}, %d Bytes, Offset %d)\r\n", srM2MFile[id].name, aFileCmd[cmd], len, ofs ); */

/*     switch( cmd ) */
/*     { */
/*         case FT_CMD_NONE: */
/* 	{ */
/* 	} */
/*         case FT_CMD_UNLOCK: */
/*         { */
/* 	    if( srM2MFile[ id ].flags == FT_FLAG_READ ) */
/* 	    { */
/* 		srM2MFile[ id ].size = 0; */
/* 		srM2MFile[ id ].stamp = 0; */
/* 	    } */
/*         } */
/*         case FT_CMD_LIST: */
/*         { */
/*             FT_SetProps */
/*             ( id */
/*             , srM2MFile[ id ].stamp */
/*             , srM2MFile[ id ].size */
/*             , CRC32 */
/*               ( srM2MData[ id ] */
/*               , srM2MFile[ id ].size */
/*               ) */
/*             , srM2MFile[ id ].flags */
/*             ); */
/*         } */
/*         case FT_CMD_READ: */
/*         { */
/*             rM2M_GetPackedB */
/*             ( srM2MData[ id ] */
/*             , ofs */
/*             , cFileBuffer */
/*             , len */
/*             ); */

/*             FT_Read */
/*             ( id */
/*             , cFileBuffer */
/*             , cFileLen */
/*             ); */

/*             //printf( "Read: %s\r\n", cFileBuffer ); */
/*         } */
/*         case FT_CMD_STORE: */
/*         { */
/* 	    FT_Accept( id ); */
/*         } */
/*         case FT_CMD_WRITE: */
/*         { */
/*             if( ( ofs + len ) > 10240 ) */
/*             { */
/*                 FT_Error(id); */
/*                 return; */
/*             } */

/*             rM2M_SetPackedB */
/*             ( srM2MData[id] */
/*             , ofs */
/*             , data */
/*             , len */
/*             ); */

/*             rM2M_SetPackedB */
/*             ( cFileBuffer */
/*             , 0 */
/*             , data */
/*             , len */
/*             ); */

/*             srM2MFile[ id ].size = ofs + len; */
/*             /\* cUartLen = len; *\/ */
/*             /\* cFileId = id; *\/ */
/*             /\* cFileLen = len; *\/ */
/*             /\* cFileBufferTx = cFileBuffer; *\/ */

/*             // printf( "Written: %s\r\n", cFileBuffer ); */
/*         } */
/*         case FT_CMD_DELETE: */
/*         { */
/* 	    FT_Unregister( id ); */
/*         } */
/*     } */
/* } */


forward public Handle_SMS( const SmsTel[], const SmsText[] );

forward public Display_Task();

main()
{
    Led_Init( LED_MODE_SCRIPT );
    Switch_Init( SWITCH_MODE_SCRIPT, funcidx( "KeyChanged" ) );
    Display_Init();

    new timeHour;
    new timeMinute;
    new timeSecond;
    new timeStamp;
    timeStamp = rM2M_GetTime( timeHour, timeMinute, timeSecond );
    printf( "time: %d:%d:%d | %d\r\n", timeHour, timeMinute, timeSecond, timeStamp );

    
    /* new i; */
    /* for( i = 0; i < 3; i++ ) */
    /* { */
    /* 	FT_Register */
    /*     ( srM2MFile[i].name */
    /* 	, i */
    /* 	, funcidx( "FileCmd" ) */
    /* 	); */

    /* 	FT_SetProps */
    /* 	( i */
    /* 	, srM2MFile[i].stamp */
    /* 	, srM2MFile[i].size */
    /* 	, srM2MFile[i].crc */
    /* 	, srM2MFile[i].flags */
    /* 	); */
    /* } */
    
    /* rM2M_GpioDir( 0, RM2M_GPIO_OUTPUT ); */
    /* rM2M_GpioSet( 0, 1 ); */
    /* rM2M_GpioDir( 1, RM2M_GPIO_OUTPUT ); */
    /* rM2M_GpioSet( 1, 0 ); */
    /* rM2M_GpioDir( 2, RM2M_GPIO_OUTPUT ); */
    /* rM2M_GpioSet( 2, 0 ); */
    /* rM2M_GpioDir( 3, RM2M_GPIO_OUTPUT ); */
    /* rM2M_GpioSet( 3, 0 ); */
    /* rM2M_GpioDir( 4, RM2M_GPIO_OUTPUT ); */
    /* rM2M_GpioSet( 4, 1 ); */
    /* rM2M_GpioDir( 5, RM2M_GPIO_OUTPUT ); */
    /* rM2M_GpioSet( 5, 0 ); */

    ReadConfig( CFG_BASIC_INDEX );

    iTxTimer  = 0;
    iRecTimer = 0;
    rM2M_TxSetMode( iTxMode );
    rM2M_CfgOnChg( funcidx( "ReadConfig" ) );
    rM2M_SmsInit( funcidx( "Handle_SMS" ), 0 );
    rM2M_TimerAdd( funcidx( "Timer1s" ) );
    rM2M_TimerAddExt( funcidx( "Display_Task" ), true, 10 );
}


static uptime = -2;

public Timer1s()
{
    Handle_Led();
    Handle_Transmission();
    Handle_Record();

    printf( "api %i, levelsignal strength %i\r\n", getapilevel(), rM2M_GSMGetRSSI() );
}

public Display_Task()
{    
    if( uptime == -2 )
    {
	Display_Powerup();
    }
    if( uptime == -1 )
    {
	Display_Setup();
    }
    if( uptime >= 0 && uptime < 192 )
    {
    	Display_TestImage();
    }
    if( uptime == 192 )
    {
    	Display_Refresh();
    }
    // Display_Sleep();

    uptime = uptime + 1;
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
    iTxTimer--;
    if( iTxTimer <= 0 )
    {
	rM2M_TxStart();
	iTxTimer = iTxItv;
    }
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

public KeyChanged(iKeyState)
{
    // iKeyState (0=release, 1=press)
    //printf("K:%d\r\n", iKeyState);
    
    if( !iKeyState )
    {
	iTxTimer = 0;
    }
}

const ITV_RECORD        = 1 * 60;               // Record interval [sec.], default 1 min
const ITV_TRANSMISSION  = 1 * 60 * 60;          // Transmission interval [sec.], default 60 min

public ReadConfig(cfg)
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
}
