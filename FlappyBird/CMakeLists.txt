# Flappy Bird clone CMakeLists.txt
add_library(FlappyBird STATIC
    src/Background.cpp
    src/Bird.cpp
    src/ButtonElement.cpp
    src/FlappyBirdWorld.cpp
    src/Floor.cpp
    src/GameOverScreen.cpp
    src/HUDScreen.cpp
    src/LabelElement.cpp
    src/ObstaclePair.cpp
    src/PanelElement.cpp
    src/ScoreManager.cpp
    src/ScoreNumber.cpp
    src/SFXManager.cpp
    src/StartScreen.cpp
)

target_include_directories(FlappyBird PUBLIC include)

target_link_libraries(FlappyBird
    PUBLIC
        Core
        Framework
        SFML::Audio
    PRIVATE
        # Only FlappyBirdWorld.cpp uses ImGui for debug panel.
         # Only in Debug: ImGui does not link in Release.
        $<$<CONFIG:Debug>:ImGui-SFML::ImGui-SFML>
)
