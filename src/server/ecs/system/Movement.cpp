/*
** EPITECH PROJECT, 2021
** RType
** File description:
** Movement.cpp
*/

#include "include/protocole.hpp"
#include "ecs/component/Collision.hpp"
#include "ecs/event/Update.hpp"
#include "ecs/event/Die.hpp"
#include "ecs/system/Movement.hpp"
#include "ecs/component/Position.hpp"
#include "ecs/component/Speed.hpp"

namespace rtype::server::ecs::system {
	Movement::Movement(
		EventManager &eventManager,
		EntityManager &entityManager,
		ComponentManager &componentManager)
		: _eventManager(eventManager),
		_componentManager(componentManager)
	{
		registerComponentEntity(entityManager);
	}

	bool Movement::needEntityForWork() const noexcept
	{ return true; }

	bool Movement::checkEntityIsValid(std::size_t entityId) const noexcept
	{
		return (_componentManager.hasComponentForEntity<component::Position>(entityId)
			&& _componentManager.hasComponentForEntity<component::Speed>(entityId));
	}

	void Movement::registerComponentEntity(EntityManager &entityManager)
	{
		for (auto &entity: entityManager.getEntityList()) {
			registerEntity(entity.id);
		}
	}

	void Movement::registerEntity(std::size_t entityId)
	{
		if (checkEntityIsValid(entityId))
			_linkedEntities.insert(entityId);
	}

	void Movement::unregisterEntity(std::size_t entityId) noexcept
	{
		_linkedEntities.erase(entityId);
	}

	void Movement::unregisterAll() noexcept
	{
		_linkedEntities.clear();
	}

	void Movement::updateEntity(std::size_t entityId)
	{
		if (!checkEntityIsValid(entityId)) {
			unregisterEntity(entityId);
			return;
		}
		process(entityId);
	}

	void Movement::update()
	{
		if (_eventManager.isEmptyEvent<rtype::server::ecs::event::Update>())
			return;
		for (auto it = _linkedEntities.begin(); it != _linkedEntities.end();) {
			if (!checkEntityIsValid(*it))
				it = _linkedEntities.erase(it);
			else
				++it;
		}
		for (auto entityId : _linkedEntities) {
			process(entityId);
		}
	}

	void Movement::process(std::size_t entityId)
	{
		auto &pos = _componentManager
			.getComponentForEntity<component::Position>(entityId);
		auto &speed = _componentManager
			.getComponentForEntity<component::Speed>(entityId);
		pos.posx += speed.dirx;
		pos.posy += speed.diry;
		if (_componentManager.hasComponentForEntity<component::Collision>(entityId)) {
			auto &collision = _componentManager
				.getComponentForEntity<component::Collision>(entityId);
			if (pos.posx + collision.width < 0
				|| pos.posy + collision.height < 0
				|| pos.posx > rtype::WINDOW_WIDTH
				|| pos.posy > rtype::WINDOW_HEIGHT)
				_eventManager.sendEvent<event::Die>(entityId);
		} else if (pos.posx < 0 || pos.posy  < 0
				|| pos.posx > rtype::WINDOW_WIDTH
				|| pos.posy > rtype::WINDOW_HEIGHT) {
			_eventManager.sendEvent<event::Die>(entityId);
		}
	}
}
