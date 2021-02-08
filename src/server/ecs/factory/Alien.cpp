/*
** EPITECH PROJECT, 2021
** RType
** File description:
** factory.cpp
*/

#include "ecs/factory/Alien.hpp"
#include "ecs/component/Faction.hpp"
#include "ecs/component/Position.hpp"
#include "ecs/component/Speed.hpp"
#include "ecs/component/Damage.hpp"
#include "ecs/component/HitPoint.hpp"
#include "ecs/component/Collision.hpp"
#include "ecs/component/Render.hpp"
#include "ecs/component/Weapon.hpp"

namespace rtype::server::ecs::factory {
	const Entity createAlien(EntityManager &entityManager,
		ComponentManager &componentManager,
		SystemManager &systemManager,
		double posy)
	{
		namespace component = rtype::server::ecs::component;
		auto entity = entityManager.newEntity();

		componentManager.addComponentForEntity<component::Render>
			(entity.id, rtype::SpriteId::enemy1, 1.0);
		componentManager.addComponentForEntity<component::Collision>
			(entity.id, 90.0, 84.0);
		componentManager.addComponentForEntity<component::Faction>
			(entity.id, component::Faction::Type::ALIEN);
		componentManager.addComponentForEntity<component::Position>
			(entity.id, rtype::WINDOW_WIDTH - 1, posy);
		componentManager.addComponentForEntity<component::Speed>
			(entity.id, -3, 0.0);
		componentManager.addComponentForEntity<component::Weapon>
			(entity.id, component::Weapon::Type::BASIC, 10, 0.3, 25, -15);
		componentManager.addComponentForEntity<component::Damage>
			(entity.id, 10);
		componentManager.addComponentForEntity<component::HitPoint>
			(entity.id, 40);
		systemManager.registerEntity(entity.id);
		return entity;
	}


	const Entity createAlien(ECS &ecs, double posy)
	{
		return createAlien(ecs.getEntityManager(), ecs.getComponentManager(), ecs.getSystemManager(), posy);
	}
}
