/*
 * Copyright (c) Subtixx (Dominic Hock) 2023.
 * Copyright (c) 2014 Alec Lofquist
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 */

#include "FileData.h"

#include "utils/FileSystemUtil.h"
#include "utils/StringUtil.h"
#include "utils/TimeUtil.h"
#include "AudioManager.h"
#include "CollectionSystemManager.h"
#include <loguru.hpp>
#include "MameNames.h"
#include "platform.h"
#include "Scripting.h"
#include "SystemData.h"
#include "VolumeControl.h"
#include "Window.h"
#include "views/UIModeController.h"
#include "SystemConf.h"
#include "InputManager.h"
#include "scrapers/ThreadedScraper.h"
#include "Gamelist.h" 
#include "ApiSystem.h"
#include <ctime>
#include <algorithm>
#include "LangParser.h"
#include "resources/ResourceManager.h"
#include "RetroAchievements.h"
#include "SaveStateRepository.h"
#include "TextToSpeech.h"
#include "LocaleES.h"
#include "guis/GuiMsgBox.h"
#include "Paths.h"
#include "resources/TextureData.h"

#include "FolderFileData.h"

FileData* FileData::mRunningGame = nullptr;

FileData::FileData(FileType type, const std::string& path, SystemData* system)
	: mPath(path), mType(type), mSystem(system), mParent(nullptr), mDisplayName(nullptr), mMetadata(type == GAME ? GAME_METADATA : FOLDER_METADATA) // metadata is REALLY set in the constructor!
{
	// metadata needs at least a name field (since that's what getName() will return)
	if (mMetadata.get(MetaDataId::Name).empty() && !mPath.empty())
		mMetadata.set(MetaDataId::Name, getDisplayName());
	
	mMetadata.resetChangedFlag();
}

const std::string FileData::getPath() const
{
	if (mPath.empty())
		return getSystemEnvData()->mStartPath;

	return mPath;
}

const std::string FileData::getBreadCrumbPath()
{
	std::vector<std::string> paths;

	FileData* root = getSystem()->getParentGroupSystem() != nullptr ? getSystem()->getParentGroupSystem()->getRootFolder() : getSystem()->getRootFolder();

	FileData* parent = (getType() == GAME ? getParent() : this);
	parent = (getType() == GAME ? getParent() : this);
	while (parent != nullptr)
	{
		if (parent == root->getSystem()->getRootFolder() && !parent->getSystem()->isCollection())
			break;
		
		if (parent->getSystem()->getName() == CollectionSystemManager::get()->getCustomCollectionsBundle()->getName())
			break;

		if (parent->getSystem()->isGroupChildSystem() && 
			parent->getSystem()->getParentGroupSystem() != nullptr && 
			parent->getParent() == parent->getSystem()->getParentGroupSystem()->getRootFolder() && 			
			parent->getSystem()->getName() != "windows_installers")
			break;

		paths.push_back(parent->getName());
		parent = parent->getParent();
	}

	std::reverse(paths.begin(), paths.end());
	return Utils::String::join(paths, " > ");
}


const std::string FileData::getConfigurationName()
{
	std::string gameConf = Utils::FileSystem::getFileName(getPath());
	gameConf = Utils::String::replace(gameConf, "=", "");
	gameConf = Utils::String::replace(gameConf, "#", "");
	gameConf = getSourceFileData()->getSystem()->getName() + std::string("[\"") + gameConf + std::string("\"]");
	return gameConf;
}

inline SystemEnvironmentData* FileData::getSystemEnvData() const
{ 
	return mSystem->getSystemEnvData(); 
}

std::string FileData::getSystemName() const
{
	return mSystem->getName();
}

FileData::~FileData()
{
	if (mDisplayName)
		delete mDisplayName;

	if (mParent)
		mParent->removeChild(this);

	if (mType == GAME)
		mSystem->removeFromIndex(this);
}

std::string& FileData::getDisplayName()
{
	if (mDisplayName == nullptr)
	{
		std::string stem = Utils::FileSystem::getStem(getPath());
		if (mSystem && (mSystem->hasPlatformId(PlatformIds::ARCADE) || mSystem->hasPlatformId(PlatformIds::NEOGEO)))
			stem = MameNames::getInstance()->getRealName(stem);

		mDisplayName = new std::string(stem);
	}

	return *mDisplayName;
}

std::string FileData::getCleanName()
{
	return Utils::String::removeParenthesis(getDisplayName());
}

std::string FileData::findLocalArt(const std::string& type, std::vector<std::string> exts)
{
	if (Settings::getInstance()->getBool("LocalArt"))
	{
		for (auto ext : exts)
		{
			std::string path = getSystemEnvData()->mStartPath + "/images/" + getDisplayName() + (type.empty() ? "" :  "-" + type) + ext;
			if (Utils::FileSystem::exists(path))
				return path;

			if (type == "video")
			{
				path = getSystemEnvData()->mStartPath + "/videos/" + getDisplayName() + "-" + type + ext;
				if (Utils::FileSystem::exists(path))
					return path;

				path = getSystemEnvData()->mStartPath + "/videos/" + getDisplayName() + ext;
				if (Utils::FileSystem::exists(path))
					return path;
			}
		}
	}

	return "";
}

const std::string FileData::getThumbnailPath()
{
	std::string thumbnail = getMetadata(MetaDataId::Thumbnail);

	// no thumbnail, try image
	if (thumbnail.empty())
	{
		thumbnail = findLocalArt("thumb");
		if (!thumbnail.empty())
			setMetadata(MetaDataId::Thumbnail, thumbnail);

		// no image, try to use local image

		if (thumbnail.empty())
			thumbnail = getMetadata(MetaDataId::Image);

		if (thumbnail.empty())
			thumbnail = findLocalArt("image");

		if (thumbnail.empty())
			thumbnail = findLocalArt();

		if (thumbnail.empty() && getType() == GAME && getSourceFileData()->getSystem()->hasPlatformId(PlatformIds::IMAGEVIEWER))
		{
			if (getType() == FOLDER && ((FolderData*)this)->mChildren.size())
				return ((FolderData*)this)->mChildren[0]->getThumbnailPath();
			else if (getType() == GAME)
			{
				thumbnail = getPath();

				auto ext = Utils::String::toLower(Utils::FileSystem::getExtension(thumbnail));
				if (TextureData::PdfHandler == nullptr && ext == ".pdf" && ResourceManager::getInstance()->fileExists(":/pdf.jpg"))
					return ":/pdf.jpg";
			}
		}

	}

	return thumbnail;
}

const bool FileData::getFavorite()
{
	return getMetadata(MetaDataId::Favorite) == "true";
}

const bool FileData::getHidden()
{
	return getMetadata(MetaDataId::Hidden) == "true";
}

const bool FileData::getKidGame()
{
	auto data = getMetadata(MetaDataId::KidGame);
	return data != "false" && !data.empty();
}

const bool FileData::hasCheevos()
{
	if (Utils::String::toInteger(getMetadata(MetaDataId::CheevosId)) > 0)
		return getSourceFileData()->getSystem()->isCheevosSupported();

	return false;
}

bool FileData::hasAnyMedia()
{
	if (Utils::FileSystem::exists(getImagePath()) || Utils::FileSystem::exists(getThumbnailPath()) || Utils::FileSystem::exists(getVideoPath()))
		return true;

	for (auto mdd : mMetadata.getMDD())
	{
		if (mdd.type != MetaDataType::MD_PATH)
			continue;

		std::string path = mMetadata.get(mdd.key);
		if (path.empty())
			continue;

		if (mdd.id == MetaDataId::Manual || mdd.id == MetaDataId::Magazine)
		{
			if (Utils::FileSystem::exists(path))
				return true;
		}
		else if (mdd.id != MetaDataId::Image && mdd.id != MetaDataId::Thumbnail)
		{
			if (Utils::FileSystem::isImage(path))
				continue;

			if (Utils::FileSystem::exists(path))
				return true;
		}
	}

	return false;
}

std::vector<std::string> FileData::getFileMedias()
{
	std::vector<std::string> ret;

	for (auto mdd : mMetadata.getMDD())
	{
		if (mdd.type != MetaDataType::MD_PATH)
			continue;

		if (mdd.id == MetaDataId::Video || mdd.id == MetaDataId::Manual || mdd.id == MetaDataId::Magazine)
			continue;

		std::string path = mMetadata.get(mdd.key);
		if (path.empty())
			continue;

		if (!Utils::FileSystem::isImage(path))
			continue;
		
		if (Utils::FileSystem::exists(path))
			ret.push_back(path);
	}

	return ret;
}

void FileData::resetSettings() 
{
	
}

const std::string& FileData::getName()
{
	if (mSystem != nullptr && mSystem->getShowFilenames())
		return getDisplayName();

	return mMetadata.getName();
}

const std::string FileData::getVideoPath()
{
	std::string video = getMetadata(MetaDataId::Video);
	
	// no video, try to use local video
	if (video.empty())
	{
		video = findLocalArt("video", { ".mp4" });
		if (!video.empty())
			setMetadata(MetaDataId::Video, video);
	}
	
	if (video.empty() && getSourceFileData()->getSystem()->hasPlatformId(PlatformIds::IMAGEVIEWER))
	{
		if (getType() == FOLDER && ((FolderData*)this)->mChildren.size())
			return ((FolderData*)this)->mChildren[0]->getVideoPath();
		else if (getType() == GAME)
		{
			if (Utils::FileSystem::isVideo(getPath()))
				return getPath();

			if (Utils::FileSystem::isAudio(getPath()))
				return getPath();
		}
	}

	return video;
}

const std::string FileData::getMarqueePath()
{
	std::string marquee = getMetadata(MetaDataId::Marquee);

	// no marquee, try to use local marquee
	if (marquee.empty())
	{
		marquee = findLocalArt("marquee");
		if (!marquee.empty())
			setMetadata(MetaDataId::Marquee, marquee);
	}
	
	return marquee;
}

const std::string FileData::getImagePath()
{
	std::string image = getMetadata(MetaDataId::Image);

	// no image, try to use local image
	if (image.empty())
	{
		auto romExt = Utils::String::toLower(Utils::FileSystem::getExtension(getPath()));
		if (romExt == ".png" || (getSystemName() == "pico8" && romExt == ".p8"))
			return getPath();

		if (image.empty())
			image = findLocalArt("image");

		if (image.empty())
			image = findLocalArt();

		if (!image.empty())
			setMetadata(MetaDataId::Image, image);

		if (image.empty() && getSourceFileData()->getSystem()->hasPlatformId(PlatformIds::IMAGEVIEWER))
		{
			if (getType() == FOLDER && ((FolderData*)this)->mChildren.size())
				return ((FolderData*)this)->mChildren[0]->getImagePath();
			else if (getType() == GAME)
			{
				image = getPath();

				auto ext = Utils::String::toLower(Utils::FileSystem::getExtension(image));
				if (TextureData::PdfHandler == nullptr && ext == ".pdf" && ResourceManager::getInstance()->fileExists(":/pdf.jpg"))
					return ":/pdf.jpg";

				if (Utils::FileSystem::isAudio(image) && ResourceManager::getInstance()->fileExists(":/mp3.jpg"))
					return ":/mp3.jpg";
			}
		}
	}

	return image;
}

std::string FileData::getKey()
{
	return getFileName();
}

const bool FileData::isArcadeAsset()
{
	if (mSystem && (mSystem->hasPlatformId(PlatformIds::ARCADE) || mSystem->hasPlatformId(PlatformIds::NEOGEO)))
	{	
		const std::string stem = Utils::FileSystem::getStem(getPath());
		return MameNames::getInstance()->isBios(stem) || MameNames::getInstance()->isDevice(stem);		
	}

	return false;
}

const bool FileData::isVerticalArcadeGame()
{
	if (mSystem && mSystem->hasPlatformId(PlatformIds::ARCADE))
		return MameNames::getInstance()->isVertical(Utils::FileSystem::getStem(getPath()));

	return false;
}

const bool FileData::isLightGunGame()
{
	return MameNames::getInstance()->isLightgun(Utils::FileSystem::getStem(getPath()), mSystem->getName(), mSystem && mSystem->hasPlatformId(PlatformIds::ARCADE));
	//return Genres::genreExists(&getMetadata(), GENRE_LIGHTGUN);
}

FileData* FileData::getSourceFileData()
{
	return this;
}

static std::string formatCommandLineArgument(const std::string& name)
{
	if (name.find(" ") != std::string::npos)
		return "\"" + Utils::String::replace(name, "\"", "\\\"") + "\"";

	return Utils::String::replace(name, "\"", "\\\"");
};

std::string FileData::getLaunchCommand(LaunchGameOptions& options, bool includeControllers)
{
	FileData* gameToUpdate = getSourceFileData();
	if (gameToUpdate == nullptr)
		return "";

	SystemData* system = gameToUpdate->getSystem();
	if (system == nullptr)
		return "";

	// must really;-) be done before window->deinit while it closes joysticks
	std::string controllersConfig = InputManager::getInstance()->configureEmulators();

#if WIN32
	if (gameToUpdate->isLightGunGame())
#else
	if (InputManager::getInstance()->getGuns().size() && gameToUpdate->isLightGunGame())
#endif
		controllersConfig = controllersConfig + "-lightgun ";
	
	std::string systemName = system->getName();
	std::string emulator = getEmulator();
	std::string core = getCore();

	bool forceCore = false;

	if (options.netPlayMode == CLIENT && !options.core.empty() && core != options.core)
	{
		for (auto& em : system->getEmulators())
		{
			for (auto& cr : em.cores)
			{
				if (cr.name == options.core)
				{
					emulator = em.name;
					core = cr.name;
					forceCore = true;
					break;
				}
			}

			if (forceCore)
				break;
		}
	}	
	/*else if (!isExtensionCompatible())
	{
		auto extension = Utils::String::toLower(Utils::FileSystem::getExtension(gameToUpdate->getPath()));

		for (auto emul : system->getEmulators())
		{
			if (std::find(emul.incompatibleExtensions.cbegin(), emul.incompatibleExtensions.cend(), extension) == emul.incompatibleExtensions.cend())
			{
				for (auto coreZ : emul.cores)
				{
					if (std::find(coreZ.incompatibleExtensions.cbegin(), coreZ.incompatibleExtensions.cend(), extension) == coreZ.incompatibleExtensions.cend())
					{
						emulator = emul.name;
						core = coreZ.name;
						break;
					}
				}
			}
		}
	}*/
	
	std::string command = system->getLaunchCommand(emulator, core);

	if (forceCore)
	{
		if (command.find("%EMULATOR%") == std::string::npos && command.find("-emulator") == std::string::npos)
			command = command + " -emulator %EMULATOR%";

		if (command.find("%CORE%") == std::string::npos && command.find("-core") == std::string::npos)
			command = command + " -core %CORE%";
	}

	const std::string rom = Utils::FileSystem::getEscapedPath(getPath());
	const std::string basename = Utils::FileSystem::getStem(getPath());
	const std::string rom_raw = Utils::FileSystem::getPreferredPath(getPath());
	
	command = Utils::String::replace(command, "%SYSTEM%", systemName);
	command = Utils::String::replace(command, "%ROM%", rom);
	command = Utils::String::replace(command, "%BASENAME%", basename);
	command = Utils::String::replace(command, "%ROM_RAW%", rom_raw);
	command = Utils::String::replace(command, "%EMULATOR%", emulator);
	command = Utils::String::replace(command, "%CORE%", core);
	command = Utils::String::replace(command, "%HOME%", Paths::getHomePath());
	command = Utils::String::replace(command, "%GAMENAME%", formatCommandLineArgument(gameToUpdate->getName()));
	command = Utils::String::replace(command, "%SYSTEMNAME%", formatCommandLineArgument(system->getFullName()));

	// Export Game info XML is requested
#ifdef WIN32
	std::string fileInfo = Utils::FileSystem::combine(Utils::FileSystem::getTempPath(), "game.xml");
#else
	std::string fileInfo = "/tmp/game.xml";
#endif

	if (command.find("%GAMEINFOXML%") != std::string::npos && saveToXml(gameToUpdate, fileInfo, true))
		command = Utils::String::replace(command, "%GAMEINFOXML%", Utils::FileSystem::getEscapedPath(fileInfo));
	else
	{
		command = Utils::String::replace(command, "%GAMEINFOXML%", "");
		Utils::FileSystem::removeFile(fileInfo);
	}
	
	if (includeControllers)
		command = Utils::String::replace(command, "%CONTROLLERSCONFIG%", controllersConfig);

	if (options.netPlayMode != DISABLED && (forceCore || gameToUpdate->isNetplaySupported()) && command.find("%NETPLAY%") == std::string::npos)
		command = command + " %NETPLAY%"; // Add command line parameter if the netplay option is defined at <core netplay="true"> level

	if (options.netPlayMode == CLIENT || options.netPlayMode == SPECTATOR)
	{
		std::string mode = (options.netPlayMode == SPECTATOR ? "spectator" : "client");
		std::string pass;

		if (!options.netplayClientPassword.empty())
			pass = " -netplaypass " + options.netplayClientPassword;

#if WIN32
		if (Utils::String::toLower(command).find("retroarch.exe") != std::string::npos)
			command = Utils::String::replace(command, "%NETPLAY%", "--connect " + options.ip + " --port " + std::to_string(options.port) + " --nick " + SystemConf::getInstance()->get("global.netplay.nickname"));
		else
#endif
			command = Utils::String::replace(command, "%NETPLAY%", "-netplaymode " + mode + " -netplayport " + std::to_string(options.port) + " -netplayip " + options.ip + pass);
	}
	else if (options.netPlayMode == SERVER)
	{
#if WIN32
		if (Utils::String::toLower(command).find("retroarch.exe") != std::string::npos)
			command = Utils::String::replace(command, "%NETPLAY%", "--host --port " + SystemConf::getInstance()->get("global.netplay.port") + " --nick " + SystemConf::getInstance()->get("global.netplay.nickname"));
		else
#endif
			command = Utils::String::replace(command, "%NETPLAY%", "-netplaymode host");
	}
	else
		command = Utils::String::replace(command, "%NETPLAY%", "");

	int monitorId = Settings::getInstance()->getInt("MonitorID");
	if (monitorId >= 0 && command.find(" -system ") != std::string::npos)
		command = command + " -monitor " + std::to_string(monitorId);

	if (SaveStateRepository::isEnabled(this))
		command = options.saveStateInfo.setupSaveState(this, command);

	return command;
}

std::string FileData::getMessageFromExitCode(int exitCode)
{
	switch (exitCode)
	{
	case 200:
		return _("THE EMULATOR EXITED UNEXPECTEDLY");
	case 201:
		return _("BAD COMMAND LINE ARGUMENTS");
	case 202:
		return _("INVALID CONFIGURATION");
	case 203:
		return _("UNKNOWN EMULATOR");
	case 204:
		return _("EMULATOR IS MISSING");
	case 205:
		return _("CORE IS MISSING");
	case 299:
		{
	#if WIN32
			std::string messageFile = Utils::FileSystem::combine(Utils::FileSystem::getTempPath(), "launch_error.log");
	#else
			std::string messageFile = "/tmp/launch_error.log";
	#endif
			if (Utils::FileSystem::exists(messageFile))
			{
				auto message = Utils::FileSystem::readAllText(messageFile);
				Utils::FileSystem::removeFile(messageFile);

				if (!message.empty())
					return message;
			}
		}
	}

	return _("UKNOWN ERROR") + " : " + std::to_string(exitCode);
}

bool FileData::launchGame(Window* window, LaunchGameOptions options)
{
	LOG_S(INFO) << "Attempting to launch game...";

	FileData* gameToUpdate = getSourceFileData();
	if (gameToUpdate == nullptr)
		return false;

	SystemData* system = gameToUpdate->getSystem();
	if (system == nullptr)
		return false;

	std::string command = getLaunchCommand(options);
	if (command.empty())
		return false;

	AudioManager::getInstance()->deinit();
	VolumeControl::getInstance()->deinit();

	bool hideWindow = Settings::getInstance()->getBool("HideWindow");
	window->deinit(hideWindow);
	
	const std::string rom = Utils::FileSystem::getEscapedPath(getPath());
	const std::string basename = Utils::FileSystem::getStem(getPath());

	Scripting::fireEvent("game-start", rom, basename, getName());

	time_t tstart = time(NULL);

	LOG_S(INFO) << "	" << command;

	auto p2kConv = convertP2kFile();

	mRunningGame = gameToUpdate;

	int exitCode = runSystemCommand(command, getDisplayName(), hideWindow ? NULL : window);
	if (exitCode != 0)
		LOG_S(WARNING) << "...launch terminated with nonzero exit code " << exitCode << "!";

	mRunningGame = nullptr;

	if (SaveStateRepository::isEnabled(this))
	{
		options.saveStateInfo.onGameEnded(this);
		getSourceFileData()->getSystem()->getSaveStateRepository()->refresh();
	}

	if (!p2kConv.empty()) // delete .keys file if it has been converted from p2k
		Utils::FileSystem::removeFile(p2kConv);

	Scripting::fireEvent("game-end");
	
	if (!hideWindow && Settings::getInstance()->getBool("HideWindowFullReinit"))
	{
		ResourceManager::getInstance()->reloadAll();
		window->deinit();
		window->init();
		window->setCustomSplashScreen(gameToUpdate->getImagePath(), gameToUpdate->getName());
	}
	else
		window->init(hideWindow);

	VolumeControl::getInstance()->init();
	AudioManager::getInstance()->init();

	window->normalizeNextUpdate();

	//update number of times the game has been launched
	if (exitCode == 0)
	{
		int timesPlayed = gameToUpdate->getMetadata().getInt(MetaDataId::PlayCount) + 1;
		gameToUpdate->setMetadata(MetaDataId::PlayCount, std::to_string(static_cast<long long>(timesPlayed)));

		// How long have you played that game? (more than 10 seconds, otherwise
		// you might have experienced a loading problem)
		time_t tend = time(NULL);
		long elapsedSeconds = difftime(tend, tstart);
		long gameTime = gameToUpdate->getMetadata().getInt(MetaDataId::GameTime) + elapsedSeconds;
		if (elapsedSeconds >= 10)
			gameToUpdate->setMetadata(MetaDataId::GameTime, std::to_string(static_cast<long>(gameTime)));

		//update last played time
		gameToUpdate->setMetadata(MetaDataId::LastPlayed, std::string (Utils::Time::DateTime(Utils::Time::now())));
		CollectionSystemManager::get()->refreshCollectionSystems(gameToUpdate);
		saveToGamelistRecovery(gameToUpdate);
	}

	window->reactivateGui();

	if (system != nullptr && system->getTheme() != nullptr)
		AudioManager::getInstance()->changePlaylist(system->getTheme(), true);
	else
		AudioManager::getInstance()->playRandomMusic();

	if (exitCode >= 200 && exitCode <= 300)
		window->pushGui(new GuiMsgBox(window, _("AN ERROR OCCURED") + ":\r\n" + getMessageFromExitCode(exitCode), _("OK"), nullptr, GuiMsgBoxIcon::ICON_ERROR));

	return exitCode == 0;
}


bool FileData::hasContentFiles()
{
	if (mPath.empty())
		return false;

	std::string ext = Utils::String::toLower(Utils::FileSystem::getExtension(mPath));
	if (ext == ".m3u" || ext == ".cue" || ext == ".ccd" || ext == ".gdi")
		return getSourceFileData()->getSystemEnvData()->isValidExtension(ext) && getSourceFileData()->getSystemEnvData()->mSearchExtensions.size() > 1;

	return false;
}

static std::vector<std::string> getTokens(const std::string& string)
{
	std::vector<std::string> tokens;

	bool inString = false;
	int startPos = 0;
	int i = 0;
	for (;;)
	{
		char c = string[i];

		switch (c)
		{
		case '\"':
			inString = !inString;
			if (inString)
				startPos = i + 1;

		case '\0':
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			if (!inString)
			{
				std::string value = string.substr(startPos, i - startPos);
				if (!value.empty())
					tokens.push_back(value);

				startPos = i + 1;
			}
			break;
		}

		if (c == '\0')
			break;

		i++;
	}

	return tokens;
}

std::set<std::string> FileData::getContentFiles()
{
	std::set<std::string> files;

	if (mPath.empty())
		return files;

	if (Utils::FileSystem::isDirectory(mPath))
	{
		for (auto file : Utils::FileSystem::getDirContent(mPath, true, true))
			files.insert(file);
	}
	else if (hasContentFiles())
	{
		auto path = Utils::FileSystem::getParent(mPath);
		auto ext = Utils::String::toLower(Utils::FileSystem::getExtension(mPath));

		if (ext == ".cue")
		{
			std::string start = "FILE";

			std::ifstream cue(WINSTRINGW(mPath));
			if (cue && cue.is_open())
			{
				std::string line;
				while (std::getline(cue, line))
				{
					if (!Utils::String::startsWith(line, start))
						continue;

					auto tokens = getTokens(line);
					if (tokens.size() > 1)
						files.insert(path + "/" + tokens[1]);
				}

				cue.close();
			}
		}
		else if (ext == ".ccd")
		{
			std::string stem = Utils::FileSystem::getStem(mPath);
			files.insert(path + "/" + stem + ".cue");
			files.insert(path + "/" + stem + ".img");
			files.insert(path + "/" + stem + ".bin");
			files.insert(path + "/" + stem + ".sub");
		}
		else if (ext == ".m3u")
		{
			std::ifstream m3u(WINSTRINGW(mPath));
			if (m3u && m3u.is_open())
			{
				std::string line;
				while (std::getline(m3u, line))
				{
					auto trim = Utils::String::trim(line);
					if (trim[0] == '#' || trim[0] == '\\' || trim[0] == '/')
						continue;

					files.insert(path + "/" + trim);
				}

				m3u.close();
			}
		}
		else if (ext == ".gdi")
		{
			std::ifstream gdi(WINSTRINGW(mPath));
			if (gdi && gdi.is_open())
			{
				std::string line;
				while (std::getline(gdi, line))
				{
					auto tokens = getTokens(line);
					if (tokens.size() > 5 && tokens[4].find(".") != std::string::npos)
						files.insert(path + "/" + tokens[4]);
				}

				gdi.close();
			}			
		}
	}

	return files;
}

void FileData::deleteGameFiles()
{
	for (auto mdd : mMetadata.getMDD())
	{
		if (mMetadata.getType(mdd.id) != MetaDataType::MD_PATH)
			continue;

		Utils::FileSystem::removeFile(mMetadata.get(mdd.id));
	}

	Utils::FileSystem::removeFile(getPath());

	for (auto contentFile : getContentFiles())
		Utils::FileSystem::removeFile(contentFile);
}

const std::string FileData::getCore(bool resolveDefault)
{
#if WIN32 && !_DEBUG
	std::string core = getMetadata(MetaDataId::Core);
#else
	std::string core = SystemConf::getInstance()->get(getConfigurationName() + ".core");	
#endif

	if (core == "auto")
		core = "";

	if (!core.empty())
	{
		// Check core exists 
		std::string emulator = getEmulator();
		if (emulator.empty())
			core = "";
		else
		{
			bool exists = false;

			for (auto emul : getSourceFileData()->getSystem()->getEmulators())
			{
				if (emul.name == emulator)
				{
					for (auto cr : emul.cores)
					{
						if (cr.name == core)
						{
							exists = true;
							break;
						}
					}

					if (exists)
						break;
				}
			}

			if (!exists)
				core = "";
		}
	}

	if (resolveDefault && core.empty())
		core = getSourceFileData()->getSystem()->getCore();

	return core;
}

const std::string FileData::getEmulator(bool resolveDefault)
{
#if WIN32 && !_DEBUG
	std::string emulator = getMetadata(MetaDataId::Emulator);
#else
	std::string emulator = SystemConf::getInstance()->get(getConfigurationName() + ".emulator");
#endif

	if (emulator == "auto")
		emulator = "";

	if (!emulator.empty())
	{
		// Check emulator exists 
		bool exists = false;

		for (auto emul : getSourceFileData()->getSystem()->getEmulators())
			if (emul.name == emulator) { exists = true; break; }

		if (!exists)
			emulator = "";
	}

	if (resolveDefault && emulator.empty())
		emulator = getSourceFileData()->getSystem()->getEmulator();

	return emulator;
}

void FileData::setCore(const std::string& value)
{
#if WIN32 && !_DEBUG
	setMetadata(MetaDataId::Core, value == "auto" ? "" : value);
#else
	SystemConf::getInstance()->set(getConfigurationName() + ".core", value);
#endif
}

void FileData::setEmulator(const std::string& value)
{
#if WIN32 && !_DEBUG
	setMetadata(MetaDataId::Emulator, value == "auto" ? "" : value);
#else
	SystemConf::getInstance()->set(getConfigurationName() + ".emulator", value);
#endif
}

bool FileData::isNetplaySupported()
{
	if (!SystemConf::getInstance()->getBool("global.netplay"))
		return false;

	auto file = getSourceFileData();
	if (file->getType() != GAME)
		return false;

	auto system = file->getSystem();
	if (system == nullptr)
		return false;
	
	std::string emulName = getEmulator();
	std::string coreName = getCore();

	if (!CustomFeatures::FeaturesLoaded)
	{
		std::string command = system->getLaunchCommand(emulName, coreName);
		if (command.find("%NETPLAY%") != std::string::npos)
			return true;
	}
	
	for (auto emul : system->getEmulators())
		if (emulName == emul.name)
			for (auto core : emul.cores)
				if (coreName == core.name)
					return core.netplay;
					
	return false;
}

void FileData::detectLanguageAndRegion(bool overWrite)
{
	if (!overWrite && (!getMetadata(MetaDataId::Language).empty() || !getMetadata(MetaDataId::Region).empty()))
		return;

	if (getSystem()->isCollection() || getType() == FOLDER)
		return;

	auto info = LangInfo::parse(getSourceFileData()->getPath(), getSourceFileData()->getSystem());
	if (info.languages.size() > 0)
		mMetadata.set(MetaDataId::Language, info.getLanguageString());
	if (!info.region.empty())
		mMetadata.set(MetaDataId::Region, info.region);
}

void FileData::checkCrc32(bool force)
{
	if (getSourceFileData() != this && getSourceFileData() != nullptr)
	{
		getSourceFileData()->checkCrc32(force);
		return;
	}

	if (!force && !getMetadata(MetaDataId::Crc32).empty())
		return;

	SystemData* system = getSystem();
	if (system == nullptr)
		return;

	auto crc = ApiSystem::getInstance()->getCRC32(getPath(), system->shouldExtractHashesFromArchives());
	if (!crc.empty())
	{
		getMetadata().set(MetaDataId::Crc32, Utils::String::toUpper(crc));
		saveToGamelistRecovery(this);
	}
}

void FileData::checkMd5(bool force)
{
	if (getSourceFileData() != this && getSourceFileData() != nullptr)
	{
		getSourceFileData()->checkMd5(force);
		return;
	}

	if (!force && !getMetadata(MetaDataId::Md5).empty())
		return;

	SystemData* system = getSystem();
	if (system == nullptr)
		return;

	auto crc = ApiSystem::getInstance()->getMD5(getPath(), system->shouldExtractHashesFromArchives());
	if (!crc.empty())
	{
		getMetadata().set(MetaDataId::Md5, Utils::String::toUpper(crc));
		saveToGamelistRecovery(this);
	}
}


void FileData::checkCheevosHash(bool force)
{
	if (getSourceFileData() != this)
	{
		getSourceFileData()->checkCheevosHash(force);
		return;
	}

	if (!force && !getMetadata(MetaDataId::CheevosHash).empty())
		return;

	SystemData* system = getSystem();
	if (system == nullptr)
		return;

	auto crc = RetroAchievements::getCheevosHash(system, getPath());
	getMetadata().set(MetaDataId::CheevosHash, Utils::String::toUpper(crc));
	saveToGamelistRecovery(this);
}

std::string FileData::getKeyboardMappingFilePath()
{
	if (Utils::FileSystem::isDirectory(getSourceFileData()->getPath()))
		return getSourceFileData()->getPath() + "/padto.keys";

	return getSourceFileData()->getPath() + ".keys";
}

bool FileData::hasP2kFile()
{
	std::string p2kPath = getSourceFileData()->getPath() + ".p2k.cfg";
	if (Utils::FileSystem::isDirectory(getSourceFileData()->getPath()))
		p2kPath = getSourceFileData()->getPath() + "/.p2k.cfg";

	return Utils::FileSystem::exists(p2kPath);
}

void FileData::importP2k(const std::string& p2k)
{
	if (p2k.empty())
		return;

	std::string p2kPath = getSourceFileData()->getPath() + ".p2k.cfg";
	if (Utils::FileSystem::isDirectory(getSourceFileData()->getPath()))
		p2kPath = getSourceFileData()->getPath() + "/.p2k.cfg";

	Utils::FileSystem::writeAllText(p2kPath, p2k);

	std::string keysPath = getKeyboardMappingFilePath();
	if (Utils::FileSystem::exists(keysPath))
		Utils::FileSystem::removeFile(keysPath);
}

std::string FileData::convertP2kFile()
{
	std::string p2kPath = getSourceFileData()->getPath() + ".p2k.cfg";
	if (Utils::FileSystem::isDirectory(getSourceFileData()->getPath()))
		p2kPath = getSourceFileData()->getPath() + "/.p2k.cfg";

	if (!Utils::FileSystem::exists(p2kPath))
		return "";

	std::string keysPath = getKeyboardMappingFilePath();
	if (Utils::FileSystem::exists(keysPath))
		return "";

	auto map = KeyMappingFile::fromP2k(p2kPath);
	if (map.isValid())
	{
		map.save(keysPath);
		return keysPath;
	}

	return "";
}

bool FileData::hasKeyboardMapping()
{
	if (!Utils::FileSystem::exists(getKeyboardMappingFilePath()))
		return hasP2kFile();

	return true;
}

KeyMappingFile FileData::getKeyboardMapping()
{
	KeyMappingFile ret;
	auto path = getKeyboardMappingFilePath();

	// If pk2.cfg file but no .keys file, then convert & load
	if (!Utils::FileSystem::exists(path) && hasP2kFile())
	{
		convertP2kFile();

		ret = KeyMappingFile::load(path);
		Utils::FileSystem::removeFile(path);
		return ret;
	}
		
	if (Utils::FileSystem::exists(path))
		ret = KeyMappingFile::load(path);
	else
		ret = getSystem()->getKeyboardMapping(); // if .keys file does not exist, take system config as predefined mapping

	ret.path = path;
	return ret;
}

bool FileData::isFeatureSupported(EmulatorFeatures::Features feature)
{
	auto system = getSourceFileData()->getSystem();
	return system->isFeatureSupported(getEmulator(), getCore(), feature);
}

bool FileData::isExtensionCompatible()
{
	auto game = getSourceFileData();
	auto extension = Utils::String::toLower(Utils::FileSystem::getExtension(game->getPath()));

	auto system = game->getSystem();
	auto emulName = game->getEmulator();
	auto coreName = game->getCore();

	for (auto emul : system->getEmulators())
	{
		if (emulName == emul.name)
		{
			if (std::find(emul.incompatibleExtensions.cbegin(), emul.incompatibleExtensions.cend(), extension) != emul.incompatibleExtensions.cend())
				return false;

			for (auto core : emul.cores)
			{
				if (coreName == core.name)
					return std::find(core.incompatibleExtensions.cbegin(), core.incompatibleExtensions.cend(), extension) == core.incompatibleExtensions.cend();
			}

			break;
		}
	}

	return true;
}

std::string FileData::getCurrentGameSetting(const std::string& settingName)
{
	FileData* src = getSourceFileData();

	std::string value = SystemConf::getInstance()->get(getConfigurationName() + "." + settingName);
	if (!value.empty() && value != "auto")
		return value;

	value = SystemConf::getInstance()->get(src->getSystem()->getName() + "." + settingName);
	if (!value.empty() && value != "auto")
		return value;

	return SystemConf::getInstance()->get("global." + settingName);
}

void FileData::setSelectedGame()
{
	TextToSpeech::getInstance()->say(getName(), false);

	Scripting::fireEvent("game-selected", getSourceFileData()->getSystem()->getName(), getPath(), getName());

	std::string desc = getMetadata(MetaDataId::Desc);
	if (!desc.empty())
		TextToSpeech::getInstance()->say(desc, true);	
}

std::string FileData::getProperty(const std::string& name)
{
	if (name == "name")
		return getName();

	if (name == "rom")
		return Utils::FileSystem::getFileName(getPath());

	if (name == "path")
		return getPath();
	
	if (name == "favorite")
		return getFavorite() ? _("YES") : _("NO");

	if (name == "hidden")
		return getHidden() ? _("YES") : _("NO");

	if (name == "kidGame")
		return getKidGame() ? _("YES") : _("NO");

	if (name == "gunGame")
		return isLightGunGame() ? _("YES") : _("NO");

	if (name == "system")
		return getSourceFileData()->getSystemName();

	if (name == "systemName")
		return getSourceFileData()->getSystem()->getFullName();

	if (name == "gameTime")
	{
		int seconds = atol(getMetadata(MetaDataId::GameTime).c_str());
		if (seconds == 0)
			return "";
		
		int h = 0, m = 0, s = 0;
		h = (seconds / 3600) % 24;
		m = (seconds / 60) % 60;
		s = seconds % 60;

		if (h > 0)
			return Utils::String::format("%02d:%02d:%02d", h, m, s);

		return Utils::String::format("%02d:%02d", m, s);		
	}

	return getMetadata().get(name);	
}