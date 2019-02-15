#include "Square.h"




Square::Square(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, glm::vec4 colour) : Rigidbody(BOX, position, velocity, 0, mass)
{
	m_extents = extents;
	m_colour = colour;
}

Square::~Square()
{
}

void Square::makeGizmo()
{
	aie::Gizmos::add2DAABB(m_position, m_extents, m_colour);
}

bool Square::checkCollision(PhysicsObject * pOther)
{
	return false;
}

