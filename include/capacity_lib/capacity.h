#ifndef CAPACITY_LIB_CAPACITY_H
#define CAPACITY_LIB_CAPACITY_H

/* capacity-lib: the one capacity rule shared by meridian-telco and vantage-telco.

     maintenance_buffer = CAPACITY_MAINTENANCE_BUFFER_PCT % of total (whole mbps, rounded down)
     available          = max(total - allocated - maintenance_buffer, 0)
     can_support        = available >= requested

   All figures are in mbps. The C API is what non-C++ consumers bind to
   (e.g. Python via ctypes); C++ callers can use the capacity_lib namespace. */

#define CAPACITY_LIB_VERSION "1.0.0"
#define CAPACITY_MAINTENANCE_BUFFER_PCT 10

#ifdef __cplusplus
extern "C" {
#endif

int capacity_available_capacity(int total_mbps, int alloc_mbps, int buffer_mbps);
int capacity_maintenance_buffer_mbps(int total_mbps);
int capacity_can_support(int total_mbps, int alloc_mbps, int requested_mbps);
int capacity_maintenance_buffer_pct(void);
const char *capacity_rule(void);
const char *capacity_lib_version(void);

#ifdef __cplusplus
}

namespace capacity_lib {

int available_capacity(int total_mbps, int alloc_mbps, int buffer_mbps);
int maintenance_buffer_mbps(int total_mbps);
bool can_support(int total_mbps, int alloc_mbps, int requested_mbps);
const char *rule();

}  // namespace capacity_lib
#endif

#endif
