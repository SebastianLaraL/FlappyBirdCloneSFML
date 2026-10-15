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

# Modules.
add_subdirectory(Core)
add_subdirectory(Framework)
add_subdirectory(FlappyBird)

# Executable.
add_executable(main src/main.cpp)

target_link_libraries(main PRIVATE FlappyBird)

# Do not display a window in Release config on Windows.
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
#if(MSVC)
#    target_compile_options(main PRIVATE
#        $<$<CONFIG:Debug>:/Od>
#        $<$<CONFIG:Debug>:/Zi>
#        $<$<CONFIG:Release>:/O2>
#    )
#endif()
# ==========================================
# MAXIMUM RELEASE OPTIMIZATIONS
# ==========================================
if(MSVC)
    add_compile_options(
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Release>:/Ob3>
        $<$<CONFIG:Release>:/Oi>
        $<$<CONFIG:Release>:/Ot>
        $<$<CONFIG:Release>:/GL>
        $<$<CONFIG:Release>:/Gy>
    )

    add_link_options(
        $<$<CONFIG:Release>:/LTCG>
        $<$<CONFIG:Release>:/OPT:REF>
        $<$<CONFIG:Release>:/OPT:ICF>
    )
else()
    add_compile_options(
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-DNDEBUG>
        $<$<CONFIG:Release>:-ffunction-sections>
        $<$<CONFIG:Release>:-fdata-sections>
        $<$<CONFIG:Release>:-flto>
    )

    add_link_options(
        $<$<CONFIG:Release>:-flto>
        $<$<CONFIG:Release>:-Wl,--gc-sections>
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