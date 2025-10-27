#pragma once
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Lib::Graphics {
class FontMap;
/**
 * 全てのフォントを管理するクラスです。
 */
class FontFactory {
public:
    ~FontFactory();
    /**
     * 指定のフォントからフォントマップをキャッシュして返します。
     * @param path
     */
    std::shared_ptr<FontMap> load(const std::string& path);

    /**
     * フォントに関するエラーが発生していれば true を返します。
     * @return
     */
    bool isOccurredError() const;

    /**
     * 唯一のインスタンスを返します。
     * @return
     */
    static std::shared_ptr<FontFactory> getInstance();

#if SOLID_ENABLE_INTERNAL
    static void destroy();
#endif

private:
    explicit FontFactory();
    static std::shared_ptr<FontFactory> m_instance;
    class Impl;
    std::unique_ptr<Impl> m_impl;
    std::map<std::string, std::shared_ptr<FontMap>> m_fontRegistry;
    bool m_occurredError;
};
}
