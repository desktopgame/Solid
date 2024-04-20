#pragma once
#include <memory>
#include <wrl/client.h>

namespace Lib::Graphics {
class Buffer {
public:
    static std::shared_ptr<Buffer> create();

    ~Buffer();

    void allocate(size_t size);
    void update(const void* data);

    uint64_t getVirtualAddress() const;
    size_t getSize() const;

private:
    Buffer();

    size_t m_size;

    class Impl;
    std::shared_ptr<Impl> m_impl;
};
}