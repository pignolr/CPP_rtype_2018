/*
** EPITECH PROJECT, 2021
** RType
** File description:
** SystemManager.cpp
*/

#include "ecs/SystemManager.hpp"
#include "ecs/system/RandomLevel.hpp"
#include "ecs/system/Movement.hpp"
#include "ecs/system/Collision.hpp"
#include "ecs/system/Draw.hpp"
#include "ecs/system/Shoot.hpp"
#include "ecs/system/Speed.hpp"

namespace rtype::server::ecs {
	SystemManager::SystemManager(EntityManager &entityManager,
		ComponentManager &componentManager,
		EventManager &eventManager,
		rtype::AffSprite &affSprite
		)
		: _entityManager(entityManager),
		_componentManager(componentManager),
		_eventManager(eventManager)
	{
		setSystem<system::RandomLevel>(_eventManager, _entityManager, _componentManager, *this);
		setSystem<system::Speed>(_eventManager, _entityManager, _componentManager);
		setSystem<system::Movement>(_eventManager, _entityManager, _componentManager);
		setSystem<system::Collision>(_eventManager, _entityManager, _componentManager);
		setSystem<system::Shoot>(_eventManager, _entityManager, _componentManager, *this);
		setSystem<system::Draw>(_eventManager, _entityManager, _componentManager, affSprite);
	}

	void SystemManager::update()
	{
		for (auto &system : _systemList) {
			system->update();
		}
	}
}
