/*
** EPITECH PROJECT, 2021
** RType
** File description:
** Bullet.cpp
*/

#include "ecs/factory/Bullet.hpp"
#include "ecs/component/Render.hpp"
#include "ecs/component/Collision.hpp"
#include "ecs/component/Faction.hpp"
#include "ecs/component/Position.hpp"
#include "ecs/component/Speed.hpp"
#include "ecs/component/Damage.hpp"
#include "ecs/component/HitPoint.hpp"

namespace rtype::server::ecs::factory {
	namespace component = rtype::server::ecs::component;

	const Entity createBullet(EntityManager &entityManager,
		ComponentManager &componentManager,
		SystemManager &systemManager,
		rtype::SpriteId spriteId,
		component::Faction::Type faction,
		double x, double y,
		double dirx, double diry,
		int damage)
	{
		auto entity = entityManager.newEntity();

		componentManager.addComponentForEntity<component::Render>
			(entity.id, spriteId, 1.0);
		componentManager.addComponentForEntity<component::Collision>
			(entity.id, 35.0, 30.0, 1);
		componentManager.addComponentForEntity<component::Faction>
			(entity.id, faction);
		componentManager.addComponentForEntity<component::Position>
			(entity.id, x, y);
		componentManager.addComponentForEntity<component::Speed>
			(entity.id, dirx, diry);
		componentManager.addComponentForEntity<component::Damage>
			(entity.id, damage);
		componentManager.addComponentForEntity<component::HitPoint>
			(entity.id, 1);
		systemManager.registerEntity(entity.id);
		return entity;
	}

	const Entity createBullet(ECS &ecs,
		rtype::SpriteId spriteId,
		component::Faction::Type faction,
		double x, double y,
		double dirx, double diry,
		int damage)
	{
		return createBullet(ecs.getEntityManager(),
			ecs.getComponentManager(),
			ecs.getSystemManager(),
			spriteId, faction, x, y, dirx, diry, damage
			);
	}
}
