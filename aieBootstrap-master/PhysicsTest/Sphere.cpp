#include "Sphere.h"


Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : Rigidbody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 10, m_colour);
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	Sphere* circle = dynamic_cast<Sphere*>(pOther);
	if (circle != NULL)
	{
		if (m_radius + circle->m_radius < glm::distance(m_position, circle->m_position))
		{
			return true;
		}
		else
			return false;
	}
}
