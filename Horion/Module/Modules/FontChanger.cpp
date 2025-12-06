#include "FontChanger.h"

FontChanger::FontChanger() : IModule(0, Category::CLIENT, "Font Changer.") {
	registerIntSetting("Font", &fontMode, fontMode, 0, 1);
}

FontChanger::~FontChanger() {
}

const char* FontChanger::getModuleName() {
	return "CustomFont";
}