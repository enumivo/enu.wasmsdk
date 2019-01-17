#include <enulib/enumivo.hpp>
#include <enumivo/native/tester.hpp>

using namespace enumivo::native;

ENUMIVO_TEST_BEGIN(name_test)
   //silence_output(true);
   enumivo_assert( enumivo::name{"azaa34"}.value == "azaa34"_n.value, "enumivo::name != azaa34" );
   enumivo_assert( enumivo::name{0}.value == 0, "enumivo::name != 0" );
   enumivo_assert( enumivo::name{"aa11"}.value == "aa11"_n.value, "enumivo::name != aa11" );
   enumivo_assert( enumivo::name{"z11"}.value == "z11"_n.value, "enumivo::name != z11" );
   
   auto testa = []() {
      enumivo_assert( enumivo::name{"bb"}.value == "aa"_n.value, "bb != aa" );
   };
   REQUIRE_ASSERT("bb != aa", testa);
   REQUIRE_ASSERT("character is not in allowed character set for names",
         ([]() {
            enumivo::name{"!"}.value;
         }));
   silence_output(false);
ENUMIVO_TEST_END

ENUMIVO_TEST_BEGIN(is_account_test)
   intrinsics::set_intrinsic<intrinsics::is_account>([](uint64_t a) { 
         if (a == 3)
            return true;
         if (a == 4)
            return true;
         return false;
         });

   CHECK_ASSERT("is not an account", ([]() {
      enumivo_assert(is_account(5), "is not an account");
      }));
   CHECK_EQUAL(is_account(3), true);
   CHECK_EQUAL(is_account(4), true);
   CHECK_EQUAL(is_account(6), true);
   REQUIRE_EQUAL(is_account(7), true);

ENUMIVO_TEST_END

int main(int argc, char** argv) {
   ENUMIVO_TEST(name_test);
   ENUMIVO_TEST(is_account_test);
   return has_failed();
}
