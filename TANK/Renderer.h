#pragma once
#include "Event.h"
#include <list>
#include "SDL\AnimationFactory.h"

struct SDL_Window;
struct SDL_Renderer;
class Scene;

class Renderer :
	public EventInterface
{
public:

	static constexpr int RENDER = 0x88181;

	Renderer(SDL_Window *window, bool VSync = false);
	~Renderer();

	Renderer();

	const AnimationFactory &animationFactory() const {
		return m_animationFactory;
	}

	//获取SDL渲染器。
	SDL_Renderer *renderer()
	{
		return m_renderer;
	}

	//获取渲染器的目标窗口。
	SDL_Window *window() {
		return m_window;
	}

	void addScene(Scene *s);

	//参数 flip：参见SDL_RendererFlip
	void renderTexture(SDL_Texture *texture, const SDL_Rect &destRect, const SDL_Rect &srcRect, 
		Uint32 angle, Uint8 alpha = SDL_ALPHA_OPAQUE, int flip = 0);

private:
	virtual void userEventHookProc(const SDL_UserEvent &event) override;
	virtual void eventHookProc(const SDL_Event & event) override;
	void render();

	std::list<Scene *> m_renderQueue;
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	AnimationFactory m_animationFactory;
	SIZE m_windowOriginalSize, m_windowSize;
	float m_windowWidthScale, m_windowHeightScale, m_windowWHScale;
};

