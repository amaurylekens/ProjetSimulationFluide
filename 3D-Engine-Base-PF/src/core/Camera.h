#ifndef CAMERA_H
#define CAMERA_H

#include "opengl.h"
#include "Input.h"
#include "vmath.h"

using namespace vmath;


class Camera
{
    public:
        Camera();
        Camera(vec3 position, vec3 pointCible, vec3 axeVertical);
        ~Camera();

        void orienter(int xRel, int yRel);
        void deplacer(Input const &input);
        void lookAt(mat4 &modelview);
        void setPointcible(vec3 pointCible);

    private:
        float m_phi;
        float m_theta;

        vec3 m_axeVertical;

        vec3 m_orientation;
        vec3 m_deplacementLateral;

        vec3 m_position;
        vec3 m_pointCible;

};
#endif 
