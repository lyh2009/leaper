set(ThirdPartyFolder "${CMAKE_SOURCE_DIR}/engine/3rdparty")

if(NOT TARGET glad)
    include(glad.cmake)
endif()


if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(glfw)
endif()

if(NOT TARGET glm)
    option(BUILD_STATIC_LIBS "" ON)
    option(BUILD_TESTING "" OFF)
    option(GLM_TEST_ENABLE "" OFF)
    add_subdirectory(glm)
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

if(NOT TARGET FMOD)
    include(FMOD.cmake)
endif()

if(NOT TARGET lua)
    include(lua.cmake)
endif()