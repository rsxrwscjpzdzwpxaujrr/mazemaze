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

#include "OptionsGraphics.hpp"

#include "../MainMenu.hpp"

#include "../../utils.hpp"
#include "../../Settings.hpp"

#include <memory>

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

    cameraBobbingCheck->GetSignal(Widget::OnLeftClick).Connect([this] () {
        settings.setCameraBobbing(cameraBobbingCheck->IsActive());
    });
}

void
OptionsGraphics::initAntialiasingCombo() {
    int maxAntialiasing = settings.getMaxAntialiasing();

    antialiasingCombo->AppendItem("");

    for (int i = 2; i <= maxAntialiasing; i *= 2)
        antialiasingCombo->AppendItem(fmt("%dx", i));
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

    styleCombo->AppendItem("");
    styleCombo->AppendItem("");
    styleCombo->AppendItem("");
    styleCombo->AppendItem("");
    styleCombo->SelectItem(settings.getRenderer());

    cameraBobbingCheck->SetActive(settings.getCameraBobbing());
}

OptionsGraphics::OptionsGraphics(MainMenu& mainMenu, Settings& settings) :
        Options(mainMenu, settings, "OptionsGraphics"),
        fullscreenCheck   (CheckButton::Create(L"")),
        vsyncCheck        (CheckButton::Create(L"")),
        antialiasingCombo (ComboBox::Create()),
        styleCombo        (ComboBox::Create()),
        cameraBobbingCheck(CheckButton::Create(L"")),
        fullscreenOpt   (Option("", fullscreenCheck)),
        vsyncOpt        (Option("", vsyncCheck)),
        antialiasingOpt (Option("", antialiasingCombo)),
        styleOpt        (Option("", styleCombo)),
        cameraBobbingOpt(Option("", cameraBobbingCheck)) {
    windowBox->Pack(fullscreenOpt.toWidget());
    windowBox->Pack(antialiasingOpt.toWidget());
    windowBox->Pack(vsyncOpt.toWidget());
    windowBox->Pack(styleOpt.toWidget());
    windowBox->Pack(cameraBobbingOpt.toWidget());

    initSignals();
    initOptions();

    resetText();

    center();
}

void
OptionsGraphics::onResetText() {
    fullscreenOpt   .changeText(pgtx("options", "Fullscreen"));
    vsyncOpt        .changeText(pgtx("options", "V-Sync"));
    antialiasingOpt .changeText(pgtx("options", "Antialiasing"));
    styleOpt        .changeText(pgtx("options", "Style"));
    cameraBobbingOpt.changeText(pgtx("options", "Camera Bobbing"));

    styleCombo->ChangeItem(0, pgtx("options", "Classic"));
    styleCombo->ChangeItem(1, pgtx("options", "Gray"));
    styleCombo->ChangeItem(2, pgtx("options", "Brick"));
    styleCombo->ChangeItem(3, pgtx("options", "Night Brick"));

    styleCombo->RequestResize();

    antialiasingCombo->ChangeItem(0, pgtx("options", "No"));

    antialiasingCombo->RequestResize();
}

OptionsGraphics::~OptionsGraphics() = default;

}
}
}
