/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once

#include "crypto.h"
#include "fixed_bytes.hpp"
#include "varint.hpp"
#include "serialize.hpp"

#include <array>

#warning "<enumivolib/crypto.hpp> is deprecated use <enumivo/crypto.hpp>"

namespace enumivo {

   /**
   *  @defgroup public_key Public Key Type
   *  @ingroup types
   *  @brief Specifies public key type
   *
   *  @{
   */

   /**
    * Enumivo Public Key
    * @brief Enumivo Public Key
    */
   struct public_key {
      /**
       * Type of the public key, could be either K1 or R1
       * @brief Type of the public key
       */
      unsigned_int        type;

      /**
       * Bytes of the public key
       *
       * @brief Bytes of the public key
       */
      std::array<char,33> data;

      friend bool operator == ( const public_key& a, const public_key& b ) {
        return std::tie(a.type,a.data) == std::tie(b.type,b.data);
      }
      friend bool operator != ( const public_key& a, const public_key& b ) {
        return std::tie(a.type,a.data) != std::tie(b.type,b.data);
      }
      ENULIB_SERIALIZE( public_key, (type)(data) )
   };

   /// @} publickeytype

   /**
   *  @defgroup signature Signature type
   *  @ingroup types
   *  @brief Specifies signature type
   *
   *  @{
   */

   /**
    * Enumivo Signature
    * @brief Enumivo Signature
    */
   struct signature {
      /**
       * Type of the signature, could be either K1 or R1
       * @brief Type of the signature
       */
      unsigned_int        type;

      /**
       * Bytes of the signature
       *
       * @brief Bytes of the signature
       */
      std::array<char,65> data;

      friend bool operator == ( const signature& a, const signature& b ) {
        return std::tie(a.type,a.data) == std::tie(b.type,b.data);
      }
      friend bool operator != ( const signature& a, const signature& b ) {
        return std::tie(a.type,a.data) != std::tie(b.type,b.data);
      }
      ENULIB_SERIALIZE( signature, (type)(data) )
   };

   /// @} signaturetype

   /**
    *  @defgroup crypto Chain API
    *  @ingroup core
    *  @brief Defines API for calculating and checking hashes
    *  @{
    */

   /**
    *  Tests if the SHA256 hash generated from data matches the provided digest.
    *  This method is optimized to a NO-OP when in fast evaluation mode.
    *  @brief Tests if the sha256 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    */
   void assert_sha256( const char* data, uint32_t length, const enumivo::checksum256& hash );

   /**
    *  Tests if the SHA1 hash generated from data matches the provided digest.
    *  This method is optimized to a NO-OP when in fast evaluation mode.
    *  @brief Tests if the sha1 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    */
   void assert_sha1( const char* data, uint32_t length, const enumivo::checksum160& hash );

   /**
    *  Tests if the SHA512 hash generated from data matches the provided digest.
    *  This method is optimized to a NO-OP when in fast evaluation mode.
    *  @brief Tests if the sha512 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    */
   void assert_sha512( const char* data, uint32_t length, const enumivo::checksum512& hash );

   /**
    *  Tests if the RIPEMD160 hash generated from data matches the provided digest.
    *  @brief Tests if the ripemd160 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    */
   void assert_ripemd160( const char* data, uint32_t length, const enumivo::checksum160& hash );

   /**
    *  Hashes `data` using SHA256.
    *  @brief Hashes `data` using SHA256.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return enumivo::checksum256 - Computed digest
    */
   enumivo::checksum256 sha256( const char* data, uint32_t length );

   /**
    *  Hashes `data` using SHA1.
    *  @brief Hashes `data` using SHA1.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return enumivo::checksum160 - Computed digest
    */
   enumivo::checksum160 sha1( const char* data, uint32_t length );

   /**
    *  Hashes `data` using SHA512.
    *  @brief Hashes `data` using SHA512.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return enumivo::checksum512 - Computed digest
    */
   enumivo::checksum512 sha512( const char* data, uint32_t length );

   /**
    *  Hashes `data` using RIPEMD160.
    *  @brief Hashes `data` using RIPEMD160.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return enumivo::checksum160 - Computed digest
    */
   enumivo::checksum160 ripemd160( const char* data, uint32_t length );

   /**
    *  Calculates the public key used for a given signature on a given digest.
    *  @brief Calculates the public key used for a given signature on a given digest.
    *
    *  @param digest - Digest of the message that was signed
    *  @param sig - Signature
    *  @return enumivo::public_key - Recovered public key
    */
   enumivo::public_key recover_key( const enumivo::checksum256& digest, const enumivo::signature& sig );

   /**
    *  Tests a given public key with the recovered public key from digest and signature.
    *  @brief Tests a given public key with the recovered public key from digest and signature.
    *
    *  @param digest - Digest of the message that was signed
    *  @param sig - Signature
    *  @param pubkey - Public key
    */
   void assert_recover_key( const enumivo::checksum256& digest, const enumivo::signature& sig, const enumivo::public_key& pubkey );

   /// }@cryptoapi
}
