cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Default Debug if using a only-one configuration (Ninja or Make)
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

include(FetchContent)

# SFML 3.1.0
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

# ImGui
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9b
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)

# ImGui-SFML
FetchContent_Declare(imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
)

set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML imgui)

# ImGui-SFML will use the already-existing SFML targets.
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "" FORCE)

FetchContent_MakeAvailable(imgui-sfml)

# Executable
add_executable(main src/main.cpp)

target_compile_features(main PRIVATE cxx_std_20)

target_compile_definitions(main PRIVATE
    SFML_STATIC

    # Specific definitions for each config.
    $<$<CONFIG:Debug>:GAME_DEBUG>
    $<$<CONFIG:Release>:GAME_RELEASE>
)

target_link_libraries(main PRIVATE
    SFML::Graphics
    SFML::Audio
    ImGui-SFML::ImGui-SFML
)

# Do not display a window in Release config.
# Debug keeps it to allow logs.
if(WIN32)
    set_property(TARGET main PROPERTY
        WIN32_EXECUTABLE "$<CONFIG:Release>"
    )

    # Allows to keep main() as the entry point in MSVC
    # although Release uses the Windows subsystem.
    if(MSVC)
        target_link_options(main PRIVATE
            "$<$<CONFIG:Release>:/ENTRY:mainCRTStartup>"
        )
    endif()
endif()

# Debugging symbols and optimization:
# CMake automatically applies the standard Debug/Release flags.
# Add explicit options for MSVC.
if(MSVC)
    target_compile_options(main PRIVATE
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Release>:/O2>
    )
endif()

# Startup project in Visual Studio.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT main
)

# Copy Assets next to the executable in both configs.
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Assets"
        "$<TARGET_FILE_DIR:main>/Assets"
    COMMENT "Copying Assets to build folder"
)