#include <Graphics/Internal/Error.hpp>
#include <d3d12.h>
#include <iostream>

namespace Lib::Graphics::Internal {
int64_t Error::show(int64_t hr, const char* file, int line)
{
    LONG nHr = static_cast<LONG>(hr);
    switch (nHr) {
    case D3D12_ERROR_ADAPTER_NOT_FOUND:
        std::cout << "D3D12_ERROR_ADAPTER_NOT_FOUND" << " " << file << ":" << line << std::endl;
        break;
    case D3D12_ERROR_DRIVER_VERSION_MISMATCH:
        std::cout << "D3D12_ERROR_DRIVER_VERSION_MISMATCH" << " " << file << ":" << line << std::endl;
        break;
    case DXGI_ERROR_INVALID_CALL:
        std::cout << "DXGI_ERROR_INVALID_CALL" << " " << file << ":" << line << std::endl;
        break;
    case DXGI_ERROR_WAS_STILL_DRAWING:
        std::cout << "DXGI_ERROR_WAS_STILL_DRAWING" << " " << file << ":" << line << std::endl;
        break;
    case E_FAIL:
        std::cout << "E_FAIL" << " " << file << ":" << line << std::endl;
        break;
    case E_INVALIDARG:
        std::cout << "E_INVALIDARG" << " " << file << ":" << line << std::endl;
        break;
    case E_OUTOFMEMORY:
        std::cout << "E_OUTOFMEMORY" << " " << file << ":" << line << std::endl;
        break;
    case E_NOTIMPL:
        std::cout << "E_NOTIMPL" << " " << file << ":" << line << std::endl;
        break;
    default:
        if (FAILED(nHr)) {
            std::cout << std::showbase << std::hex << nHr << std::noshowbase << " " << file << ":" << line << std::endl;
        }
        break;
    }
    return hr;
}
}