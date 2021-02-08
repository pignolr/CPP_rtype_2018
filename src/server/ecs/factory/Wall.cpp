/*
** EPITECH PROJECT, 2021
** RType
** File description:
** wall.cpp
*/

#include "ecs/factory/Wall.hpp"
#include "ecs/component/Render.hpp"
#include "ecs/component/Collision.hpp"
#include "ecs/component/Faction.hpp"
#include "ecs/component/Position.hpp"
#include "ecs/component/Speed.hpp"
#include "ecs/component/HitPoint.hpp"

namespace rtype::server::ecs::factory {
	const Entity createWall(EntityManager &entityManager,
		ComponentManager &componentManager,
		SystemManager &systemManager,
		double posy)
	{
		namespace component = rtype::server::ecs::component;
		auto entity = entityManager.newEntity();

		componentManager.addComponentForEntity<component::Render>
			(entity.id, rtype::SpriteId::wall1, 1.0);
		componentManager.addComponentForEntity<component::Collision>
			(entity.id, 156.0, 63.0, 0, false);
		componentManager.addComponentForEntity<component::Faction>
			(entity.id, component::Faction::Type::NEUTRAL);
		componentManager.addComponentForEntity<component::Position>
			(entity.id, rtype::WINDOW_WIDTH - 1, posy);
		componentManager.addComponentForEntity<component::Speed>
			(entity.id, -3, .0);
		componentManager.addComponentForEntity<component::HitPoint>
			(entity.id, 100);
		systemManager.registerEntity(entity.id);
		return entity;
	}

	const Entity createWall(ECS &ecs, double posy)
	{
		return createWall(ecs.getEntityManager(), ecs.getComponentManager(), ecs.getSystemManager(), posy);
	}
}
