#include "Rigidbody.h"
#define MIN_LINEAR_THRESHOLD 0.1f


Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) : PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_mass = mass;
	m_elasticity = 1;
	m_linearDrag = 0.1f;
	m_friction = 0.2;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	applyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
	m_velocity -= m_velocity * m_linearDrag * timeStep;
//	glm::vec2 friction = -m_velocity * m_friction;

	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
}

void Rigidbody::debug()
{
}

void Rigidbody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void Rigidbody::applyForceToActor(Rigidbody * actor2, glm::vec2 force)
{
	applyForce(-force);
	actor2->applyForce(force);
}

glm::vec2 Rigidbody::setPosition(glm::vec2 position)
{
	m_position = position;
	return m_position;
}

float Rigidbody::setElasticity(float elasticity)
{
	m_elasticity = elasticity;
	return m_elasticity;
}

float Rigidbody::SetLinearDrag(float drag)
{
	m_linearDrag = drag;
	return m_linearDrag;
}

void Rigidbody::resolveCollision(Rigidbody * actor2)
{	

	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / 
			  glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())));

	glm::vec2 force = normal * j;
	applyForceToActor(actor2, force);
}
