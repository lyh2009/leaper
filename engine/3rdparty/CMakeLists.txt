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

if(NOT TARGET assimp)
    OPTION( BUILD_SHARED_LIBS
      "Build package with shared libraries."
      OFF
    )
    OPTION( ASSIMP_BUILD_FRAMEWORK
      "Build package as Mac OS X Framework bundle."
      OFF
    )
    OPTION( ASSIMP_DOUBLE_PRECISION
      "Set to ON to enable double precision processing"
      OFF
    )
    OPTION( ASSIMP_OPT_BUILD_PACKAGES
      "Set to ON to generate CPack configuration files and packaging targets"
      OFF
    )
    OPTION( ASSIMP_ANDROID_JNIIOSYSTEM
      "Android JNI IOSystem support is active"
      OFF
    )
    OPTION( ASSIMP_NO_EXPORT
      "Disable Assimp's export functionality."
      OFF
    )
    OPTION( ASSIMP_BUILD_ZLIB
      "Build your own zlib"
      OFF
    )
    OPTION( ASSIMP_BUILD_ASSIMP_TOOLS
      "If the supplementary tools for Assimp are built in addition to the library."
      OFF
    )
    OPTION ( ASSIMP_BUILD_SAMPLES
      "If the official samples are built as well (needs Glut)."
      OFF
    )
    OPTION ( ASSIMP_BUILD_TESTS
      "If the test suite for Assimp is built in addition to the library."
      OFF
    )
    OPTION ( ASSIMP_COVERALLS
      "Enable this to measure test coverage."
      OFF
    )
    OPTION( ASSIMP_INSTALL
      "Disable this if you want to use assimp as a submodule."
      OFF
    )
    OPTION ( ASSIMP_ERROR_MAX
      "Enable all warnings."
      OFF
    )
    OPTION ( ASSIMP_ASAN
      "Enable AddressSanitizer."
      OFF
    )
    OPTION ( ASSIMP_UBSAN
      "Enable Undefined Behavior sanitizer."
      OFF
    )
    OPTION ( ASSIMP_BUILD_DOCS
      "Build documentation using Doxygen."
      OFF
    )
    OPTION( ASSIMP_INJECT_DEBUG_POSTFIX
      "Inject debug postfix in .a/.so/.dll lib names"
      OFF
    )

    OPTION ( ASSIMP_IGNORE_GIT_HASH
       "Don't call git to get the hash."
       OFF
    )
    add_subdirectory(assimp)
endif()