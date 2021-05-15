#pragma once
#include <WiFiEspAT.h>
//#include <TimeLib.h> // in LibraryManager as "Time"
#include <string.h>

/*
 *  Internal defines, loop unrolling for the static hash macro's.
 */
#define HASH_INT_LEN(s) ((sizeof(s) / sizeof(s[0])) - 1)
#define HASH_INT_1(s, i, x) (x * 65599u + (unsigned char)s[(i) < HASH_INT_LEN(s) ? HASH_INT_LEN(s) - 1 - (i) : HASH_INT_LEN(s)])
#define HASH_INT_4(s, i, x) HASH_INT_1(s, i, HASH_INT_1(s, i + 1, HASH_INT_1(s, i + 2, HASH_INT_1(s, i + 3, x))))
#define HASH_INT_16(s, i, x) HASH_INT_4(s, i, HASH_INT_4(s, i + 4, HASH_INT_4(s, i + 8, HASH_INT_4(s, i + 12, x))))
#define HASH_INT_64(s, i, x) HASH_INT_16(s, i, HASH_INT_16(s, i + 16, HASH_INT_16(s, i + 32, HASH_INT_16(s, i + 48, x))))
#define HASH_INT_256(s, i, x) HASH_INT_64(s, i, HASH_INT_64(s, i + 64, HASH_INT_64(s, i + 128, HASH_INT_64(s, i + 192, x))))

/*
 *  Assert macro.
 */
#define assert_static(e)        \
  do                            \
  {                             \
    enum                        \
    {                           \
      assert_static__ = 1 / (e) \
    };                          \
  } while (0)

/*
 *  Hash macro's for external use.
 */
#define HASH_S256(s) ((unsigned int)(HASH_INT_256(s, 0, 0) ^ (HASH_INT_256(s, 0, 0) >> 16))) ///< Hash value from string with a max length of 256 bytes.
#define HASH_S64(s) ((unsigned int)(HASH_INT_64(s, 0, 0) ^ (HASH_INT_64(s, 0, 0) >> 16)))    ///< Hash value from string with a max length of 64 bytes.
#define HASH_S16(s) ((unsigned int)(HASH_INT_16(s, 0, 0) ^ (HASH_INT_16(s, 0, 0) >> 16)))    ///< Hash value from string with a max length of 16 bytes.

/*
 *  Functions.
 */

/**
 *  \brief  Hash function
 *
 *  This HASH function produce the same hash value as the static hash functions HASH_S256 and HASH_S16. This function can be used on
 *  any character string/pointer.
 *
 *  \param  String is a pointer to a null terminated character string.
 *  \return HASH value.
 */


void time_parser();
unsigned int hash_string(char *String);
void get_my_time();