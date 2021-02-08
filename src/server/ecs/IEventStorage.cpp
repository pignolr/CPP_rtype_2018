/*
** EPITECH PROJECT, 2021
** RType
** File description:
** IEventStorage.cpp
*/

#include <iostream>
#include <algorithm>
#include "ecs/IEventStorage.hpp"

namespace rtype::server::ecs {
	void EventStorageManager::push_back(IEventStorage *store)
	{
		_storageList.push_back(store);
	};

	void EventStorageManager::remove(IEventStorage *store) noexcept
	{
		_storageList.erase(
			std::remove(_storageList.begin(), _storageList.end(), store),
			_storageList.end());
	}

	void EventStorageManager::update() noexcept
	{
		for (auto store : _storageList) {
			store->update();
		}
	}

	void EventStorageManager::clear() noexcept
	{
		for (auto store : _storageList) {
			store->clear();
		}
	}

	IEventStorage::IEventStorage()
	{
		reg(this);
	}

	IEventStorage::~IEventStorage()
	{
		unreg(this);
	}

	void IEventStorage::unreg(IEventStorage *store) noexcept
	{
		eventStorageManager.remove(store);
	}

	void IEventStorage::reg(IEventStorage *store)
	{
		eventStorageManager.push_back(store);
	}
}
