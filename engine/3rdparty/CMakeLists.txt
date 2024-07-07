set(ThirdPartyFolder "ThirdParty")

if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(glfw)
endif()

if(NOT TARGET spdlog)
    option(SPDLOG_BUILD_EXAMPLE "" OFF)
    option(SPDLOG_INSTALL "" OFF)
    add_subdirectory(spdlog)
endif()

if(NOT TARGET box2d)
    option(BOX2D_BUILD_UNIT_TESTS "Build the Box2D unit tests" OFF)
    option(BOX2D_BUILD_TESTBED "Build the Box2D testbed" OFF)
    add_subdirectory(box2d)
endif()

add_subdirectory(toml11)

if(NOT TARGET lua)
    include(lua.cmake)
endif()