#include <enulib/enu.hpp>

using namespace enumivo;

CONTRACT hello : public enumivo::contract {
  public:
      using contract::contract;

      ACTION hi( name user );

      // accessor for external contracts to easily send inline actions to your contract
      using hi_action = action_wrapper<"hi"_n, &hello::hi>;
};
