# capacity-lib

The one capacity rule shared by **meridian-telco** (C++ mediation / inventory-api)
and **vantage-telco** (Python / FastAPI). Both estates vendor this repo as a git
submodule at `vendor/capacity-lib` and call it instead of carrying their own
capacity arithmetic, so a location's sellable bandwidth is the same number
whichever system you ask.

## The rule

```
maintenance_buffer = 10% of total            (whole mbps, rounded down)
available          = max(total - allocated - maintenance_buffer, 0)
can_support        = available >= requested
```

The buffer policy is vantage-telco's: it withheld a maintenance buffer per
location and 10% of total was its standard figure. meridian-telco previously
withheld nothing (`available = total - allocated`).

All figures are integer mbps.

## API

`include/capacity_lib/capacity.h`

| C API (stable ABI, used via ctypes) | C++ (`capacity_lib::`) |
| --- | --- |
| `int capacity_available_capacity(int total_mbps, int alloc_mbps, int buffer_mbps)` | `available_capacity(...)` |
| `int capacity_maintenance_buffer_mbps(int total_mbps)` | `maintenance_buffer_mbps(...)` |
| `int capacity_can_support(int total_mbps, int alloc_mbps, int requested_mbps)` (0/1) | `bool can_support(...)` |
| `int capacity_maintenance_buffer_pct(void)` | `CAPACITY_MAINTENANCE_BUFFER_PCT` |
| `const char *capacity_rule(void)` | `rule()` |
| `const char *capacity_lib_version(void)` | `CAPACITY_LIB_VERSION` |

`can_support` applies the standard buffer itself, so callers only pass the
link's total and allocated figures.

## Build and test

```bash
make            # build/libcapacity.a, build/libcapacity.so, test binaries
make check      # C++ unit tests + a plain-C test of the header
```

or with CMake:

```bash
cmake -S . -B build-cmake && cmake --build build-cmake && ctest --test-dir build-cmake
```

## Consuming it

```bash
git submodule add https://github.com/COG-GTM/capacity-lib.git vendor/capacity-lib
```

- C/C++: add `-Ivendor/capacity-lib/include` and compile
  `vendor/capacity-lib/src/capacity.cpp` into your binary (meridian-telco does
  this from its Makefile), or link `build/libcapacity.a`.
- Python: `make -C vendor/capacity-lib lib` and load `build/libcapacity.so`
  with `ctypes` (vantage-telco's `app/inventory/capacity_lib.py`).
