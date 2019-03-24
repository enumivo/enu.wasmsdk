/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once
#include "../../core/enumivo/time.hpp"
#include "../../core/enumivo/check.hpp"

namespace enumivo {
  namespace internal_use_do_not_use {
    extern "C" {
      __attribute__((enumivo_wasm_import, noreturn))
      void enumivo_exit( int32_t code );
    }
  }

  /**
   *  @addtogroup system System
   *  @ingroup contracts
   *  @brief Defines time related functions and enumivo_exit
   */

   /**
    *  This method will abort execution of wasm without failing the contract. This is used to bypass all cleanup / destructors that would normally be called.
    *
    *  @ingroup system
    *  @param code - the exit code
    *  Example:
    *
    *  @code
    *  enumivo_exit(0);
    *  enumivo_exit(1);
    *  enumivo_exit(2);
    *  enumivo_exit(3);
    *  @endcode
    */
   inline void enumivo_exit( int32_t code ) {
     internal_use_do_not_use::enumivo_exit(code);
   }

   /**
   *  Returns the time in microseconds from 1970 of the current block as a time_point
   *
   *  @ingroup system
   *  @return time in microseconds from 1970 of the current block as a time_point
   */
   time_point current_time_point();

   /**
   *  Returns the time in microseconds from 1970 of the current block as a block_timestamp
   *
   *  @ingroup system
   *  @return time in microseconds from 1970 of the current block as a block_timestamp
   */
   block_timestamp current_block_time();
}
