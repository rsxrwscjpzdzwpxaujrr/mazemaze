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
#include <gettext.h>

#ifdef WIN32
# include <windows.h>
#else
# include <regex>
#endif

#include "GraphicEngine.hpp"
#include "Game.hpp"

namespace mazemaze {

Settings::Settings(bool readConfig) :
        configFile("config.cfg"),
        fallbackLang("en"),
        supportedLangsCount(4),
        supportedLangs(new Language[4] {Language(L"English",    "en"),
                                        Language(L"Русский",    "ru"),
                                        Language(L"Українська", "uk"),
                                        Language(L"Қазақша",    "kk")}),
        renderer(0) {
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

    controls["up"]    = sf::Keyboard::W;
    controls["down"]  = sf::Keyboard::S;
    controls["right"] = sf::Keyboard::D;
    controls["left"]  = sf::Keyboard::A;
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

int
Settings::getRenderer() const {
    return renderer;
}

bool
Settings::getShowFps() const {
    return showFps;
}

sf::Keyboard::Key
Settings::getKey(const std::string& control) {
    return controls[control];
}

const Settings::Language*
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
        if (supportedLangs[i].code == lang) {
            fallback = false;
            break;
        }

    if (fallback)
        setLang(fallbackLang);
    else {
        Settings::lang = lang;
        langEnv = "LANGUAGE=" + lang;

        putenv(const_cast<char*>(langEnv.c_str()));

        setlocale(LC_ALL, "");
        setlocale(LC_NUMERIC, "C");
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

void
Settings::setRenderer(int id) {
    renderer = id;
}

void
Settings::setShowFps(bool showFps) {
    Settings::showFps = showFps;
}

void
Settings::setKey(const std::string& control, sf::Keyboard::Key key) {
    controls[control] = key;
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
    addAndSet(root, Setting::TypeBoolean, "showFps",      getShowFps());

    if(!root.exists("graphics"))
        root.add("graphics", Setting::TypeGroup);

    Setting& graphics = root["graphics"];

    addAndSet(graphics, Setting::TypeInt,     "antialiasing", static_cast<int>(getAntialiasing()));
    addAndSet(graphics, Setting::TypeBoolean, "fullscreen",   getFullscreen());
    addAndSet(graphics, Setting::TypeBoolean, "vsync",        getVsync());
    addAndSet(graphics, Setting::TypeInt,     "style",        getRenderer());

    if(!root.exists("controls"))
        root.add("controls", Setting::TypeGroup);

    Setting& controls = root["controls"];

    for(auto it = Settings::controls.begin(); it != Settings::controls.end(); it++) {
        addAndSet(controls, Setting::TypeInt, it->first.c_str(), it->second);
    }

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

    const Setting& controls = root["controls"];

    for(auto it = controls.begin(); it != controls.end(); it++) {
        auto control = it->getName();
        auto key     = static_cast<sf::Keyboard::Key>(it->operator unsigned int());

        Settings::controls[control] = key;
    }

    const Setting& graphics = root["graphics"];

    setAntialiasing(graphics["antialiasing"]);
    setFullscreen(graphics["fullscreen"]);
    setVsync(graphics["vsync"]);
    setRenderer(graphics["style"]);

    setLang(root["lang"]);
    setAutosave(root["autosave"]);
    setAutosaveTime(root["autosaveTime"]);
    setShowFps(root["showFps"]);

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
