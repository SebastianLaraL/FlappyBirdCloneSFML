# Core: basic types and math utilities. Only headers.
add_library(Core INTERFACE)

target_include_directories(Core INTERFACE include)

target_compile_features(Core INTERFACE cxx_std_20)

# Compliation settings of the entire project.
# Any target that links with Core inherits them.
target_compile_definitions(Core INTERFACE
    SFML_STATIC

    # Specific definitions for each config.
    $<$<CONFIG:Debug>:GAME_DEBUG>
    $<$<CONFIG:Release>:GAME_RELEASE>
)

# Debug symbols and optimization:
# Add explicit settings for MSVC.
if(MSVC)
    target_compile_options(Core INTERFACE
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Release>:/O2>
    )
endif()
