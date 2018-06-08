#include "Camera.h"

Camera::Camera() : m_phi(0.0), m_theta(0.0), m_axeVertical(0,0,1), m_orientation(),
m_deplacementLateral(), m_position(), m_pointCible()
{
}

Camera::Camera(vec3 position, vec3 pointCible, vec3 axeVertical) :  m_phi(0.0), m_theta(0.0),
m_axeVertical(axeVertical), m_orientation(), m_deplacementLateral(), m_position(position), m_pointCible(pointCible)
{
    setPointcible(pointCible);
}

Camera::~Camera()
{
}

void Camera::orienter(int xRel, int yRel)
{
    m_phi += -yRel * 0.5;
    m_theta += -xRel * 0.5;

    if(m_phi > 89.0)
    {
        m_phi = 89.0;
    }

    else if(m_phi < -89.0)
    {
        m_phi = -89.0;
    }

    float phiRadian = m_phi * M_PI / 180;
    float thetaRadian = m_theta * M_PI / 180;

    if(m_axeVertical[0] == 1.0)
    {
        m_orientation[0] = sin(phiRadian);
        m_orientation[1] = cos(phiRadian) * cos(thetaRadian);
        m_orientation[2] = cos(phiRadian) * sin(thetaRadian);
    }

    else if(m_axeVertical[1] == 1.0)
    {
        m_orientation[0] = cos(phiRadian) * sin(thetaRadian);
        m_orientation[1] = sin(phiRadian);
        m_orientation[2] = cos(phiRadian) * cos(thetaRadian);
    }

    else
    {
        m_orientation[0] = cos(phiRadian) * cos(thetaRadian);
        m_orientation[1] = cos(phiRadian) * sin(thetaRadian);
        m_orientation[2] = sin(phiRadian);
    }

    m_deplacementLateral = cross(m_axeVertical, m_orientation);
    m_deplacementLateral = normalize(m_deplacementLateral);


    m_pointCible = m_position + m_orientation;
}

void Camera::deplacer(Input const &input)
{
    if(input.mouseMove())
    {
        orienter(input.getXRel(), input.getYRel());
    }

    if(input.getKey(SDL_SCANCODE_UP))
    {
        m_position = m_position + m_orientation * 0.5f;
        m_pointCible = m_position + m_orientation;
    }

    if(input.getKey(SDL_SCANCODE_DOWN))
    {
        m_position = m_position - m_orientation * 0.5f;
        m_pointCible = m_position + m_orientation;
    }

    if(input.getKey(SDL_SCANCODE_LEFT))
    {
        m_position = m_position + m_deplacementLateral * 0.5f;
        m_pointCible = m_position + m_orientation;
    }

    if(input.getKey(SDL_SCANCODE_RIGHT))
    {
        m_position = m_position - m_deplacementLateral * 0.5f;
        m_pointCible = m_position + m_orientation;
    }
}

void Camera::lookAt(mat4 &modelview)
{
    modelview = lookat(m_position, m_pointCible, m_axeVertical);
}

void Camera::setPointcible(vec3 pointCible)
{
    m_orientation = m_pointCible - m_position;
    m_orientation = normalize(m_orientation);

    if(m_axeVertical[0] == 1.0)
    {
        m_phi = asin(m_orientation[0]);
        m_theta = acos(m_orientation[1] / cos(m_phi));

        if(m_orientation[1] < 0)
        {
            m_theta *= -1;
        }
    }

    else if(m_axeVertical[1] == 1.0)
    {
        m_phi = asin(m_orientation[1]);
        m_theta = acos(m_orientation[2] / cos(m_phi));

        if(m_orientation[2] < 0)
        {
            m_theta *= -1;
        }
    }

    else
    {

        m_phi = asin(m_orientation[0]);
        m_theta = acos(m_orientation[2] / cos(m_phi));

        if(m_orientation[2]  < 0)
        {
             m_theta *= -1;
        }
    }

    m_phi = m_phi * 180 / M_PI;
    m_theta = m_theta * 180 / M_PI;
}
