#include "XorionFunctions.h"

JsValueRef CALLBACK XorionFunctions::getCommandManager(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return reinterpret_cast<ContextObjects*>(callbackState)->commandManager;
}

JsValueRef CALLBACK XorionFunctions::getModuleManager(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return reinterpret_cast<ContextObjects*>(callbackState)->moduleManager;
}

JsValueRef CALLBACK XorionFunctions::getDrawUtils(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return reinterpret_cast<ContextObjects*>(callbackState)->drawUtils;
}