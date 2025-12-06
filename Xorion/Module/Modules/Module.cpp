#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/Json.hpp"
#include "../../../Utils/Logger.h"
#include "../../../Memory/GameData.h"

using json = nlohmann::json;

// Most IModule methods are inline in Module.h
// Only define methods that are declared but not defined inline
// callWhenDisabled is now inline in Module.h

