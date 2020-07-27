/*
 * Copyright (c) 2019-2020, Мира Странная <rsxrwscjpzdzwpxaujrr@yahoo.com>
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

#pragma once

#include <string>
#include <map>

#include <SFML/Window/Keyboard.hpp>

namespace mazemaze {

class Game;

namespace gui {

class MainMenu;

}

class Settings {
public:
    struct Language {
        const std::wstring name;
        const std::string code;

        Language (const wchar_t* name, const char* code) :
                name(name),
                code(code) {}
    };

    explicit Settings(bool readConfig=true);
    ~Settings();

    std::string       getLang() const;
    unsigned int      getAntialiasing() const;
    unsigned int      getMaxAntialiasing() const;
    bool              getFullscreen() const;
    bool              getVsync() const;
    bool              getAutosave() const;
    float             getAutosaveTime() const;
    int               getRenderer() const;
    bool              getShowFps() const;
    sf::Keyboard::Key getKey(const std::string& control);
    const Language*   getSupportedLangs() const;
    int               getSupportedLangsCount() const;
    float             getSensitivity() const;
    std::string       getDataDir() const;

    void setMainMenu(gui::MainMenu* mainMenu);

    void setLang(const std::string &lang);
    void setAntialiasing(unsigned int antialiasing);
    void setFullscreen(bool fullscreen);
    void setVsync(bool vsync);
    void setAutosave(bool autosave);
    void setAutosaveTime(float autosaveTime);
    void setRenderer(int id);
    void setShowFps(bool showFps);
    void setKey(const std::string& control, sf::Keyboard::Key key);
    void setSensitivity(float sensitivity);

private:
    std::string dataDir;
    std::string configFile;

    gui::MainMenu* mainMenu;

    std::string lang;
    int supportedLangsCount;
    Language* supportedLangs;
    unsigned int antialiasing;
    bool autosave;
    float autosaveTime;
    int renderer;
    bool showFps;
    float sensitivity;

    std::map<std::string, sf::Keyboard::Key> controls;

    std::string resetLocales();

#ifdef _WIN32

    void setEnvironment();

#endif

    void initDataDir();
    void writeConfig();
    bool readConfig();
};

}
