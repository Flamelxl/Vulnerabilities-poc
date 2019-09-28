#include "stdafx.h"
#include "Gpu.h"

namespace {

  D3DKMT_HANDLE InitGpuAdapter(HDC hdc) {
    D3DKMT_OPENADAPTERFROMHDC open_adapter_data = {};
    open_adapter_data.hDc = hdc;
    if (D3DKMTOpenAdapterFromHdc(&open_adapter_data))
      return 0;
    return open_adapter_data.hAdapter;
  }

  D3DKMT_HANDLE CreateGpuDevice(D3DKMT_HANDLE hAdapter) {
    D3DKMT_CREATEDEVICE create_device_data = {};
    create_device_data.hAdapter = hAdapter;
    if (D3DKMTCreateDevice(&create_device_data))
      return 0;

    return create_device_data.hDevice;
  }

}

NvEscape* MakeEscape(DWORD code, DWORD magic2, size_t payload_size) {
  size_t total_size = sizeof(NvEscape) + payload_size;
  NvEscape* escape = reinterpret_cast<NvEscape*>(malloc(total_size));
  memset(escape, 0, total_size);
  escape->magic = 0x4e564441;
  escape->magic2 = magic2;
  escape->unknown_6 = 1;
  escape->size = sizeof(NvEscape) + payload_size;
  escape->code = code;

  return escape;
}

Gpu::Gpu() {
}

Gpu::~Gpu() {
}

bool Gpu::Init() {
  HDC hdc = GetDC(nullptr);
  adapter_handle_ = InitGpuAdapter(hdc);
  if (!adapter_handle_)
    return false;

  device_handle_ = CreateGpuDevice(adapter_handle_);
  if (!device_handle_)
    return false;

  return true;
}

NTSTATUS Gpu::Escape(void* data, size_t size) {
  D3DKMT_ESCAPE escape_data = {};
  escape_data.hAdapter = adapter_handle_;
  escape_data.hDevice = device_handle_;
  escape_data.pPrivateDriverData = data;
  escape_data.PrivateDriverDataSize = static_cast<UINT>(size);

  return D3DKMTEscape(&escape_data);
}
