#pragma once

#include "../../../Memory/GameData.h"
#include "../../../SDK/Entity.h"
#include "../ScriptManager.h"

// Define EntityDataType as macro to avoid redefinition issues
#ifndef ENTITY_DATA_TYPE
#define ENTITY_DATA_TYPE 1
#endif

// EntityInfo structure for script entity tracking
struct EntityInfo {
	int dataType = ENTITY_DATA_TYPE;
	bool isLocalPlayer = false;
	__int64 runtimeId = 0;
	
	EntityInfo(__int64 id) : runtimeId(id) {}
};

class EntityFunctions {
public:
	static Entity* getEntityFromValue(JsValueRef value) {
		JsValueType type;
		chakra.JsGetValueType_(value, &type);
		if (type != JsObject)
			return 0;

		bool hasExternalData = false;
		chakra.JsHasExternalData_(value, &hasExternalData);
		if (!hasExternalData)
			return 0;

		EntityInfo* entityInfo;
		chakra.JsGetExternalData_(value, reinterpret_cast<void**>(&entityInfo));
		if (entityInfo->dataType != EntityDataType)
			return 0;

	if (entityInfo->isLocalPlayer)
		return g_Data.getLocalPlayer();

	// TODO: Entity::entityRuntimeId doesn't exist
	return nullptr;
	/*
	Entity* foundEntity;
	auto func = [](Entity* ent, bool b, __int64 idCaptured, Entity** foundEntityCaptured) {
			if (ent->entityRuntimeId == idCaptured) {
				*foundEntityCaptured = ent;
			}
		};
	g_Data.forEachEntity(std::bind(func, std::placeholders::_1, std::placeholders::_2, entityInfo->runtimeId, &foundEntity));
	return foundEntity;
	*/
	}

	DECL_FUN(isValid);
	DECL_FUN(getPosition);
	DECL_FUN(getInterpolatedPosition);
	DECL_FUN(getVelocity);
	DECL_FUN(isOnGround);
	DECL_FUN(isInvisible);
	DECL_FUN(isInWater);
	DECL_FUN(isInLava);
	DECL_FUN(isSneaking);
	DECL_FUN(getSize);
	DECL_FUN(toString);
	DECL_FUN(getViewAngles);
	DECL_FUN(getPitch);
	DECL_FUN(getYaw);
};
