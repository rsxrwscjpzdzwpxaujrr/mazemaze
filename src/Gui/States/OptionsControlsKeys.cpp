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

#include "OptionsControls.hpp"

#include "../../utils.hpp"

#define DEFAULT_KEY_NAME(key) case Keyboard::Key::key: return #key

namespace mazemaze {
namespace gui {
namespace states {

sf::String
OptionsControls::key_name(sf::Keyboard::Key key) {
    using namespace sf;

    switch (key) {
        DEFAULT_KEY_NAME(A);
        DEFAULT_KEY_NAME(B);
        DEFAULT_KEY_NAME(C);
        DEFAULT_KEY_NAME(D);
        DEFAULT_KEY_NAME(E);
        DEFAULT_KEY_NAME(F);
        DEFAULT_KEY_NAME(G);
        DEFAULT_KEY_NAME(H);
        DEFAULT_KEY_NAME(I);
        DEFAULT_KEY_NAME(J);
        DEFAULT_KEY_NAME(K);
        DEFAULT_KEY_NAME(L);
        DEFAULT_KEY_NAME(M);
        DEFAULT_KEY_NAME(N);
        DEFAULT_KEY_NAME(O);
        DEFAULT_KEY_NAME(P);
        DEFAULT_KEY_NAME(Q);
        DEFAULT_KEY_NAME(R);
        DEFAULT_KEY_NAME(S);
        DEFAULT_KEY_NAME(T);
        DEFAULT_KEY_NAME(U);
        DEFAULT_KEY_NAME(V);
        DEFAULT_KEY_NAME(W);
        DEFAULT_KEY_NAME(X);
        DEFAULT_KEY_NAME(Y);
        DEFAULT_KEY_NAME(Z);

        DEFAULT_KEY_NAME(Escape);

        DEFAULT_KEY_NAME(F1 );
        DEFAULT_KEY_NAME(F2 );
        DEFAULT_KEY_NAME(F3 );
        DEFAULT_KEY_NAME(F4 );
        DEFAULT_KEY_NAME(F5 );
        DEFAULT_KEY_NAME(F6 );
        DEFAULT_KEY_NAME(F7 );
        DEFAULT_KEY_NAME(F8 );
        DEFAULT_KEY_NAME(F9 );
        DEFAULT_KEY_NAME(F10);
        DEFAULT_KEY_NAME(F11);
        DEFAULT_KEY_NAME(F12);
        DEFAULT_KEY_NAME(F13);
        DEFAULT_KEY_NAME(F14);
        DEFAULT_KEY_NAME(F15);

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

        case Keyboard::Key::LControl:
            return pgtxf("options|keys", "Left %s", "Ctrl");

        case Keyboard::Key::LShift:
            return pgtxf("options|keys", "Left %s", "Shift");

        case Keyboard::Key::LAlt:
            return pgtxf("options|keys", "Left %s", "Alt");

        case Keyboard::Key::LSystem:
            return pgtxf("options|keys", "Left %s", "System");

        case Keyboard::Key::RControl:
            return pgtxf("options|keys", "Right %s", "Ctrl");

        case Keyboard::Key::RShift:
            return pgtxf("options|keys", "Right %s", "Shift");

        case Keyboard::Key::RAlt:
            return pgtxf("options|keys", "Right %s", "Alt");

        case Keyboard::Key::RSystem:
            return pgtxf("options|keys", "Right %s", "System");

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

        default:
            return pgtx("options|keys", "Unknown key");
    }
}

}
}
}
