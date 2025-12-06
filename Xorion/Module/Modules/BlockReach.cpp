#include "BlockReach.h"

BlockReach::BlockReach() : IModule(0, Category::PLAYER, "Exteneded reach for placing/breaking blocks.") {
	registerFloatSetting("Reach", &blockReach, blockReach, 7, 255);
}

BlockReach::~BlockReach() {
}

const char* BlockReach::getModuleName() {
	return "BlockReach";
}
