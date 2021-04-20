#include "config.h"

#include <GLFW/glfw3.h>

#include <sstream>

namespace Engine
{

bool Config::OnKeyPressed(int key, const std::set<int>& pressedKeys)
{
    if (key == GLFW_KEY_EQUAL || key == GLFW_KEY_MINUS)
    {
        float delta = key == GLFW_KEY_MINUS ? -1.0f : 1.0f;
        if (pressedKeys.count(GLFW_KEY_LEFT_CONTROL))
        {
            if (pressedKeys.count(GLFW_KEY_LEFT_SHIFT))
            {
                depth += static_cast<uint32_t>(delta);
            }
            else
            {
                waveCount += delta * 0.1;
            }
        }
        else if (pressedKeys.count(GLFW_KEY_LEFT_ALT))
        {
            speed += delta * 0.01;
        }
        else if (pressedKeys.count(GLFW_KEY_LEFT_SHIFT))
        {
            amplitude += delta * 0.01;
        }
    }
    else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_3)
    {
        texture = static_cast<uint32_t>(key - GLFW_KEY_1);
    }
    else if (key == GLFW_KEY_W &&
        pressedKeys.count(GLFW_KEY_LEFT_CONTROL) &&
        pressedKeys.count(GLFW_KEY_LEFT_SHIFT))
    {
        wireframe = !wireframe;
        return true;
    }
    return false;
}

std::string Config::ToString() const
{
    std::stringstream ss;
    ss << "amplitude: " << amplitude << "; ";
    ss << "waveCount: " << waveCount << "; ";
    ss << "speed: " << speed << "; ";
    ss << "depth: " << depth << "; ";
    ss << "texture: " << texture << "; ";
    ss << "wireframe: " << wireframe << ";";
    return ss.str();
}

}
