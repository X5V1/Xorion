#include "NoRender.h"

NoRender::NoRender() : IModule(0, Category::VISUAL, "See through water and lava like glass.") {
}

NoRender::~NoRender() {}

const char* NoRender::getModuleName() {
	return "NoRender";
}
