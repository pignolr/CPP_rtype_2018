/*
** EPITECH PROJECT, 2021
** RType
** File description:
** EventStorage.cpp
*/


#include "ecs/EventManager.hpp"

namespace rtype::server::ecs {
	EventManager::~EventManager()
	{
		eventStorageManager.clear();
	}

	void EventManager::update()
	{
		eventStorageManager.update();
	}
}
