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

#include "OptionsGraphics.hpp"

#include "../MainMenu.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"

using namespace sfg;

namespace mazemaze {
namespace gui {
namespace states {

void
OptionsGraphics::initSignals() {
    fullscreenCheck->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.setFullscreen(fullscreenCheck->IsActive());
    });

    antialiasingCombo->GetSignal(ComboBox::OnSelect).Connect([this] () {
        int item = antialiasingCombo->GetSelectedItem();
        unsigned int antialiasing = 1;

        for (int i = 0; i < item; i++)
            antialiasing *= 2;

        if (antialiasing == 1)
            antialiasing = 0;

        settings.setAntialiasing(antialiasing);
    });

    vsyncCheck->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.setVsync(vsyncCheck->IsActive());
    });

    styleCombo->GetSignal(ComboBox::OnSelect).Connect([this] () {
        settings.setRenderer(styleCombo->GetSelectedItem());
    });
}

void
OptionsGraphics::initAntialiasingCombo() {
    int maxAntialiasing = settings.getMaxAntialiasing();

    antialiasingCombo->AppendItem(pgtx("options", "No"));

    for (int i = 2; i <= maxAntialiasing; i *= 2)
        antialiasingCombo->AppendItem(format("%dx", i));
}

void
OptionsGraphics::initOptions() {
    fullscreenCheck->SetActive(settings.getFullscreen());

    initAntialiasingCombo();

    unsigned int antialiasing = settings.getAntialiasing();

    if (antialiasing == 0)
        antialiasing = 1;

    for (unsigned int i = 0, j = 1; j <= antialiasing; i++, j *= 2)
        if (j == antialiasing) {
            antialiasingCombo->SelectItem(i);
            break;
        }

    vsyncCheck->SetActive(settings.getVsync());

    styleCombo->AppendItem(pgtx("options", "Classic"));
    styleCombo->AppendItem(pgtx("options", "Gray"));
    styleCombo->AppendItem(pgtx("options", "Brick"));
    styleCombo->AppendItem(pgtx("options", "Night Brick"));
    styleCombo->SelectItem(settings.getRenderer());
}

OptionsGraphics::OptionsGraphics(MainMenu& mainMenu, Settings& settings) :
        Options(mainMenu, settings),
        fullscreenCheck  (CheckButton::Create(L"")),
        vsyncCheck       (CheckButton::Create(L"")),
        antialiasingCombo(ComboBox::Create()),
        styleCombo       (ComboBox::Create()) {
    windowBox->Pack(makeOption(pgtx("options", "Fullscreen"), fullscreenCheck));
    windowBox->Pack(makeOption(pgtx("options", "Antialiasing"), antialiasingCombo));
    windowBox->Pack(makeOption(pgtx("options", "V-Sync"), vsyncCheck));
    windowBox->Pack(makeOption(pgtx("options", "Style"), styleCombo));

    initSignals();
    initOptions();

    center();
}

OptionsGraphics::~OptionsGraphics() = default;

}
}
}
