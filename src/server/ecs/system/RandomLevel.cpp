/*
** EPITECH PROJECT, 2021
** RType
** File description:
** RandomLevel.cpp
*/

#include "ecs/event/Update.hpp"
#include "ecs/system/RandomLevel.hpp"
#include "ecs/component/Render.hpp"
#include "ecs/component/Position.hpp"
#include "ecs/factory/Alien.hpp"
#include "ecs/factory/Wall.hpp"

namespace rtype::server::ecs::system {
	RandomLevel::RandomLevel(
		EventManager &eventManager,
		EntityManager &entityManager,
		ComponentManager &componentManager,
		SystemManager &systemManager)
		: _eventManager(eventManager),
		_entityManager(entityManager),
		_componentManager(componentManager),
		_systemManager(systemManager),
		_randomDevice()
	{}

	bool RandomLevel::needEntityForWork() const noexcept
	{ return false; }

	bool RandomLevel::checkEntityIsValid(std::size_t entityId) const noexcept
	{
		return (true);
	}

	void RandomLevel::registerComponentEntity(EntityManager &entityManager)
	{
		for (auto &entity: entityManager.getEntityList()) {
			registerEntity(entity.id);
		}
	}

	void RandomLevel::registerEntity(std::size_t)
	{}

	void RandomLevel::unregisterEntity(std::size_t) noexcept
	{}

	void RandomLevel::unregisterAll() noexcept
	{}

	void RandomLevel::updateEntity(std::size_t)
	{}

	void RandomLevel::update()
	{
		if (_eventManager.isEmptyEvent<rtype::server::ecs::event::Update>())
			return;
		process();
	}

	bool RandomLevel::process()
	{
		int randNumber = _randomDevice() % 200;
		double posy;
		switch (randNumber) {
		case 0:
			posy = _randomDevice() % (rtype::WINDOW_HEIGHT - 10);
			factory::createAlien(_entityManager, _componentManager, _systemManager, posy);
		case 1:
			posy = _randomDevice() % (rtype::WINDOW_HEIGHT - 10);
			factory::createAlien(_entityManager, _componentManager, _systemManager, posy);
			break;
		case 2:
			posy = _randomDevice() % (rtype::WINDOW_HEIGHT - 30);
			factory::createWall(_entityManager, _componentManager, _systemManager, posy);
		case 3:
			posy = _randomDevice() % (rtype::WINDOW_HEIGHT - 30);
			factory::createWall(_entityManager, _componentManager, _systemManager, posy);
			break;
		case 4:
			posy = _randomDevice() % (rtype::WINDOW_HEIGHT - 30);
			factory::createWall(_entityManager, _componentManager, _systemManager, posy);
			break;
		default:
			break;
		}
	}
}

