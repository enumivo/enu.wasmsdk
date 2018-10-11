#include <enulib/enu.hpp>

using namespace enumivo;

CONTRACT hello : public enumivo::contract {
  public:
      using contract::contract;

      ACTION hi( name user ) {
         print( "Hello, ", name{user} );
      }
};

ENUMIVO_DISPATCH( hello, (hi) )
