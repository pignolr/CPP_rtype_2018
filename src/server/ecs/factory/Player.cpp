/*
** EPITECH PROJECT, 2021
** RType
** File description:
** factory.cpp
*/

#include "ecs/factory/Player.hpp"
#include "ecs/component/Faction.hpp"
#include "ecs/component/Player.hpp"
#include "ecs/component/Position.hpp"
#include "ecs/component/Speed.hpp"
#include "ecs/component/Damage.hpp"
#include "ecs/component/HitPoint.hpp"
#include "ecs/component/Life.hpp"
#include "ecs/component/Collision.hpp"
#include "ecs/component/Render.hpp"
#include "ecs/component/Weapon.hpp"

namespace rtype::server::ecs::factory {
	const Entity createPlayer(EntityManager &entityManager,
		ComponentManager &componentManager,
		SystemManager &systemManager)
	{
		namespace component = rtype::server::ecs::component;
		auto entity = entityManager.newEntity();

		componentManager.addComponentForEntity<component::Render>
			(entity.id, rtype::SpriteId::player1, 1.0);
		componentManager.addComponentForEntity<component::Collision>
			(entity.id, 160.0, 60.0);
		componentManager.addComponentForEntity<component::Faction>
			(entity.id, component::Faction::Type::PLAYER);
		componentManager.addComponentForEntity<component::Player>
			(entity.id, entity.id);
		componentManager.addComponentForEntity<component::Position>
			(entity.id, 30.0, rtype::WINDOW_HEIGHT / 2);
		componentManager.addComponentForEntity<component::Speed>
			(entity.id, 0., 0.);
		componentManager.addComponentForEntity<component::Weapon>
			(entity.id, component::Weapon::Type::BASIC, 10, 0.3, 25, 15);
		auto &weapon = componentManager.getComponentForEntity<component::Weapon>(entity.id);
		weapon.cool_down = 15;
		componentManager.addComponentForEntity<component::Damage>
			(entity.id, 1);
		componentManager.addComponentForEntity<component::HitPoint>
			(entity.id, 1);
		componentManager.addComponentForEntity<component::Life>
			(entity.id, 3);
		systemManager.registerEntity(entity.id);
		return entity;
	}

	const Entity createPlayer(ECS &ecs)
	{
		return createPlayer(ecs.getEntityManager(), ecs.getComponentManager(), ecs.getSystemManager());
	}
}
