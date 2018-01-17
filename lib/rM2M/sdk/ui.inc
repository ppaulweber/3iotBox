/**
 * UI channel module (UI = universal input)
 *
 * Includes functions for controlling UI channel modes, sample rate and to get actual value or
 * reset counter value of UI channel.
 */
#pragma library UI

/**
 * <summary>
 *   Measurement modes of UI channels.
 * </summary>
 */
const
{
  UI_CHT_SI_NONE = 0,   ///< channel off
  UI_CHT_SI_DIGITAL,    ///< digital
  UI_CHT_SI_DCTR,       ///< counter
  UI_CHT_SI_DFREQ,      ///< frequency
  UI_CHT_SI_DPWM,       ///< pulse width modulation
  UI_CHT_SI_A020MA,     ///< 0..20mA
  UI_CHT_SI_A002V,      ///< 0..2V
  UI_CHT_SI_A010V,      ///< 0..10V
  UI_CHT_SI_DIRECT,     ///< direct
};

/**
 * <summary>
 *   Sample rates for analog (0..20mA, 0..2V, 0..10V, direct)
 *   and digital measurement in [Hz].
 * </summary>
 *
 * <note>
 *   Higher sample rates result in higher current consumption!
 * </note>
 */
const
{
  UI_SAMPLE_RATE_2   =   2,
  UI_SAMPLE_RATE_4   =   4,
  UI_SAMPLE_RATE_8   =   8,
  UI_SAMPLE_RATE_16  =  16,
  UI_SAMPLE_RATE_32  =  32,
  UI_SAMPLE_RATE_64  =  64,
  UI_SAMPLE_RATE_128 = 128,
};

/**
 * <summary>
 *   Init an UI channel, configure channel mode and filter time.
 * </summary>
 *
 * <param name="channel">
 *   UI channel to configure (see device specific include file).
 * </param>
 * <param name="mode">
 *   Mode of channel (see UI_CHT_... channel modes).
 * </param>
 * <param name="filtertime">
 *   Filtertime for channel in [ms].
 *   Used for fast averaging of analog measurements (0..20mA, 0..2V, 0..10V, direct)
 *   or debouncing of digital value.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case of invalid parameters, 
 *   <em>ERROR_NOT_SUPPORTED</em> in case mode is not supported on this channel.
 * </returns>
 *
 * <note>
 *   Each configured channel increases the current consumption!
 * </note>
 */
native UI_Init(channel, mode, filtertime);

/**
 * <summary>
 *   Close and deactivate an UI channel.
 * </summary>
 *
 * <param name="channel">
 *   UI channel to close.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case of invalid parameter.
 * </returns>
 */
native UI_Close(channel);

/**
 * <summary>
 *   Get actual value of an UI channel.
 * </summary>
 *
 * <param name="channel">
 *   UI Channel to get value from.
 * </param>
 * <param name="value">
 *   Returns the value of the UI channel (channel mode dependant).
 *     UI_CHT_SI_DIGITAL  [0/1]
 *     UI_CHT_SI_DCTR     [-]
 *     UI_CHT_SI_DFREQ    [Hz]
 *     UI_CHT_SI_DPWM     [%]
 *     UI_CHT_SI_A020MA   [uA]
 *     UI_CHT_SI_A002V    [mV]
 *     UI_CHT_SI_A010V    [mV]
 *     UI_CHT_SI_DIRECT   [mV]
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case of invalid parameters.
 * </returns>
 */
native UI_GetValue(channel, &value=0);

/**
 * <summary>
 *   Set sample rate of measurement.
 * </summary>
 *
 * <param name="samplerate">
 *   Samplerate in [Hz] (only UI_SAMPLE_RATE_... values allowed!).
 *   Default samplerate is 16Hz.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, <em>ERROR</em> in case of invalid parameter.
 * </returns>
 *
 * <note>
 *   Higher sample rates result in higher current consumption!
 * </note>
 */
native UI_SetSampleRate(samplerate);

/**
 * <summary>
 *   Reset counters and get value.
 * </summary>
 *
 * <param name="channel">
 *   UI Channel to get value and reset counter.
 * </param>
 *
 * <returns>
 *   Count of channel, <em>ERROR</em> in case of invalid parameter.
 * </returns>
 */
native UI_ResetCounter(channel);

