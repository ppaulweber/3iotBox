#include <float>

#pragma library Crypt

/**
 * <summary>
 *   Crypt return codes.
 * </summary>
 */
const
{
  CRYPT_OK = 0,
  CRYPT_ERROR = -1,            /* Generic error */
  CRYPT_INVALID_KEYSIZE = -2,  /* Invalid key size given */
  CRYPT_INVALID_CIPHER = -3,   /* Invalid cipher specified */
  CRYPT_MEM = -4,              /* Out of memory */
  CRYPT_INVALID_ARG = -5,      /* Generic invalid argument */
};

/**
 * <summary>
 *   Supported AES modes.
 * </summary>
 */
const
{
  CRYPT_AES128_CBC = 0,
  CRYPT_AES128_CBC_MAC = 1,
};

/**
 * <summary>
 *   Configuration structure for AES.
 * </summary>
 */
#define TCrypt_AESCfg[.mode, .iv{32}, .key{32}, .internal{1024}]

/**
 * <summary>
 *   Open required AES cipher
 * </summary>
 *
 * <param name="cfg">
 *   Configuration structure (see TCrypt_AESCfg).
 * </param>
 * <param name="len">
 *   Size of provided configuration structure
 * </param>
 *
 * <returns>
 *   <em>CRYPT_OK</em> if successful, otherwise &lt; <em>CRYPT_OK</em> (see Crypt return codes)
 * </returns>
 */
native Crypt_AESInit(cfg[TCrypt_AESCfg], len=sizeof cfg);

/**
 * <summary>
 *   Close AES cipher
 * </summary>
 *
 * <param name="cfg">
 *   Configuration structure (see TCrypt_AESCfg).
 * </param>
 * <param name="len">
 *   Size of provided configuration structure
 * </param>
 *
 * <returns>
 *   <em>CRYPT_OK</em> if successful, otherwise &lt; <em>CRYPT_OK</em> (see Crypt return codes)
 * </returns>
 */
native Crypt_AESClose(cfg[TCrypt_AESCfg], len=sizeof cfg);

/**
 * <summary>
 *   AES encryption 
 * </summary>
 *
 * <param name="cfg">
 *   Configuration structure (see TCrypt_AESCfg).
 * </param>
 * <param name="in">
 *   input data (original)
 * </param>
 * <param name="out>
 *   output data (encrypted)
 * </param>
 * <param name="len">
 *   number of bytes to encrypt
 * </param>
 *
 * <returns>
 *   <em>CRYPT_OK</em> if successful, otherwise &lt; <em>CRYPT_OK</em> (see Crypt return codes)
 * </returns>
 */
native Crypt_AESEncrypt(cfg[TCrypt_AESCfg], in{}, out{}, len);

/**
 * <summary>
 *   AES decryption 
 * </summary>
 *
 * <param name="cfg">
 *   Configuration structure (see TCrypt_AESCfg).
 * </param>
 * <param name="in">
 *   input data (encrypted)
 * </param>
 * <param name="out>
 *   output data (original)
 * </param>
 * <param name="len">
 *   number of bytes to decrypt
 * </param>
 *
 * <returns>
 *   <em>CRYPT_OK</em> if successful, otherwise &lt; <em>CRYPT_OK</em> (see Crypt return codes)
 * </returns>
 */
native Crypt_AESDecrypt(cfg[TCrypt_AESCfg], in{}, out{}, len);

/**
 * <summary>
 *   Calculate AES MAC 
 * </summary>
 *
 * <param name="cfg">
 *   Configuration structure (see TCrypt_AESCfg).
 * </param>
 * <param name="in">
 *   input data
 * </param>
 * <param name="out>
 *   output data (MAC value)
 * </param>
 * <param name="len">
 *   number of bytes for MAC calculation
 * </param>
 *
 * <returns>
 *   <em>CRYPT_OK</em> if successful, otherwise &lt; <em>CRYPT_OK</em> (see Crypt return codes)
 * </returns>
 */
native Crypt_AESMac(cfg[TCrypt_AESCfg], in{}, out{}, len);

/**
 * <summary>
 *   Get random number
 * </summary>
 *
 * <param name="max">
 *   the limit for the random number
 * </param>
 *
 * <returns>
 *   random number in the range 0 - <em>max</em>-1
 * </returns>
 */
native Crypt_Rand(max);

/**
 * <summary>
 *   Fill buffer with random data
 * </summary>
 *
 * <param name="buf">
 *   buffer for storing the random bytes.
 * </param>
 * <param name="len">
 *   byte length of buffer (random number)
 * </param>
 *
 * <returns>
 *   <em>CRYPT_OK</em> if successful, otherwise &lt; <em>CRYPT_OK</em> (see Crypt return codes)
 * </returns>
 */
native Crypt_RandBuf(buf{}, len);

