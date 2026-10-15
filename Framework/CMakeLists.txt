add_library(Framework STATIC
    src/Application.cpp
    src/Entity.cpp
    src/UIScreen.cpp
    src/World.cpp
)

target_include_directories(Framework PUBLIC include)

target_link_libraries(Framework
    PUBLIC
        Core
        SFML::Graphics
    PRIVATE
        # Only Application.cpp uses ImGui; none of the framework's headers expose it.
        ImGui-SFML::ImGui-SFML
         # Only DEBUG: Release config does not link ImGui.
        $<$<CONFIG:Debug>:ImGui-SFML::ImGui-SFML>
)

# Precompiled header (STL + types). It is passed to the targets that link with Framework.
target_precompile_headers(Framework PUBLIC include/PCH.hpp)
