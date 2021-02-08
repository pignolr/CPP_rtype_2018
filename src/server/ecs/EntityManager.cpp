/*
** EPITECH PROJECT, 2021
** RType
** File description:
** EntityManager.cpp
*/

#include "ecs/EntityManager.hpp"
#include "ecs/event/Die.hpp"

namespace rtype::server::ecs {
	EntityManager::EntityManager(EventManager &eventManager)
	: _eventManager(eventManager), _idCounter(0)
	{}

	Entity EntityManager::newEntity()
	{
		_entityList.emplace_back(generateId(), this);
		_entityIndex.emplace(_entityList.back().id, _entityList.size() - 1);
		return _entityList.back();
	}

	std::size_t EntityManager::generateId() noexcept
	{
		return _idCounter++;
	}

	const std::vector<Entity> &EntityManager::getEntityList() const
	{
		return _entityList;
	}

	void EntityManager::removeEntity(std::size_t entityId)
	{
		_entityIndex[_entityList[_entityList.size() - 1].id] = _entityIndex[entityId];
		std::swap(_entityList[_entityIndex[entityId]], _entityList[_entityList.size() - 1]);
		_entityList.pop_back();
	}

	void EntityManager::update()
	{
		if (!_eventManager.isEmptyEvent<event::Die>()) {
			const auto &deadEntities = _eventManager.getAllEvent<event::Die>();
			for (const auto & [ key, value ] : deadEntities) {
				removeEntity(key);
			}
		}
	}
}
