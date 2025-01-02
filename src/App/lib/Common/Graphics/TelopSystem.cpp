#include <Common/Graphics/Telop.hpp>
#include <Common/Graphics/TelopSystem.hpp>

namespace App::Common::Graphics {
std::vector<std::shared_ptr<Telop>> TelopSystem::s_freeTable;
std::vector<std::shared_ptr<Telop>> TelopSystem::s_usedTable;

std::shared_ptr<FontMap> TelopSystem::s_fontMap;
std::shared_ptr<CpuBuffer> TelopSystem::s_vertexBuffer;
std::shared_ptr<CpuBuffer> TelopSystem::s_indexBuffer;
int32_t TelopSystem::s_indexLength;

void TelopSystem::initialize()
{
    s_fontMap = FontFactory::getInstance()->load("./assets/Fonts/NotoSansJP-Regular.ttf");
    s_fontMap->load(Telop::k_fontSize, u"あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほられるれろまみうめもやゆよらりるれろわをん");
    s_fontMap->load(Telop::k_fontSize, u"0123456789");
    s_fontMap->load(Telop::k_fontSize, u"abcdefghijklmnopqrstuvwxyz");
    s_fontMap->load(Telop::k_fontSize, u"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    std::vector<VertexTexCoord2D> vertices;
    std::vector<uint32_t> indices;
    Polygon::generateRect(vertices, indices);

    s_vertexBuffer = CpuBuffer::create();
    s_vertexBuffer->allocate(sizeof(VertexTexCoord2D) * vertices.size());
    s_vertexBuffer->update(vertices.data());

    s_indexBuffer = CpuBuffer::create();
    s_indexBuffer->allocate(sizeof(uint32_t) * indices.size());
    s_indexBuffer->update(indices.data());
    s_indexLength = indices.size();
}

void TelopSystem::request(const std::u16string& text, const Vector3& position, const Vector3& color, float duration)
{
    if (s_freeTable.size() > 0) {
        std::shared_ptr<Telop> telop = s_freeTable.back();
        telop->text = text;
        telop->position = position;
        telop->color = color;
        telop->duration = duration;
        s_freeTable.pop_back();

        telop->initialize();
        s_usedTable.emplace_back(telop);
    } else {
        std::shared_ptr<Telop> telop = std::make_shared<Telop>();
        telop->text = text;
        telop->position = position;
        telop->color = color;
        telop->duration = duration;

        telop->initialize();
        s_usedTable.emplace_back(telop);
    }
}

void TelopSystem::draw()
{
    for (auto& telop : s_usedTable) {
        telop->update();
    }

    std::vector<std::shared_ptr<Telop>>& src = s_usedTable;
    std::vector<std::shared_ptr<Telop>>& dst = s_freeTable;

    for (auto& s : src) {
        if (s->isExpired()) {
            s->destroy();
            dst.emplace_back(s);
        }
    }

    auto iter = std::remove_if(src.begin(), src.end(), [](const auto& e) -> bool {
        return e->isExpired();
    });
    src.erase(iter, src.end());

    for (auto& telop : s_usedTable) {
        telop->draw(s_fontMap, s_vertexBuffer, s_indexBuffer, s_indexLength);
    }
}

void TelopSystem::stop()
{
    std::vector<std::shared_ptr<Telop>>& src = s_usedTable;
    std::vector<std::shared_ptr<Telop>>& dst = s_freeTable;

    for (auto& s : src) {
        s->destroy();
        dst.emplace_back(s);
    }

    src.clear();
}

void TelopSystem::destroy()
{
    s_freeTable.clear();
    s_usedTable.clear();

    s_vertexBuffer = nullptr;
    s_indexBuffer = nullptr;
}
}