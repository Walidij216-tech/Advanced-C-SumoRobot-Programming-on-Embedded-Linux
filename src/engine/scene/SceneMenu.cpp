#include "SceneMenu.h"
#include "ImGuiOverlay.h"
#include "Camera.h"

SceneMenu::SceneMenu(Scene*& scene) :
    m_currentScene(scene)
{
}

void SceneMenu::render()
{
    ImGuiOverlay::begin("Scene menu");
    for (auto& scene : m_scenes)
    {
        if (ImGuiOverlay::button(scene.first.c_str())) {
            delete m_currentScene;
            m_currentScene = scene.second();
            Camera::reset();
        }
    }
    ImGuiOverlay::end();
}