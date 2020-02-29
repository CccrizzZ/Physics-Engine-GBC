#include "MakingPlankSystem.h"
#include "ParticleContactEvent.h"
#include "TransformComponent.h"
#include "ParticleSphereComponent.h"

namespace Reality
{
	MakingPlankSystem::MakingPlankSystem()
	{
	}

	void MakingPlankSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			if (e.hasComponent<MakingPlankComponent>())
			{
				auto& plank = e.getComponent<MakingPlankComponent>();

				auto& point1Transform = plank.point1.getComponent<TransformComponent>();
				auto& point2Transform = plank.point2.getComponent<TransformComponent>();
				auto& point3Transform = plank.point3.getComponent<TransformComponent>();

				// Checking For Sphere
				for (auto e2 : getEntities())
				{

					if (e2.hasComponent<ParticleSphereComponent>())
					{

						auto& sphereParticle = e2.getComponent<ParticleSphereComponent>();
						auto& sphereTransform = e2.getComponent<TransformComponent>();

						// Distance Lines
						Vector3 vert1 = point2Transform.position - point1Transform.position;
						Vector3 vert2 = point3Transform.position - point1Transform.position;
						
						// normal
						vert1 = glm::normalize(vert1);
						vert2 = glm::normalize(vert2);
						Vector3 normal = glm::cross(vert1, vert2);

						float D = -glm::dot(normal, point1Transform.position);

						float d = glm::dot(normal, sphereTransform.position) + D;

						if (abs(d) < sphereParticle.radius)
						{
					
							Vector3 contactPosition = sphereTransform.position + (-normal * d);

							Vector3 point1Contact = point1Transform.position - contactPosition;
							Vector3 point2Contact = point2Transform.position - contactPosition;
							Vector3 point3Contact = point3Transform.position - contactPosition;

						
							float angle1Contact = acos(glm::dot(point1Contact, point2Contact) / (glm::length(point1Contact) * glm::length(point2Contact)));
							angle1Contact = glm::degrees(angle1Contact);

							float angle2Contact = acos(glm::dot(point2Contact, point3Contact) / (glm::length(point2Contact) * glm::length(point3Contact)));
							angle2Contact = glm::degrees(angle2Contact);

							float angle3Contact = acos(glm::dot(point3Contact, point1Contact) / (glm::length(point3Contact) * glm::length(point1Contact)));
							angle3Contact = glm::degrees(angle3Contact);

							float totals = angle1Contact + angle2Contact + angle3Contact;
							
							//Changing Color.
							getWorld().data.renderUtil->DrawTriangle(
								plank.point1.getComponent<TransformComponent>().position,
								plank.point2.getComponent<TransformComponent>().position,
								plank.point3.getComponent<TransformComponent>().position,
								Color::Red);
							//Bounds for the Plank Triangles
							if (totals > 359.7f && totals < 360.3)
							{
								getWorld().getEventManager().emitEvent<ParticleContactEvent>(
									e2,
									e,
									0.8f,
									normal,
									fabs(sphereParticle.radius - d) * 0.05);

							}
						}

					}

				}

				getWorld().data.renderUtil->DrawTriangle(
					plank.point1.getComponent<TransformComponent>().position, 
					plank.point2.getComponent<TransformComponent>().position,
					plank.point3.getComponent<TransformComponent>().position, 
					Color::White);

			}
			
		}
	}
}