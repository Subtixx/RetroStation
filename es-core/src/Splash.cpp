#include "Splash.h"
#include "ThemeData.h"
#include "Window.h"
#include "resources/TextureResource.h"
#include "SystemConf.h"
#include "platform.h"

#include <algorithm>
#include <SDL_events.h>

Splash::Splash(Window* window, const std::string image, bool fullScreenBackGround) :
	mBackground(window),
	mText(window),
	mInactiveProgressbar(window),
	mActiveProgressbar(window)	
{
	mBackgroundColor = 0x000000FF;
	mRoundCorners = 0.01;
	mPercent = -1;

	// Theme
	auto theme = std::make_shared<ThemeData>();

	std::string themeFilePath = fullScreenBackGround ? ":/splash.xml" : ":/gamesplash.xml";
	themeFilePath = ResourceManager::getInstance()->getResourcePath(themeFilePath);

	std::map<std::string, ThemeSet> themeSets = ThemeData::getThemeSets();
	auto themeset = themeSets.find(Settings::getInstance()->getString("ThemeSet"));
	if (themeset != themeSets.cend())
	{
		std::string path = themeset->second.path + (fullScreenBackGround ? "/splash.xml" : "/gamesplash.xml");
		if (Utils::FileSystem::exists(path))
			themeFilePath = path;
	}

	if (Utils::FileSystem::exists(themeFilePath))
	{
		std::map<std::string, std::string> sysData;
        // Global variables
        sysData["global.help"] = Settings::getInstance()->getBool("ShowHelpPrompts") ? "true" : "false";
        sysData["global.clock"] = Settings::DrawClock() ? "true" : "false";
        sysData["global.architecture"] = getArchString();

        sysData["global.cheevos"] = SystemConf::getInstance()->getBool("global.retroachievements") ? "true" : "false";
        sysData["global.cheevos.username"] = SystemConf::getInstance()->getBool("global.retroachievements") ? SystemConf::getInstance()->get("global.retroachievements.username") : "";

        sysData["global.netplay"] = SystemConf::getInstance()->getBool("global.netplay") ? "true" : "false";
        sysData["global.netplay.username"] = SystemConf::getInstance()->getBool("global.netplay") ? SystemConf::getInstance()->get("global.netplay.nickname") : "";

        // Screen
        sysData["screen.width"] = std::to_string(Renderer::getScreenWidth());
        sysData["screen.height"] = std::to_string(Renderer::getScreenHeight());
        sysData["screen.ratio"] = Renderer::getAspectRatio();
        sysData["screen.vertical"] = Renderer::isVerticalScreen() ? "true" : "false";

        // Retrocompatibility
        sysData["cheevos.username"] = SystemConf::getInstance()->getBool("global.retroachievements") ? SystemConf::getInstance()->get("global.retroachievements.username") : "";

		try { theme->loadFile("splash", sysData, themeFilePath); }
		catch(...) { }
	}

	// Background image
	std::string imagePath = image;

	auto backGroundImageTheme = theme->getElement("splash", "background", "image");
	if (backGroundImageTheme && backGroundImageTheme->has("path") && Utils::FileSystem::exists(backGroundImageTheme->get<std::string>("path")))
		imagePath = backGroundImageTheme->get<std::string>("path");

	bool linearSmooth = false;
	if (backGroundImageTheme && backGroundImageTheme->has("linearSmooth"))
		linearSmooth = backGroundImageTheme->get<bool>("linearSmooth");
	
	if (fullScreenBackGround)
	{
		mBackground.setOrigin(0.5, 0.5);
		mBackground.setPosition(Renderer::getScreenWidth() / 2, Renderer::getScreenHeight() / 2);

		if (backGroundImageTheme)
			mBackground.setMaxSize(Renderer::getScreenWidth(), Renderer::getScreenHeight());
		else
		{
			mBackground.setMinSize(Renderer::getScreenWidth(), Renderer::getScreenHeight());
			linearSmooth = true;
		}
	}

	// Label
	auto font = Font::get(FONT_SIZE_MEDIUM);

    mText.setColor(0xFFFFFFFF);

	mText.setHorizontalAlignment(ALIGN_CENTER);
	mText.setFont(font);

	if (fullScreenBackGround)
		mText.setPosition(0, Renderer::getScreenHeight() * 0.78f);
	else
		mText.setPosition(0, Renderer::getScreenHeight() * 0.835f);

	mText.setSize(Renderer::getScreenWidth(), font->getLetterHeight());

	Vector2f mProgressPosition;
	Vector2f mProgressSize;

	if (backGroundImageTheme)
		mBackground.applyTheme(theme, "splash", "background", ThemeFlags::ALL ^ (ThemeFlags::PATH));

	auto maxSize = mBackground.getMaxSizeInfo();
	mTexture = TextureResource::get(imagePath, false, linearSmooth, true, false, false, &maxSize);

	if (!fullScreenBackGround)
		ResourceManager::getInstance()->removeReloadable(mTexture);

	mBackground.setImage(mTexture);

	if (theme->getElement("splash", "label", "text"))
		mText.applyTheme(theme, "splash", "label", ThemeFlags::ALL ^ (ThemeFlags::TEXT));
	else if (fullScreenBackGround)
	{
		mText.setGlowColor(0x00000020);
		mText.setGlowSize(2);
		mText.setGlowOffset(1, 1);
	}

	// Splash background
	auto elem = theme->getElement("splash", "splash", "splash");
	if (elem && elem->has("backgroundColor"))
		mBackgroundColor = elem->get<unsigned int>("backgroundColor");

	// Progressbar
	float baseHeight = 0.036f;

	float w = Renderer::getScreenWidth() / 2.0f;
	float h = Renderer::getScreenHeight() * baseHeight;
	float x = Renderer::getScreenWidth() / 2.0f - w / 2.0f;
	float y = Renderer::getScreenHeight() - (Renderer::getScreenHeight() * 3 * baseHeight);

	auto blankTexture = TextureResource::get("", false, true, true, false, false);

	mInactiveProgressbar.setImage(blankTexture);
	mActiveProgressbar.setImage(blankTexture);

	mInactiveProgressbar.setPosition(x, y);
	mInactiveProgressbar.setSize(w, h);
	mInactiveProgressbar.setResize(w, h);
	mActiveProgressbar.setPosition(x, y);
	mActiveProgressbar.setSize(w, h);
	mActiveProgressbar.setResize(w, h);

	mInactiveProgressbar.setColorShift(0x6060607F);
	mInactiveProgressbar.setColorShiftEnd(0x9090907F);
	mInactiveProgressbar.setColorGradientHorizontal(true);

	elem = theme->getElement("splash", "progressbar", "image");
	if (elem)
	{
		mInactiveProgressbar.applyTheme(theme, "splash", "progressbar", ThemeFlags::ALL);
		mActiveProgressbar.applyTheme(theme, "splash", "progressbar", ThemeFlags::ALL);

		mRoundCorners = mInactiveProgressbar.getRoundCorners();
	}

    mActiveProgressbar.setColorShift(0xDF1010FF);
    mActiveProgressbar.setColorShiftEnd(0x4F0000FF);

	mActiveProgressbar.setColorGradientHorizontal(true);

	elem = theme->getElement("splash", "progressbar:active", "image");
	if (elem)
		mActiveProgressbar.applyTheme(theme, "splash", "progressbar:active", ThemeFlags::ALL);

	mInactiveProgressbar.setRoundCorners(0);
	mActiveProgressbar.setRoundCorners(0);

	mExtras = ThemeData::makeExtras(theme, "splash", window, true);

	if (!fullScreenBackGround)
	{
		for (auto im : mExtras)
			if (im->isKindOf<ImageComponent>())
				ResourceManager::getInstance()->removeReloadable(((ImageComponent*)im)->getTexture());
	}

	std::stable_sort(mExtras.begin(), mExtras.end(), [](GuiComponent* a, GuiComponent* b) { return b->getZIndex() > a->getZIndex(); });

	// Don't waste time in waiting for vsync
	SDL_GL_SetSwapInterval(0);
}

Splash::~Splash()
{
	Renderer::setSwapInterval();

	for (auto extra : mExtras)
		delete extra;

	mExtras.clear();
}

void Splash::update(std::string text, float percent)
{
	mText.setText(text);
	mPercent = percent;
}

void Splash::render(float opacity, bool swapBuffers)
{
	if (opacity == 0)
		return;

	unsigned char alpha = (unsigned char)(opacity * 255);

	mText.setOpacity(alpha);

	Transform4x4f trans = Transform4x4f::Identity();
	Renderer::setMatrix(trans);
	Renderer::drawRect(0, 0, Renderer::getScreenWidth(), Renderer::getScreenHeight(), (mBackgroundColor & 0xFFFFFF00) | alpha);

	for (auto extra : mExtras)
		extra->setOpacity(alpha);

	for (auto extra : mExtras)
		if (extra->getZIndex() < 5)
			extra->render(trans);

	mBackground.setOpacity(alpha);
	mBackground.render(trans);

	for (auto extra : mExtras)
		if (extra->getZIndex() >= 5 && extra->getZIndex() < 10)
			extra->render(trans);

	if (mPercent >= 0)
	{
		Renderer::setMatrix(trans);

		auto pos = mInactiveProgressbar.getPosition();
		auto sz = mInactiveProgressbar.getSize();

		mInactiveProgressbar.setOpacity(alpha);
		mActiveProgressbar.setOpacity(alpha);
		mActiveProgressbar.setSize(sz.x()  * mPercent, sz.y());

		float radius = Math::max(sz.x(), sz.y()) * mRoundCorners;

		if (radius > 1)
			Renderer::enableRoundCornerStencil(pos.x(), pos.y(), sz.x(), sz.y(), radius);

		mInactiveProgressbar.render(trans);
		mActiveProgressbar.render(trans);

		if (radius > 1)
			Renderer::disableStencil();
	}

	if (!mText.getText().empty())
	{
		// Ensure font is loaded
		auto font = mText.getFont();
		if (font != nullptr)
			font->reload();
		else
			Font::get(FONT_SIZE_MEDIUM)->reload();

		mText.render(trans);
	}

	for (auto extra : mExtras)
		if (extra->getZIndex() >= 10)
			extra->render(trans);

	if (swapBuffers)
	{
		Renderer::swapBuffers();

#if defined(_WIN32)
		// Avoid Window Freezing on Windows
		SDL_Event event;
		while (SDL_PollEvent(&event));
#endif
	}
}
