#include "NoBreak.h"

NoBreak::NoBreak() : IModule(0, Category::WORLD, "Makes anything that can break unbreakable.") {
}

NoBreak::~NoBreak() {
}

const char* NoBreak::getModuleName() {
	return "NoBreak";
}
