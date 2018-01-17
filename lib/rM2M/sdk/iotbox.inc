/**
 * rapidM2M IoT-Box specific include file
 */
#pragma library IoTbox

#include "ui"

/**
 * <summary>
 *   Available UI Channels.
 * </summary>
 */
const
{
  UI_CHANNEL1 = 0,
  UI_CHANNEL2,
  UI_NUM_CHANNELS,
};

/**
 * <summary>
 *   Sys info structure.
 * </summary>
 * <param name="VBat">
 *   Battery Voltage [mV]
 * </param>
 * <param name="VUsb">
 *   USB voltage [mV]
 * </param>
 * <param name="Temp">
 *   temperature [0,1°C]
 * </param>
 */
#define TIoTbox_SysValue[.VBat, .VUsb, .Temp]

/**
 * <summary>
 *   Led Modes.
 * </summary>
 */
const
{
  LED_MODE_INTERNAL = 0, // led's are controlled by FW
  LED_MODE_SCRIPT,       // led's are controlled by Script
};

/**
 * <summary>
 *   Retrieve sys values of device.
 * </summary>
 *
 * <param name="values">
 *   Structure where sys values are filled in.
 * </param>
 * <param name="len">
 *   length of values structure, max size according to TIoTbox_SysValue
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>used size of structure</em></li>
 *     <li><em>ERROR</em> - if address/length pair for structure is invalid or a sys value is invalid.</li>
 *   </ul>
 * </returns>
 */
native IoTbox_GetSysValues(values[TIoTbox_SysValue], len=sizeof values);

/**
 * <summary>
 *   Init Led usage.
 * </summary>
 *
 * <param name="mode">
 *   Led mode (see Led Modes).
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case of invalid parameters.
 * </returns>
 */
native Led_Init(mode);

/**
 * <summary>
 *   Close Led usage (neither FW nor Script control led's).
 * </summary>
 *
 * <returns>
 *   <em>OK</em>.
 * </returns>
 */
native Led_Close();

/**
 * <summary>
 *   Turn on led.
 * </summary>
 *
 * <param name="green">
 *   Green led on if true.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case of invalid parameters.
 * </returns>
 */
native Led_On(color);

/**
 * <summary>
 *   Turn off led.
 * </summary>
 *
 * <param name="green">
 *   Green led off if true.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case of invalid parameters.
 * </returns>
 */
native Led_Off();

/**
 * <summary>
 *   Blink with led (500ms On, 500ms Off).
 * </summary>
 *
 * <param name="green">
 *   Green led blink count or forever if 0.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case of invalid parameters.
 * </returns>
 */
native Led_Blink(count, color);

/**
 * <summary>
 *   Flash with led (500ms period, single flash On).
 * </summary>
 *
 * <param name="green">
 *   Green led flash count or forever if 0.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case of invalid parameters.
 * </returns>
 */
native Led_Flash(count, color);

/**
 * <summary>
 *   Flicker with led
 * </summary>
 *
 * <param name="green">
 *   Green led flicker count or forever if 0 .
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case of invalid parameters.
 * </returns>
 */
native Led_Flicker(count, color);

/**
 * <summary>
 *   Switch Modes.
 * </summary>
 */
const
{
  SWITCH_MODE_INTERNAL = 0, // switch is controlled by FW
  SWITCH_MODE_SCRIPT,       // switch is controlled by Script
};

/**
 * <summary>
 *   Init and configure switch interface.
 * </summary>
 *
 * <param name="mode">
 *   see Switch Modes.
 * </param>
 * <param name="funcidx">
 *   Index of callback function if script mode. Function is called on switch state change.
 *   Has to be of type: public func(key);
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native Switch_Init(mode, funcidx=-1);

/**
 * <summary>
 *   Close and deactivate switch interface.
 * </summary>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native Switch_Close();

/**
 * <summary>
 *   Identification structure of BLE chip.
 * </summary>
 * <param name="FWVersion">
 *   Firmware Version of BLE chip
 * </param>
 */
#define TBLE_Id[.FWVersion]

/**
 * <summary>
 *   Retrieve identification of BLE chip.
 * </summary>
 *
 * <param name="id">
 *   Structure where identification is filled in.
 * </param>
 * <param name="len">
 *   length of id Structure, max size according to TBLEGW_Id
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
native BLE_GetId(id[TBLE_Id], len=sizeof id);

#define TBLE_Scan[.addr_type, .addr{6}, .rssi, .name{32+1}, .msd_len, .msd{32}]
#define TBLE_Notify[.handle, .data_len, .data{32}]
#define TBLE_Read[.handle, .offset, .data_len, .data{32}]

/**
 * <summary>
 *   BLE states.
 * </summary>
 */
const
{
  BLE_STATE_OFF = 0,
  BLE_STATE_INIT,
  BLE_STATE_READY,
  BLE_STATE_BUSY
};

/**
 * <summary>
 *   BLE events.
 * </summary>
 */
const
{
  BLE_EVENT_SCAN = 0,
  BLE_EVENT_SCAN_RSP,
  BLE_EVENT_NOTIFY,
  BLE_EVENT_READ,
};

/**
 * <summary>
 *   Init and configure BLE interface.
 * </summary>
 *
 * <param name="funcidx">
 *   Index of event callback function. Function is called on ble events.
 *   Has to be of type: public func(event, connhandle, const data{}, len);
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native BLE_Init(funcidx);

/**
 * <summary>
 *   Close and deactivate BLE interface.
 * </summary>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native BLE_Close();

/**
 * <summary>
 *   Reset BLE interface.
 * </summary>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native BLE_Reset();

/**
 * <summary>
 *   Get BLE interface state.
 * </summary>
 *
 * <returns>
 *   State of BLE interface (see BLE states).
 * </returns>
 */
native BLE_GetState();

/**
 * <summary>
 *   Start scan
 * </summary>
 *
 * <param name="time">
 *   Time to scan in [s].
 * </param>
 *
 * <param name="flags">
 *   Flags for scanning.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native BLE_Scan(time = 10, flags = 0);

/**
 * <summary>
 *   Connect to BLE device.
 * </summary>
 *
 * <param name="addr">
 *   BLE address of device.
 * </param>
 *
 * <param name="itv">
 *   Connection interval in [ms] (valid 8..1000).
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native BLE_Connect(addr{6}, itv = -1);

/**
 * <summary>
 *   Disconnect from BLE device (device must be connected).
 * </summary>
 *
 * <param name="connhandle">
 *   Connection handle (reserved for future use, set to 0).
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful
 *   <em>ERROR</em> if error
 *   <em>ERROR-1</em> if BLE device isn't connected.
 * </returns>
 */
native BLE_Disconnect(connhandle = 0);

/**
 * <summary>
 *   Get connection state.
 * </summary>
 *
 * <param name="connhandle">
 *   Connection handle (reserved for future use, set to 0).
 * </param>
 *
 * <returns>
 *   Connection state (0 = disconnected, >0 = connected).
 * </returns>
 */
native BLE_GetConnState(connhandle = 0);

/**
 * <summary>
 *   Write to BLE device (device must be connected).
 * </summary>
 *
 * <param name="connhandle">
 *   Connection handle (reserved for future use, set to 0).
 * </param>
 *
 * <param name="handle">
 *   Handle.
 * </param>
 *
 * <param name="data">
 *   Data.
 * </param>
 *
 * <param name="size">
 *   Length of data.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful
 *   <em>ERROR</em> if error
 *   <em>ERROR-1</em> if BLE device isn't connected.
 * </returns>
 */
native BLE_Write(connhandle = 0, handle, const data{}, size);

/**
 * <summary>
 *   Read from BLE device (device must be connected).
 * </summary>
 *
 * <param name="connhandle">
 *   Connection handle (reserved for future use, set to 0).
 * </param>
 *
 * <param name="handle">
 *   Handle.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful
 *   <em>ERROR</em> if error
 *   <em>ERROR-1</em> if BLE device isn't connected.
 * </returns>
 */
native BLE_Read(connhandle = 0, handle);

/**
 * <summary>
 *   Change connection intervall (device must be connected).
 * </summary>
 *
 * <param name="connhandle">
 *   Connection handle (reserved for future use, set to 0).
 * </param>
 *
 * <param name="conitv">
 *   Connection interval in [ms] (valid 8..1000).
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful
 *   <em>ERROR</em> if error
 *   <em>ERROR-1</em> if BLE device isn't connected.
 * </returns>
 */
native BLE_ChgConItv(connhandle = 0, conitv);
