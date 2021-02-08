/*
** EPITECH PROJECT, 2021
** RType
** File description:
** IComponentStorage.cpp
*/

#include <iostream>
#include <algorithm>
#include "ecs/IComponentStorage.hpp"

namespace rtype::server::ecs {
	void ComponentStorageManager::push_back(IComponentStorage *store)
	{
		_storageList.push_back(store);
	};

	void ComponentStorageManager::remove(IComponentStorage *store) noexcept
	{
		_storageList.erase(
			std::remove(_storageList.begin(), _storageList.end(), store),
			_storageList.end());
	}

	void ComponentStorageManager::clear() noexcept
	{
		for (auto store : _storageList) {
			store->clearComponent();
		}
	}

	IComponentStorage::IComponentStorage()
	{
		reg(this);
	}

	IComponentStorage::~IComponentStorage()
	{
		unreg(this);
	}

	void IComponentStorage::unreg(IComponentStorage *store) noexcept
	{
		componentStorageManager.remove(store);
	}

	void IComponentStorage::reg(IComponentStorage *store)
	{
		componentStorageManager.push_back(store);
	}

	const std::vector<IComponentStorage *> &ComponentStorageManager::getList() const
	{
		return _storageList;
	}

}
