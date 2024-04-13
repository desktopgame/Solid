#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <iostream>
#include <main.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <tchar.h>
#include <vector>
#include <wrl/client.h>

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

int main(int argc, char* argv[])
{
    WNDCLASSEX w = {};
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProcedure;
    w.lpszClassName = _T("DX12");
    w.hInstance = GetModuleHandle(nullptr);
    RegisterClassEx(&w);

    RECT wrc = { 0, 0, 800, 600 };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    HWND hwnd = CreateWindow(
        w.lpszClassName,
        _T("DX12"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wrc.right - wrc.left,
        wrc.bottom - wrc.top,
        nullptr,
        nullptr,
        w.hInstance,
        nullptr);
    ShowWindow(hwnd, SW_SHOW);

    using Microsoft::WRL::ComPtr;
    // Debug Layer
    ComPtr<ID3D12Debug> debugController = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
    }
    // Factory
    ComPtr<IDXGIFactory6> dxgiFactory = nullptr;
    UINT flags = DXGI_CREATE_FACTORY_DEBUG;
    if (FAILED(CreateDXGIFactory2(flags, IID_PPV_ARGS(&dxgiFactory)))) {
        throw std::runtime_error("failed CreateDXGIFactory2()");
    }
    // Adapter
    std::vector<ComPtr<IDXGIAdapter>> adapters;
    ComPtr<IDXGIAdapter> mainAdapter = nullptr;
    for (uint32_t i = 0; dxgiFactory->EnumAdapters(i, &mainAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
        adapters.emplace_back(mainAdapter);
    }
    if (adapters.empty()) {
        throw std::runtime_error("failed EnumAdapters()");
    }
    // first adapter is primary adapter.
    // see: https://learn.microsoft.com/ja-jp/windows/win32/api/dxgi/nf-dxgi-idxgifactory-enumadapters
    mainAdapter = adapters.at(0);
    adapters.clear();
    // Feature level
    std::optional<D3D_FEATURE_LEVEL> useLevel;
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
    };
    ComPtr<ID3D12Device> device = nullptr;
    for (D3D_FEATURE_LEVEL level : levels) {
        HRESULT deviceStatus = D3D12CreateDevice(mainAdapter.Get(), level, IID_PPV_ARGS(&device));
        if (SUCCEEDED(deviceStatus)) {
            useLevel = level;
            break;
        }
    }
    if (!useLevel) {
        throw std::runtime_error("failed D3D12CreateDevice()");
    }

    MSG msg = {};
    while (true) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT) {
            break;
        }
    }
    UnregisterClass(w.lpszClassName, w.hInstance);
    return 0;
}