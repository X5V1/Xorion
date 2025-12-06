#pragma once

#include "../ScriptManager.h"
#include <vector>
#include "../../../SDK/Entity.h"
#include "../../../SDK/BlockLegacy.h"
#include "../../../Memory/GameData.h"

class LevelFunctions {
public:
	DECL_FUN(isValid);
	DECL_FUN(getAllEntities);
	DECL_FUN(getAllTargetEntities);
	DECL_FUN(getBlock);
};
