/**
 *  @file
 *  @copyright defined in enumivo/LICENSE
 */
#pragma once
#include <enulib/action.hpp>
#include <enulib/print.hpp>
#include <enulib/multi_index.hpp>
#include <enulib/dispatcher.hpp>
#include <enulib/contract.hpp>

/**
 * Helper macros to reduce the verbosity for common contracts
 */
#define CONTRACT class [[enumivo::contract]]
#define ACTION   [[enumivo::action]] void
#define TABLE struct [[enumivo::table]]
