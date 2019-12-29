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

#include "OptionsControls.hpp"

#include "../../utils.hpp"

namespace mazemaze {
namespace gui {
namespace states {

sf::String
OptionsControls::getKeyName(sf::Keyboard::Key key) {
    using namespace sf;

    switch (key) {
        case Keyboard::Key::A:
            return L"A";

        case Keyboard::Key::B:
            return L"B";

        case Keyboard::Key::C:
            return L"C";

        case Keyboard::Key::D:
            return L"D";

        case Keyboard::Key::E:
            return L"E";

        case Keyboard::Key::F:
            return L"F";

        case Keyboard::Key::G:
            return L"G";

        case Keyboard::Key::H:
            return L"H";

        case Keyboard::Key::I:
            return L"I";

        case Keyboard::Key::J:
            return L"J";

        case Keyboard::Key::K:
            return L"K";

        case Keyboard::Key::L:
            return L"L";

        case Keyboard::Key::M:
            return L"M";

        case Keyboard::Key::N:
            return L"N";

        case Keyboard::Key::O:
            return L"O";

        case Keyboard::Key::P:
            return L"P";

        case Keyboard::Key::Q:
            return L"Q";

        case Keyboard::Key::R:
            return L"R";

        case Keyboard::Key::S:
            return L"S";

        case Keyboard::Key::T:
            return L"T";

        case Keyboard::Key::U:
            return L"U";

        case Keyboard::Key::V:
            return L"V";

        case Keyboard::Key::W:
            return L"W";

        case Keyboard::Key::X:
            return L"X";

        case Keyboard::Key::Y:
            return L"Y";

        case Keyboard::Key::Z:
            return L"Z";

        case Keyboard::Key::Num0:
            return L"0";

        case Keyboard::Key::Num1:
            return L"1";

        case Keyboard::Key::Num2:
            return L"2";

        case Keyboard::Key::Num3:
            return L"3";

        case Keyboard::Key::Num4:
            return L"4";

        case Keyboard::Key::Num5:
            return L"5";

        case Keyboard::Key::Num6:
            return L"6";

        case Keyboard::Key::Num7:
            return L"7";

        case Keyboard::Key::Num8:
            return L"8";

        case Keyboard::Key::Num9:
            return L"9";

        case Keyboard::Key::Escape:
            return L"Escape";

        case Keyboard::Key::LControl:
            return pgtx("options|keys", "Left Ctrl");

        case Keyboard::Key::LShift:
            return pgtx("options|keys", "Left Shift");

        case Keyboard::Key::LAlt:
            return pgtx("options|keys", "Left Alt");

        case Keyboard::Key::LSystem:
            return pgtx("options|keys", "Left System");

        case Keyboard::Key::RControl:
            return pgtx("options|keys", "Right Ctrl");

        case Keyboard::Key::RShift:
            return pgtx("options|keys", "Right Shift");

        case Keyboard::Key::RAlt:
            return pgtx("options|keys", "Right Alt");

        case Keyboard::Key::RSystem:
            return pgtx("options|keys", "Right System");

        case Keyboard::Key::Menu:
            return L"Menu";

        case Keyboard::Key::LBracket:
            return L"[";

        case Keyboard::Key::RBracket:
            return L"]";

        case Keyboard::Key::Semicolon:
            return L";";

        case Keyboard::Key::Comma:
            return L",";

        case Keyboard::Key::Period:
            return L".";

        case Keyboard::Key::Quote:
            return L"'";

        case Keyboard::Key::Slash:
            return L"/";

        case Keyboard::Key::Backslash:
            return L"\\";

        case Keyboard::Key::Tilde:
            return L"~";

        case Keyboard::Key::Equal:
            return L"=";

        case Keyboard::Key::Hyphen:
            return L"-";

        case Keyboard::Key::Space:
            return pgtx("options|keys", "Space");

        case Keyboard::Key::Enter:
            return L"Enter";

        case Keyboard::Key::Backspace:
            return L"Backspace";

        case Keyboard::Key::Tab:
            return L"Tab";

        case Keyboard::Key::PageUp:
            return L"Pg Up";

        case Keyboard::Key::PageDown:
            return L"Pg Down";

        case Keyboard::Key::End:
            return L"End";

        case Keyboard::Key::Home:
            return L"Home";

        case Keyboard::Key::Insert:
            return L"Insert";

        case Keyboard::Key::Delete:
            return L"Delete";

        case Keyboard::Key::Add:
            return L"+";

        case Keyboard::Key::Subtract:
            return L"Num -";

        case Keyboard::Key::Multiply:
            return L"Num *";

        case Keyboard::Key::Divide:
            return L"Num /";

        case Keyboard::Key::Left:
            return pgtx("options|keys", "Arrow Left");

        case Keyboard::Key::Right:
            return pgtx("options|keys", "Arrow Right");

        case Keyboard::Key::Up:
            return pgtx("options|keys", "Arrow Up");

        case Keyboard::Key::Down:
            return pgtx("options|keys", "Arrow Down");

        case Keyboard::Key::Numpad0:
            return L"Num 0";

        case Keyboard::Key::Numpad1:
            return L"Num 1";

        case Keyboard::Key::Numpad2:
            return L"Num 2";

        case Keyboard::Key::Numpad3:
            return L"Num 3";

        case Keyboard::Key::Numpad4:
            return L"Num 4";

        case Keyboard::Key::Numpad5:
            return L"Num 5";

        case Keyboard::Key::Numpad6:
            return L"Num 6";

        case Keyboard::Key::Numpad7:
            return L"Num 7";

        case Keyboard::Key::Numpad8:
            return L"Num 8";

        case Keyboard::Key::Numpad9:
            return L"Num 9";

        case Keyboard::Key::F1:
            return L"F1";

        case Keyboard::Key::F2:
            return L"F2";

        case Keyboard::Key::F3:
            return L"F3";

        case Keyboard::Key::F4:
            return L"F4";

        case Keyboard::Key::F5:
            return L"F5";

        case Keyboard::Key::F6:
            return L"F6";

        case Keyboard::Key::F7:
            return L"F7";

        case Keyboard::Key::F8:
            return L"F8";

        case Keyboard::Key::F9:
            return L"F9";

        case Keyboard::Key::F10:
            return L"F10";

        case Keyboard::Key::F11:
            return L"F11";

        case Keyboard::Key::F12:
            return L"F12";

        case Keyboard::Key::F13:
            return L"F13";

        case Keyboard::Key::F14:
            return L"F14";

        case Keyboard::Key::F15:
            return L"F15";

        case Keyboard::Key::Pause:
            return L"Pause";

        default:
            return pgtx("options|keys", "Unknown key");
    }
}

}
}
}
