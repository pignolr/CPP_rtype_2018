#include "server.hh"
#include "ecs/EventManager.hpp"

namespace rtype::server
{
	Server::Server(unsigned short port) :
		_io(),
		_server_endpoint(udp::v4(), port),
		_socket(_io, _server_endpoint),
		_nextClientId(0)

	{
		_threads.create_thread(boost::bind(&boost::asio::io_service::run, &_io));
		_io.post(boost::bind(&Server::run_service, this));
		_io.run();
	}

	Server::~Server()
	{
		_socket.close();
		_io.stop();
		_threads.join_all();
	}

	void Server::run_service()
	{
		start_receive();

		while (!_io.stopped()) {
			try {
				_io.run();
			} catch (const std::exception &e) {
				std::stringstream err_msg;
				err_msg << "run_service : Error : Network Exception : " << e.what();
				rtype::log::Error(err_msg.str());
			}
		}
	}

	void Server::start_receive()
	{
		_socket.async_receive_from(boost::asio::buffer(_recv_buff), _remote_endpoint,
			boost::bind(&Server::handle_receive, this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

	void Server::handle_receive(const boost::system::error_code &err, std::size_t bytes_transferred)
	{
		if (!err) {
			try {
				auto message = ClientMsg(std::string(_recv_buff.data(), _recv_buff.data() + bytes_transferred),
					add_or_getClient(_remote_endpoint));
				if (!message.first.empty())
					_messageQueue.push_back(message);
			} catch (const std::exception &e) {
				std::stringstream err_msg;
				err_msg << "handle_receive : Error while parsing incoming message : " << e.what();
				rtype::log::Error(err_msg.str());
			}
		} else {
			std::stringstream err_msg;
			err_msg << "handle_receive : Error : " << err.message() << " while receiving from " << _remote_endpoint;
			rtype::log::Error(err_msg.str());
			handle_error(err, _remote_endpoint);
		}
	}

	uint8_t Server::add_or_getClient(udp::endpoint new_client)
	{
		for (const auto &client : _clients) {
			if (client.second == new_client)
				return client.first;
		}
		uint8_t id = ++_nextClientId;
		_clients.insert(std::pair<uint8_t, udp::endpoint>(id, new_client));
		return id;
	}

	void Server::handle_error(const std::error_code &err, udp::endpoint endpoint)
	{
		bool found = false;
		uint8_t client_id = 0;
		for (const auto &client : _clients) {
			if (client.second == endpoint) {
				found = true;
				client_id = client.first;
				break;
			}
		}
		if (!found)
			return;
		_clients.erase(client_id);
	}

	void handleInput(std::size_t clientId, rtype::Input &input)
	{
		rtype::server::ecs::eventStorage<Input>.sendEvent(clientId, input);
	}
}