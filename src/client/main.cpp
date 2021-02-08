#include <chrono>
#include <thread>
#include <iostream>
#include "sfml/sfml.hpp"
#include "ecs/ECS.hpp"
#include "ecs/EventManager.hpp"
#include "ecs/event/Die.hpp"
#include "include/protocole.hpp"

int game()
{
	try {
		namespace ecs = rtype::server::ecs;
		rtype::client::sfml::sfml sfmlManager;
		ecs::ECS ecsManager;
		rtype::Input inp;
		rtype::AffSprite &affSprite = ecsManager.getAffSprite();
		auto playerEntity = ecsManager.createPlayer();
		std::chrono::milliseconds tick(1000 / 30);

		while (true) {
			auto start = std::chrono::system_clock::now();
			inp = sfmlManager.getInput();
			ecs::eventStorage<rtype::Input>.sendEvent(playerEntity.id, inp);
			ecsManager.update();
			sfmlManager.setSprites(affSprite._sprites);
			sfmlManager.display();
			if (ecs::eventStorage<ecs::event::Die>.getEvent(playerEntity.id) != nullptr)
				break;
			if (inp.esc == true)
				break;
			// Pause for tickrate
			auto end = std::chrono::system_clock::now();
			auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(tick - (end - start));
			if (diff.count() > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(diff));
			}
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int main()
{
	return game();
}
