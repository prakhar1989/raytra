#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../raytra.h"

TEST_CASE ("detect blank strings correctly") {
    REQUIRE(!Raytra::is_blank("asdad"));
    REQUIRE(Raytra::is_blank("  "));
}
