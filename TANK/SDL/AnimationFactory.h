#pragma once
#include <map>
#include <string>
#include <SDL_rect.h>

struct SDL_Texture;
class Renderer;

struct Animation
{
	SDL_Texture *texture = 0;
	SDL_Rect clipRect = { 0, 0, 0, 0};
	Uint32 frameCount = 0;
	Uint32 frameTimeInterval = 0;
	Uint32 frameInterval = 0;
};

struct SIZE
{
	Uint32 w, h;
};

class AnimationFactory
{
public:
	AnimationFactory();
	void createFactory(Renderer *renderer);
	const Animation &findAnimation(const std::string &name) const;
	~AnimationFactory();
private:
	std::map<std::string, SDL_Texture *> m_texturePool;
	SDL_Texture *m_texture;
	std::map<std::string, Animation> m_animationPool;
	Renderer *m_renderer;
};

