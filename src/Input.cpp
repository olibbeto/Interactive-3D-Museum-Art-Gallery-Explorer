#include "Input.h"
#include <ctype.h>

Input::Input() {
    for (int i = 0; i < 256; ++i) {
        keys[i] = false;
    }
}

void Input::setKeyDown(unsigned char key) {
    keys[tolower(key)] = true;
}

void Input::setKeyUp(unsigned char key) {
    keys[tolower(key)] = false;
}

bool Input::isKeyDown(unsigned char key) const {
    return keys[tolower(key)];
}
