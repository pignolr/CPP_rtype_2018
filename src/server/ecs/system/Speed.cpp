/*
** EPITECH PROJECT, 2021
** RType
** File description:
** Speed.cpp
*/

#include "ecs/event/Update.hpp"
#include "ecs/system/Speed.hpp"
#include "ecs/component/Speed.hpp"
#include "ecs/component/Player.hpp"

namespace rtype::server::ecs::system {
	Speed::Speed(
		EventManager &eventManager,
		EntityManager &entityManager,
		ComponentManager &componentManager)
		: _eventManager(eventManager),
		_componentManager(componentManager)
	{
		registerComponentEntity(entityManager);
	}

	bool Speed::needEntityForWork() const noexcept
	{ return true; }

	bool Speed::checkEntityIsValid(std::size_t entityId) const noexcept
	{
		return (_componentManager.hasComponentForEntity<component::Player>(entityId)
		&& _componentManager.hasComponentForEntity<component::Speed>(entityId)
		);
	}

	void Speed::registerComponentEntity(EntityManager &entityManager)
	{
		for (auto &entity: entityManager.getEntityList()) {
			registerEntity(entity.id);
		}
	}

	void Speed::registerEntity(std::size_t entityId)
	{
		if (checkEntityIsValid(entityId))
			_linkedEntities.insert(entityId);
	}

	void Speed::unregisterEntity(std::size_t entityId) noexcept
	{
		_linkedEntities.erase(entityId);
	}

	void Speed::unregisterAll() noexcept
	{
		_linkedEntities.clear();
	}

	void Speed::updateEntity(std::size_t entityId)
	{
		if (!checkEntityIsValid(entityId)) {
			unregisterEntity(entityId);
			return;
		}
		const auto event = _eventManager.getEvent<rtype::Input>(entityId);
		if (!event)
			return;
		process(entityId, event);
	}

	void Speed::update()
	{
		if (_eventManager.isEmptyEvent<rtype::server::ecs::event::Update>()
		        || _eventManager.isEmptyEvent<rtype::Input>())
			return;
		for (auto it = _linkedEntities.begin(); it != _linkedEntities.end();) {
			if (!checkEntityIsValid(*it))
				it = _linkedEntities.erase(it);
			else
				++it;
		}
		const auto &eventList = _eventManager.getAllEvent<rtype::Input>();
		for (const auto & [ entityId, input ] : eventList) {
			process(entityId, &input);
		}
	}

	bool Speed::process(std::size_t entityId, const rtype::Input *input)
	{
		auto &speed = _componentManager.getComponentForEntity<component::Speed>(entityId);
		int down = input->down;
		int up = input->up;
		int right = input->right;
		int left = input->left;

		int dirX = right - left;
		int dirY = down - up;

		// Speed Value can change
		double speedValue = 10;
		if (dirX && dirY) {
			speed.dirx = speedValue * 0.7 * dirX;
			speed.diry = speedValue * 0.7 * dirY;
		} else if (dirX) {
			speed.dirx = speedValue * dirX;
			speed.diry = 0;
		} else if (dirY) {
			speed.dirx = 0;
			speed.diry = speedValue * dirY;
		} else {
			speed.dirx = 0;
			speed.diry = 0;
		}
	}

}

