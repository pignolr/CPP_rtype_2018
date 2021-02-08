#include <utility>
#include "sfml/sfml.hpp"

namespace rtype::client::sfml {
	sfml::sfml()
	{
		resetInput();
		_win = new sf::RenderWindow(sf::VideoMode(_width, _height), "R-TYPE");
		_win->clear();
		_font.loadFromFile("./font/arial.ttf");
		_tbg.loadFromFile("./textures/bg.png");
		_mvm = 0;
		initSprites();
	}

	sfml::~sfml()
	{ _win->close(); }

	void sfml::getKeydown(const sf::Event &event)
	{
		switch (event.key.code)
		{
			case sf::Keyboard::Key::Up:
				_in.up = true;
				break ;
			case sf::Keyboard::Key::Right:
				_in.right = true;
				break ;
			case sf::Keyboard::Key::Down:
				_in.down = true;
				break ;
			case sf::Keyboard::Key::Left:
				_in.left = true;
				break ;
			case sf::Keyboard::Key::Escape:
				_in.esc = true;
				break ;
			case sf::Keyboard::Key::Space:
				_in.shoot = true;
				break ;
		}
	}

	void sfml::getKeyup(const sf::Event &event)
	{
		switch (event.key.code)
		{
			case sf::Keyboard::Key::Up:
				_in.up = false;
				break ;
			case sf::Keyboard::Key::Right:
				_in.right = false;
				break ;
			case sf::Keyboard::Key::Down:
				_in.down = false;
				break ;
			case sf::Keyboard::Key::Left:
				_in.left = false;
				break ;
			case sf::Keyboard::Key::Escape:
				_in.esc = false;
				break ;
			case sf::Keyboard::Key::Space:
				_in.shoot = false;
				break ;
		}
	}

	const rtype::Input &sfml::getInput()
	{
		sf::Event event;

		while (_win->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				_win->close();
				break ;
			}
			else if (event.type == sf::Event::KeyPressed)
				getKeydown(event);
			else if (event.type == sf::Event::KeyReleased)
				getKeyup(event);
		}
		return (_in);
	}

	const rtype::Input &sfml::resetInput()
	{
		_in.up = false;
		_in.right = false;
		_in.left = false;
		_in.down = false;
		_in.shoot = false;
		_in.esc = false;
		return (_in);
	}

	void sfml::initSprites()
	{
		_spirites[rtype::SpriteId::player1] = initSprite("./textures/r-typesheet69.png", 32, 12, rtype::ALLIES_UP);
		_spirites[rtype::SpriteId::player2] = initSprite("./textures/r-typesheet70.png", 32, 12, rtype::ALLIES_UP);
		_spirites[rtype::SpriteId::player3] = initSprite("./textures/r-typesheet71.png", 32, 12, rtype::ALLIES_UP);
		_spirites[rtype::SpriteId::player4] = initSprite("./textures/r-typesheet72.png", 32, 12, rtype::ALLIES_UP);
		_spirites[rtype::SpriteId::player5] = initSprite("./textures/r-typesheet73.png", 32, 12, rtype::ALLIES_UP);
		_spirites[rtype::SpriteId::player_shoot1] = initSprite("./textures/r-typesheet500.png", 7, 6, rtype::ALLIES_UP);
		_spirites[rtype::SpriteId::player_laser1] = initSprite("./textures/r-typesheet500.png", 7, 6, rtype::ALLIES_UP);
		_spirites[rtype::SpriteId::enemy1] = initSprite("./textures/r-typesheet161.png", 30, 28, rtype::ENEMIES_UP);
		_spirites[rtype::SpriteId::enemy_wall1] = initSprite("./textures/r-typesheet133.png", 54, 61, rtype::ENEMIES_UP);
		_spirites[rtype::SpriteId::enemy_wall2] = initSprite("./textures/r-typesheet134.png", 54, 61, rtype::ENEMIES_UP);
		_spirites[rtype::SpriteId::enemy_shoot1] = initSprite("./textures/r-typesheet501.png", 9, 8, rtype::ENEMIES_UP);
		_spirites[rtype::SpriteId::wall1] = initSprite("./textures/r-typesheet96.png", 52, 21, rtype::ENEMIES_UP);
		_spirites[rtype::SpriteId::wall2] = initSprite("./textures/r-typesheet97.png", 52, 21, rtype::ENEMIES_UP);
	}

	SpriteSheet sfml::initSprite(const std::string &path,
		int w, int h, int s)
	{
		SpriteSheet tmp;

		tmp.text.loadFromFile(path);
		tmp.spt.setTexture(tmp.text);
		tmp.width = w;
		tmp.height = h;
		tmp.scale = s;
		return (tmp);
	}

	void sfml::setSprites(const std::list<Sprite> & sp)
	{
		_sprites.clear();
		for (auto it = sp.begin(); it != sp.end(); ++it)
		{
			for (int i = 0; i < rtype::SpriteId::maxId; ++i)
			{
				if (it->id == i)
				{
					_sprites.push_back(_spirites[it->id]);
					_sprites.back().posX = it->posX;
					_sprites.back().posY = it->posY;
				}
			}
		}
	}

	void sfml::drawSprites()
	{
		_bg = sf::Sprite(_tbg, sf::IntRect(_mvm, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
		_win->draw(_bg);
		for (auto it = _sprites.begin(); it != _sprites.end(); ++it)
		{
			it->spt.setScale(it->scale, it->scale);
			it->spt.setPosition(it->posX, it->posY);
			it->spt.setTexture(it->text);
			_win->draw(it->spt);
		}
		_mvm = (_mvm < 500 ? _mvm + 1 : 0);
	}

	void sfml::display()
	{
		_win->clear();
		drawSprites();
		_win->display();
	}
}