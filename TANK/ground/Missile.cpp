#include "Missile.h"
#include "Ground.h"
#include <SDL_timer.h>
#include "Tank.h"
#include "../Timer.h"

Missile::Missile(Ground * ground, Tank * sender, int power)
	:Spirit(ground, "missile"), m_sender(sender), m_destory(false), m_boom(false), m_ground(ground), m_power(power)
{
	m_startTime = Timer::current();

	int lenght;

	switch (sender->direction())
	{
	case Mover::UP:
		m_position.x = sender->pixelPosition().x + Tank::colSize / 2 - Missile::missileSize / 2;
		m_position.y = sender->pixelPosition().y;
		lenght = m_position.y;
		break;
	case Mover::RIGHT:
		m_position.x = sender->pixelPosition().x + Tank::colSize - Missile::missileSize;
		m_position.y = sender->pixelPosition().y + Tank::colSize / 2 - Missile::missileSize / 2;
		lenght = (MAP_SIZE * GRID_SIZE) - m_position.x;
		break;
	case Mover::DOWN:
		m_position.x = sender->pixelPosition().x + Tank::colSize / 2 - Missile::missileSize / 2;
		m_position.y = sender->pixelPosition().y + Tank::colSize - Missile::missileSize;
		lenght = (MAP_SIZE * GRID_SIZE) - m_position.y;
		break;
	case Mover::LEFT:
		m_position.x = sender->pixelPosition().x;
		m_position.y = sender->pixelPosition().y + Tank::colSize / 2 - Missile::missileSize / 2;
		lenght = m_position.x;
		break;
	default:
		break;
	}
	m_mover.move(m_position, sender->direction(), lenght, m_startTime, 1);

	m_driver = m_sender->driver();
}

void Missile::update(Uint32 time)
{
	if (m_destory)
		return;

	if (m_mover.state()) { //飞弹移动中。
		m_position = m_mover.current(time);  //更新位置。
		//检查碰撞。
		auto result = m_ground->MissilepositionUpdate(this);
		if (result) {  //遇到碰撞.
			m_boomTarget = static_cast<TARGET>(result);
			m_mover.endMove();
		}
	}

	if(!m_mover.state() && !m_boom) {
		//开始爆炸
		m_mover.endMove();
		m_startTime = time;
		m_boom = true;
		m_position.x -= 15;
		m_position.y -= 15;
		setAnimation("boom");

		SDL_UserEvent user;
		user.type = BEGIN_BOOM;
		user.code = m_boomTarget;
		user.data1 = this;
		user.data2 = m_sender;
		user.timestamp = time;

		director->userEventTrigger(user);
		
	}

	if (m_boom) {
		if (time - m_startTime >= 250) {
			SDL_UserEvent user;
			user.type = END_BOOM;
			user.code = m_boomTarget;
			user.data1 = this;
			user.data2 = m_sender;
			user.timestamp = time;

			director->userEventTrigger(user);
			m_destory = true;  //生命期结束，等待对象被销毁。
			m_sender->reload();
		}
	}
}

void Missile::render()
{
	Spirit::renderFrame(m_position);
}
