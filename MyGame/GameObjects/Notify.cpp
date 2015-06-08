#include "Notify.h"

//==============================================================================================================
//
// Notify class
//
//==============================================================================================================

Notify::Notify()
{
}

Notify::~Notify()
{
}

void Notify::init(Font& font)
{
	m_inUse = false;
	m_label.init("", MyVec2(-1, -1), font, "", MyColor(1, 1, 0), 1.2f);
}

void Notify::update(Timer& timer)
{
	m_countTime += timer.getElapsedTime();
	if (m_countTime >= m_nTime) m_inUse = false;	
}

void Notify::render(Camera& camera, SpriteBatch& spriteBatch)
{
	if (m_inUse)
	{
		if ((fabs(m_label.getPos().x + 1) <= 0.01) && (fabs(m_label.getPos().y + 1) <= 0.01))
		{
			int w, h;
			getWindowDimension(w, h);
			MyVec2 posS = project(m_position, w, h, camera.getView(), camera.getProj());

			m_label.setPos(posS);
		}
		else
			m_label.setPos(m_label.getPos() + m_vTranslate);

		m_label.render(spriteBatch);
	}
}

void Notify::respawn(const MyString& content,
	const MyVec3& position,
	const MyVec2& vTranslate,
	float nTime)
{
	m_content = content;
	m_position = position;
	m_vTranslate = vTranslate;
	m_nTime = nTime;
	m_inUse = true;

	m_label.setPos(MyVec2(-1, -1));
	m_label.setText(content);
	m_countTime = 0;
}

bool Notify::inUse()const
{
	return m_inUse;
}
//==============================================================================================================
//
// NotifyPool class
//
//==============================================================================================================

NotifyPool::NotifyPool()
{
}

NotifyPool::~NotifyPool()
{
}

void NotifyPool::init()
{	
	m_font.init(resolveAssetsPath("Fonts/Consolas12.pak"));
	for (int i = 0; i < MAX_NUM_NOTIFIES; i++)
	{
		Notify* notify = m_notifies + i;
		notify->init(m_font);
	}
}

void NotifyPool::update(Timer& timer)
{
	for (int i = 0; i < MAX_NUM_NOTIFIES; i++)
	{
		Notify* notify = m_notifies + i;
		if (notify->inUse())
		{
			notify->update(timer);
		}
	}
}

void NotifyPool::render(Camera& camera, SpriteBatch& spriteBatch)
{
	for (int i = 0; i < MAX_NUM_NOTIFIES; i++)
	{
		Notify* notify = m_notifies + i;
		if (notify->inUse())
		{
			notify->render(camera, spriteBatch);
		}
	}
}

void NotifyPool::spawnNotify(const MyString& content,
	const MyVec3& position,
	const MyVec2& vTranslate,
	float nTime)
{
	Notify* notify = getFreeSlot();
	if (notify != nullptr)
	{
		notify->respawn(content, position, vTranslate, nTime);
	}
}

Notify* NotifyPool::getFreeSlot()
{
	for (int i = 0; i < MAX_NUM_NOTIFIES; i++)
	{
		Notify* notify = m_notifies + i;
		if (!notify->inUse())
		{
			return notify;
		}
	}

	return nullptr;
}