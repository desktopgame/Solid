#include <Utils.hpp>
#include <cassert>

int main(int argc, char* argv[])
{
    assert(std::string("Hello") == Lib::Utils::String::toMultiByteString(L"Hello"));
    assert(std::string("こんにちわ") == Lib::Utils::String::toMultiByteString(L"こんにちわ"));
    return 0;
}