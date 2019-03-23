#include <enumivo/enumivo.hpp>
#include <enumivo/tester.hpp>

using namespace enumivo::native;

ENUMIVO_TEST_BEGIN(print_test)
   silence_output(false);
   CHECK_PRINT("27", [](){ enumivo::print((uint8_t)27); });
   CHECK_PRINT("34", [](){ enumivo::print((int)34); });
   CHECK_PRINT([](std::string s){return s[0] == 'a';},  [](){ enumivo::print((char)'a'); });
   CHECK_PRINT([](std::string s){return s[0] == 'b';},  [](){ enumivo::print((int8_t)'b'); });
   CHECK_PRINT("202", [](){ enumivo::print((unsigned int)202); });
   CHECK_PRINT("-202", [](){ enumivo::print((int)-202); });
   CHECK_PRINT("707", [](){ enumivo::print((unsigned long)707); });
   CHECK_PRINT("-707", [](){ enumivo::print((long)-707); });
   CHECK_PRINT("909", [](){ enumivo::print((unsigned long long)909); });
   CHECK_PRINT("-909", [](){ enumivo::print((long long)-909); });
   CHECK_PRINT("404", [](){ enumivo::print((uint32_t)404); });
   CHECK_PRINT("-404", [](){ enumivo::print((int32_t)-404); });
   CHECK_PRINT("404000000", [](){ enumivo::print((uint64_t)404000000); });
   CHECK_PRINT("-404000000", [](){ enumivo::print((int64_t)-404000000); });
   CHECK_PRINT("0x0066000000000000", [](){ enumivo::print((uint128_t)102); });
   CHECK_PRINT("0xffffff9affffffffffffffffffffffff", [](){ enumivo::print((int128_t)-102); });
   silence_output(false);
ENUMIVO_TEST_END

int main(int argc, char** argv) {
   ENUMIVO_TEST(print_test);
   return has_failed();
}
