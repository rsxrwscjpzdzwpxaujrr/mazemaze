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
#include <libconfig.h++>

#ifdef WIN32
# include <windows.h>
#else
# include <regex>
#endif

#include "GraphicEngine.hpp"

namespace mazemaze {

Settings::Settings(bool readConfig) : configFile("config.cfg") {
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
Settings::getMaxAntialiasing() {
    return GraphicEngine::getInstance().getMaxAntialiasing();
}

bool
Settings::getFullscreen() {
    return GraphicEngine::getInstance().getFullscreen();
}

bool
Settings::getVsync() {
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

void
Settings::setLang(const std::string &lang) {
    Settings::lang = lang;

    langEnv = "LANGUAGE=" + lang;
    putenv(const_cast<char*>(langEnv.c_str()));
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

    defaultLang = tempLang;
    delete tempLang;

#else

    std::smatch match;
    std::string locale(std::locale("").name());
    std::regex  regex("([a-z]{2,3})(?:_)");

    std::regex_search(locale, match, regex);

    if (!match.empty())
        systemLang = match[1].str();
    else
        systemLang = "en";

#endif

    return systemLang;
}

void
Settings::writeConfig() {
    libconfig::Config config;
    config.setAutoConvert(true);

    try {
        config.readFile(configFile.c_str());
    } catch(const libconfig::FileIOException) {

    } catch(const libconfig::ParseException) {

    }

    libconfig::Setting& root = config.getRoot();

    if(!root.exists("lang"))
        root.add("lang", libconfig::Setting::TypeString);

    if(!root.exists("autosave"))
        root.add("autosave", libconfig::Setting::TypeBoolean);

    if(!root.exists("autosaveTime"))
        root.add("autosaveTime", libconfig::Setting::TypeFloat);

    root["lang"] = getLang();
    root["autosave"] = getAutosave();
    root["autosaveTime"] = getAutosaveTime();

    if(!root.exists("graphics"))
        root.add("graphics", libconfig::Setting::TypeGroup);

    libconfig::Setting& graphics = root["graphics"];

    if(!graphics.exists("antialiasing"))
        graphics.add("antialiasing", libconfig::Setting::TypeInt);

    if(!graphics.exists("fullscreen"))
        graphics.add("fullscreen", libconfig::Setting::TypeBoolean);

    if(!graphics.exists("vsync"))
        graphics.add("vsync", libconfig::Setting::TypeBoolean);

    graphics["antialiasing"] = static_cast<int>(getAntialiasing());
    graphics["fullscreen"] = getFullscreen();
    graphics["vsync"] = getVsync();

    config.writeFile(configFile.c_str());
}

bool
Settings::readConfig() {
    libconfig::Config config;
    config.setAutoConvert(true);

    try {
        config.readFile(configFile.c_str());
    } catch(const libconfig::FileIOException) {
        return false;
    } catch(const libconfig::ParseException) {
        return false;
    }

    const libconfig::Setting& root = config.getRoot();

    libconfig::Setting& graphics = root["graphics"];

    setAntialiasing(graphics["antialiasing"]);
    setFullscreen(graphics["fullscreen"]);
    setVsync(graphics["vsync"]);

    setLang(root["lang"]);
    setAutosave(root["autosave"]);
    setAutosaveTime(root["autosaveTime"]);

    return true;
}

}
