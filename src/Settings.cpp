/*
 * Copyright (c) 2019-2021, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#include <stdlib.h>
#include <fstream>
#include <gettext.h>
#include <json/json.h>

#ifdef _WIN32
# include <windows.h>
# include <processenv.h>
# include <shlobj.h>
#else
# include <regex>
# include <wordexp.h>
# include <errno.h>
# include <unistd.h>
# include <sys/stat.h>
#endif

#include "utils.hpp"
#include "path_separator.hpp"
#include "GraphicEngine.hpp"
#include "Game.hpp"
#include "Logger.hpp"

#include "Gui/MainMenu.hpp"

namespace mazemaze {

const char* FALLBACK_LANG = "en_US";

#ifdef _WIN32

int
setenv(const char *name, const char *value, int overwrite) {
    if (!overwrite)
        if (getenv(name) != nullptr)
            return 0;

    return _putenv_s(name, value);
}

#else

bool
mkdirp(const char* path, mode_t mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) {
    std::string s(path);
    std::string tmp;

    for (char c : s) {
        tmp += c;

        if (c == PATH_SEPARATOR[0] || tmp.size() == s.size()) {
            if (mkdir(tmp.c_str(), mode) == -1 && errno != EEXIST) {
                return false;
            }
        }
    }

    return true;
}

#endif

Settings::Settings(bool readConfig) :
        mainMenu(nullptr),
        supportedLangs{
            Language(L"English",    "en_US"),
            Language(L"Русский",    "ru_RU"),
            Language(L"Українська", "uk_UA"),
            Language(L"Deutsch",    "de_DE")
        },
        renderer(0) {
    initDataDir();
    configFile = dataDir + PATH_SEPARATOR "config.json";

    if (readConfig) {
        if (Settings::readConfig())
            return;
    }

    Logger::inst().log_debug("Config not read. Using defaults.");

#ifdef _WIN32

    setEnvironment();
    resetLocales();

#else

    std::string tempLang = resetLocales();
    int encFound = tempLang.find('.');

    if (encFound != std::string::npos)
        setLang(tempLang.substr(0, encFound));
    else
        setLang(tempLang);

#endif

    Logger::inst().log_debug("Setting default settings.");

    antialiasing = 0;
    autosave = true;
    autosaveTime = 30.0f;
    sensitivity = 0.001f;
    renderer = 3;
    showFps = false;
    setVsync(true);

    controls["up"]    = sf::Keyboard::W;
    controls["down"]  = sf::Keyboard::S;
    controls["right"] = sf::Keyboard::D;
    controls["left"]  = sf::Keyboard::A;
}

Settings::~Settings() {
    writeConfig();
}

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
    return GraphicEngine::inst().getMaxAntialiasing();
}

bool
Settings::getFullscreen() const {
    return GraphicEngine::inst().getFullscreen();
}

bool
Settings::getVsync() const {
    return GraphicEngine::inst().getVsync();
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

const std::vector<Settings::Language>&
Settings::getSupportedLangs() const {
    return supportedLangs;
}

float
Settings::getSensitivity() const {
    return sensitivity;
}

std::string
Settings::getDataDir() const {
    return dataDir;
}

void
Settings::setMainMenu(gui::MainMenu* mainMenu) {
    Settings::mainMenu = mainMenu;

    mainMenu->showFps(showFps);
}

void
Settings::setLang(const std::string &lang) {
    Logger::inst().log_debug(fmt("Setting language to %s.", lang.c_str()));

    bool langExist = false;
    std::string usingLang;

    for (Language& language : supportedLangs) {
        if (language.code == lang) {
            usingLang = lang;
            langExist = true;
            break;
        }
    }

    if (!langExist) {
        for (Language& language : supportedLangs) {
            if (language.code.substr(0, 2) == lang.substr(0, 2)) {
                usingLang = language.code;
                langExist = true;
                break;
            }
        }
    }

    if (!langExist) {
        usingLang = FALLBACK_LANG;
    }

    if (usingLang != lang) {
        Logger::inst().log_warn(fmt("Can not use language %s, using %s instead.",
                                    lang.c_str(),
                                    usingLang.c_str()));
    }

    setenv("LANGUAGE", usingLang.c_str(), true);

    resetLocales();

    if (mainMenu)
        mainMenu->resetText();

    Settings::lang = usingLang;
}

void
Settings::setAntialiasing(unsigned int antialiasing) {
    Logger::inst().log_debug(fmt("Setting antialiasing to %d.", antialiasing));

    GraphicEngine::inst().setAntialiasing(antialiasing);

    Settings::antialiasing = antialiasing;
}

void
Settings::setFullscreen(bool fullscreen) {
    Logger::inst().log_debug(fmt("Setting fullsreen to %s.", fullscreen ? "true" : "false"));

    GraphicEngine::inst().setFullscreen(fullscreen);
}

void
Settings::setVsync(bool vsync) {
    Logger::inst().log_debug(fmt("Setting V-Sync to %s.", vsync ? "true" : "false"));

    GraphicEngine::inst().setVsync(vsync);
}

void
Settings::setAutosave(bool autosave) {
    Logger::inst().log_debug(fmt("Setting autosave to %s.", autosave ? "true" : "false"));

    Settings::autosave = autosave;
}

void
Settings::setAutosaveTime(float autosaveTime) {
    Logger::inst().log_debug(fmt("Setting autosave time to %f.", autosaveTime));

    Settings::autosaveTime = autosaveTime;
}

void
Settings::setRenderer(int id) {
    Logger::inst().log_debug(fmt("Setting renderer to %d.", id));

    renderer = id;
}

void
Settings::setShowFps(bool showFps) {
    Logger::inst().log_debug(fmt("Setting showFps to %s.", showFps ? "true" : "false"));

    Settings::showFps = showFps;

    if (mainMenu)
        mainMenu->showFps(showFps);
}

void
Settings::setKey(const std::string& control, sf::Keyboard::Key key) {
    Logger::inst().log_debug("Changing controls.");

    controls[control] = key;
}

void
Settings::setSensitivity(float sensitivity) {
    Logger::inst().log_debug(fmt("Setting sensitivity to %f.", sensitivity));

    Settings::sensitivity = sensitivity;
}

std::string
Settings::resetLocales() {
    char* resultPtr = setlocale(LC_ALL, "");

    std::string result;

    if (resultPtr != nullptr)
        result = resultPtr;
    else
        result = "";

    setlocale(LC_NUMERIC, "C");

    return result;
}

#ifdef _WIN32

void
Settings::setEnvironment() {
    char langName[4];
    char countryName[4];

    GetLocaleInfo(LOCALE_USER_DEFAULT,
                  LOCALE_SISO639LANGNAME,
                  langName,
                  sizeof(langName) / sizeof(char));

    GetLocaleInfo(LOCALE_USER_DEFAULT,
                  LOCALE_SISO3166CTRYNAME,
                  countryName,
                  sizeof(countryName) / sizeof(char));

    setLang(fmt("%s_%s", langName, countryName));
}

#endif

void
Settings::initDataDir() {
#ifdef _WIN32
    char localAppData[256];

    ExpandEnvironmentStringsA("%LOCALAPPDATA%", localAppData, 254);

    dataDir = localAppData;

#else

    const char* xdg_data_home = getenv("XDG_DATA_HOME");

    if (xdg_data_home == nullptr)
        xdg_data_home = "$HOME/.local/share";

    wordexp_t p;
    wordexp(xdg_data_home, &p, 0);

    const char* xdg_data_home_exp = p.we_wordv[0];

    dataDir = xdg_data_home_exp;

#endif

    dataDir += PATH_SEPARATOR "mazemaze";

#ifdef _WIN32

    SHCreateDirectoryExA(nullptr, dataDir.c_str(), nullptr);

#else

    mkdirp(dataDir.c_str());
    wordfree(&p);

#endif
}

void
Settings::writeConfig() {
    Logger::inst().log_debug("Writing config.");

    std::ofstream ofs(configFile);

    Json::StyledStreamWriter writer;
    Json::Value config;

    config["lang"] = getLang();
    config["autosave"] = getAutosave();
    config["autosaveTime"] = getAutosaveTime();
    config["showFps"] = getShowFps();

    Json::Value graphics = Json::objectValue;

    graphics["antialiasing"] = getAntialiasing();
    graphics["fullscreen"] = getFullscreen();
    graphics["vsync"] = getVsync();
    graphics["style"] = getRenderer();

    config["graphics"] = graphics;

    Json::Value controls = Json::objectValue;

    for (auto it = Settings::controls.begin(); it != Settings::controls.end(); it++) {
        controls[it->first] = it->second;
    }

    controls["sensitivity"] = getSensitivity();

    config["controls"] = controls;

    writer.write(ofs, config);
}

bool
Settings::readConfig() {
    Logger::inst().log_debug("Reading config.");

    std::ifstream ifs(configFile);

    Json::Reader reader;
    Json::Value config;

    if (reader.parse(ifs, config)) {
        Json::Value controls = config["controls"];

        for (auto it = controls.begin(); it != controls.end(); it++) {
            auto control = it.key().asString();
            auto key     = static_cast<sf::Keyboard::Key>(it->asUInt());

            Settings::controls[control] = key;
        }

        setSensitivity(controls["sensitivity"].asFloat());

        Json::Value graphics = config["graphics"];

        setAntialiasing(graphics["antialiasing"].asUInt());
        setFullscreen(graphics["fullscreen"].asBool());
        setVsync(graphics["vsync"].asBool());
        setRenderer(graphics["style"].asInt());

        setLang(config["lang"].asString());
        setAutosave(config["autosave"].asBool());
        setAutosaveTime(config["autosaveTime"].asFloat());
        setShowFps(config["showFps"].asBool());

        return reader.good();
    }

    return false;
}

}
