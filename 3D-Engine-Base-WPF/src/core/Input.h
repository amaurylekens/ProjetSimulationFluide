#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <sstream>

class Input
{
    public:
        Input();
        ~Input();
        void updateEvents();
        bool isEnd() const;

        bool getKey(const SDL_Scancode key) const;
        bool mouseMove() const;
        bool getRightButton() const;

        int getX() const;
        int getY() const;

        int getXRel() const;
        int getYRel() const;

        void displayPointer(bool response) const;
        void capturePointer(bool response) const;

    private :
        SDL_Event events;
        bool keys[SDL_NUM_SCANCODES];
        bool mouseButtons[8];

        int x;
        int y;
        int xRel;
        int yRel;

        bool end;

};
#endif // DEF_INPUT
