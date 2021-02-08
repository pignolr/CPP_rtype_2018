/*
** EPITECH PROJECT, 2021
** RType
** File description:
** ecs.cpp
*/

#include "ecs/ECS.hpp"
#include "ecs/component/Faction.hpp"
#include "ecs/component/Position.hpp"
#include "ecs/component/Speed.hpp"
#include "ecs/component/HitPoint.hpp"
#include "ecs/component/Damage.hpp"
#include "ecs/event/Update.hpp"
#include "ecs/factory/Player.hpp"

namespace rtype::server::ecs {
	ECS::ECS()
	:
	_affSprite(),
	_eventManager(),
	_entityManager(_eventManager),
	_componentManager(_eventManager),
	_systemManager(_entityManager, _componentManager, _eventManager, _affSprite)
	{}

	ECS::~ECS()
	{}

	const ComponentManager &ECS::getConstComponentManager() const noexcept
	{ return _componentManager; }

	ComponentManager &ECS::getComponentManager() noexcept
	{ return _componentManager; }

	const EntityManager &ECS::getConstEntityManager() const noexcept
	{ return _entityManager; }

	EntityManager &ECS::getEntityManager() noexcept
	{ return _entityManager; }

	const SystemManager &ECS::getConstSystemManager() const noexcept
	{ return _systemManager; }

	SystemManager &ECS::getSystemManager() noexcept
	{ return _systemManager; }

	const EventManager &ECS::getConstEventManager() const noexcept
	{ return _eventManager; }

	EventManager &ECS::getEventManager() noexcept
	{ return _eventManager; }

	const AffSprite &ECS::getConstAffSprite() const noexcept
	{ return _affSprite; }

	AffSprite &ECS::getAffSprite() noexcept
	{ return _affSprite; }


	void ECS::registerEntity(std::size_t entityId)
	{
		_systemManager.registerEntity(entityId);
	}

	void ECS::update()
	{
		sendEvent<event::Update>(0);
		_eventManager.update();
		_entityManager.update();
		_componentManager.update();
		_systemManager.update();
	}

	const Entity ECS::createPlayer()
	{
		return factory::createPlayer(_entityManager, _componentManager, _systemManager);
	}
}
