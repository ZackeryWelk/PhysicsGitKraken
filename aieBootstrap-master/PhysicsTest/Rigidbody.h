#pragma once
#include "PhysicsObject.h"
class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force);
	void applyForceToActor(Rigidbody* actor2, glm::vec2 force);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	glm::vec2 setPosition(glm::vec2 position);
	glm::vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }

	float setElasticity(float elasticity);
	float getElasticity() { return m_elasticity; }

	float SetLinearDrag(float drag);
	float getLinearDrag() { return m_linearDrag; }

	void stop() { m_velocity = glm::vec2(0,0); }

	void resolveCollision(Rigidbody* actor2);

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_rotation; //2D so we need only one float for rotation

	float m_linearDrag;

	float m_elasticity;

	float m_friction;

};

