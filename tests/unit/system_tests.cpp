#include <enulib/enu.hpp>
#include <enumivo/native/tester.hpp>

using namespace enumivo::native;

ENUMIVO_TEST_BEGIN(system_test)
   silence_output(true);
   auto test_check1 = [](bool b) { enumivo::check(b, "asserted"); }; 
   test_check1(true);
   CHECK_ASSERT("asserted", test_check1, false);

   auto test_check2 = [](bool b) { std::string s="asserted"; enumivo::check(b, s); }; 
   test_check2(true);
   REQUIRE_ASSERT("asserted", test_check2, false);

   auto test_check3 = [](bool b) { enumivo::check(b, "asserted with more text", 8); }; 
   test_check3(true);
   REQUIRE_ASSERT("asserted", test_check3, false);

   auto test_check4 = [](bool b) { std::string s="asserted with more text"; enumivo::check(b, s, 8); }; 
   test_check4(true);
   REQUIRE_ASSERT("asserted", test_check4, false);

   auto test_check5 = [](bool b) { enumivo::check(b, 13); }; 
   test_check5(true);
   REQUIRE_ASSERT("13", test_check5, false);
   silence_output(false);
ENUMIVO_TEST_END

int main(int argc, char** argv) {
   ENUMIVO_TEST(system_test);
   return has_failed();
}
