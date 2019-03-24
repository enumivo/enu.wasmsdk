#pragma once
#include "action.hpp"

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>

#include <boost/mp11/tuple.hpp>

namespace enumivo {

  /**
   * @defgroup dispatcher Dispatcher
   * @ingroup contracts
   * @brief Defines C++ functions to dispatch action to proper action handler inside a contract
   */


   /// @cond IMPLEMENTATIONS

   template<typename Contract, typename FirstAction>
   bool dispatch( uint64_t code, uint64_t act ) {
      if( code == FirstAction::get_account() && FirstAction::get_name() == act ) {
         Contract().on( unpack_action_data<FirstAction>() );
         return true;
      }
      return false;
   }

   /// @endcond

   /**
    * This method will dynamically dispatch an incoming set of actions to
    *
    * ```
    * static Contract::on( ActionType )
    * ```
    *
    * For this to work the Actions must be derived from enumivo::contract
    *
    * @ingroup dispatcher
    *
    */
   template<typename Contract, typename FirstAction, typename SecondAction, typename... Actions>
   bool dispatch( uint64_t code, uint64_t act ) {
      if( code == FirstAction::get_account() && FirstAction::get_name() == act ) {
         Contract().on( unpack_action_data<FirstAction>() );
         return true;
      }
      return enumivo::dispatch<Contract,SecondAction,Actions...>( code, act );
   }




   /**
    * Unpack the received action and execute the correponding action handler
    *
    * @ingroup dispatcher
    * @tparam T - The contract class that has the correponding action handler, this contract should be derived from enumivo::contract
    * @tparam Q - The namespace of the action handler function
    * @tparam Args - The arguments that the action handler accepts, i.e. members of the action
    * @param obj - The contract object that has the correponding action handler
    * @param func - The action handler
    * @return true
    */
   template<typename T, typename... Args>
   bool execute_action( name self, name code, void (T::*func)(Args...)  ) {
      size_t size = action_data_size();

      //using malloc/free here potentially is not exception-safe, although WASM doesn't support exceptions
      constexpr size_t max_stack_buffer_size = 512;
      void* buffer = nullptr;
      if( size > 0 ) {
         buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
         read_action_data( buffer, size );
      }

      std::tuple<std::decay_t<Args>...> args;
      datastream<const char*> ds((char*)buffer, size);
      ds >> args;

      T inst(self, code, ds);

      auto f2 = [&]( auto... a ){
         ((&inst)->*func)( a... );
      };

      boost::mp11::tuple_apply( f2, args );
      if ( max_stack_buffer_size < size ) {
         free(buffer);
      }
      return true;
   }

  /// @cond INTERNAL

 // Helper macro for ENUMIVO_DISPATCH_INTERNAL
 #define ENUMIVO_DISPATCH_INTERNAL( r, OP, elem ) \
    case enumivo::name( BOOST_PP_STRINGIZE(elem) ).value: \
       enumivo::execute_action( enumivo::name(receiver), enumivo::name(code), &OP::elem ); \
       break;

 // Helper macro for ENUMIVO_DISPATCH
 #define ENUMIVO_DISPATCH_HELPER( TYPE,  MEMBERS ) \
    BOOST_PP_SEQ_FOR_EACH( ENUMIVO_DISPATCH_INTERNAL, TYPE, MEMBERS )

/// @endcond

/**
 * Convenient macro to create contract apply handler
 *
 * @ingroup dispatcher
 * @note To be able to use this macro, the contract needs to be derived from enumivo::contract
 * @param TYPE - The class name of the contract
 * @param MEMBERS - The sequence of available actions supported by this contract
 *
 * Example:
 * @code
 * ENUMIVO_DISPATCH( enumivo::bios, (setpriv)(setalimits)(setglimits)(setprods)(reqauth) )
 * @endcode
 */
#define ENUMIVO_DISPATCH( TYPE, MEMBERS ) \
extern "C" { \
   [[enumivo::wasm_entry]] \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      if( code == receiver ) { \
         switch( action ) { \
            ENUMIVO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: enumivo_exit(0); */ \
      } \
   } \
} \

}