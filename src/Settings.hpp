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

#pragma once

#include <string>
#include <libconfig.h++>

namespace mazemaze {

class Settings {
public:
    explicit Settings(bool readConfig=true);
    ~Settings();

    std::string  getLang() const;
    unsigned int getAntialiasing() const;
    unsigned int getMaxAntialiasing() const;
    bool         getFullscreen() const;
    bool         getVsync() const;
    bool         getAutosave() const;
    float        getAutosaveTime() const;

    const std::string* getSupportedLangs() const;
    int getSupportedLangsCount() const;

    void setLang(const std::string &lang);
    void setAntialiasing(unsigned int antialiasing);
    void setFullscreen(bool fullscreen);
    void setVsync(bool vsync);
    void setAutosave(bool autosave);
    void setAutosaveTime(float autosaveTime);

private:
    std::string configFile;

    std::string lang;
    std::string langEnv;
    const std::string fallbackLang;
    int supportedLangsCount;
    const std::string* const supportedLangs;
    unsigned int antialiasing;
    bool autosave;
    float autosaveTime;

    std::string getSystemLang();

    void writeConfig();
    bool readConfig();

    template <class T>
    libconfig::Setting& addAndSet(libconfig::Setting& parent,
                                  libconfig::Setting::Type type,
                                  const char* name,
                                  T value);
};

}
