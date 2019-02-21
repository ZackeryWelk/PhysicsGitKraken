#include "PhysicsScene.h"



PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	if (std::find(m_actors.begin(), m_actors.end(), actor) == m_actors.end())
	{
		m_actors.push_back(actor);
	}
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i] == actor)
		{
			m_actors.erase(m_actors.begin() + i);
		}
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::update(float dt)
{
	checkForCollisions();
	static std::list<PhysicsObject*> dirty;
	// update the physics at a fixed time step

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;


		//check for collisions

		//for (auto pActor : m_actors)
		//{
		//	for (auto pOther : m_actors)
		//	{
		//
		//
		//		if (pActor == pOther)
		//		{
		//			continue;
		//		}
		//		if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&
		//			std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
		//			continue;
		//
		//		Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
		//		if (pRigid->checkCollision(pOther) == true)
		//		{
		//			pRigid->applyForceToActor(dynamic_cast<Rigidbody*>(pOther), pRigid->getVelocity() * pRigid->getMass());
		//			dirty.push_back(pRigid);
		//			dirty.push_back(pOther);
		//		}
		//	}
		//}
		//dirty.clear();
	}
}

void PhysicsScene::updatedGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane,	PhysicsScene::plane2Sphere,  PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane,	PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane,	PhysicsScene::box2Sphere,	 PhysicsScene::box2Box
};

void PhysicsScene::checkForCollisions()
{
	int actorCount = m_actors.size();

	//need to check for collisions against all objects except this one
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			//using function pointers
			int functionIdx = (shapeId1 * SHAPECOUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				//did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool PhysicsScene::plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//isnt possible
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{	
	Plane *plane = dynamic_cast<Plane*>(obj1);
	Square *box = dynamic_cast<Square*>(obj2);
	
	if (box != nullptr && plane != nullptr)
	{
		box->SetLinearDrag(0.2);
		glm::vec2 collisionNormal = plane->getNormal();
		float boxToPlane = glm::dot(box->getPosition(), plane->getNormal()) - plane->getDistance();
	
		float intersectionX = box->getExtents().x - boxToPlane;
		float intersectionY = box->getExtents().y - boxToPlane;
	
		if (intersectionX > 0 || intersectionY > 0)
		{		
			box->setPosition(box->getPosition() + (collisionNormal * intersectionX) + (collisionNormal * intersectionY));
			plane->resolveCollision(box);
			box->SetLinearDrag(0.01);
			return true;
		}
	
	}			
	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	Plane *plane = dynamic_cast<Plane*>(obj2);

	//if this check is successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		sphere->SetLinearDrag(0.2);
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();
		
		//if we are behind plane then we flip the normal
		//activate this to allow collision from one side (will need to change restitution)
		//if (sphereToPlane < 0)
		//{
		//	collisionNormal *= -1;
		//	sphereToPlane *= -1;
		//}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			sphere->setPosition(sphere->getPosition() + collisionNormal * intersection);

			plane->resolveCollision(sphere);
			sphere->SetLinearDrag(0.01);
			return true;
		}
		
	}
	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere *sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere *sphere2 = dynamic_cast<Sphere*>(obj2);
	//if it is successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		glm::vec2 displacement = sphere1->getPosition() - sphere2->getPosition();
		if (glm::length(displacement) < (sphere1->getRadius() + sphere2->getRadius()))
		{
			
			float overlap = (sphere1->getRadius() + sphere2->getRadius()) - (glm::length(displacement));
			glm::vec2 offset = glm::normalize(displacement) * overlap;


			sphere1->setPosition(sphere1->getPosition() + offset * 0.5f);
			sphere2->setPosition(sphere2->getPosition() + -offset * 0.5f);

			sphere1->resolveCollision(sphere2);
				
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	Square *box = dynamic_cast<Square*>(obj2);

	if (sphere != nullptr && box != nullptr)
	{

		if (sphere->getPosition().x + sphere->getRadius() < box->getPosition().x - box->getExtents().x ||
			sphere->getPosition().y + sphere->getRadius() < box->getPosition().y - box->getExtents().y ||
			sphere->getPosition().x - sphere->getRadius() > box->getPosition().x + box->getExtents().x ||
			sphere->getPosition().y - sphere->getRadius() > box->getPosition().y + box->getExtents().y)
		{
			return false;
		}
		else
		{
			sphere->resolveCollision(box);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{

	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Square *box1 = dynamic_cast<Square*>(obj1);
	Square *box2 = dynamic_cast<Square*>(obj2);


	if (box1 != nullptr && box2 != nullptr)
	{

	//	float overlapX = (box1->getPosition().x + box1->getExtents().x) + (box2->getPosition().x + box2->getExtents().x)
	//					- glm::distance(box1->getPosition().x + box1->getExtents().x, box2->getPosition().x + box2->getExtents().x);
	//	
	//	float overlapY = (box1->getPosition().y + box1->getExtents().y) + (box2->getPosition().y + box2->getExtents().y)
	//					- glm::distance(box1->getPosition().y + box1->getExtents().y, box2->getPosition().y + box2->getExtents().y);

		//glm::vec2 overlap(
		//	(box1->getPosition() + box1->getExtents()) + (box2->getPosition() + box2->getExtents()) - glm::distance(box1->getPosition() + box1->getExtents(), box2->getPosition() + box2->getExtents()));
			
		//glm::vec2 offset = glm::normalize(box1->getPosition() - box2->getPosition() * overlap);

		if (box1->getPosition().x + box1->getExtents().x < box2->getPosition().x -box2->getExtents().x ||
			box2->getPosition().x + box2->getExtents().x < box1->getPosition().x -box1->getExtents().x)
		{
			return false;
		}
		if (box1->getPosition().y + box1->getExtents().y < box2->getPosition().y -box2->getExtents().y ||
			box2->getPosition().y + box2->getExtents().y < box1->getPosition().y -box1->getExtents().y)
		{
			return false;
		}
		else
		{
			//box1->setPosition((box1->getPosition() + offset) * 0.5f);
			//box2->setPosition((box2->getPosition() + -offset) * 0.5f);

			box1->resolveCollision(box2);

			return true;
		}
	}
	return false;
}
