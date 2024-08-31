#include <Graphics/Constant.hpp>
#include <Graphics/Texture.hpp>

namespace Lib::Graphics {
using Microsoft::WRL::ComPtr;
std::vector<std::shared_ptr<Constant>> Constant::s_freeVec;
std::vector<std::shared_ptr<Constant>> Constant::s_usedVec;
// public
std::shared_ptr<Constant> Constant::rent(Layout layout)
{
    auto iter = std::find_if(s_freeVec.begin(), s_freeVec.end(), [layout](const std::shared_ptr<Constant>& constant) -> bool {
        return layout == constant->getLayout();
    });
    if (iter == s_freeVec.end()) {
        auto param = std::shared_ptr<Constant>(new Constant(layout));
        s_usedVec.emplace_back(param);
        return param;
    }
    auto cache = *iter;
    s_freeVec.erase(iter);
    s_usedVec.emplace_back(cache);
    return cache;
}
void Constant::release()
{
    for (auto cache : s_usedVec) {
        s_freeVec.emplace_back(cache);
    }
    s_usedVec.clear();
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
        if (m_layout.useLightDirection()) {
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
        if (m_layout.useLightDirection()) {
            defineConstant((sizeof(Math::Vector3) + 0xff) & ~0xff);
        }
        // define view
        defineConstantView(CbMatrixIndex, 0);
        if (m_layout.useTexture()) {
            defineTextureView(m_texture, 1);
            if (m_layout.useColor()) {
                defineConstantView(CbColorIndex, 2);
            } else if (m_layout.useLightDirection()) {
                defineConstantView(CbLightDirectionIndex, 2);
            }
        } else if (m_layout.useColor()) {
            defineConstantView(CbColorIndex, 1);
            if (m_layout.useLightDirection()) {
                defineConstantView(CbLightDirectionIndex, 2);
            }
        }
    }
    if (m_isDirty) {
        // transfrom
        {
            void* mapMatrix = nullptr;
            if (FAILED(m_resources.at(CbMatrixIndex)->Map(0, nullptr, (void**)&mapMatrix))) {
                throw std::runtime_error("failed Map()");
            } else {
                // Math::Matrix matrix = getTransform();
                // ::memcpy(mapMatrix, matrix.data(), sizeof(Math::Matrix));
                // m_resources.at(CbMatrixIndex)->Unmap(0, nullptr);
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
        // light direction
        if (m_layout.useLightDirection()) {
            void* mapLightDirection = nullptr;
            if (FAILED(m_resources.at(CbLightDirectionIndex)->Map(0, nullptr, (void**)&mapLightDirection))) {
                throw std::runtime_error("failed Map()");
            } else {
                Math::Vector3 lightDirection = getLightDirection();
                ::memcpy(mapLightDirection, lightDirection.data(), sizeof(Math::Vector3));
                m_resources.at(CbLightDirectionIndex)->Unmap(0, nullptr);
            }
        }
    }
    m_isDirty = false;
}

void Constant::setModelMatrix(const Math::Matrix& modelMatrix)
{
    m_isDirty = true;
    m_modelMatrix = modelMatrix;
}
Math::Matrix Constant::getModelMatrix() const { return m_modelMatrix; }

void Constant::setViewMatrix(const Math::Matrix& viewMatrix)
{
    m_isDirty = true;
    m_viewMatrix = viewMatrix;
}
Math::Matrix Constant::getViewMatrix() const { return m_viewMatrix; }

void Constant::setProjectionMatrix(const Math::Matrix& projectionMatrix)
{
    m_isDirty = true;
    m_projectionMatrix = projectionMatrix;
}
Math::Matrix Constant::getProjectionMatrix() const { return m_projectionMatrix; }

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

void Constant::setLightDirection(const Math::Vector3& lightDirection)
{
    if (!m_layout.useLightDirection()) {
        throw std::runtime_error("missmatch interface.");
    }
    m_isDirty = true;
    m_lightDirection = lightDirection;
}
Math::Vector3 Constant::getLightDirection() const { return m_lightDirection; }

Constant::Layout Constant::getLayout() const { return m_layout; }
// internal
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Constant::getID3D12DescriptorHeap() const { return m_descriptorHeap; }
// private
Constant::Constant(Layout layout)
    : m_isDirty(true)
    , m_modelMatrix()
    , m_viewMatrix()
    , m_projectionMatrix()
    , m_texture()
    , m_color()
    , m_lightDirection()
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