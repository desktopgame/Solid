cmake_minimum_required(VERSION 3.25)
project(
    App
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Common Variables
#
set(MODS "${CMAKE_CURRENT_SOURCE_DIR}/mod")
#
# Function
#
function(mod_test target)
    add_subdirectory("${MODS}/${target}/test")
    add_test("${target}-Test" "${CMAKE_CURRENT_BINARY_DIR}/test/Lib-${target}-Test.exe")
    add_custom_target("${target}-Test" COMMAND ${CMAKE_CTEST_COMMAND} --verbose DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/test/Lib-${target}-Test.exe")
endfunction()
#
# Submodules
#
add_subdirectory(./mod/Json)
add_subdirectory(./mod/Input)
add_subdirectory(./mod/Math)
add_subdirectory(./mod/Graphics)
add_subdirectory(./mod/OS)
add_subdirectory(./mod/Scene)
add_subdirectory(./mod/Utils)
#
# Build Options
#
set(SOURCES
    ./include/main.hpp
    ./include/test.hpp
    ./include/Common/Graphics/Node.hpp
    ./include/Common/Graphics/NodeRegistry.hpp
    ./include/Common/Graphics/IParticle.hpp
    ./include/Common/Graphics/ParticleBase.hpp
    ./include/Common/Graphics/ParticleSystem.hpp
    ./include/Common/Graphics/SphericalParticle.hpp
    ./include/Common/Graphics/Telop.hpp
    ./include/Common/Graphics/TelopSystem.hpp
    ./include/Common/Util/FpsController.hpp
    ./include/Scenes/Debug/DebugScene.hpp
    ./include/Scenes/Demo/DemoScene.hpp
    ./include/Scenes/Game/GameScene.hpp
    ./include/Scenes/Game/System/Field.hpp
    ./include/Scenes/Game/System/Entity.hpp
    ./include/Scenes/Game/System/DamageSource.hpp
    ./include/Scenes/Game/System/Entities/BasicEntity.hpp
    ./include/Scenes/Game/System/Entities/PlayerEntity.hpp
    ./include/Scenes/Game/System/Entities/EnemyEntity.hpp
    ./include/Scenes/Game/System/Entities/SlimeEntity.hpp
    ./include/Scenes/Game/System/Entities/ProjectileEntity.hpp
    ./lib/main.cpp
    ./lib/test.cpp
    ./lib/Common/Graphics/Node.cpp
    ./lib/Common/Graphics/NodeRegistry.cpp
    ./lib/Common/Graphics/ParticleSystem.cpp
    ./lib/Common/Graphics/Telop.cpp
    ./lib/Common/Graphics/TelopSystem.cpp
    ./lib/Common/Util/FpsController.cpp
    ./lib/Scenes/Debug/DebugScene.cpp
    ./lib/Scenes/Demo/DemoScene.cpp
    ./lib/Scenes/Game/GameScene.cpp
    ./lib/Scenes/Game/System/Field.cpp
    ./lib/Scenes/Game/System/Entity.cpp
    ./lib/Scenes/Game/System/DamageSource.cpp
    ./lib/Scenes/Game/System/Entities/BasicEntity.cpp
    ./lib/Scenes/Game/System/Entities/PlayerEntity.cpp
    ./lib/Scenes/Game/System/Entities/EnemyEntity.cpp
    ./lib/Scenes/Game/System/Entities/SlimeEntity.cpp
    ./lib/Scenes/Game/System/Entities/ProjectileEntity.cpp
)
source_group(
    TREE
    ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
    ${SOURCES}
)
add_executable(
    App
    ${SOURCES}
)
set_property(
    TARGET
    App
    PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/../../"
)
app_target_compile_params(App)
app_target_link_params(App)
target_include_directories(
    App
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${MODS}/Input/include"
    "${MODS}/Graphics/include"
    "${MODS}/Math/include"
    "${MODS}/OS/include"
    "${MODS}/Scene/include"
    "${MODS}/Utils/include"
)
target_link_libraries(
    App
    PRIVATE
    Lib-Json
    Lib-Input
    Lib-Math
    Lib-Graphics
    Lib-OS
    Lib-Scene
    Lib-Utils
)

enable_testing()
add_subdirectory(./test)
add_test(NAME Test COMMAND "${CMAKE_CURRENT_BINARY_DIR}/test/App-Test.exe" WORKING_DIRECTORY ${REPOSITORY_ROOT})
add_custom_target(Test COMMAND ${CMAKE_CTEST_COMMAND} --verbose DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/test/App-Test.exe")