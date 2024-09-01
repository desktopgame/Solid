#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Lib::Graphics {
class FontMap;
class FontFactory {
public:
    ~FontFactory();
    std::shared_ptr<FontMap> load(const std::string& path);
    bool isOccurredError() const;
    static std::shared_ptr<FontFactory> getInstance();

private:
    explicit FontFactory();
    static std::shared_ptr<FontFactory> m_instance;
    class Impl;
    std::unique_ptr<Impl> m_impl;
    std::map<std::string, std::shared_ptr<FontMap>> m_fontRegistry;
    bool m_occurredError;
};
}
