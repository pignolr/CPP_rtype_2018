cmake_minimum_required(VERSION 3.1.0)

project(RType)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(SRCS_CLIENT
	src/client/main.cpp
	src/client/sfml/sfml.cpp

	src/server/ecs/ECS.cpp
	src/server/ecs/IEventStorage.cpp
	src/server/ecs/EventManager.cpp
	src/server/ecs/EntityManager.cpp
	src/server/ecs/IComponentStorage.cpp
	src/server/ecs/ComponentManager.cpp
	src/server/ecs/SystemManager.cpp
	src/server/ecs/factory/Player.cpp
	src/server/ecs/factory/Bullet.cpp
	src/server/ecs/factory/Wall.cpp
	src/server/ecs/factory/Alien.cpp
	src/server/ecs/system/Movement.cpp
	src/server/ecs/system/Collision.cpp
	src/server/ecs/system/Speed.cpp
	src/server/ecs/system/Shoot.cpp
	src/server/ecs/system/Draw.cpp
	src/server/ecs/system/RandomLevel.cpp
        )

set(HDRS_CLIENT
	include/client/sfml/sfml.hpp
	include/server/ecs/ECS.hpp
	include/server/ecs/IEventStorage.hpp
	include/server/ecs/EventStorage.hpp
	include/server/ecs/EventManager.hpp
	include/server/ecs/Entity.hpp
	include/server/ecs/EntityManager.hpp
	include/server/ecs/IComponentStorage.hpp
	include/server/ecs/ComponentStorage.hpp
	include/server/ecs/SystemManager.hpp
	include/server/ecs/System.hpp
	include/server/ecs/event/Update.hpp
	include/server/ecs/event/Die.hpp
	include/server/ecs/ComponentManager.hpp
	include/server/ecs/component/Position.hpp
	include/server/ecs/component/Speed.hpp
	include/server/ecs/component/Faction.hpp
	include/server/ecs/component/Render.hpp
	include/server/ecs/component/Collision.hpp
	include/server/ecs/component/Life.hpp
	include/server/ecs/component/Weapon.hpp
	include/server/ecs/component/Player.hpp
	include/server/ecs/factory/Player.hpp
	include/server/ecs/factory/Alien.hpp
	include/server/ecs/factory/Wall.hpp
	include/server/ecs/factory/Bullet.hpp
	include/server/ecs/system/Movement.hpp
	include/server/ecs/system/Collision.hpp
	include/server/ecs/system/Draw.hpp
	include/server/ecs/system/Shoot.hpp
	include/server/ecs/system/Speed.hpp
	include/server/ecs/system/RandomLevel.hpp
	)

set(INCLUDE_CLIENT include/client include include/server)

set(SRCS_SERVER
        src/server/main.cpp
#	src/server/server.cpp
	src/server/ecs/ECS.cpp
	src/server/ecs/IEventStorage.cpp
	src/server/ecs/EventManager.cpp
	src/server/ecs/EntityManager.cpp
	src/server/ecs/IComponentStorage.cpp
	src/server/ecs/ComponentManager.cpp
	src/server/ecs/SystemManager.cpp
	src/server/ecs/factory/Player.cpp
	src/server/ecs/factory/Bullet.cpp
	src/server/ecs/factory/Wall.cpp
	src/server/ecs/factory/Alien.cpp
	src/server/ecs/system/Movement.cpp
	src/server/ecs/system/Collision.cpp
	src/server/ecs/system/Speed.cpp
	src/server/ecs/system/Shoot.cpp
	src/server/ecs/system/Draw.cpp
	src/server/ecs/system/RandomLevel.cpp)

set(HDRS_SERVER
#	include/server/server.hh
#	include/server/log.hpp
	include/server/ecs/ECS.hpp
	include/server/ecs/IEventStorage.hpp
	include/server/ecs/EventStorage.hpp
	include/server/ecs/EventManager.hpp
	include/server/ecs/Entity.hpp
	include/server/ecs/EntityManager.hpp
	include/server/ecs/IComponentStorage.hpp
	include/server/ecs/ComponentStorage.hpp
	include/server/ecs/SystemManager.hpp
	include/server/ecs/System.hpp
	include/server/ecs/event/Update.hpp
	include/server/ecs/event/Die.hpp
	include/server/ecs/ComponentManager.hpp
	include/server/ecs/component/Position.hpp
	include/server/ecs/component/Speed.hpp
	include/server/ecs/component/Faction.hpp
	include/server/ecs/component/Render.hpp
	include/server/ecs/component/Collision.hpp
	include/server/ecs/component/Life.hpp
	include/server/ecs/component/Weapon.hpp
	include/server/ecs/component/Player.hpp
	include/server/ecs/factory/Player.hpp
	include/server/ecs/factory/Alien.hpp
	include/server/ecs/factory/Wall.hpp
	include/server/ecs/factory/Bullet.hpp
	include/server/ecs/system/Movement.hpp
	include/server/ecs/system/Collision.hpp
	include/server/ecs/system/Draw.hpp
	include/server/ecs/system/Shoot.hpp
	include/server/ecs/system/Speed.hpp
	include/server/ecs/system/RandomLevel.hpp)

set(INCLUDE_SERVER include/server)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

message(STATUS ${SRCS_CLIENT})
message(STATUS ${SRCS_SERVER})

add_executable(r-type_client ${SRCS_CLIENT} ${HDRS_CLIENT})
target_include_directories(r-type_client PRIVATE ${INCLUDE_CLIENT})
target_link_libraries(r-type_client ${CONAN_LIBS})

add_executable(r-type_server ${SRCS_SERVER} ${HDRS_SERVER})
target_include_directories(r-type_server PRIVATE ${INCLUDE_SERVER})
target_link_libraries(r-type_server ${CONAN_LIBS})
