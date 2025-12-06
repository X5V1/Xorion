#include "LocalPlayerFunctions.h"
#include "EntityFunctions.h"
#include "Vector3Functions.h"
#include "../../../SDK/GameMode.h"

#ifndef ENTITY_INVALID
#define ENTITY_INVALID                                                                                        \
	THROW(L"Entity is invalid! Check if your entity is still valid with entity.isValid()")

#endif

JsValueRef CALLBACK LocalPlayerFunctions::setPosition(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}
	auto vecOpt = Vector3Functions::getVec3FromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	ent->setPos(vecOpt.value());
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::setVelocity(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	auto vecOpt = Vector3Functions::getVec3FromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	// TODO: Entity::velocity removed
	// ent->velocity = vecOpt.value();
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::toString(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		const wchar_t* name = L"LocalPlayer(invalid)";
		JsValueRef ref;
		chakra.JsPointerToString_(name, wcslen(name), &ref);
		return ref;
	}
	
	const wchar_t* name = L"LocalPlayer(isValid=true)";
	JsValueRef ref;
	chakra.JsPointerToString_(name, wcslen(name), &ref);
	return ref;
}

JsValueRef CALLBACK LocalPlayerFunctions::setViewAngles(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	auto vecOpt = Vector3Functions::getVec3FromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	// TODO: Entity::viewAngles removed
	// vec2_t temp = vec2_t(vecOpt.value().x, vecOpt.value().y).sub(ent->viewAngles);
	// reinterpret_cast<LocalPlayer*>(ent)->applyTurnDelta(&temp);
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::setIsOnGround(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}
	auto isOnGroundOptional = chakra.tryGetBoolFromArgs(&arguments[1], argumentCount - 1);

	if (!isOnGroundOptional.has_value()) {
		THROW(L"Invalid boolean!");
	}

	// TODO: Entity::onGround removed
	// ent->onGround = isOnGroundOptional.value();
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::getInventory(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}
  
	return reinterpret_cast<ContextObjects*>(callbackState)->inventoryObject;

	// TODO: Rest of function uses removed C_LocalPlayer and getItemStack APIs
	/*
	const auto plr = reinterpret_cast<C_LocalPlayer*>(ent);

	JsValueRef inventoryArr;
	chakra.JsCreateArray_(35, &inventoryArr);

	const wchar_t* airName = L"air";

	for (int i = 0; i < 36; i++) {
		auto stack = plr->getSupplies()->inventory->getItemStack(i);

		if (stack == nullptr || !stack->isValid()) {
			JsValueRef jsItem;
			chakra.JsCreateObject_(&jsItem);

			JsValueRef itemName;
			chakra.JsPointerToString_(airName, wcslen(airName), &itemName);

			chakra.addPropertyToObj(jsItem, L"name", itemName);
			chakra.addPropertyToObj(jsItem, L"id", chakra.toNumber(0));
			chakra.addPropertyToObj(jsItem, L"amount", chakra.toNumber(0));

			chakra.arraySet(inventoryArr, i, jsItem);
		} else {
			JsValueRef jsItem;
			chakra.JsCreateObject_(&jsItem);

			auto name = stack->getItem()->name.getText();
			JsValueRef itemName;

			std::wstring charName = Utils::stringToWstring(name);

			chakra.JsPointerToString_(charName.c_str(), wcslen(charName.c_str()), &itemName);

			chakra.addPropertyToObj(jsItem, L"name", itemName);
			chakra.addPropertyToObj(jsItem, L"id", chakra.toNumber(stack->getItem()->itemId));
			chakra.addPropertyToObj(jsItem, L"amount", chakra.toNumber(stack->count));

			chakra.arraySet(inventoryArr, i, jsItem);
		}
	};

	return inventoryArr;
	*/
}

JsValueRef CALLBACK LocalPlayerFunctions::breakBlock(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);

	auto vecOpt = Vector3Functions::getVec3FromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	Vec3 pos = vecOpt.value();
	Vec3i blockPos(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));

	g_Data.getGameMode()->destroyBlock(blockPos, 1);
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::placeBlock(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);

	auto vecOpt = Vector3Functions::getVec3FromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	Vec3 pos = vecOpt.value();
	Vec3i blockPos(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));

	g_Data.getGameMode()->buildBlock(&blockPos, 1, true);
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::breakBlockRelativeToPlr(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);

	auto vecOpt = Vector3Functions::getVec3FromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	Vec3 pos = vecOpt.value().add(plr->getPos());
	Vec3i blockPos(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));

	g_Data.getGameMode()->destroyBlock(blockPos, 1);
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::placeBlockRelativeToPlr(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);

	auto vecOpt = Vector3Functions::getVec3FromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	Vec3 pos = vecOpt.value().add(plr->getPos());
	Vec3i blockPos(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));

	g_Data.getGameMode()->buildBlock(&blockPos, 1, true);
	return chakra.trueValue();
}