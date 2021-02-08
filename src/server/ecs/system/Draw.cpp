/*
** EPITECH PROJECT, 2021
** RType
** File description:
** Draw.cpp
*/

#include "ecs/event/Update.hpp"
#include "ecs/system/Draw.hpp"
#include "ecs/component/Render.hpp"
#include "ecs/component/Position.hpp"

namespace rtype::server::ecs::system {
	Draw::Draw(
		EventManager &eventManager,
		EntityManager &entityManager,
		ComponentManager &componentManager,
		rtype::AffSprite &affSprite)
		: _eventManager(eventManager),
		_componentManager(componentManager),
		_affSprite(affSprite)
	{
		registerComponentEntity(entityManager);
	}

	bool Draw::needEntityForWork() const noexcept
	{ return true; }

	bool Draw::checkEntityIsValid(std::size_t entityId) const noexcept
	{
		return (_componentManager.hasComponentForEntity<component::Render>(entityId)
			&& _componentManager.hasComponentForEntity<component::Position>(entityId)
		);
	}

	void Draw::registerComponentEntity(EntityManager &entityManager)
	{
		for (auto &entity: entityManager.getEntityList()) {
			registerEntity(entity.id);
		}
	}

	void Draw::registerEntity(std::size_t entityId)
	{
		if (checkEntityIsValid(entityId))
			_linkedEntities.insert(entityId);
	}

	void Draw::unregisterEntity(std::size_t entityId) noexcept
	{
		_linkedEntities.erase(entityId);
	}

	void Draw::unregisterAll() noexcept
	{
		_linkedEntities.clear();
	}

	void Draw::updateEntity(std::size_t entityId)
	{
		if (!checkEntityIsValid(entityId)) {
			unregisterEntity(entityId);
			return;
		}
		process(entityId);
	}

	void Draw::update()
	{
		if (_eventManager.isEmptyEvent<rtype::server::ecs::event::Update>())
			return;
		for (auto it = _linkedEntities.begin(); it != _linkedEntities.end();) {
			if (!checkEntityIsValid(*it))
				it = _linkedEntities.erase(it);
			else
				++it;
		}
		_affSprite._sprites.clear();
		for (auto entityId : _linkedEntities) {
			process(entityId);
		}
	}

	bool Draw::process(std::size_t entityId)
	{
		const auto &position = _componentManager.getComponentForEntity<component::Position>(entityId);
		const auto &render = _componentManager.getComponentForEntity<component::Render>(entityId);

		rtype::Sprite sprite;

		sprite.id = render.texture;
		sprite.posX = position.posx;
		sprite.posY = position.posy;
		sprite.stat = rtype::Status::immobile;
		_affSprite._sprites.push_back(sprite);
	}

}

