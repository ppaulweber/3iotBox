/**
 * Helper functions
 */
#pragma library Helper

/**
 * <summary>
 *   Table point structure.
 * </summary>
 */
#define TablePoint[.key, .value]

/**
 * <summary>
 *   Table point structure with floating point values.
 * </summary>
 */
#define TablePointF[Float:.key, Float:.value]

/**
 * <summary>
 *   Error return codes of function CalcTable.
 * </summary>
 */
const
{
  TAB_ERR_FLOOR = -1, // key is below first table point -> limit
  TAB_ERR_CEIL = -2,  // key is above last table point -> limit
};

/**
 * <summary>
 *   Returns the implemented API Level of the script engine.
 * </summary>
 */
native getapilevel();

/**
 * <summary>
 *   Check if runtime API function is available.
 * </summary>
 *
 * <param name="name">
 *   Name of API function.
 * </param>
 *
 * <returns>
 *   true if function is available, false if not
 * </returns>
 *
 */
native exists(const name[]);

/**
 * <summary>
 *   Prints the supplied string to standard output.
 * </summary>
 *
 * <param name="string">
 *   The string to print.
 * </param>
 *
 * <returns>
 *   OK
 * </returns>
 *
 */
native print(const string[]);

/**
 * <summary>
 *   Prints the supplied format string to standard output.
 * </summary>
 *
 * <param>
 *   The values to print.
 * </param>
 *
 * <returns>
 *   number of printed characters if successful, otherwise <em>ERROR</em>
 * </returns>
 */
native printf(const format[], {Float,Fixed,_}:...);

/**
 * <summary>
 *   Returns the calculated Modbus CRC16 from data.
 * </summary>
 */
native CRC16(data{}, len);

/**
 * <summary>
 *   Returns the calculated Ethernet CRC32 from data.
 * </summary>
 */
native CRC32(data{}, len, initial=0);

/**
 * <summary>
 *   Searches a given <em>key</em> in supplied <em>table</em>
 *   and returns corresponding <em>value</em>. Interpolates between two points
 *   if <em>key</em> lies between.
 * </summary>
 *
 * <param name="key">
 *   The value to be used for the search.
 * </param>
 * <param name="value">
 *   Will hold the result value upon return.
 * </param>
 * <param name="table">
 *   The table to be searched. Has to be of type <em>TablePoint</em>.
 * </param>
 * <param name="size">
 *   The number of table entries.
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>OK</em> - if found corresponding value.</li>
 *     <li><em>TAB_ERR_FLOOR</em> - if <em>key</em> is below first table point,
 *       <em>value</em> is set to first table point</li>
 *     <li><em>TAB_ERR_CEIL</em> - if <em>key</em> is above last table point,
 *       <em>value</em> is set to last table point</li>
 *   </ul>
 * </returns>
 */
native CalcTable(key, &value, const table[][TablePoint], size = sizeof table);

/**
 * <summary>
 *   Searches a given <em>key</em> in supplied <em>table</em>
 *   and returns corresponding <em>value</em>. Interpolates between two points
 *   if <em>key</em> lies between.
 * </summary>
 *
 * <param name="key">
 *   The value to be used for the search.
 * </param>
 * <param name="value">
 *   Will hold the result value upon return.
 * </param>
 * <param name="table">
 *   The table to be searched. Has to be of type <em>TablePointF</em>.
 * </param>
 * <param name="size">
 *   The number of table entries.
 * </param>
 *
 * <returns>
 *   Returns:
 *   <ul>
 *     <li><em>OK</em> - if found corresponding value.</li>
 *     <li><em>TAB_ERR_FLOOR</em> - if <em>key</em> is below first table point,
 *       <em>value</em> is set to first table point</li>
 *     <li><em>TAB_ERR_CEIL</em> - if <em>key</em> is above last table point,
 *       <em>value</em> is set to last table point</li>
 *   </ul>
 * </returns>
 */
native CalcTableF(Float:key, &Float:value, const table[][TablePointF], size = sizeof table);
