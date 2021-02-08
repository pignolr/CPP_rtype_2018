/*
** EPITECH PROJECT, 2021
** RType
** File description:
** ComponentManager.cpp
*/

#include "ecs/ComponentManager.hpp"
#include "ecs/event/Die.hpp"

namespace rtype::server::ecs {
	ComponentManager::ComponentManager(EventManager &eventManager)
	: _eventManager(eventManager)
	{}

	ComponentManager::~ComponentManager()
	{
		componentStorageManager.clear();
	}


	void ComponentManager::removeEntity(std::size_t entityId)
	{
		int i = 0;
		for (auto store: componentStorageManager._storageList) {
			store->removeComponentForEntity(entityId);
			++i;
		}
	}

	void ComponentManager::update()
	{
		if (!_eventManager.isEmptyEvent<event::Die>()) {
			auto &deadEntities = _eventManager.getAllEvent<event::Die>();
			for (const auto & [ key, value ] : deadEntities) {
				removeEntity(key);
			}
		}
	}

}
