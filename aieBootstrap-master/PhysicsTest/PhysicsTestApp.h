#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <Gizmos.h>
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Square.h"


class PhysicsTestApp : public aie::Application {
public:

	PhysicsTestApp();
	virtual ~PhysicsTestApp();

	virtual bool startup();
	virtual void shutdown();

	void setupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity);

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	PhysicsScene*		m_physicsScene;
	Sphere*				m_sphere;
};