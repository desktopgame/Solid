#include <App/Scenes/Game/System/PieceRegistry.hpp>

namespace App::Scenes::Game::System {
std::vector<std::shared_ptr<PieceInfo>> PieceRegistry::s_pieces;
// public
void PieceRegistry::initialize()
{
    s_pieces.emplace_back(std::make_shared<System::PieceInfo>(std::vector<System::Cell> {
                                                                  System::Cell(IntVector2({ 0, 0 })),
                                                                  System::Cell(IntVector2({ 1, 0 })),
                                                                  System::Cell(IntVector2({ 2, 0 })),
                                                              },
        u"サンプルテキスト1"));
    s_pieces.emplace_back(std::make_shared<System::PieceInfo>(std::vector<System::Cell> {
                                                                  System::Cell(IntVector2({ 0, 0 })),
                                                                  System::Cell(IntVector2({ 0, 1 })),
                                                                  System::Cell(IntVector2({ 0, 2 })),
                                                              },
        u"サンプルテキスト2"));
    s_pieces.emplace_back(std::make_shared<System::PieceInfo>(std::vector<System::Cell> {
                                                                  System::Cell(IntVector2({ 0, 0 })),
                                                                  System::Cell(IntVector2({ 1, 0 })),
                                                                  System::Cell(IntVector2({ 2, 0 })),
                                                                  System::Cell(IntVector2({ 3, 0 })),
                                                                  System::Cell(IntVector2({ 3, 1 })),
                                                                  System::Cell(IntVector2({ 3, 2 })),
                                                              },
        u"サンプルテキスト3"));
    s_pieces.emplace_back(std::make_shared<System::PieceInfo>(std::vector<System::Cell> {
                                                                  System::Cell(IntVector2({ 0, 0 })),
                                                                  System::Cell(IntVector2({ 1, 0 })),
                                                                  System::Cell(IntVector2({ 2, 0 })),
                                                                  System::Cell(IntVector2({ 3, 0 })),
                                                                  System::Cell(IntVector2({ 4, 0 })),
                                                                  System::Cell(IntVector2({ 5, 0 })),
                                                              },
        u"サンプルテキスト4"));
    s_pieces.emplace_back(std::make_shared<System::PieceInfo>(std::vector<System::Cell> {
                                                                  System::Cell(IntVector2({ 0, 0 })),
                                                                  System::Cell(IntVector2({ 0, 1 })),
                                                                  System::Cell(IntVector2({ 0, 2 })),
                                                                  System::Cell(IntVector2({ 0, 3 })),
                                                                  System::Cell(IntVector2({ 0, 4 })),
                                                                  System::Cell(IntVector2({ 0, 5 })),
                                                              },
        u"サンプルテキスト5"));
    s_pieces.emplace_back(std::make_shared<System::PieceInfo>(std::vector<System::Cell> { System::Cell(IntVector2({ 0, 0 })) }, u"サンプルテキスト6"));
    s_pieces.emplace_back(std::make_shared<System::PieceInfo>(std::vector<System::Cell> { System::Cell(IntVector2({ 0, 0 })) }, u"サンプルテキスト7"));
    s_pieces.emplace_back(std::make_shared<System::PieceInfo>(std::vector<System::Cell> { System::Cell(IntVector2({ 0, 0 })) }, u"サンプルテキスト8"));
    s_pieces.emplace_back(std::make_shared<System::PieceInfo>(std::vector<System::Cell> { System::Cell(IntVector2({ 0, 0 })) }, u"サンプルテキスト9"));
    s_pieces.emplace_back(std::make_shared<System::PieceInfo>(std::vector<System::Cell> { System::Cell(IntVector2({ 0, 0 })) }, u"サンプルテキスト10"));
}
void PieceRegistry::destroy()
{
    s_pieces.clear();
}
}