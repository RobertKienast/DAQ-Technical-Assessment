#include <catch2/catch_all.hpp>
#include "../decoder.hpp"

TEST_CASE("DOES THIS SHIT WORK", "[a test tag]") {
    REQUIRE(1 == 1);
    REQUIRE(convertHextoBinary('A') == 10);
}
