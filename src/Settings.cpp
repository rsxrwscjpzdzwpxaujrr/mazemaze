/*
 * Copyright (c) 2019, Мира Странная <miraityan2004@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Settings.hpp"

#include <cstdlib>

#ifdef WIN32
# include <windows.h>
#else
# include <regex>
#endif

#include "GraphicEngine.hpp"

namespace mazemaze {

Settings::Settings(bool readConfig) :
        configFile("config.cfg"),
        fallbackLang("en"),
        supportedLangsCount(4),
        supportedLangs(new std::string[supportedLangsCount]
                       {"en", "ru", "uk", "kk"}) {
    if (readConfig) {
        if(Settings::readConfig())
            return;
    }

    char* envLang = getenv("LANGUAGE");
    std::string defaultLang;

    if (envLang == nullptr)
        defaultLang = getSystemLang();
    else
        defaultLang = envLang;

    setLang(defaultLang);
    antialiasing = 0;
    autosave = true;
    autosaveTime = 30.0f;
}

Settings::~Settings() {
    writeConfig();
    delete [] supportedLangs;
};

std::string
Settings::getLang() const {
    return lang;
}

unsigned int
Settings::getAntialiasing() const {
    return antialiasing;
}

unsigned int
Settings::getMaxAntialiasing() const {
    return GraphicEngine::getInstance().getMaxAntialiasing();
}

bool
Settings::getFullscreen() const {
    return GraphicEngine::getInstance().getFullscreen();
}

bool
Settings::getVsync() const {
    return GraphicEngine::getInstance().getVsync();
}

bool
Settings::getAutosave() const {
    return autosave;
}

float
Settings::getAutosaveTime() const {
    return autosaveTime;
}

const std::string*
Settings::getSupportedLangs() const {
    return supportedLangs;
}

int
Settings::getSupportedLangsCount() const {
    return supportedLangsCount;
}

void
Settings::setLang(const std::string &lang) {
    bool fallback = true;

    for (int i = 0; i < supportedLangsCount; i++)
        if (supportedLangs[i] == lang) {
            fallback = false;
            break;
        }

    if (fallback)
        setLang(fallbackLang);
    else {
        Settings::lang = lang;

        langEnv = "LANGUAGE=" + lang;
        putenv(const_cast<char*>(langEnv.c_str()));
    }
}

void
Settings::setAntialiasing(unsigned int antialiasing) {
    GraphicEngine::getInstance().setAntialiasing(antialiasing);

    Settings::antialiasing = antialiasing;
}

void
Settings::setFullscreen(bool fullscreen) {
    GraphicEngine::getInstance().setFullscreen(fullscreen);
}

void
Settings::setVsync(bool vsync) {
    GraphicEngine::getInstance().setVsync(vsync);
}

void
Settings::setAutosave(bool autosave) {
    Settings::autosave = autosave;
}

void
Settings::setAutosaveTime(float autosaveTime) {
    Settings::autosaveTime = autosaveTime;
}


std::string
Settings::getSystemLang() {
    std::string systemLang;

#ifdef WIN32

    char* tempLang = new char[3];

    GetLocaleInfo(LOCALE_USER_DEFAULT,
                  LOCALE_SISO639LANGNAME,
                  tempLang,
                  sizeof(tempLang) / sizeof(char));

    systemLang = tempLang;
    delete [] tempLang;

#else

    std::smatch match;
    std::string locale(std::locale("").name());
    std::regex  regex("([a-z]{2,3})(?:_)");

    std::regex_search(locale, match, regex);

    if (!match.empty())
        systemLang = match[1].str();
    else
        systemLang = fallbackLang;

#endif

    return systemLang;
}

void
Settings::writeConfig() {
    using namespace libconfig;

    Config config;
    config.setAutoConvert(true);

    try {
        config.readFile(configFile.c_str());
    } catch(const FileIOException&) {

    } catch(const ParseException&) {

    }

    Setting& root = config.getRoot();

    addAndSet(root, Setting::TypeString,  "lang",         getLang());
    addAndSet(root, Setting::TypeBoolean, "autosave",     getAutosave());
    addAndSet(root, Setting::TypeFloat,   "autosaveTime", getAutosaveTime());

    if(!root.exists("graphics"))
        root.add("graphics", Setting::TypeGroup);

    Setting& graphics = root["graphics"];

    addAndSet(graphics, Setting::TypeInt,     "antialiasing", static_cast<int>(getAntialiasing()));
    addAndSet(graphics, Setting::TypeBoolean, "fullscreen",   getFullscreen());
    addAndSet(graphics, Setting::TypeBoolean, "vsync",        getVsync());

    config.writeFile(configFile.c_str());
}

bool
Settings::readConfig() {
    using namespace libconfig;

    Config config;
    config.setAutoConvert(true);

    try {
        config.readFile(configFile.c_str());
    } catch(const FileIOException&) {
        return false;
    } catch(const ParseException&) {
        return false;
    }

    const Setting& root = config.getRoot();

    Setting& graphics = root["graphics"];

    setAntialiasing(graphics["antialiasing"]);
    setFullscreen(graphics["fullscreen"]);
    setVsync(graphics["vsync"]);

    setLang(root["lang"]);
    setAutosave(root["autosave"]);
    setAutosaveTime(root["autosaveTime"]);

    return true;
}

template<class T>
libconfig::Setting&
Settings::addAndSet(libconfig::Setting& parent,
                    libconfig::Setting::Type type,
                    const char* name,
                    T value) {
    using namespace libconfig;

    if(!parent.exists(name))
        parent.add(name, type);

    Setting& setting = parent[name];
    setting = value;
    return setting;
}

}
