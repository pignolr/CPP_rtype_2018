/*
** EPITECH PROJECT, 2021
** RType
** File description:
** Shoot.cpp
*/

#include "include/protocole.hpp"
#include "ecs/factory/Bullet.hpp"
#include "ecs/event/Update.hpp"
#include "ecs/system/Shoot.hpp"
#include "ecs/component/Weapon.hpp"
#include "ecs/component/Player.hpp"
#include "ecs/component/Collision.hpp"

namespace rtype::server::ecs::system {
	Shoot::Shoot(
		EventManager &eventManager,
		EntityManager &entityManager,
		ComponentManager &componentManager,
		SystemManager &systemManager)
		: _eventManager(eventManager),
		_entityManager(entityManager),
		_componentManager(componentManager),
		_systemManager(systemManager)
	{
		registerComponentEntity(entityManager);
	}

	bool Shoot::needEntityForWork() const noexcept
	{ return true; }

	bool Shoot::checkEntityIsValid(std::size_t entityId) const noexcept
	{
		return (_componentManager.hasComponentForEntity<component::Weapon>(entityId)
			&& _componentManager.hasComponentForEntity<component::Position>(entityId)
			&& _componentManager.hasComponentForEntity<component::Collision>(entityId)
			&& _componentManager.hasComponentForEntity<component::Faction>(entityId)
			);
	}

	void Shoot::registerComponentEntity(EntityManager &entityManager)
	{
		for (auto &entity: entityManager.getEntityList()) {
			registerEntity(entity.id);
		}
	}

	void Shoot::registerEntity(std::size_t entityId)
	{
		if (checkEntityIsValid(entityId)) {
			_linkedEntities.insert(entityId);
		}
	}

	void Shoot::unregisterEntity(std::size_t entityId) noexcept
	{
		_linkedEntities.erase(entityId);
	}

	void Shoot::unregisterAll() noexcept
	{
		_linkedEntities.clear();
	}

	void Shoot::updateEntity(std::size_t entityId)
	{
		if (!checkEntityIsValid(entityId)) {
			unregisterEntity(entityId);
			return;
		}
		process(entityId);
	}

	void Shoot::update()
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

	void Shoot::process(std::size_t entityId)
	{
		auto &weapon = _componentManager.getComponentForEntity<component::Weapon>(entityId);
		const auto &pos = _componentManager.getComponentForEntity<component::Position>(entityId);
		const auto &collision = _componentManager.getComponentForEntity<component::Collision>(entityId);
		const auto &faction = _componentManager.getComponentForEntity<component::Faction>(entityId);

		if (weapon.current_cool_down) {
			--weapon.current_cool_down;
		} else {
			rtype::SpriteId spriteId = rtype::SpriteId::enemy_shoot1;
			if (_componentManager.hasComponentForEntity<component::Player>(entityId)) {
				auto input = _eventManager.getEvent<rtype::Input>(entityId);
				if (_eventManager.getEvent<rtype::Input>(entityId) == nullptr)
					return;
				if (!input->shoot && !weapon.current_charge) {
					return;
				} else if (input->shoot) {
					if (weapon.current_charge < weapon.max_charge)
						++weapon.current_charge;
					return;
				}
				spriteId = rtype::SpriteId::player_laser1;
			}
			double posX = collision.width + pos.posx;
			double posY;
			if (collision.height != 0)
				posY = collision.height / 2 + pos.posy;
			else
				posY = pos.posy;

			int damage;
			if (weapon.current_charge == weapon.max_charge)
				damage = weapon.max_damage;
			else
				damage = weapon.basic_damage + (weapon.current_charge * weapon.damage_rate);
			factory::createBullet(_entityManager, _componentManager, _systemManager,
				spriteId, faction.faction,
				posX, posY, weapon.dirx, weapon.diry, damage);
			weapon.current_cool_down = weapon.cool_down;
			weapon.current_charge = 0;
		}
	}

}

