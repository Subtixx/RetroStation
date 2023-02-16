#include "Scripting.h"
#include <loguru.hpp>
#include "platform.h"
#include "utils/FileSystemUtil.h"
#include "Paths.h"

namespace Scripting
{
	void fireEvent(const std::string& eventName, const std::string& arg1, const std::string& arg2, const std::string& arg3)
	{
		LOG_S(1) << "fireEvent: " << eventName << " " << arg1 << " " << arg2 << " " << arg3;

        std::list<std::string> scriptDirList;
        std::string test;

        // check in homepath
		test = Paths::getUserEmulationStationPath() + "/scripts/" + eventName;
        if(Utils::FileSystem::exists(test))
            scriptDirList.push_back(test);

		// check in getSharedConfigPath ( or exe path )
		test = Paths::getEmulationStationPath() + "/scripts/" + eventName;
		if (Utils::FileSystem::exists(test))
			scriptDirList.push_back(test);

        for(std::list<std::string>::const_iterator dirIt = scriptDirList.cbegin(); dirIt != scriptDirList.cend(); ++dirIt) {
            std::list<std::string> scripts = Utils::FileSystem::getDirContent(*dirIt);
            for (std::list<std::string>::const_iterator it = scripts.cbegin(); it != scripts.cend(); ++it) {
                std::string script = *it;

                for (auto arg : { arg1, arg2, arg3 })
                {
                    if (arg.empty())
                        break;

                    script += " \"" + arg + "\"";
                }
                LOG_S(1) << "  executing: " << script;
                runSystemCommand(script, "", nullptr);
            }
        }
	}

} // Scripting::
