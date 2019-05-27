#include "../ResultView.h"
#include "../TANKS.h"
#include "../Renderer.h"
#include "../FileLoader.h"
#include <sstream>

ResultView::ResultView(Renderer * renderer)
	:Scene(renderer, { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT })
{

	setBackdropColor({ 0, 0, 9, 255 });

	//�������������λ�����ݡ�
	std::vector<std::string> data;
	if (fileLoad("resultViewLayout", &data) != 5)
		return;

	std::vector<std::istringstream> stream(data.begin(), data.end());

	//׼�����⡣
	stream[0] >> m_title1Position.x >> m_title1Position.y >> m_title2Position.x >> m_title2Position.y;
	m_title1.setScene(this);
	m_title1.setScene(this);
	m_title1.setTextRenderFlags(Text::biend);
	m_title2.setTextRenderFlags(Text::biend);
	m_title1.setFontColor(title1Color);
	m_title2.setFontColor(title2Color);
	m_title1.setFontSize(buttonFontSize);
	m_title2.setFontSize(buttonFontSize);
	m_title1.setString(L"Player I");
	m_title2.setString(L"Player II");

	//׼���б���
	SDL_Point layoutOffset, modelIconPos, killCountPos, scorePos;
	stream[1] >> layoutOffset.x >> layoutOffset.y >> modelIconPos.x >> modelIconPos.y;
	stream[2] >> killCountPos.x >> killCountPos.y >> scorePos.x >> scorePos.y;
	for (auto i = 0; i != m_lists.size(); ++i) {
		for (auto j = 0; j != m_lists[i].size(); ++j) {
			std::string iconName;
			stream[1] >> iconName;
			auto a = renderer->animationFactory().findAnimation(iconName);
			a.frameCount = 1;

			m_lists[i][j].icon.setScene(this);
			m_lists[i][j].killCountBox.setScene(this);
			m_lists[i][j].scoreBox.setScene(this);
			m_lists[i][j].icon.setAnimation(a);
			m_lists[i][j].iconPosition = modelIconPos;
			m_lists[i][j].killCountBox.setTextRenderFlags(Text::biend);
			m_lists[i][j].killCountBox.setFontSize(dataFontSize);
			m_lists[i][j].killCountBox.setFontColor(color);
			m_lists[i][j].killCountBox.setString(L"�� " + std::to_wstring(m_lists[i][j].killCount));
			m_lists[i][j].scoreBox.setTextRenderFlags(Text::biend);
			m_lists[i][j].scoreBox.setFontSize(30);
			m_lists[i][j].scoreBox.setFontColor(color);
			m_lists[i][j].scoreBox.setString(L"= " + std::to_wstring(m_lists[i][j].score));
			m_lists[i][j].killCountPosition = { killCountPos.x, killCountPos.y + (i * layoutOffset.y) };
			m_lists[i][j].scorePosition = { scorePos.x + (i * layoutOffset.x), scorePos.y + (j * layoutOffset.y) };
		}
	}

	//׼��ͳ�ƿ�
	for (auto &c : m_countBox) {
		c.label.setScene(this);
		c.numberBox.setScene(this);
		c.label.setTextRenderFlags(Text::biend);
		c.numberBox.setTextRenderFlags(Text::biend);
		c.label.setFontColor(color);
		c.numberBox.setFontColor(color);
		c.label.setFontSize(dataFontSize);
		c.numberBox.setFontSize(dataFontSize);
		c.numberBox.setString(std::to_wstring(c.data));
		stream[3] >> c.labelPosition.x >> c.labelPosition.y >> c.numberBoxPosition.x >> c.numberBoxPosition.y;
	}
	m_countBox[0].label.setString(L"KILL: ");
	m_countBox[1].label.setString(L"TOTAL: ");
	m_countBox[2].label.setString(L"KILL: ");
	m_countBox[3].label.setString(L"TOTAL: ");

	//׼����ť��
	m_select = 0;
	m_cursor.setScene(this);
	m_home.text.setScene(this);
	m_next.text.setScene(this);
	m_cursor.setAnimation("P1");
	m_cursor.setAngle(90);
	stream[4] >> m_home.cursorPosition.x >> m_home.cursorPosition.y >> m_home.position.x >> m_home.position.y
		>> m_next.cursorPosition.x >> m_next.cursorPosition.y >> m_next.position.x >> m_next.position.y;
	m_home.text.setTextRenderFlags(Text::biend);
	m_next.text.setTextRenderFlags(Text::biend);
	m_home.text.setFontColor(color);
	m_next.text.setFontColor(color);
	m_home.text.setFontSize(buttonFontSize);
	m_next.text.setFontSize(buttonFontSize);
	m_home.text.setString(L"HOME");
	m_next.text.setString(L"NEXT");

	loadKey("P1control");
	loadKey("P2control");
	
}

ResultView::~ResultView()
{
	close();
}

void ResultView::open(void * data, int code)
{
	setEventHook(SDL_KEYDOWN);
	setEventHook(SDL_MOUSEMOTION);
	setState(true);
}

void ResultView::close()
{
	setState(false);
	unsetEventHook(SDL_KEYDOWN);
	unsetEventHook(SDL_MOUSEMOTION);
}

int ResultView::render()
{
	m_title1.renderFrame(m_title1Position);
	m_title2.renderFrame(m_title2Position);

	for (int i = 0; i != m_lists.size(); ++i) {
		for (int j = 0; j != m_lists[i].size(); ++j) {
			m_lists[i][j].icon.renderFrame(m_lists[i][j].iconPosition);
			m_lists[i][j].killCountBox.renderFrame(m_lists[i][j].killCountPosition);
			m_lists[i][j].scoreBox.renderFrame(m_lists[i][j].scorePosition);
		}
	}

	m_home.text.renderFrame(m_home.position);
	m_next.text.renderFrame(m_next.position);
	m_cursor.renderFrame((!m_select) ? m_home.cursorPosition : m_next.cursorPosition);

	for (auto c : m_countBox) {
		c.label.renderFrame(c.labelPosition);
		c.numberBox.renderFrame(c.numberBoxPosition);
	}

	return 0;
}

void ResultView::update(Uint32 time)
{
}

void ResultView::eventHookProc(const SDL_Event & event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		break;
	case SDL_MOUSEMOTION:
		//TODO::�������ѡ��ѡ��Ĺ��ܡ�
		break;
	default:
		break;
	}
}

void ResultView::loadKey(const std::string & dataGroup)
{
	std::vector<std::string> control;
	if (fileLoad(dataGroup, &control) != 6)
		return;

	for (auto i = 0; i != 4; ++i) {
		SDL_Keycode code = SDL_GetKeyFromName(control[i].c_str());
		if (code != SDLK_UNKNOWN)
			m_switchKeys.insert(code);
	}

	for (auto i = 4; i != control.size(); ++i) {
		SDL_Keycode code = SDL_GetKeyFromName(control[i].c_str());
		if (code != SDLK_UNKNOWN)
			m_selectKeys.insert(code);
	}
}