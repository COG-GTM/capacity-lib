#include <climits>
#include <cstdio>
#include <cstring>

#include "capacity_lib/capacity.h"

static int failures = 0;

#define CHECK_EQ(actual, expected)                                                   \
  do {                                                                               \
    long long a_ = (actual), e_ = (expected);                                        \
    if (a_ != e_) {                                                                  \
      std::printf("FAIL %s:%d  %s == %lld, expected %lld\n", __FILE__, __LINE__,      \
                  #actual, a_, e_);                                                  \
      failures++;                                                                    \
    }                                                                                \
  } while (0)

using namespace capacity_lib;

static void test_buffer_is_ten_percent_of_total() {
  CHECK_EQ(maintenance_buffer_mbps(500), 50);
  CHECK_EQ(maintenance_buffer_mbps(1000), 100);
  CHECK_EQ(maintenance_buffer_mbps(2000), 200);
  CHECK_EQ(maintenance_buffer_mbps(4000), 400);
  CHECK_EQ(maintenance_buffer_mbps(10000), 1000);
}

static void test_buffer_rounds_down_to_whole_mbps() {
  CHECK_EQ(maintenance_buffer_mbps(105), 10);
  CHECK_EQ(maintenance_buffer_mbps(9), 0);
}

static void test_buffer_of_empty_or_bad_total_is_zero() {
  CHECK_EQ(maintenance_buffer_mbps(0), 0);
  CHECK_EQ(maintenance_buffer_mbps(-100), 0);
}

static void test_buffer_does_not_overflow() {
  CHECK_EQ(maintenance_buffer_mbps(INT_MAX), INT_MAX / 10);
}

static void test_available_subtracts_allocation_and_buffer() {
  CHECK_EQ(available_capacity(1000, 600, 100), 300);
  CHECK_EQ(available_capacity(1000, 600, 0), 400);
}

static void test_available_never_negative() {
  CHECK_EQ(available_capacity(1000, 950, 100), 0);
  CHECK_EQ(available_capacity(100, 200, 0), 0);
  CHECK_EQ(available_capacity(INT_MIN, INT_MAX, INT_MAX), 0);
}

static void test_can_support_uses_the_standard_buffer() {
  /* RIV-01: 1000 total, 600 allocated -> 100 buffer, 300 available */
  CHECK_EQ(can_support(1000, 600, 300), 1);
  CHECK_EQ(can_support(1000, 600, 301), 0);
  CHECK_EQ(can_support(1000, 600, 0), 1);
}

static void test_can_support_full_link() {
  CHECK_EQ(can_support(1000, 1000, 1), 0);
  CHECK_EQ(can_support(1000, 1000, 0), 1);
}

static void test_c_api_matches_cpp_api() {
  for (int total = 0; total <= 10000; total += 250) {
    for (int alloc = 0; alloc <= total; alloc += 125) {
      int buffer = maintenance_buffer_mbps(total);
      CHECK_EQ(capacity_maintenance_buffer_mbps(total), buffer);
      CHECK_EQ(capacity_available_capacity(total, alloc, buffer), available_capacity(total, alloc, buffer));
      CHECK_EQ(capacity_can_support(total, alloc, 350), can_support(total, alloc, 350) ? 1 : 0);
    }
  }
  CHECK_EQ(capacity_maintenance_buffer_pct(), 10);
  CHECK_EQ(std::strcmp(capacity_rule(), rule()), 0);
  CHECK_EQ(std::strcmp(capacity_lib_version(), CAPACITY_LIB_VERSION), 0);
}

int main() {
  test_buffer_is_ten_percent_of_total();
  test_buffer_rounds_down_to_whole_mbps();
  test_buffer_of_empty_or_bad_total_is_zero();
  test_buffer_does_not_overflow();
  test_available_subtracts_allocation_and_buffer();
  test_available_never_negative();
  test_can_support_uses_the_standard_buffer();
  test_can_support_full_link();
  test_c_api_matches_cpp_api();
  if (failures) {
    std::printf("%d check(s) failed\n", failures);
    return 1;
  }
  std::printf("capacity-lib: all C++ checks passed\n");
  return 0;
}
