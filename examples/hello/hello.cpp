#include "hello.hpp"
using namespace enumivo;

ACTION hello::hi( name user ) {
   print_f( "Hello % from hello", user );
}

ENUMIVO_DISPATCH( hello, (hi) )
