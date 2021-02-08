/*
** EPITECH PROJECT, 2021
** RType
** File description:
** main.cpp
*/

#include <chrono>
#include <iostream>
#include <include/server/ecs/event/Update.hpp>
#include <include/server/ecs/event/Die.hpp>
#include "ecs/ECS.hpp"
#include "ecs/EventManager.hpp"
#include "ecs/component/Position.hpp"
#include "ecs/factory/Player.hpp"
#include "ecs/system/Movement.hpp"

void aff(auto &entity, auto &pos, auto &entity2, auto &pos2)
{
	std::cout << "id: " << entity.id << std::endl
		<< "xpos: " << pos.posx << std::endl
		<< "ypos: " << pos.posy << std::endl;
	std::cout << "id: " << entity2.id << std::endl
		<< "xpos: " << pos2.posx << std::endl
		<< "ypos: " << pos2.posy << std::endl;
}

void affonlyone(auto &entity, auto &pos)
{
	std::cout << "id: " << entity.id << std::endl
		<< "xpos: " << pos.posx << std::endl
		<< "ypos: " << pos.posy << std::endl;
}

int main(int ac, char **av)
{
	std::cout << "lol" << std::endl << std::endl;
	namespace ecs = rtype::server::ecs;
	ecs::ECS ecs_ob;
	auto &systemManager = ecs_ob.getSystemManager();

	auto &entity = ecs::factory::createPlayer(ecs_ob);
	auto &pos = ecs_ob.getComponentManager().getComponentForEntity
	        <rtype::server::ecs::component::Position>(entity.id);
	std::cout << &pos << std::endl;

	std::cout << std::endl;
	auto &entity2 = ecs::factory::createPlayer(ecs_ob);
	auto &pos2 = ecs_ob.getComponentManager().getComponentForEntity
		<rtype::server::ecs::component::Position>(entity2.id);
	std::cout << &pos2 << std::endl;

	auto event = ecs::EventManager();
	event.sendEvent<ecs::component::Position>(0, 1., 1.);

	std::cout << std::endl;
	aff(entity, pos, entity2, pos2);

	std::cout << std::endl;
	auto move = ecs::system::Movement(
		ecs_ob.getEventManager(),
		ecs_ob.getEntityManager(),
		ecs_ob.getComponentManager());

	systemManager.updateEntity<ecs::system::Movement>(entity.id);
	std::cout << "Do movement with entity with manager:" << std::endl;
	aff(entity, pos, entity2, pos2);
	std::cout << std::endl;

	move.updateEntity(entity.id);
	std::cout << "Do movement with entity:" << std::endl;
	aff(entity, pos, entity2, pos2);
	std::cout << std::endl;

	move.update();
	std::cout << "Do movement with all without event:" << std::endl;
	aff(entity, pos, entity2, pos2);
	std::cout << std::endl;

	ecs_ob.getEventManager().sendEvent<ecs::event::Update>(0);
	ecs_ob.getEventManager().update();

	move.update();
	std::cout << "Do movement with all with event:" << std::endl;
	aff(entity, pos, entity2, pos2);
	std::cout << std::endl;

	ecs_ob.update();
	std::cout << "Do movement with update manager:" << std::endl;
	aff(entity, pos, entity2, pos2);
	std::cout << std::endl;

	std::cout << "nb entity: " << ecs_ob.getEntityManager().getEntityList().size() << std::endl;
	ecs_ob.getEventManager().sendEvent<ecs::event::Die>(entity.id);
	ecs_ob.update();
	std::cout << "player1 die" << std::endl;
	std::cout << "nb entity: " << ecs_ob.getEntityManager().getEntityList().size() << std::endl;

	std::cout << "Do movement after die" << std::endl;
	affonlyone(entity2, pos2);
	std::cout << std::endl;

	auto start = std::chrono::system_clock::now();
	for (int i = 0; i < 30; ++i) {
		ecs_ob.update();
		std::cout << ecs_ob.getEntityManager().getEntityList().size() << std::endl;
	}
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;

	std::cout << "elapsed time: " << elapsed_seconds.count() << "ms\n";

	return EXIT_SUCCESS;
}
