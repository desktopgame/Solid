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

    std::basic_stringbuf<char> ibuf("1,2,3,4");
    std::vector<Lib::Math::Vector4> v;
    std::istream is(&ibuf);
    IO::deserializeTile(is, v);
    assert(v.size() == 1);
    assert(v.at(0) == Lib::Math::Vector4({ 1, 2, 3, 4 }));

    std::basic_stringbuf<char> obuf;
    std::ostream os(&obuf);
    IO::serializeTile(os, v);
    assert(obuf.str() == "1,2,3,4\n");
    return 0;
}