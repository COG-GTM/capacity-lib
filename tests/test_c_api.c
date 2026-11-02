/* compiled as C to prove the header is usable from plain C */
#include <stdio.h>
#include <string.h>

#include "capacity_lib/capacity.h"

int main(void) {
  int failures = 0;
  if (capacity_maintenance_buffer_mbps(1000) != 100) failures++;
  if (capacity_available_capacity(1000, 600, capacity_maintenance_buffer_mbps(1000)) != 300) failures++;
  if (capacity_can_support(1000, 600, 300) != 1) failures++;
  if (capacity_can_support(1000, 600, 350) != 0) failures++;
  if (strstr(capacity_rule(), "maintenance_buffer") == NULL) failures++;
  if (failures) {
    printf("%d C API check(s) failed\n", failures);
    return 1;
  }
  printf("capacity-lib: all C API checks passed\n");
  return 0;
}
