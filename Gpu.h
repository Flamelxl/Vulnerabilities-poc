#pragma once

#include <windows.h>
#include <d3dkmthk.h>

struct NvEscape {
  DWORD magic;
  WORD unknown_4;
  WORD unknown_6;
  DWORD size;
  DWORD magic2;
  DWORD code;
  DWORD unknown[7];
  BYTE payload[0];
};

static_assert(sizeof(NvEscape) == 0x30, "sizeof(NvEscape)");

NvEscape* MakeEscape(DWORD code, DWORD magic2, size_t payload_size);

class Gpu {
public:
  Gpu();
  ~Gpu();

  bool Init();
  NTSTATUS Escape(void* data, size_t size);

private:
  D3DKMT_HANDLE adapter_handle_ = 0;
  D3DKMT_HANDLE device_handle_ = 0;
};

