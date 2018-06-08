#include "Input.h"

Input::Input() : x(0), y(0), xRel(0), yRel(0), end(false)
{
    // Initialize all the SDL2 input

    for(int i(0); i < SDL_NUM_SCANCODES; i++)
    {
        keys[i] = false;
    }

    for(int i(0); i < 8; i++)
    {
        mouseButtons[i] = false;
    }
}

Input::~Input()
{
}

void Input::updateEvents()
{
    // Check and update all the SDL2 input

    xRel = 0;
    yRel = 0;

    while(SDL_PollEvent(&events))
    {
        switch(events.type)
        {
            case SDL_KEYDOWN:
                keys[events.key.keysym.scancode] = true;
                break;

            case SDL_KEYUP:
                keys[events.key.keysym.scancode] = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                mouseButtons[events.button.button] = true;
                break;

            case SDL_MOUSEBUTTONUP:
                mouseButtons[events.button.button] = false;
                break;

            case SDL_MOUSEMOTION:
                x = events.motion.x;
                y = events.motion.y;

                xRel = events.motion.xrel;
                yRel = events.motion.yrel;

                break;

            case SDL_WINDOWEVENT:
                if(events.window.event == SDL_WINDOWEVENT_CLOSE)
                    end = true;
                break;
        }
    }
}

bool Input::isEnd() const
{
    return end;
}

bool Input::getKey(const SDL_Scancode key) const
{
    // Get the state of a specified key

    return keys[key];
}

bool Input::getRightButton() const
{
    return mouseButtons[SDL_BUTTON_RIGHT];
}

bool Input::mouseMove() const
{
    // Check if the mouse has moved

    if(xRel == 0 && yRel == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int Input::getX() const
{
    return x;
}

int Input::getY() const
{
    return y;
}

int Input::getXRel() const
{
    return xRel;
}

int Input::getYRel() const
{
    return yRel;
}

void Input::displayPointer(bool response) const
{
    // The pointer is (true) or not (false) display in the SDL2 window

    if(response)
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
    else
    {
        SDL_ShowCursor(SDL_DISABLE);
    }
}

void Input::capturePointer(bool response) const
{
    // The pointer is (true) or not (false) captured in the SDL2 window

    if(response)
        SDL_SetRelativeMouseMode(SDL_TRUE);

    else
        SDL_SetRelativeMouseMode(SDL_FALSE);
}

