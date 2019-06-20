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

#include "GraphicEngine.hpp"

namespace mazemaze {

Settings::Settings(bool readConfig) : configFile("config.cfg"),
                                      langenvchar(new char[12]) {
    if (readConfig)
        if (!Settings::readConfig()) {
            char* defaultLang = getenv("LANGUAGE");

            if (defaultLang == nullptr)
                defaultLang = const_cast<char*>("en");

            setLang(defaultLang);
            antialiasing = 0;
            autosave = true;
            autosaveTime = 30.0f;
        }
}

Settings::~Settings() {
    writeConfig();

    delete [] langenvchar;
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

    std::string langenvstring = "LANGUAGE=" + lang;

    langenvstring.copy(langenvchar, 12 * sizeof (char));
    langenvchar[11] = '\0';

    putenv(langenvchar);
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

void
Settings::writeConfig() {
    libconfig::Config config;

    try {
        config.readFile(configFile.c_str());
    } catch(const libconfig::FileIOException) {

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
