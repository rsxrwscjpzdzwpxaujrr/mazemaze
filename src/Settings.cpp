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
        main_menu(nullptr),
        supported_langs{
            Language(L"English",    "en_US"),
            Language(L"Русский",    "ru_RU"),
            Language(L"Українська", "uk_UA"),
            Language(L"Deutsch",    "de_DE")
        },
        renderer(0) {
    init_data_dir();
    config_file = data_dir + PATH_SEPARATOR "config.json";

    if (readConfig) {
        if (Settings::read_config())
            return;
    }

    Logger::inst().log_debug("Config not read. Using defaults.");

#ifdef _WIN32

    setEnvironment();
    resetLocales();

#else

    std::string temp_lang = reset_locales();
    int enc_found = temp_lang.find('.');

    if (enc_found != std::string::npos)
        set_lang(temp_lang.substr(0, enc_found));
    else
        set_lang(temp_lang);

#endif

    Logger::inst().log_debug("Setting default settings.");

    antialiasing = 0;
    autosave = true;
    autosave_time = 30.0f;
    sensitivity = 0.001f;
    renderer = 3;
    show_fps = false;
    set_vsync(true);
    camera_bobbing = true;

    controls["up"]    = sf::Keyboard::W;
    controls["down"]  = sf::Keyboard::S;
    controls["right"] = sf::Keyboard::D;
    controls["left"]  = sf::Keyboard::A;
}

Settings::~Settings() {
    write_config();
}

std::string
Settings::get_lang() const {
    return lang;
}

unsigned int
Settings::get_antialiasing() const {
    return antialiasing;
}

unsigned int
Settings::get_max_antialiasing() const {
    return GraphicEngine::inst().get_max_antialiasing();
}

bool
Settings::get_fullscreen() const {
    return GraphicEngine::inst().get_fullscreen();
}

bool
Settings::get_vsync() const {
    return GraphicEngine::inst().get_vsync();
}

bool
Settings::get_autosave() const {
    return autosave;
}

float
Settings::get_autosave_time() const {
    return autosave_time;
}

int
Settings::get_renderer() const {
    return renderer;
}

bool
Settings::get_show_fps() const {
    return show_fps;
}

sf::Keyboard::Key
Settings::get_key(const std::string& control) {
    return controls[control];
}

const std::vector<Settings::Language>&
Settings::get_supported_langs() const {
    return supported_langs;
}

float
Settings::get_sensitivity() const {
    return sensitivity;
}

std::string
Settings::get_data_dir() const {
    return data_dir;
}

bool
Settings::get_camera_bobbing() const {
    return camera_bobbing;
}

void
Settings::set_main_menu(gui::MainMenu* main_menu) {
    Settings::main_menu = main_menu;

    main_menu->show_fps(show_fps);
}

void
Settings::set_lang(const std::string &lang) {
    Logger::inst().log_debug(fmt("Setting language to %s.", lang.c_str()));

    bool lang_exist = false;
    std::string using_lang;

    for (Language& language : supported_langs) {
        if (language.code == lang) {
            using_lang = lang;
            lang_exist = true;
            break;
        }
    }

    if (!lang_exist) {
        for (Language& language : supported_langs) {
            if (language.code.substr(0, 2) == lang.substr(0, 2)) {
                using_lang = language.code;
                lang_exist = true;
                break;
            }
        }
    }

    if (!lang_exist) {
        using_lang = FALLBACK_LANG;
    }

    if (using_lang != lang) {
        Logger::inst().log_warn(fmt("Can not use language %s, using %s instead.",
                                    lang.c_str(),
                                    using_lang.c_str()));
    }

    setenv("LANGUAGE", using_lang.c_str(), true);

    reset_locales();

    if (main_menu)
        main_menu->reset_text();

    Settings::lang = using_lang;
}

void
Settings::set_antialiasing(unsigned int antialiasing) {
    Logger::inst().log_debug(fmt("Setting antialiasing to %d.", antialiasing));

    GraphicEngine::inst().set_antialiasing(antialiasing);

    Settings::antialiasing = antialiasing;
}

void
Settings::set_fullscreen(bool fullscreen) {
    Logger::inst().log_debug(fmt("Setting fullsreen to %s.", fullscreen ? "true" : "false"));

    GraphicEngine::inst().set_fullscreen(fullscreen);
}

void
Settings::set_vsync(bool vsync) {
    Logger::inst().log_debug(fmt("Setting V-Sync to %s.", vsync ? "true" : "false"));

    GraphicEngine::inst().set_vsync(vsync);
}

void
Settings::set_autosave(bool autosave) {
    Logger::inst().log_debug(fmt("Setting autosave to %s.", autosave ? "true" : "false"));

    Settings::autosave = autosave;
}

void
Settings::set_autosave_time(float autosave_time) {
    Logger::inst().log_debug(fmt("Setting autosave time to %f.", autosave_time));

    Settings::autosave_time = autosave_time;
}

void
Settings::set_renderer(int id) {
    Logger::inst().log_debug(fmt("Setting renderer to %d.", id));

    renderer = id;
}

void
Settings::set_show_fps(bool show_fps) {
    Logger::inst().log_debug(fmt("Setting showFps to %s.", show_fps ? "true" : "false"));

    Settings::show_fps = show_fps;

    if (main_menu)
        main_menu->show_fps(show_fps);
}

void
Settings::set_key(const std::string& control, sf::Keyboard::Key key) {
    Logger::inst().log_debug("Changing controls.");

    controls[control] = key;
}

void
Settings::set_sensitivity(float sensitivity) {
    Logger::inst().log_debug(fmt("Setting sensitivity to %f.", sensitivity));

    Settings::sensitivity = sensitivity;
}

void
Settings::set_camera_bobbing(float camera_bobbing) {
    Settings::camera_bobbing = camera_bobbing;
}

std::string
Settings::reset_locales() {
    char* result_ptr = setlocale(LC_ALL, "");

    std::string result;

    if (result_ptr != nullptr)
        result = result_ptr;
    else
        result = "";

    setlocale(LC_NUMERIC, "C");

    return result;
}

#ifdef _WIN32

void
Settings::setEnvironment() {
    char lang_name[4];
    char country_name[4];

    GetLocaleInfo(LOCALE_USER_DEFAULT,
                  LOCALE_SISO639LANGNAME,
                  lang_name,
                  sizeof(lang_name) / sizeof(char));

    GetLocaleInfo(LOCALE_USER_DEFAULT,
                  LOCALE_SISO3166CTRYNAME,
                  country_name,
                  sizeof(country_name) / sizeof(char));

    setLang(fmt("%s_%s", lang_name, country_name));
}

#endif

void
Settings::init_data_dir() {
#ifdef _WIN32
    char local_app_data[256];

    ExpandEnvironmentStringsA("%LOCALAPPDATA%", local_app_data, 254);

    data_dir = local_app_data;

#else

    const char* xdg_data_home = getenv("XDG_DATA_HOME");

    if (xdg_data_home == nullptr)
        xdg_data_home = "$HOME/.local/share";

    wordexp_t p;
    wordexp(xdg_data_home, &p, 0);

    const char* xdg_data_home_exp = p.we_wordv[0];

    data_dir = xdg_data_home_exp;

#endif

    data_dir += PATH_SEPARATOR "mazemaze";

#ifdef _WIN32

    SHCreateDirectoryExA(nullptr, dataDir.c_str(), nullptr);

#else

    mkdirp(data_dir.c_str());
    wordfree(&p);

#endif
}

void
Settings::write_config() {
    Logger::inst().log_debug("Writing config.");

    std::ofstream ofs(config_file);

    Json::StyledStreamWriter writer;
    Json::Value config;

    config["lang"] = get_lang();
    config["autosave"] = get_autosave();
    config["autosaveTime"] = get_autosave_time();
    config["showFps"] = get_show_fps();

    Json::Value graphics = Json::objectValue;

    graphics["antialiasing"] = get_antialiasing();
    graphics["fullscreen"] = get_fullscreen();
    graphics["vsync"] = get_vsync();
    graphics["style"] = get_renderer();
    graphics["cameraBobbing"] = get_camera_bobbing();

    config["graphics"] = graphics;

    Json::Value controls = Json::objectValue;

    for (auto it = Settings::controls.begin(); it != Settings::controls.end(); it++) {
        controls[it->first] = it->second;
    }

    controls["sensitivity"] = get_sensitivity();

    config["controls"] = controls;

    writer.write(ofs, config);
}

bool
Settings::read_config() {
    Logger::inst().log_debug("Reading config.");

    std::ifstream ifs(config_file);

    Json::Reader reader;
    Json::Value config;

    if (reader.parse(ifs, config)) {
        Json::Value controls = config["controls"];

        for (auto it = controls.begin(); it != controls.end(); it++) {
            auto control = it.key().asString();
            auto key     = static_cast<sf::Keyboard::Key>(it->asUInt());

            Settings::controls[control] = key;
        }

        set_sensitivity(controls["sensitivity"].asFloat());

        Json::Value graphics = config["graphics"];

        set_antialiasing(graphics["antialiasing"].asUInt());
        set_fullscreen(graphics["fullscreen"].asBool());
        set_vsync(graphics["vsync"].asBool());
        set_renderer(graphics["style"].asInt());
        set_camera_bobbing(graphics["cameraBobbing"].asBool());

        set_lang(config["lang"].asString());
        set_autosave(config["autosave"].asBool());
        set_autosave_time(config["autosaveTime"].asFloat());
        set_show_fps(config["showFps"].asBool());

        return reader.good();
    }

    return false;
}

}
