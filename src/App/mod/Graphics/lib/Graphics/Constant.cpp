#include <Graphics/Constant.hpp>
#include <Graphics/Texture.hpp>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
// public
std::shared_ptr<Constant> Constant::create(Layout layout)
{
    auto param = std::shared_ptr<Constant>(new Constant(layout));
    return param;
}
Constant::~Constant() { }
void Constant::update()
{
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    if (!m_descriptorHeap) {
        // descriptor heap
        D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
        descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        descHeapDesc.NodeMask = 0;
        descHeapDesc.NumDescriptors = 1;
        if (m_layout.useTexture()) {
            descHeapDesc.NumDescriptors++;
        }
        if (m_layout.useColor()) {
            descHeapDesc.NumDescriptors++;
        }
        descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        if (FAILED(device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descriptorHeap)))) {
            throw std::runtime_error("failed CreateDescriptorHeap()");
        }
        // define constants
        defineConstant((sizeof(Math::Matrix) + 0xff) & ~0xff);
        if (m_layout.useColor()) {
            defineConstant((sizeof(Math::Vector4) + 0xff) & ~0xff);
        }
        // define view
        defineConstantView(CbMatrixIndex, 0);
        if (m_layout.useTexture()) {
            defineTextureView(m_texture, 1);
            if (m_layout.useColor()) {
                defineConstantView(CbColorIndex, 2);
            }
        } else if (m_layout.useColor()) {
            defineConstantView(CbColorIndex, 1);
        }
    }
    if (m_isDirty) {
        // transfrom
        {
            void* mapMatrix = nullptr;
            if (FAILED(m_resources.at(CbMatrixIndex)->Map(0, nullptr, (void**)&mapMatrix))) {
                throw std::runtime_error("failed Map()");
            } else {
                Math::Matrix matrix = getTransform();
                ::memcpy(mapMatrix, matrix.data(), sizeof(Math::Matrix));
                m_resources.at(CbMatrixIndex)->Unmap(0, nullptr);
            }
        }
        // color
        if (m_layout.useColor()) {
            void* mapColor = nullptr;
            if (FAILED(m_resources.at(CbColorIndex)->Map(0, nullptr, (void**)&mapColor))) {
                throw std::runtime_error("failed Map()");
            } else {
                Math::Vector4 color = getColor();
                ::memcpy(mapColor, color.data(), sizeof(Math::Vector4));
                m_resources.at(CbColorIndex)->Unmap(0, nullptr);
            }
        }
    }
    m_isDirty = false;
}

void Constant::setTransform(const Math::Matrix& transform)
{
    m_isDirty = true;
    m_transform = transform;
}
Math::Matrix Constant::getTransform() const { return m_transform; }

void Constant::setTexture(const std::shared_ptr<Texture>& texture)
{
    if (!m_layout.useTexture()) {
        throw std::runtime_error("missmatch interface.");
    }
    m_isDirty = true;
    m_texture = texture;
}
std::shared_ptr<Texture> Constant::getTexture() const { return m_texture; }

void Constant::setColor(const Math::Vector4& color)
{
    if (!m_layout.useColor()) {
        throw std::runtime_error("missmatch interface.");
    }
    m_isDirty = true;
    m_color = color;
}
Math::Vector4 Constant::getColor() const { return m_color; }

Constant::Layout Constant::getLayout() const { return m_layout; }
// internal
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Constant::getID3D12DescriptorHeap() const { return m_descriptorHeap; }
// private
Constant::Constant(Layout layout)
    : m_isDirty(true)
    , m_transform()
    , m_texture()
    , m_color()
    , m_layout(layout)
    , m_descriptorHeap(nullptr)
    , m_resources()
{
}

void Constant::defineConstant(uint64_t width)
{
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    D3D12_RESOURCE_DESC resDesc = {};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = width;
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.Format = DXGI_FORMAT_UNKNOWN;
    resDesc.SampleDesc.Count = 1;
    resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    m_resources.emplace_back(nullptr);
    if (FAILED(device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_resources.back())))) {
        throw std::runtime_error("failed CreateCommittedResource()");
    }
}

void Constant::defineConstantView(int32_t constantIndex, int32_t slotIndex)
{
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    uint32_t unitSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    heapHandle.ptr += unitSize * slotIndex;

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvColorDesc = {};
    auto constant = m_resources.at(constantIndex);
    cbvColorDesc.BufferLocation = constant->GetGPUVirtualAddress();
    cbvColorDesc.SizeInBytes = constant->GetDesc().Width;
    device->CreateConstantBufferView(&cbvColorDesc, heapHandle);
}

void Constant::defineTextureView(const std::shared_ptr<Texture>& texture, int32_t slotIndex)
{
    auto device = Engine::getInstance()->getDevice()->getID3D12Device();
    uint32_t unitSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    heapHandle.ptr += unitSize * slotIndex;

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    auto texBuff = std::any_cast<ComPtr<ID3D12Resource>>(texture->getID3D12Resource());
    device->CreateShaderResourceView(texBuff.Get(), &srvDesc, heapHandle);
}
}