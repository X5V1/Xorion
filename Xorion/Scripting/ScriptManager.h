#pragma once
#include <memory>
#include "../../include/chakra/ChakraCore.h"
#include "../../Utils/ChakraHelper.h"
#include "../../SDK/Vec2.h"
#include "../../SDK/Vec3.h"

// Forward declarations
class IModule;
class JavascriptModule;

// Scripting error handling macros
#define THROW(msg) do { JsValueRef error; JsValueRef msgVal; JsPointerToString(msg, wcslen(msg), &msgVal); JsCreateError(msgVal, &error); JsSetException(error); } while(0)
#define THROW_IF_ERROR(err) do { if ((err) != JsNoError) { THROW(L"JavaScript error"); } } while(0)

// Macro for declaring JavaScript native function prototypes
#define DECL_FUN(name) static JsValueRef CALLBACK name(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)

// Forward declare classes defined elsewhere
class ScriptManager;
class ScriptInstance;

// External globals
extern ChakraApi chakra;
extern ScriptManager scriptMgr;

// DataType constants for external data type checking
constexpr int EntityDataType = 1;
constexpr int Vector3DataType = 2;
constexpr int Vector2DataType = 3;
constexpr int ModuleDataType = 4;

struct JVector2 {
    int dataType = Vector2DataType;
    vec2_t vec;
    JVector2(vec2_t v) : vec(v) {}
    void set(float x, float y) { vec.x = x; vec.y = y; }
};

struct JVector3 {
    int dataType = Vector3DataType;
    vec3_t vec;
    JVector3(vec3_t v) : vec(v) {}
    void set(float x, float y, float z) { vec.x = x; vec.y = y; vec.z = z; }
};

struct JModule {
    int dataType = ModuleDataType;
    std::shared_ptr<IModule> module;
    JModule(std::shared_ptr<IModule> m) : module(m) {}
};

namespace Script {
    enum Error {
        Invalid,
        Other
    };
}

// Forward declaration for ContextObjects (defined elsewhere)
struct ContextObjects {
	ScriptInstance* scriptInstance = nullptr;
	JsValueRef levelObject = JS_INVALID_REFERENCE;
	JsValueRef commandManager = JS_INVALID_REFERENCE;
	JsValueRef vec2Prototype = JS_INVALID_REFERENCE;
	JsValueRef vec3Prototype = JS_INVALID_REFERENCE;
	JsValueRef entityPrototype = JS_INVALID_REFERENCE;
	JsValueRef localPlayerPrototype = JS_INVALID_REFERENCE;
	JsValueRef drawUtils = JS_INVALID_REFERENCE;
	JsValueRef inventoryObject = JS_INVALID_REFERENCE;
	JsValueRef jsModulePrototype = JS_INVALID_REFERENCE;
	JsValueRef moduleManager = JS_INVALID_REFERENCE;
	JsValueRef modulePrototype = JS_INVALID_REFERENCE;
};

// ScriptManager class stub - methods defined in ScriptManager.cpp
class ScriptManager {
public:
    JsValueRef prepareEntity(long long runtimeId, ContextObjects* objs);
    void prepareGlobals(JsValueRef global, ContextObjects* obj);
    void prepareVector3Prototype(JsValueRef global, ContextObjects* obj);
    void prepareVector2Prototype(JsValueRef global, ContextObjects* obj);
    void prepareEntityPrototype(JsValueRef proto, ContextObjects* objs);
    void prepareLocalPlayerPrototype(JsValueRef proto, ContextObjects* objs);
    void prepareInventoryFunctions(JsValueRef proto, ContextObjects* objs);
    void prepareGameFunctions(JsValueRef global, ContextObjects* objs);
    void prepareXorionFunctions(JsValueRef global, ContextObjects* obj);
    void prepareDrawFunctions(JsValueRef global, ContextObjects* objs);
    void prepareCommandManagerFunctions(JsValueRef global, ContextObjects* objs);
    void prepareModuleManagerFunctions(JsValueRef global, ContextObjects* obj);
    void prepareModuleFunctions(JsValueRef proto, ContextObjects* obj);
    void prepareJsModuleFunctions(JsValueRef proto, ContextObjects* obj);
    void prepareContext(JsContextRef* ctx, ContextObjects* obj);
    
    JsValueRef prepareVector3(vec3_t vec, ContextObjects* objs);
    JsValueRef prepareVector2(vec2_t vec, ContextObjects* objs);
    JsValueRef prepareModule(std::shared_ptr<IModule> mod, ContextObjects* objs);
    JsValueRef prepareJsModule(std::shared_ptr<JavascriptModule> mod, ContextObjects* objs);
    void prepareLevelFunctions(JsValueRef global, ContextObjects* objs);
    JsValueRef getLocalPlayer(ContextObjects* obs);
    
    // Additional methods
    void runScript(const std::string& script) { /* stub */ }
    void importScriptFolder(const std::string& folder) { /* stub */ }
    void unloadAllScripts() { /* stub */ }
};
