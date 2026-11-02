#include "capacity_lib/capacity.h"

namespace {

const char kRule[] =
    "available = total - allocated - maintenance_buffer; maintenance_buffer = 10% of total";

int clamp_to_int(long long v) {
  if (v < 0) return 0;
  if (v > 2147483647LL) return 2147483647;
  return static_cast<int>(v);
}

}  // namespace

namespace capacity_lib {

int maintenance_buffer_mbps(int total_mbps) {
  if (total_mbps <= 0) return 0;
  return clamp_to_int(static_cast<long long>(total_mbps) * CAPACITY_MAINTENANCE_BUFFER_PCT / 100);
}

int available_capacity(int total_mbps, int alloc_mbps, int buffer_mbps) {
  return clamp_to_int(static_cast<long long>(total_mbps) - alloc_mbps - buffer_mbps);
}

bool can_support(int total_mbps, int alloc_mbps, int requested_mbps) {
  return available_capacity(total_mbps, alloc_mbps, maintenance_buffer_mbps(total_mbps)) >= requested_mbps;
}

const char *rule() { return kRule; }

}  // namespace capacity_lib

extern "C" {

int capacity_available_capacity(int total_mbps, int alloc_mbps, int buffer_mbps) {
  return capacity_lib::available_capacity(total_mbps, alloc_mbps, buffer_mbps);
}

int capacity_maintenance_buffer_mbps(int total_mbps) {
  return capacity_lib::maintenance_buffer_mbps(total_mbps);
}

int capacity_can_support(int total_mbps, int alloc_mbps, int requested_mbps) {
  return capacity_lib::can_support(total_mbps, alloc_mbps, requested_mbps) ? 1 : 0;
}

int capacity_maintenance_buffer_pct(void) { return CAPACITY_MAINTENANCE_BUFFER_PCT; }

const char *capacity_rule(void) { return capacity_lib::rule(); }

const char *capacity_lib_version(void) { return CAPACITY_LIB_VERSION; }

}
