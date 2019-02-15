#pragma once
#include "Rigidbody.h"
class Square : public Rigidbody
{
public:
	Square(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec2 extents, glm::vec4 colour);
	~Square();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);

	glm::vec2 getExtents() { return m_extents; }

	glm::vec4 getColour() { return m_colour; }


protected:
	glm::vec2 m_extents;
	glm::vec4 m_colour;
};

