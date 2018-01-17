/**
 * File Transfer functions
 */
#pragma library FileTransfer

/**
 * <summary>
 *   FileTransfer info structure.
 * </summary>
 */
#define TFT_Info[.name{256}, .stamp, .stamp256, .size, .crc, .flags]

/**
 * <summary>
 *   FileTransfer flags.
 * </summary>
 */
const
{
  FT_FLAG_READ   = 0x0001, /* file is readable */
  FT_FLAG_WRITE  = 0x0002, /* file is writeable */
  FT_FLAG_NODE   = 0x0004  /* file is a node (used to create new files) */
  FT_FLAG_SYSTEM = 0x0008  /* file is a system file (not usable in script!) */
};

/**
 * <summary>
 *   FileTransfer commands.
 * </summary>
 */
const
{
  FT_CMD_NONE = 0,
  FT_CMD_UNLOCK,
  FT_CMD_LIST,
  FT_CMD_READ,
  FT_CMD_STORE,
  FT_CMD_WRITE,
  FT_CMD_DELETE,
};

/**
 * <summary>
 *   Register a file.
 * </summary>
 *
 * <param name="name">
 *   Name of file.
 * </param>
 * <param name="id">
 *   Identification of file for later usage.
 * </param>
 * <param name="funcidx">
 *   Index of command handler function.
 *   Has to be of type: public func(id, cmd, const data{}, len, ofs);
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native FT_Register(const name{}, id, funcidx);

/**
 * <summary>
 *   Unregister a file.
 * </summary>
 *
 * <param name="id">
 *   Identification of file.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native FT_Unregister(id);

/**
 * <summary>
 *   Set file properties.
 * </summary>
 *
 * <param name="id">
 *   Identification of file.
 * </param>
 * <param name="stamp">
 *   Timestamp of file.
 * </param>
 * <param name="size">
 *   Size of file (in bytes).
 * </param>
 * <param name="crc">
 *   Ethernet CRC32 of file content.
 * </param>
 * <param name="flags">
 *   File flags (see FT_FLAG_...).
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native FT_SetProps(id, stamp, size, crc, flags);

/**
 * <summary>
 *   Set extended file properties.
 * </summary>
 *
 * <param name="id">
 *   Identification of file.
 * </param>
 * <param name="props">
 *   File properties (see TFT_Info).
 * </param>
 * <param name="len">
 *   Size of props structure.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native FT_SetPropsExt(id, props[TFT_Info], len=sizeof props);

/**
 * <summary>
 *   Return read data.
 * </summary>
 *
 * <param name="id">
 *   Identification of file.
 * </param>
 * <param name="data">
 *   Data which was read.
 * </param>
 * <param name="len">
 *   Length of data.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native FT_Read(id, const data{}, len);

/**
 * <summary>
 *   Accept storage of file.
 * </summary>
 *
 * <param name="id">
 *   Identification of file.
 * </param>
 * <param name="newid">
 *   Identification of new file used for storage (-1 in case of same file used).
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native FT_Accept(id, newid=-1);

/**
 * <summary>
 *   Acknowledge written data.
 * </summary>
 *
 * <param name="id">
 *   Identification of file.
 * </param>
 * <param name="len">
 *   Length of written data.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native FT_Written(id, len);

/**
 * <summary>
 *   Indicate error in file handling.
 * </summary>
 *
 * <param name="id">
 *   Identification of file.
 * </param>
 *
 * <returns>
 *   <em>OK</em> if successful, otherwise <em>ERROR</em>.
 * </returns>
 */
native FT_Error(id);
