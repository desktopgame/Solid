#include <Utils.hpp>
#include <cassert>

int main(int argc, char* argv[])
{
    using namespace Lib::Utils;
    assert(std::string("Hello") == String::toMultiByteString(L"Hello"));
    assert(std::string("こんにちわ") == String::toMultiByteString(L"こんにちわ"));

    std::string oldText = "こんにちわ、${Name}さん";
    std::string newText = "こんにちわ、dansakaさん";
    std::unordered_map<std::string, std::string> keywords;
    keywords.insert_or_assign("Name", "dansaka");
    assert(String::interpolate(oldText, keywords) == newText);
    return 0;
}