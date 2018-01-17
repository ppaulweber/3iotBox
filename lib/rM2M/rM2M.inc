#include <float>

#pragma library rM2M

/**
 * <summary>
 *   General purpose return codes.
 * </summary>
 */
const
{
  OK = 0,
  ERROR = -1,
  ERROR_PARAM = -2,               // Parameter error
  ERROR_UNKNOWN_HDL = -3,         // Unknown handler, handle or resource error
  ERROR_ALREADY_SUBSCRIBED = -4,  // Already subscribed service or resource error
  ERROR_NOT_SUBSCRIBED = -5,      // Not subscribed service error
  ERROR_FATAL = -6,               // Fatal error
  ERROR_BAD_HDL = -7,             // Bad handle or resource error
  ERROR_BAD_STATE = -8,           // Bad state error
  ERROR_PIN_KO = -9,              // Bad PIN state error
  ERROR_NO_MORE_HANDLES = -10,    // The service subscription maximum capacity is reached
  ERROR_DONE = -11,               // The required iterative process is now terminated
  ERROR_OVERFLOW = -12,           // The required operation has exceeded the function capabilities
  ERROR_NOT_SUPPORTED = -13,      // An option, required by the function, is not enabled on the CPU, the function is not supported in this configuration
  ERROR_NO_MORE_TIMERS = -14,     // The function requires a timer subscription, but no more timer resources are available
  ERROR_NO_MORE_SEMAPHORES = -15, // The function requires a semaphore allocation, but there are no more semaphores resources
  ERROR_SERVICE_LOCKED = -16,     // The function was called from a low or high lewel interrupt handler (the function is forbidden in this case)
  ERROR_MEM = -100,               // error allocating memory
  ERROR_SIM_STATE = -101,         // SIM state error
  ERROR_MODEM_DISABLED = -102     // Modem disabled
  ERROR_FEATURE_LOCKED = -103,    // feature locked
  ERROR_TXITF = -104,             // tx interface (uplink) not available (e.g. not opened, currenly closing)
};

/**
 * <summary>
 *   Identification structure.
 * </summary>
 */
#define TrM2M_Id[.string{50}, .module{10}, .hwmajor, .hwminor, .sn{8}, .fwmajor, .fwminor, .ctx{50}]

/**
 * <summary>
 *   GSM/UMTS/LTE position structure.
 *   .ta (timing advance param) is only valid for GSM cells (0 for UMTS/LTE)
 * </summary>
 */
#define TrM2M_GSMPos [.mcc, .mnc, .lac, .cellid, .rssi, .ta]

/**
 * <summary>
 *   States for managed SIM
 * </summary>
 */
const
{
  RM2M_SIM_STATE_NONE = 0,
  RM2M_SIM_STATE_PRODUCTION,
  RM2M_SIM_STATE_HOT,
  RM2M_SIM_STATE_COLD,
  RM2M_SIM_STATE_DISCARDED,
};

/**
 * <summary>
 *   GSM info structure.
 * </summary>
 */
#define TrM2M_GSMInfo[.cgmi{20}, .cgmm{20}, .cgmr{20}, .imei{16}, .imsi{16}, .iccid{21}, .mcc, .mnc, .simstate]

/**
 * <summary>
 *   Retrieve identification of module.
 * </summary>
 *
 * <param name="id">
 *   Structure where identification is filled in.
 * </param>
 * <param name="len">
 *   length of id Structure, max size according to TrM2M_Id
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li>number of filled cells for id struct</li>
 *     <li><em>ERROR</em> - if address/length pair for id struct is invalid.</li>
 *   </ul>
 * </returns>
 */
native rM2M_GetId(id[TrM2M_Id], len=sizeof id);

/**
 * <summary>
 *   Reads the actual time and stores it in three separate fields. The time is
 *   represented in UTC.
 * </summary>
 *
 * <param name="hour">
 *   Will hold the hours upon return.
 * </param>
 * <param name="minute">
 *   Will hold the minutes upon return.
 * </param>
 * <param name="second">
 *   Will hold the seconds upon return.
 * </param>
 * <param name="timestamp">
 *   0 = use actual timestamp. Otherwise timestamp provided is used.
 *   The timestamp must be provided in seconds elapsed since 31st Dec 1999.
 * </param>
 *
 * <returns>
 *   The actual timestamp in seconds elapsed since 31st Dec 1999. If timestamp
 *   is provided by user (param <em>timestamp</em> > 0), this timestamp is
 *   retourned.
 * </returns>
 */
native rM2M_GetTime(&hour=0, &minute=0, &second=0, timestamp=0);

/**
 * <summary>
 *   Reads the actual date and stores it in three separate fields. The time is
 *   represented in UTC.
 * </summary>
 *
 * <param name="year">
 *   Will hold the year number upon return.
 * </param>
 * <param name="month">
 *   Will hold the month number upon return.
 * </param>
 * <param name="day">
 *   Will hold the day number upon return.
 * </param>
 * <param name="timestamp">
 *   0 = use actual timestamp. Otherwise timestamp provided is used.
 *   The timestamp must be provided in seconds elapsed since 31st Dec 1999.
 * </param>
 *
 * <returns>
 *   The actual timestamp in seconds elapsed since 31st Dec 1999. If timestamp
 *   is provided by user (param <em>timestamp</em> > 0), this timestamp is
 *   retourned.
 * </returns>
 */
native rM2M_GetDate(&year=0, &month=0, &day=0, timestamp=0);

/**
 * <summary>
 *   DateTime structure.
 * </summary>
 * <param name="year">
 *   year (relative to the year 2000, f.e. 14 for 2014).
 * </param>
 * <param name="month">
 *   month.
 * </param>
 * <param name="day">
 *   day.
 * </param>
 * <param name="hour">
 *   hour.
 * </param>
 * <param name="minute">
 *   minute.
 * </param>
 * <param name="second">
 *   second.
 * </param>
 * <param name="DoW">
 *   day of week (where 0 is Monday and 6 is Sunday).
 * </param>
 * <param name="timestamp">
 *   timestamp (The actual timestamp in seconds elapsed since 31st Dec 1999.)
 * </param>
 * <param name="timestamp256">
 *   partial timestamp (resolution 1/256 s).
 * </param>
 */
#define TrM2M_DateTime[.year, .month, .day, .hour, .minute, .second, .DoW, .timestamp, .timestamp256]

/**
 * <summary>
 *   time flags
 * </summary>
 */
const
{
  RM2M_DATETIME_LOCALTIME = 0b00000001, // Local time is used
}

/**
 * <summary>
 *   Reads the actual date and time. The time is
 *   represented in UTC.
 * </summary>
 *
 * <param name="datetime">
 *   Structure where date and time is filled in.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful.
 *   <em>ERROR</em> in case of invalid parameter.
 * </returns>
 */
native rM2M_GetDateTime(datetime[TrM2M_DateTime]);

/**
 * <summary>
 *   Update date and time.
 * </summary>
 *
 * <param name="datetime">
 *   Structure to set datetime (if .timestamp != 0, timestamp is used to set).
 * </param>
 * <param name="flags">
 *   Flags for updating time, see time flags.
 * </param>
 *
 * <returns>
 *   number of seconds different to previous device timestamp if difference is at least
 *   5 seconds, otherwise zero.
 *   <em>ERROR</em> in case of invalid parameter.
 *   <em>ERROR-1</em> in case of stamp is in future.
 * </returns>
 */
native rM2M_SetDateTime(datetime[TrM2M_DateTime], flags=0);

/**
 * <summary>
 *   Reads the stored time-zone offset from the device. The time-zone offset is
 *   the difference between UTC and local time.
 * </summary>
 *
 * <returns>
 *   The actual time-zone offset in seconds.
 * </returns>
 */
native rM2M_GetTimezoneOffset();

/**
 * <summary>
 *   Calculates the weekday from a given timestamp.
 * </summary>
 *
 * <param name="timestamp">
 *   Timestamp to use for calculation.
 * </param>
 *
 * <returns>
 *   The day of the week, where 0 is Monday and 6 is Sunday.
 * </returns>
 */
native rM2M_DoW(timestamp);

/**
 * <summary>
 *   Adds a new 1s timer.
 * </summary>
 *
 * <param name="funcidx">
 *   Function index of timer.
 *   Has to be of type: public func();
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case no valid function
 *   supplied or internal error.
 * </returns>
 */
native rM2M_TimerAdd(funcidx);

/**
 * <summary>
 *   Removes a 1s timer.
 * </summary>
 *
 * <param name="funcidx">
 *   Function index of timer.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case no valid function
 *   supplied or internal error.
 * </returns>
 */
native rM2M_TimerRemove(funcidx);

/**
 * <summary>
 *   Adds a new millisecond timer.
 * </summary>
 *
 * <param name="funcidx">
 *   Function index of timer.
 *   Has to be of type: public func();
 * </param>
 * <param name="cyclic">
 *   <em>TRUE</em> if timer should be reloaded on expiration, <em>FALSE</em> otherwise.
 * </param>
 * <param name="time">
 *   Time in milliseconds the timer should run.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case no valid function
 *   supplied or internal error.
 * </returns>
 */
native rM2M_TimerAddExt(funcidx, bool:cyclic, time);

/**
 * <summary>
 *   Removes a millisecond timer.
 * </summary>
 *
 * <param name="funcidx">
 *   Function index of timer.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case no valid function
 *   supplied or internal error.
 * </returns>
 */
native rM2M_TimerRemoveExt(funcidx);

/**
 * <summary>
 *   Gpio direction used with rM2M_GpioDir().
 * </summary>
 */
const
{
  RM2M_GPIO_INPUT = 0, // input direction
  RM2M_GPIO_OUTPUT,    // output direction
};

/**
 * <summary>
 *   Set direction of Gpio pin.
 * </summary>
 *
 * <param name="gpio">
 *   Gpio pin number.
 * </param>
 * <param name="dir">
 *   <em>0</em> to set as input. <em>1</em> to set as output.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise &lt; <em>OK</em> (see General purpose return codes).
 * </returns>
 */
native rM2M_GpioDir(gpio, dir);

/**
 * <summary>
 *   Gpio level used with rM2M_GpioSet() & rM2M_GpioGet().
 * </summary>
 */
const
{
  RM2M_GPIO_LOW = 0, // low level
  RM2M_GPIO_HIGH,    // high level
};

/**
 * <summary>
 *   Set the output level of Gpio pin.
 * </summary>
 *
 * <param name="gpio">
 *   Gpio pin number.
 * </param>
 * <param name="level">
 *   <em>0</em> to set as LOW. <em>1</em> to set as HIGH.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR_BAD_STATE</em> if the required Gpio pin
 *   was not subscribed as an output.
 *   All other return values &lt; <em>OK</em> are internal errors (see General
 *   purpose return codes)
 * </returns>
 */
native rM2M_GpioSet(gpio, level);

/**
 * <summary>
 *   Get the input level of Gpio pin.
 * </summary>
 *
 * <param name="gpio">
 *   Gpio pin number.
 * </param>
 *
 * <returns>
 *   Current GPIO level if successful, <em>ERROR_BAD_STATE</em> if the required Gpio pin
 *   was not subscribed as an input.
 *   All other return values &lt; <em>OK</em> are internal errors (see General
 *   purpose return codes)
 * </returns>
 */
native rM2M_GpioGet(gpio);

/**
 * <summary>
 *   Spi configuration bits used with rM2M_SpiInit().
 * </summary>
 */
const
{
  RM2M_SPI_CLKPOL   = 0b00000001, // clock polarity bit
  RM2M_SPI_CLKPHA   = 0b00000010, // clock phase bit
};

/**
 * <summary>
 *   Initialise Spi interface.
 * </summary>
 *
 * <param name="spi">
 *   Spi interface number.
 * </param>
 * <param name="clock">
 *   Spi clock [Hz].
 * </param>
 * <param name="config">
 *   Clock polarity and phase configuration.
 *   Bit0 - clock polarity (0 = idle low, 1 = idle high).
 *   Bit1 - clock phase (0 = data shift on leading edge, 1 = data latch on leading edge).
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>OK</em> - if successful.</li>
 *     <li><em>ERROR</em> - if Spi interface number is not valid.</li>
 *     <li><em>ERROR_ALREADY_SUBSCRIBED</em> - if Spi interface is already initialized</li>
 *   </ul>
 *   All other return values &lt; <em>OK</em> are internal errors (see General
 *   purpose return codes)
 * </returns>
 */
native rM2M_SpiInit(spi, clock, config);

/**
 * <summary>
 *   Close Spi interface.
 * </summary>
 *
 * <param name="spi">
 *   Spi interface number.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> if Spi interface number is not valid.
 * </returns>
 */
native rM2M_SpiClose(spi);

/**
 * <summary>
 *   Performs asynchronous Spi communication. First data is sent
 *   afterwards data is received.
 * </summary>
 *
 * <param name="spi">
 *   Spi interface number.
 * </param>
 * <param name="data">
 *   Array of data to send and receive.
 * </param>
 * <param name="txlen">
 *   Number of bytes to send.
 * </param>
 * <param name="rxlen">
 *   Number of bytes to receive.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise &lt; <em>OK</em> (see General purpose return codes).
 * </returns>
 */
native rM2M_SpiCom(spi, data{}, txlen, rxlen);

/**
 * <summary>
 *   Initialise I2c interface.
 * </summary>
 *
 * <param name="i2c">
 *   I2c interface number.
 * </param>
 * <param name="clock">
 *   I2c clock [Hz].
 * </param>
 * <param name="config">
 *   Reserved for future use.
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>OK</em> - if successful.</li>
 *     <li><em>ERROR</em> - if I2c interface number is not valid.</li>
 *     <li><em>ERROR_ALREADY_SUBSCRIBED</em> - if I2c interface is already initialized</li>
 *   </ul>
 * </returns>
 */
native rM2M_I2cInit(i2c, clock, config);

/**
 * <summary>
 *   Close I2c interface.
 * </summary>
 *
 * <param name="i2c">
 *   I2c interface number.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> if I2c interface number is not valid.
 * </returns>
 */
native rM2M_I2cClose(i2c);

/**
 * <summary>
 *   Performs I2c communication.
 * </summary>
 *
 * <param name="i2c">
 *   I2c interface number.
 * </param>
 * <param name="adr">
 *   I2c address (Bit7-Bit1, Bit0 unused).
 * </param>
 * <param name="data">
 *   Array of data to send and receive.
 * </param>
 * <param name="txlen">
 *   Number of bytes to send.
 * </param>
 * <param name="rxlen">
 *   Number of bytes to receive.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise &lt; <em>OK</em> (see General purpose return codes).
 * </returns>
 */
native rM2M_I2cCom(i2c, adr, data{}, txlen, rxlen);

/**
 * <summary>
 *   Uart configuration bits used with rM2M_UartInit().
 * </summary>
 */
const
{
  RM2M_UART_1_STOPBIT   = 0b000000001, // 1 stop bit
  RM2M_UART_2_STOPBIT   = 0b000000010, // 2 stop bits
  RM2M_UART_PARITY_NONE = 0b000000000, // no parity
  RM2M_UART_PARITY_ODD  = 0b000000100, // odd parity
  RM2M_UART_PARITY_EVEN = 0b000001000, // even parity
  RM2M_UART_7_DATABIT   = 0b000000000, // 7 data bits
  RM2M_UART_8_DATABIT   = 0b000010000, // 8 data bits
  RM2M_UART_FLOW_NONE   = 0b000000000, // no flow control
  RM2M_UART_FLOW_RTSCTS = 0b001000000, // RTS/CTS handshake
  RM2M_UART_FULL_DUPLEX = 0b000000000, // full duplex mode
  RM2M_UART_HALF_DUPLEX = 0b100000000, // half duplex mode
};

/**
 * <summary>
 *   Initialise Uart interface.
 * </summary>
 *
 * <param name="uart">
 *   Uart interface number.
 * </param>
 * <param name="baudrate">
 *   Baudrate.
 * </param>
 * <param name="mode">
 *   Bit 0..1
 *     1 = 1 stop bit
 *     2 = 2 stop bit
 *   Bit 2..3
 *     0 = no parity
 *     1 = odd parity
 *     2 = even parity
 *   Bit 4..5
 *     0 = 7 data bits
 *     1 = 8 data bits
 *   Bit 6..7
 *     0 = no flow control
 *     1 = RTS/CTS handshake
 *   Bit 8
 *     0 = full duplex mode
 *     1 = half duplex mode
 * </param>
 * <param name="funcidx">
 *   Index of Uart receive function.
 *   Has to be of type: public func(const data[], len);
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_UartInit(uart, baudrate, mode, funcidx);

/**
 * <summary>
 *   Close Uart interface.
 * </summary>
 *
 * <param name="uart">
 *   Uart interface number.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_UartClose(uart);

/**
 * <summary>
 *   Send data over Uart interface.
 * </summary>
 *
 * <param name="uart">
 *   Uart interface number.
 * </param>
 * <param name="data">
 *   Array of data to send.
 * </param>
 * <param name="len">
 *   Number of bytes to send.
 * </param>
 *
 * <returns>
 *   number of consumed bytes if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_UartWrite(uart, const data{}, len);

/**
 * <summary>
 *   Irq configuration values used with rM2M_IrqInit().
 * </summary>
 */
const
{
  RM2M_IRQ_RISING  = 0, // rising edge
  RM2M_IRQ_FALLING,     // falling edge
};

/**
 * <summary>
 *   Start interrupt functionality.
 * </summary>
 *
 * <param name="irq">
 *   Irq pin number.
 * </param>
 * <param name="config">
 *   0 = irq on rising edge.
 *   1 = irq on falling edge.
 * </param>
 * <param name="funcidx">
 *   Index of interrupt function.
 *   Has to be of type: public func(pinstate);
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>OK</em> - if successful.</li>
 *     <li><em>ERROR</em>:
 *       <ul>
 *         <li>Interrupt functionality not supported for this pin</li>
 *         <li>interrupt function is not public</li>
 *       </ul>
 *     </li>
 *     <li><em>ERROR_ALREADY_SUBSCRIBED</em> - if irq is already initialized for this pin</li>
 *   </ul>
 *   All other return values &lt; <em>OK</em> are internal errors (see General
 *   purpose return codes)
 * </returns>
 */
native rM2M_IrqInit(irq, config, funcidx);

/**
 * <summary>
 *   Stops interrupt functionality.
 * </summary>
 *
 * <param name="irq">
 *   Irq pin number.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_IrqClose(irq);

/**
 * <summary>
 *   rM2M_TxStart() flags.
 * </summary>
 */
const
{
  RM2M_TX_POSUPDATE          = 0b00000001, // position update
  RM2M_TX_SERVICE            = 0b00000010, // service connection
  RM2M_TX_REFRESH_CONFIG     = 0b00000100, // refresh configuration
  RM2M_TX_SUPPRESS_POSUPDATE = 0b00001000, // suppress position update
};

/**
 * <summary>
 *   Initiate a connection to server.
 * </summary>
 *
 * <param name="flags">
 *   Bit 0 - update GSM position if set.
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>OK</em> - if successful</li>
 *     <li><em>ERROR_SIM_STATE</em> - connection currently not possible due to managed SIM state, see TrM2M_GSMInfo</li>
 *     <li><em>ERROR_MODEM_DISABLED</em> - connection currently not possible (e.g. too low voltage)</li>    
 *     <li><em>ERROR_TXITF</em> - connection currently not possible due to tx interface (e.g. not opened)
 *     <li><em>ERROR</em> - if any other error occured</li>
 *   </ul>
 * </returns>
 */
native rM2M_TxStart(flags=0);

/**
 * <summary>
 *   Communication modes used with rM2M_TxSetMode().
 * </summary>
 */
const
{
  RM2M_TXMODE_TRIG = 0, // triggered
  RM2M_TXMODE_WAKEUP,   // wakeup
  RM2M_TXMODE_ONLINE,   // online
};

/**
 * <summary>
 *   Control flags used with rM2M_TxSetMode().
 * </summary>
 */
const
{
  RM2M_TXMODE_SUPPRESS_SYNC = 0b00000001,  // no automatic server sync on mode change
};

/**
 * <summary>
 *   Set communication mode.
 * </summary>
 *
 * <param name="mode">
 *   0 = triggered (interval mode), connection to server on each rM2M_TxStart().
 *   1 = wakeup, same es previous but device is wakeupable through server.
 *   2 = online, device keeps connection with server.
 * </param>
 * <param name="flags">
 *   control flags for communication mode
 *   bit 0 = no automatic server sync if mode is changed
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>OK</em> - if successful</li>
 *     <li><em>ERROR_SIM_STATE</em> - mode currently not possible due to managed SIM state, see TrM2M_GSMInfo</li>
 *     <li><em>ERROR_MODEM_DISABLED</em> - mode currently not possible (e.g. too low voltage)</li>
 *     <li><em>ERROR_TXITF</em> - mode currently not possible due to tx interface (e.g. not opened)
 *     <li><em>ERROR</em> - if any other error occured</li>
 *   </ul>
 * </returns>
 */
native rM2M_TxSetMode(mode, flags=0);

/**
 * <summary>
 *   Connection status flags used with rM2M_TxGetStatus().
 * </summary>
 */
const
{
  RM2M_TX_FAILED         = 0b000000001, // failed connection
  RM2M_TX_ACTIVE         = 0b000000010, // active connection
  RM2M_TX_STARTED        = 0b000000100, // started connection
  RM2M_TX_RETRY          = 0b000001000, // automatic connection retry in progress
  RM2M_TX_WAKEUPABLE     = 0b000010000, // connection in wakeupable state
  RM2M_TX_EXTSIM         = 0b000100000, // external SIM active
  RM2M_TX_DISABLED       = 0b001000000, // modem is disabled
  RM2M_TX_SERVICE_ACTIVE = 0b010000000, // service connection is active
  RM2M_TX_WAKEUP         = 0b100000000, // connection triggered from wakeup SMS
};

/**
 * <summary>
 *   Connection errorcodes used with rM2M_TxGetStatus().
 * </summary>
 */
const
{
  /**< General errors */
  RM2M_TXERR_NONE = 0,
  RM2M_TXERR_CONNECTION_TIMEOUT,        // connection timed out
  RM2M_TXERR_NEWDATA_TIMEOUT,           // timeout during server sync in online mode
  RM2M_TXERR_IRREGULAR_OFF,             // irregular closed connection
  RM2M_TXERR_SERVER_NOT_AVAILABLE,      // server not available
  RM2M_TXERR_SERVER_COMMUNICATION,      // error during communication with server

  /**< General modem errors */
  RM2M_TXERR_MODEM = 10,                // unspecified modem error
  RM2M_TXERR_MODEM_TIMEOUT,             // timeout modem communication
  RM2M_TXERR_MODEM_HW_NOT_FOUND,        // modem not found
  RM2M_TXERR_MODEM_HW_UNKNOWN,          // unknown modem
  RM2M_TXERR_MODEM_INIT,                // error during init
  RM2M_TXERR_MODEM_UNS_RESTART,         // unsolicited restart (e.g. due to weak power supply)

  /**< SIM related errors  */
  RM2M_TXERR_MODEM_SIM = 30,
  RM2M_TXERR_MODEM_SIM_NO_ATTEMPT,      // only one remaining pin input attempt
  RM2M_TXERR_MODEM_SIM_PIN_WRONG,       // pin code is wrong
  RM2M_TXERR_MODEM_SIM_NO_PIN,          // pin code required but not available
  RM2M_TXERR_MODEM_EXTSIM_DENIED,       // external SIM not allowed (APN and/or feature key)
  RM2M_TXERR_MODEM_EXTSIM_MISSING,      // external SIM not found
  RM2M_TXERR_MODEM_SIM_OTHER,           // any other problem with SIM card (e.g. internal SIM not found)

  /**< Network related errors (GSM, GPRS, PDP, etc.) */
  RM2M_TXERR_MODEM_NETWORK = 50,
  RM2M_TXERR_MODEM_GSM_BAND_SEL,        // GSM not available (e.g. error with antenna)
  RM2M_TXERR_MODEM_NETLOCK,             // error registering within network (e.g. not allowed)
  RM2M_TXERR_MODEM_POSUPDATE,           // error with GSM position update
  RM2M_TXERR_MODEM_PDP_CTX,             // error activating PDP context

  /**< TCP related modem errors */
  RM2M_TXERR_MODEM_TCP = 70,            // TCP error (e.g. timeout, server not available)
  
  /**< General WiFi errors */
  RM2M_TXERR_WIFI = 200,                // unspecified WiFi error
  RM2M_TXERR_WIFI_TIMEOUT,              // timeout WiFi communication
  RM2M_TXERR_WIFI_HW_NOT_FOUND,         // WiFi device not found
  RM2M_TXERR_WIFI_INIT,                 // error during init
  RM2M_TXERR_WIFI_IO,                   // error IO communication
  
  /**< Network related WiFi errors */
  RM2M_TXERR_WIFI_NETWORK = 220,
  RM2M_TXERR_WIFI_NETWORK_TIMEOUT,      // timeout accessing network
  RM2M_TXERR_WIFI_AP_SCAN_TIMEOUT,      // timeout scanning for available access points
  RM2M_TXERR_WIFI_AP_SCAN,              // error scanning access points (e.g. currently not possible)
  RM2M_TXERR_WIFI_DHCP_TIMEOUT,         // timeout receiving IP address from DHCP server
  RM2M_TXERR_WIFI_AP_SETTINGS,          // access point settings not plausible
  RM2M_TXERR_WIFI_AP_CONNECT,           // error connecting to access point
  RM2M_TXERR_WIFI_AP_NOT_FOUND,         // access point not found during scan
  
  /**< TCP related WiFi errors */
  RM2M_TXERR_WIFI_TCP = 240,            
  RM2M_TXERR_WIFI_TCP_OPEN_TO,          // timeout opening TCP connection
  RM2M_TXERR_WIFI_TCP_SEND_TO,          // timeout sending data
  RM2M_TXERR_WIFI_TCP_CONNECT,          // error connecting to server
  RM2M_TXERR_WIFI_TCP_FAILED,           // other error concerning TCP connection */
};

/**
 * <summary>
 *   Get actual connection status.
 * </summary>
 *
 * <param name="errorcode">
 *   Error code of last failed connection attempt.
 * </param>
 *
 * <returns>
 *   Bit 0 - if set, last connection failed.
 *   Bit 1 - if set, connection is active.
 *   Bit 2 - if set, connection was started.
 *   Bit 3 - if set, automatic connection retry is in progress.
 *   Bit 4 - if set, connection is in wakeupable state.
 *   Bit 5 - if set, external SIM is active.
 *   Bit 6 - if set, modem is disabled
 *   Bit 7 - if set, service connection is active.
 *   Bit 8 - if set, connection is triggered from wakeup SMS.
 * </returns>
 */
native rM2M_TxGetStatus(&errorcode=0);

/**
 * <summary>
 *   Uplink interfaces used with rM2M_TxSelectItf().
 * </summary>
 */
const
{
  RM2M_TXITF_NONE = 0,
  RM2M_TXITF_MODEM,
  RM2M_TXITF_WIFI,
};

/**
 * <summary>
 *   Select communication interface for uplink.
 * </summary>
 *
 * <param name="itf">
 *   selected hardware interface
 *   0 = none, no uplink interface, server communication not possible
 *   1 = modem 
 *   2 = WiFi
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>OK</em> - if successful</li>
 *     <li><em>ERROR</em> - if hardware interface not supported or any other error occured</li>
 *   </ul>
 * </returns>
 */
native rM2M_TxSelectItf(itf);

/**
 * <summary>
 *   Get GSM/UMTS/LTE signal strength.
 * </summary>
 *
 * <returns>
 *   Signal strength as [dBm] in range -128 .. 127.
 *   GSM values are in range -113 .. -51 dBm.
 * </returns>
 */
native rM2M_GSMGetRSSI();

/**
 * <summary>
 *   Record data.
 * </summary>
 *
 * <param name="timestamp">
 *   0 = use actual timestamp. Otherwise timestamp provided is used.
 *   The timestamp must be provided in seconds elapsed since 31st Dec 1999.
 * </param>
 * <param name="data">
 *   Array of data to record.
 * </param>
 * <param name="len">
 *   Number of bytes to record.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise values &lt; <em>OK</em>.
 *   <em>-2</em> if data recording is temporary not possible.
 *   <em>ERROR</em> if any other error occured.
 * </returns>
 */
native rM2M_RecData(timestamp, const data{}, len);

/**
 * <summary>
 *   Send live data during online connection
 * </summary>
 *
 * <param name="data">
 *   Array of data to send.
 * </param>
 * <param name="len">
 *   Number of bytes to send.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise values &lt; <em>OK</em>.
 *   <em>ERROR</em> if any error occured (e.g. LIVE mode not activated).
 * </returns>
 */
native rM2M_LiveData(const data{}, len);

/**
 * <summary>
 *   Read recorded data.
 * </summary>
 *
 * <param name="recidx">
 *   Index of data record (-1 = last/youngest record, -2 = second last record etc.)
 * </param>
 * <param name="funcidx">
 *   Index of callback function that is called when read finished
 *   Has to be of type: public func(const data[], len, timestamp);
 * </param>
 *
 * <returns>
 *   <em>OK</em> if readout operation started, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_ReadData(recidx, funcidx);

/**
 * <summary>
 *   Pack provided value into data array on given position.
 * </summary>
 *
 * <param name="data">
 *   Array of data to be used.
 * </param>
 * <param name="pos">
 *   Byte offset in array where value should be packed.
 * </param>
 * <param name="value">
 *   Value to be packed.
 * </param>
 * <param name="size">
 *   Number of bytes to be used for packed value.
 * </param>
 * <param name="bigendian">
 *   Pack the value as Big or Little Endian.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_SetPacked(data{}, pos, &{Float,Fixed,_}:value, size=4, bool:bigendian=false);

/**
 * <summary>
 *   Pack provided data block into data array on given position.
 * </summary>
 *
 * <param name="data">
 *   Array of data to be used.
 * </param>
 * <param name="pos">
 *   Byte offset in array where block should be packed.
 * </param>
 * <param name="block">
 *   Data block to be packed.
 * </param>
 * <param name="size">
 *   Number of bytes to be used from block.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_SetPackedB(data{}, pos, const block[], size);

/**
 * <summary>
 *   Unpack a value from data array at given position.
 * </summary>
 *
 * <param name="data">
 *   Array of data to be used.
 * </param>
 * <param name="pos">
 *   Byte offset in array where value is packed.
 * </param>
 * <param name="value">
 *   Variable to be used for storing the unpacked value.
 * </param>
 * <param name="size">
 *   Number of bytes to be used for unpacking.
 * </param>
 * <param name="bigendian">
 *   Unpack the value as Big or Little Endian.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_GetPacked(const data{}, pos, &{Float,Fixed,_}:value, size=4, bool:bigendian=false);

/**
 * <summary>
 *   Unpack a data block from data array at given position.
 * </summary>
 *
 * <param name="data">
 *   Array of data to be used.
 * </param>
 * <param name="pos">
 *   Byte offset in array where block is packed.
 * </param>
 * <param name="block">
 *   Data block to be used for storing the unpacked bytes.
 * </param>
 * <param name="size">
 *   Number of bytes to be unpacked.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_GetPackedB(const data{}, pos, block[], size);

/**
 * <summary>
 *   rM2M_Pack() type flags.
 * </summary>
 */
const
{
  RM2M_PACK_GET = 0b00000001,  // 'Get Packed' if set, 'Set Packed' if not
  RM2M_PACK_BE  = 0b00000010,  // 'Big endian' if set, 'Little endian' if not
  RM2M_PACK_U8  = 0b00010000,  // 8-bit unsigned
  RM2M_PACK_S8  = 0b10010000,  // 8-bit signed
  RM2M_PACK_U16 = 0b00100000,  // 16-bit unsigned
  RM2M_PACK_S16 = 0b10100000,  // 16-bit signed
  RM2M_PACK_U32 = 0b01000000,  // 32-bit unsigned
  RM2M_PACK_S32 = 0b11000000,  // 32-bit signed
  RM2M_PACK_F32 = 0b01000000,  // 32-bit float
};

/**
 * <summary>
 *   General purpose function encapsulating 'Set Packed' and 'Get Packed'
 * </summary>
 *
 * <param name="data">
 *   Array of data to be used.
 * </param>
 * <param name="pos">
 *   Byte offset in array where value is packed.
 * </param>
 * <param name="value">
 *   Pack: Value to be packed.
 *   Unpack: Variable to be used for storing the unpacked value.
 * </param>
 * <param name="type">
 *   Type flags (Set/Get packed, little/big endian, ...) for operation
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_Pack(const data{}, pos, &{Float,Fixed,_}:value, type);

/**
 * <summary>
 *   rM2M_CfgInit() flags.
 * </summary>
 */
const
{
  RM2M_CFG_VOLATILE = 0b00000001, // volatile storage (RAM)
};

/**
 * <summary>
 *   Initialisation
 * </summary>
 *
 * <param name="cfg">
 *   Number of configuration object.
 * </param>
 * <param name="flags">
 *   Configuration flags
 *   Bit 0 - volatile storage (RAM)
 * </param>
 *
 * <returns>
 *   Returns:
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_CfgInit(cfg, flags);

/**
 * <summary>
 *   Write provided data array at given position in a configuration object.
 *   Timestamp of config is updated, so it is synchronised with server.
 * </summary>
 *
 * <param name="cfg">
 *   Number of configuration object.
 * </param>
 * <param name="pos">
 *   Byte offset in config where data should be written.
 * </param>
 * <param name="data">
 *   Array of data to be used.
 * </param>
 * <param name="size">
 *   Number of bytes to be used from data array.
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>OK</em> - if successful</li>
 *     <li><em>ERROR_MEM</em> - if currently not enough temporary memory is available.</li>
 *     <li><em>ERROR</em> - if any other error occured.</li>
 *   </ul>
 * </returns>
 */
native rM2M_CfgWrite(cfg, pos, const data{}, size);

/**
 * <summary>
 *   Flush configuration object - store object non-volatile in FLASH memory.
 * </summary>
 *
 * <param name="cfg">
 *   Number of configuration object.
 * </param>
 *
 * <returns>
 *   Returns:
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_CfgFlush(cfg);

/**
 * <summary>
 *   Read data from a configuration object at given position.
 * </summary>
 *
 * <param name="cfg">
 *   Number of configuration object.
 * </param>
 * <param name="pos">
 *   Byte offset in config where data should be read from.
 * </param>
 * <param name="data">
 *   Array of data to be used for storing the read bytes.
 * </param>
 * <param name="size">
 *   Number of bytes to be read from config.
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li>Configuration object length if successful</li>
 *     <li><em>ERROR_MEM</em> - if currently not enough temporary memory is available.</li>
 *     <li><em>ERROR</em> - if any other error occured.</li>
 *   </ul>
 * </returns>
 */
native rM2M_CfgRead(cfg, pos, data{}, size);

/**
 * <summary>
 *   Erase all data of a configuration object.
 * </summary>
 *
 * <param name="cfg">
 *   Number of configuration object.
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>OK</em> - if successful</li>
 *     <li><em>ERROR_MEM</em> - if currently not enough temporary memory is available.</li>
 *     <li><em>ERROR</em> - if any other error occured.</li>
 *   </ul>
 * </returns>
 */
native rM2M_CfgDelete(cfg);

/**
 * <summary>
 *   Sets the function which will be called when config has changed.
 * </summary>
 *
 * <param name="funcidx">
 *   Index of callback function.
 *   Has to be of type: public func(cfg);
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case no valid callback function
 *   supplied.
 * </returns>
 */
native rM2M_CfgOnChg(funcidx);

/**
 * <summary>
 *   Available registrys (reg).
 * </summary>
 */
const
{
  RM2M_REG_SYS_OTP = 0,
  RM2M_REG_SYS_FLASH,
  RM2M_REG_APP_OTP,
  RM2M_REG_APP_FLASH,
  RM2M_REG_APP_STATE,
  RM2M_REG_NUM_REGS
};

/**
 * <summary>
 *   Error codes for registry operations.
 * </summary>
 */
const
{
  RM2M_REG_ERROR_TOKENMEM  = -101,  // Not enough tokens were provided
  RM2M_REG_ERROR_INVAL     = -102,  // Invalid character inside JSON string
  RM2M_REG_ERROR_PART      = -103,  // The string is not a full JSON packet, more bytes expected

  RM2M_REG_ERROR_NOMEM     = -200,  // memory allocation failed
  RM2M_REG_ERROR_NUMTOKENS = -201,  // not enough token available for this object/array size
  RM2M_REG_ERROR_PAIR      = -202,  // found invalid pair (string : value)
  RM2M_REG_ERROR_NOTOKENS  = -203,  // not enough tokens free for appending
  RM2M_REG_ERROR_NOTFOUND  = -204,  // specified pair not found
  RM2M_REG_ERROR_TYPE      = -205,  // token type mismatch
  RM2M_REG_ERROR_PARAM     = -206,  // invalid parameters
  RM2M_REG_ERROR_SIZE      = -207,  // size exceeds maximum allowed
  RM2M_REG_ERROR_INVALID   = -208,  // JSON structure invalid
  RM2M_REG_ERROR_ISNULL    = -209,  // value is null
};

/**
 * <summary>
 *   rM2M_RegInit() flags.
 * </summary>
 */
const
{
  RM2M_REG_VOLATILE = 0b00000001, // volatile storage (RAM)
};

/**
 * <summary>
 *   Initialisation
 * </summary>
 *
 * <param name="reg">
 *   Number of registry (see available registrys).
 * </param>
 * <param name="flags">
 *   Configuration flags
 *   Bit 0 - volatile storage (RAM)
 * </param>
 * <param name="data">
 *   provided data buffer for storing volatile registry object
 * </param>
 * <param name="len">
 *   length of provided data buffer (number of cells)
 * </param>
 *
 * <returns>
 *   Returns:
 *   <em>OK</em> if successful, otherwise <em>ERROR</em> or dedicated errorcode (see RM2M_REG_ERROR_xxx).
 * </returns>
 */
native rM2M_RegInit(reg, flags, data{}, len=sizeof data);

/**
 * <summary>
 *   Get a string from a registry.
 * </summary>
 *
 * <param name="reg">
 *   Number of registry (see available registrys).
 * </param>
 * <param name="name">
 *   Name of registry entry.
 * </param>
 * <param name="string">
 *   Array used for storing the string.
 * </param>
 * <param name="len">
 *   Available size of string.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em> or dedicated errorcode (see RM2M_REG_ERROR_xxx).
 * </returns>
 */
native rM2M_RegGetString(reg, const name[], string[], len=sizeof string);

/**
 * <summary>
 *   Get a value from a registry.
 * </summary>
 *
 * <param name="reg">
 *   Number of registry (see available registrys).
 * </param>
 * <param name="name">
 *   Name of registry entry.
 * </param>
 * <param name="value">
 *   Variable used for storing.
 * </param>
 * <param name="tag">
 *   Tag distinguishes between integer or fractional conversion.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em> or dedicated errorcode (see RM2M_REG_ERROR_xxx).
 * </returns>
 */
native rM2M_RegGetValue(reg, const name[], &{Float,Fixed,_}:value, tag=tagof value);

/**
 * <summary>
 *   Set a string in a registry.
 * </summary>
 *
 * <param name="reg">
 *   Number of registry (see available registrys).
 * </param>
 * <param name="name">
 *   Name of registry entry.
 * </param>
 * <param name="string">
 *   String used for storing.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em> or dedicated errorcode (see RM2M_REG_ERROR_xxx).
 * </returns>
 */
native rM2M_RegSetString(reg, const name[], const string[]);

/**
 * <summary>
 *   Set a value in a registry.
 * </summary>
 *
 * <param name="reg">
 *   Number of registry (see available registrys).
 * </param>
 * <param name="name">
 *   Name of registry entry.
 * </param>
 * <param name="value">
 *   Value used for storing.
 * </param>
 * <param name="tag">
 *   Tag distinguishes between integer or fractional conversion.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em> or dedicated errorcode (see RM2M_REG_ERROR_xxx).
 * </returns>
 */
native rM2M_RegSetValue(reg, const name[], {Float,Fixed,_}:value, tag=tagof value);

/**
 * <summary>
 *   Delete a value in a registry.
 * </summary>
 *
 * <param name="reg">
 *   Number of registry (see available registrys).
 * </param>
 * <param name="name">
 *   Name of registry entry (key).
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em> or dedicated errorcode (see RM2M_REG_ERROR_xxx).
 * </returns>
 */
native rM2M_RegDelValue(reg, const name[]);

/**
 * <summary>
 *   Delete a key in a registry.
 * </summary>
 *
 * <param name="reg">
 *   Number of registry (see available registrys).
 * </param>
 * <param name="name">
 *   Name of registry entry (key).
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em> or dedicated errorcode (see RM2M_REG_ERROR_xxx).
 * </returns>
 */
native rM2M_RegDelKey(reg, const name[]);

/**
 * <summary>
 *   Sets the function which will be called when a registry has changed.
 * </summary>
 *
 * <param name="funcidx">
 *   Index of callback function.
 *   Has to be of type: public func(reg);
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_RegOnChg(funcidx);

/**
 * <summary>
 *   Initialise Sms receiving.
 * </summary>
 *
 * <param name="funcidx">
 *   Index of Sms receive function.
 *   Has to be of type: public func(const SmsTel[], const SmsText[]);
 * </param>
 * <param name="config">
 *   Reserved for future use.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_SmsInit(funcidx, config);

/**
 * <summary>
 *   Close Sms receiving.
 * </summary>
 *
 * <returns>
 *   <em>OK</em>.
 * </returns>
 */
native rM2M_SmsClose();

/**
 * <summary>
 *   Write new log entry.
 * </summary>
 *
 * <param name="log">
 *   Log entry to write (valid range  0 .. 999).
 * </param>
 * <param name="param">
 *   Additional parameter (valid range -32768 .. 32767).
 * </param>
 *
 * <returns>
 *   <em>OK</em>.
 * </returns>
 */
native rM2M_WriteLog(log, param);

/**
 * <summary>
 *   NMEA quality indicator.
 * </summary>
 */
const
{
  RM2M_NMEA_FIX_NOK      = 0,  // invalid
  RM2M_NMEA_FIX_GPS      = 1,  // non differental GPS fix
  RM2M_NMEA_FIX_DGPS     = 2,  // differental GPS fix
  RM2M_NMEA_FIX_PPS      = 3,  // Precise positioning service
  RM2M_NMEA_FIX_RTK      = 4,  // Real Time Kinematic
  RM2M_NMEA_FIX_FLOATRTK = 5,  // Float Real Time Kinematic
  RM2M_NMEA_FIX_EST      = 6,  // estimated fix (dead reckoning)
  RM2M_NMEA_FIX_MAN      = 7,  // Manual input mode
  RM2M_NMEA_FIX_SIM      = 8,  // Simulation mode
};

/**
 * <summary>
 *   Set GPS position information.
 * </summary>
 *
 * <param name="Lat">
 *   Latitude in Degree with a resolution of 0.000001
 *   -90 000 000 = south pole 90° S
 *             0 = equator
 *   +90 000 000 = north pole 90° N
 * </param>
 * <param name="Long">
 *   Longitude in Degree with a resolution of 0.000001
 *   -180 000 000 = 180° W
 *              0 = Greenwich meridian
 *   +180 000 000 = 180° E
 * </param>
 * <param name="Elev">
 *   Elevation - Mean Sea Level in m
 * </param>
 * <param name="Qual">
 *   Quality indicator - see RM2M_NMEA_FIX_xxx definitions
 * </param>
 * <param name="SatUsed">
 *   Number of satellites in view
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_SetPos(Lat, Long, Elev, Qual, SatUsed);

/**
 * <summary>
 *   error values used with rM2M_SetPosNMEA().
 * </summary>
 */
const
{
  RM2M_NMEA_ERR_DATATYPE  = -2, // Datatype (e.g. $GGSA) is not supported
  RM2M_NMEA_ERR_SENTENCE  = -3, // Sentence is not valid (e.g. checksum error)
  RM2M_NMEA_ERR_LATITUDE  = -4, // Latitude value is not valid
  RM2M_NMEA_ERR_LONGITUDE = -5, // Longitude value is not valid
  RM2M_NMEA_ERR_ALTITUDE  = -6, // Altitude value is not valid
  RM2M_NMEA_ERR_SAT_USED  = -7, // Number of satellites used is not valid
  RM2M_NMEA_ERR_QUAL      = -8, // quality entry is not supported
};

/**
 * <summary>
 *   list of supported GNSS (global navigation satellite system) devices
 *   Used to identify the source of the sentence (according to NMEA 0183 'Talker IDs').
 * </summary>
 */
const
{
  RM2M_NMEA_DEVICE_GP = 0x244750,  // $GP (GPS)
  RM2M_NMEA_DEVICE_GL = 0x24474C,  // $GL (GLONASS)
  RM2M_NMEA_DEVICE_GA = 0x244741,  // $GA (GALILEO)
  RM2M_NMEA_DEVICE_GN = 0x24474E,  // $GN (GENERIC GNSS)
};

/**
 * <summary>
 *   list of supported NMEA records.
 * </summary>
 */
const
{
  RM2M_NMEA_RECORD_GGA = 0x474741, // GGA
};

/**
 * <summary>
 *   GGA data structure.
 * </summary>
 *
 * <param name="Lat">
 *   Latitude in Degree with a resolution of 0.000001
 *   -90 000 000 = south pole 90° S
 *             0 = equator
 *   +90 000 000 = north pole 90° N
 * </param>
 * <param name="Long">
 *   Longitude in Degree with a resolution of 0.000001
 *   -180 000 000 = 180° W
 *              0 = Greenwich meridian
 *   +180 000 000 = 180° E
 * </param>
 * <param name="Alt">
 *   Altitude - Mean Sea Level in m
 * </param>
 * <param name="Qual">
 *   Quality indicator - see RM2M_NMEA_FIX_xxx definitions
 * </param>
 * <param name="SatUsed">
 *   Number of satellites in view
 * </param>
 * <param name="HDOP">
 *   Horizontal Dilution of Precision [0.01]
 * </param>
 */
#define TNMEA_GGA[.Lat, .Long, .Alt, .Qual, .SatUsed, .HDOP]

/**
 * <summary>
 *   Decode provided NMEA sentence.
 * </summary>
 *
 * <param name="sentence">
 *   NMEA sentence from GPS receiver (starting with character '$')
 * </param>
 * <param name="data">
 *   Destination buffer where decoded data is filled in.
 *   [0] holds the GNSS device type (see RM2M_NMEA_DEVICE_xxx definitions)
 *   [1] holds the NMEA record type (see RM2M_NMEA_RECORD_xxx defintions)
 *   The structure of the following data depends on the record type
 *   (e.g. structure TNMEA_GGA for RM2M_NMEA_RECORD_GGA).
 * </param>
 * <param name="len">
 *   length of data buffer (number of cells)
 * </param>
 *
 * <returns>
 *   <ul>
 *     <li>a positive value if successful (number of filled cells)</li>
 *     <li>a negative value if error (see RM2M_NMEA_ERR_xxx error values)</li>
 *   </ul>
 * </returns>
 */
native rM2M_DecodeNMEA(const sentence{}, data[], len=sizeof data);

/**
 * <summary>
 *   Set GPS position information in the device based on NMEA sentence.
 * </summary>
 *
 * <param name="sentence">
 *   NMEA sentence from GPS receiver (starting with character '$')
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, values &lt; <em>OK</em> if error (see RM2M_NMEA_ERR_xxx error values)
 * </returns>
 */
native rM2M_SetPosNMEA(const Sentence{});

/**
 * <summary>
 *   Get GPS position information stored in the device.
 * </summary>
 *
 * <param name="Lat">
 *   Will hold Latitude in Degree with a resolution of 0.000001
 *   -90 000 000 = south pole 90° S
 *             0 = equator
 *   +90 000 000 = north pole 90° N
 * </param>
 * <param name="Long">
 *   Will hold Longitude in Degree with a resolution of 0.000001
 *   -180 000 000 = 180° W
 *              0 = Greenwich meridian
 *   +180 000 000 = 180° E
 * </param>
 * <param name="Elev">
 *   Will hold Elevation - Mean Sea Level in m
 * </param>
 * <param name="Qual">
 *   Quality indicator - see RM2M_NMEA_FIX_xxx definitions
 * </param>
 * <param name="SatUsed">
 *   Number of satellites in view
 * </param>
 *
 * <returns>
 *   <em>OK</em> if valid position information is available, otherwise <em>ERROR</em>.
 * </returns>
 */
native rM2M_GetPos(&Lat, &Long, &Elev, &Qual=0, &SatUsed=0);

/**
 * <summary>
 *   Get cell based position information (GSM/UMTS/LTE/...) stored in the device.
 * </summary>
 *
 * <param name="posidx">
 *   posidx &lt;  0 => get number of available cells
 *   posidx &gte;= 0 => get position information for this cell
 * </param>
 * <param name="pos">
 *   Structure where position information is filled in.
 * </param>
 *
 * <returns>
 *   <ul>
 *     <li><em>OK</em> if GSM position information is available</li>
 *     <li><em>OK+1</em> if UMTS position information is available</li>
 *     <li><em>OK+2</em> if LTE position information is available</li>
 *     <li><em>ERROR</em> - if no position information is available.</li>
 *   </ul>
 * </returns>
 */
native rM2M_GetGSMPos(posidx, pos[TrM2M_GSMPos]=0);

/**
 * <summary>
 *   Get common GSM information
 * </summary>
 *
 * <param name="info">
 *   Structure where GSM information is filled in.
 * </param>
 * <param name="len">
 *   length of info structure, max size according to TrM2M_GSMInfo
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li>number of filled cells for info struct</li>
 *     <li><em>ERROR</em> - if address/length pair for info struct is invalid.</li>
 *   </ul>
 * </returns>
 */
native rM2M_GSMGetInfo(info[TrM2M_GSMInfo], len=sizeof info);

/**
 * <summary>
 *   SIM configuration structure.
 * </summary>
 */
#define TrM2M_SIMCfg[.pin{8}, .apn{40}, .username{40}, .password{40}]

/**
 * <summary>
 *   Set configuration for external SIM
 * </summary>
 *
 * <param name="cfg">
 *   Structure with configuration data.
 * </param>
 * <param name="len">
 *   length of cfg structure, max size according to TrM2M_SIMCfg
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li>number of filled cells for cfg struct</li>
 *     <li><em>ERROR</em> - if address/length pair for cfg struct is invalid.</li>
 *   </ul>
 * </returns>
 */
native rM2M_SetExtSimCfg(cfg[TrM2M_SIMCfg], len=sizeof cfg);

/**
 * <summary>
 *   Get configuration for external SIM
 * </summary>
 *
 * <param name="cfg">
 *   Structure where configuration data is filled in.
 * </param>
 * <param name="len">
 *   length of cfg structure, max size according to TrM2M_SIMCfg
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li>number of filled cells for cfg struct if valid configuration for
 *     external SIM is available</li>
 *     <li><em>ERROR</em> - if address/length pair for cfg struct is invalid
 *     or configuration for external SIM is not available</li>
 *   </ul>
 * </returns>
 */
native rM2M_GetExtSimCfg(cfg[TrM2M_SIMCfg]=0, len=sizeof cfg);

