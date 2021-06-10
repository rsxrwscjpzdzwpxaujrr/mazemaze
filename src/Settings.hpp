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

#pragma once

#include <string>
#include <vector>
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
        const std::string  code;

        Language (const wchar_t* name, const char* code) :
                name(name),
                code(code) {}
    };

    explicit Settings(bool read_config=true);
    ~Settings();

    std::string                  lang() const;
    unsigned int                 antialiasing() const;
    unsigned int                 max_antialiasing() const;
    bool                         fullscreen() const;
    bool                         vsync() const;
    bool                         autosave() const;
    float                        autosave_time() const;
    int                          renderer() const;
    bool                         show_fps() const;
    sf::Keyboard::Key            key(const std::string& control);
    const std::vector<Language>& supported_langs() const;
    float                        sensitivity() const;
    std::string                  data_dir() const;
    bool                         camera_bobbing() const;

    void set_main_menu(gui::MainMenu* main_menu);

    void set_lang(const std::string &lang);
    void set_antialiasing(unsigned int antialiasing);
    void set_fullscreen(bool fullscreen);
    void set_vsync(bool vsync);
    void set_autosave(bool autosave);
    void set_autosave_time(float autosave_time);
    void set_renderer(int id);
    void set_show_fps(bool show_fps);
    void set_key(const std::string& control, sf::Keyboard::Key key);
    void set_sensitivity(float sensitivity);
    void set_camera_bobbing(float camera_bobbing);

private:
    std::string m_data_dir;
    std::string m_config_file;

    gui::MainMenu* m_main_menu;

    std::string m_lang;
    std::vector<Language> m_supported_langs;
    unsigned int m_antialiasing;
    bool  m_autosave;
    float m_autosave_time;
    int   m_renderer;
    bool  m_show_fps;
    float m_sensitivity;
    bool  m_camera_bobbing;

    std::map<std::string, sf::Keyboard::Key> controls;

    std::string reset_locales();

#ifdef _WIN32

    void set_environment();

#endif

    void init_data_dir();
    void write_config();
    bool read_config();
};

}
