#include "EntityFunctions.h"
#include "../../DrawUtils.h"

// this fixes intellisense somehow
#ifndef ENTITY_INVALID
#define ENTITY_INVALID \
	THROW(L"Entity is invalid! Check if your entity is still valid with entity.isValid()")
#endif

JsValueRef CALLBACK EntityFunctions::isValid(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	bool isValid = EntityFunctions::getEntityFromValue(arguments[0]) != nullptr;
	JsValueRef isValidBoolean;
	chakra.JsBoolToBoolean_(isValid, &isValidBoolean);
	
	return isValidBoolean;
}

JsValueRef CALLBACK EntityFunctions::getPosition(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}
	
	return scriptMgr.prepareVector3(*ent->getPos(), reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK EntityFunctions::getInterpolatedPosition(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	vec3_t* start = ent->getPosOld();
	vec3_t* end = ent->getPos();

	auto t = DrawUtils::getLerpTime();
	vec3_t lerped = start->lerp(end, t);

	return scriptMgr.prepareVector3(lerped, reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK EntityFunctions::getVelocity(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	auto* entLoc = ent->entityLocation;
	if (entLoc != nullptr) {
		return scriptMgr.prepareVector3(entLoc->velocity, reinterpret_cast<ContextObjects*>(callbackState));
	}
	return scriptMgr.prepareVector3(Vec3(0, 0, 0), reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK EntityFunctions::isOnGround(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toBoolean(ent->isOnGround());
}

JsValueRef CALLBACK EntityFunctions::isInvisible(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toBoolean(ent->isInvisible());
}

JsValueRef CALLBACK EntityFunctions::isInWater(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toBoolean(ent->isInWater());
}

JsValueRef CALLBACK EntityFunctions::isInLava(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	// Stub: isInLava doesn't exist, return false
	return chakra.toBoolean(false);
}

JsValueRef CALLBACK EntityFunctions::isSneaking(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toBoolean(ent->isSneaking());
}

JsValueRef CALLBACK EntityFunctions::getSize(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	auto* aabbComp = ent->getAABBShapeComponent();
	if (aabbComp != nullptr) {
		float width = aabbComp->size.x;
		float height = aabbComp->size.y;
		Vec3 size = Vec3();
		size.x = width;
		size.y = height;
		size.z = width;
		return scriptMgr.prepareVector3(size, reinterpret_cast<ContextObjects*>(callbackState));
	}
	Vec3 defaultSize = Vec3();
	defaultSize.x = 0.6f;
	defaultSize.y = 1.8f;
	defaultSize.z = 0.6f;
	return scriptMgr.prepareVector3(defaultSize, reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK EntityFunctions::toString(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		const wchar_t* name = L"Entity(invalid)";
		JsValueRef ref;
		chakra.JsPointerToString_(name, wcslen(name), &ref);
		return ref;
	}

	const wchar_t* name = L"Entity(isValid=true)";
	JsValueRef ref;
	chakra.JsPointerToString_(name, wcslen(name), &ref);
	return ref;
}

JsValueRef CALLBACK EntityFunctions::getViewAngles(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	auto* rotComp = ent->getActorRotationComponent();
	if (rotComp != nullptr) {
		Vec3 angles = Vec3();
		angles.x = rotComp->rot.x;
		angles.y = rotComp->rot.y;
		angles.z = 0;
		return scriptMgr.prepareVector3(angles, reinterpret_cast<ContextObjects*>(callbackState));
	}
	return scriptMgr.prepareVector3(Vec3(), reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK EntityFunctions::getPitch(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	auto* rotComp = ent->getActorRotationComponent();
	if (rotComp != nullptr) {
		return chakra.toNumber(rotComp->rot.x);
	}
	return chakra.toNumber(0.0);
}

JsValueRef CALLBACK EntityFunctions::getYaw(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	auto* rotComp = ent->getActorRotationComponent();
	if (rotComp != nullptr) {
		return chakra.toNumber(rotComp->rot.y);
	}
	return chakra.toNumber(0.0);
}