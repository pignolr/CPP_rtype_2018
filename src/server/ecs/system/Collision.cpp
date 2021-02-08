/*
** EPITECH PROJECT, 2021
** RType
** File description:
** Collision.cpp
*/

#include "ecs/component/Damage.hpp"
#include "ecs/component/HitPoint.hpp"
#include "ecs/event/Die.hpp"
#include "ecs/component/Faction.hpp"
#include "ecs/event/Update.hpp"
#include "ecs/system/Collision.hpp"
#include "ecs/component/Collision.hpp"

namespace rtype::server::ecs::system {
	Collision::Collision(
		EventManager &eventManager,
		EntityManager &entityManager,
		ComponentManager &componentManager)
		: _eventManager(eventManager),
		_componentManager(componentManager)
	{
		registerComponentEntity(entityManager);
	}

	bool Collision::needEntityForWork() const noexcept
	{ return true; }

	bool Collision::checkEntityIsValid(std::size_t entityId) const noexcept
	{
		return (_componentManager.hasComponentForEntity<component::Position>(entityId)
			&& _componentManager.hasComponentForEntity<component::Speed>(entityId)
			&& _componentManager.hasComponentForEntity<component::Collision>(entityId)
			);
	}

	void Collision::registerComponentEntity(EntityManager &entityManager)
	{
		for (auto &entity: entityManager.getEntityList()) {
			registerEntity(entity.id);
		}
	}

	void Collision::registerEntity(std::size_t entityId)
	{
		if (checkEntityIsValid(entityId))
			_linkedEntities.insert(entityId);
	}

	void Collision::unregisterEntity(std::size_t entityId) noexcept
	{
		_linkedEntities.erase(entityId);
	}

	void Collision::unregisterAll() noexcept
	{
		_linkedEntities.clear();
	}

	void Collision::updateEntity(std::size_t entityId)
	{
		if (!checkEntityIsValid(entityId)) {
			unregisterEntity(entityId);
			return;
		}
		process(entityId);
	}

	void Collision::update()
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

	bool Collision::checkAxeCollision(double size, double otherSize, double *x, double *other_x)
	{
		return ((x[0] < other_x[0] && other_x[0] < x[1])
			|| (x[0] < other_x[1] && other_x[1] < x[1])
			|| (other_x[0] < x[0] && x[0] < other_x[1])
			|| (other_x[0] < x[1] && x[1] < other_x[1]));
	}

	void Collision::takeDamage(std::size_t entityId, std::size_t otherId, int &hitPoint, int selfDamage)
	{
		if (hitPoint) {
			if (_componentManager.hasComponentForEntity<component::Damage>(otherId))
				hitPoint -= _componentManager.getComponentForEntity<component::Damage>(otherId).damage;
			hitPoint -= selfDamage;
			if (hitPoint <= 0) {
				hitPoint = 0;
				_eventManager.sendEvent<rtype::server::ecs::event::Die>(entityId);
			}
		}
	}

	void Collision::doDamage(std::size_t entityId, std::size_t otherId, int damage, int otherSelfDamage)
	{
		if (_componentManager.hasComponentForEntity<component::HitPoint>(otherId)) {
			auto &otherHitPoint = _componentManager.getComponentForEntity<component::HitPoint>(otherId);
			otherHitPoint.hit_point -= damage;
			otherHitPoint.hit_point -= otherSelfDamage;
			if (otherHitPoint.hit_point <= 0) {
				otherHitPoint.hit_point = 0;
				_eventManager.sendEvent<rtype::server::ecs::event::Die>(otherId);
			}
		}
	}

	void Collision::replaceEntity(std::size_t otherId,
		component::Position &position, component::Position &otherPosition,
		const component::Collision &collision, const component::Collision &otherCollision,
		const component::Speed &speed)
	{
		if (collision.movable) {
			if (speed.dirx < 0)
				position.posx = otherPosition.posx + collision.width;
			else
				position.posx = otherPosition.posx - otherCollision.width;
		} else if (otherCollision.movable) {
			auto &otherSpeed = _componentManager.getComponentForEntity<component::Speed>(otherId);
			if (otherSpeed.dirx < 0)
				otherPosition.posx = position.posx + otherCollision.width;
			else
				otherPosition.posx = position.posx - collision.width;
		}
	}

	bool Collision::process(std::size_t entityId)
	{
		// get component from entity
		auto &position = _componentManager.getComponentForEntity<component::Position>(entityId);
		const auto &speed = _componentManager.getComponentForEntity<component::Speed>(entityId);
		const auto &collision = _componentManager.getComponentForEntity<component::Collision>(entityId);
		auto faction = static_cast<ecs::component::Faction::Type>(-1);
		if (_componentManager.hasComponentForEntity<component::Faction>(entityId))
			faction = _componentManager.getComponentForEntity<component::Faction>(entityId).faction;
		int damage = 0;
		if (_componentManager.hasComponentForEntity<component::Damage>(entityId))
			damage = _componentManager.getComponentForEntity<component::Damage>(entityId).damage;
		int hitPoint = 0;
		if (_componentManager.hasComponentForEntity<component::HitPoint>(entityId))
			hitPoint = _componentManager.getComponentForEntity<component::HitPoint>(entityId).hit_point;

		// collision square of entity
		double collision_x[2];
		double collision_y[2];

		collision_x[0] = position.posx;
		collision_x[1] = position.posx + collision.width;
		collision_y[0] = position.posy;
		collision_y[1] = position.posy + collision.height;

		for (auto otherId : _linkedEntities) {
			if (faction != static_cast<ecs::component::Faction::Type>(-1)
				&& _componentManager.hasComponentForEntity<component::Faction>(otherId)
			        && faction == _componentManager
				        .getComponentForEntity<component::Faction>(otherId).faction)
				continue;
			// get component other entity
			auto &otherPosition = _componentManager.getComponentForEntity<component::Position>(otherId);
			const auto &otherCollision = _componentManager.getComponentForEntity<component::Collision>(otherId);

			// collision square of other entity
			double otherCollision_x[2];
			double otherCollision_y[2];

			otherCollision_x[0] = otherPosition.posx;
			otherCollision_x[1] = otherPosition.posx + otherCollision.width;
			otherCollision_y[0] = otherPosition.posy;
			otherCollision_y[1] = otherPosition.posy + otherCollision.height;

			// check collision on x
			if (checkAxeCollision(collision.width, otherCollision.width,
				collision_x, otherCollision_x)
				&& checkAxeCollision(collision.height, otherCollision.height,
					collision_y, otherCollision_y)) {
				// do Collision
				takeDamage(entityId, otherId, hitPoint, collision.selfDamage);
				doDamage(entityId, otherId, damage, otherCollision.selfDamage);
				replaceEntity(otherId, position, otherPosition, collision, otherCollision, speed);
			}
		}
		// update hitpoint
		if (hitPoint) {
			auto &hitPointComponent = _componentManager.getComponentForEntity<component::HitPoint>(entityId);
			hitPointComponent.hit_point = hitPoint;
		}
		return true;
	}
}
