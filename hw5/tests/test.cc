#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../raytra.h"
#include "../triangle.h"

using namespace Raytra;

TEST_CASE ("detect blank strings correctly") {
    REQUIRE(!is_blank("asdad"));
    REQUIRE(is_blank("  "));
}

TEST_CASE("vector operations work correctly") {
    vec a = {.x = 3, .y = 1, .z = 2};
    vec b = {.x = 2, .y = 6, .z = 4};
    REQUIRE(dot(a, b) == 20);

    vec c = {.x = 0.8017, .y = 0.267, .z = .534};
    REQUIRE(norm(a).x == Approx(c.x).epsilon(0.01));
    REQUIRE(norm(a).y == Approx(c.y).epsilon(0.01));
    REQUIRE(norm(a).z == Approx(c.z).epsilon(0.01));

    vec d = a + b;
    REQUIRE(d.x == 5); REQUIRE(d.y == 7); REQUIRE(d.z == 6);

    vec e = 10 * a;
    REQUIRE(e.x == 30); REQUIRE(e.y == 10); REQUIRE(e.z == 20);
}

TEST_CASE("operations on points should give vectors") {
    point a = {.x = 1, .y = 2, .z = 3};
    point b = {.x = 9, .y = 4, .z = 2};
    vec c = a + b;
    REQUIRE(c.x == 10);
    REQUIRE(c.y == 6);
    REQUIRE(c.z == 5);
}

TEST_CASE("distance between two points is computed correctly") {
    point p = {.x = 1, .y = 0, .z = 0};
    point q = {.x = 0, .y = 0, .z = 0};
    point z = {.x = 1, .y = 1, .z = 1};

    REQUIRE(dist2(p, q) == 1);
    REQUIRE(dist2(p, z) == 2);
}

TEST_CASE("cross product between vectors is computed correctly") {
    vec a = {.x = 1, .y = 2, .z = 3};
    vec b = {.x = 4, .y = 1, .z = 62};
    vec c = cross(a, b);

    REQUIRE(c.x == 121);
    REQUIRE(c.y == -50);
    REQUIRE(c.z == -7);
}

TEST_CASE("triangles have correct normals") {
    const Triangle t(1, 0, 0, 0, 1, 0, 0, 0, 1);
    point p = {.x = 1, .y = 1, .z = 1};
    vec n = t.get_normal(p);

    REQUIRE(n.x == 1/sqrtf(3));
    REQUIRE(n.y == 1/sqrtf(3));
    REQUIRE(n.z == 1/sqrtf(3));
}

TEST_CASE("Sort Directions cycle correctly") {
    REQUIRE(get_next_direction(SortDirection::X) == SortDirection::Y);
    REQUIRE(get_next_direction(SortDirection::Y) == SortDirection::Z);
    REQUIRE(get_next_direction(SortDirection::Z) == SortDirection::X);
}
