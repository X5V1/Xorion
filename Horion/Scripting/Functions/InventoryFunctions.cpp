#include "InventoryFunctions.h"

const wchar_t* airName = L"air";

// TODO: createItem uses removed isValid() method - now a field
/*
JsValueRef createItem(C_ItemStack *stack) {
	JsValueRef obj;
	chakra.JsCreateObject_(&obj);

	if (stack == nullptr || !stack->isValid()) {
		JsValueRef itemName;
		chakra.JsPointerToString_(airName, wcslen(airName), &itemName);

		chakra.addPropertyToObj(obj, L"name", itemName);
		chakra.addPropertyToObj(obj, L"id", chakra.toNumber(0));
		chakra.addPropertyToObj(obj, L"amount", chakra.toNumber(0));
	} else {
		auto name = stack->getItem()->name.getText();
		JsValueRef itemName;

		std::wstring charName = Utils::stringToWstring(name);

		chakra.JsPointerToString_(charName.c_str(), charName.size(), &itemName);

		chakra.addPropertyToObj(obj, L"name", itemName);
		chakra.addPropertyToObj(obj, L"id", chakra.toNumber(stack->getItem()->itemId));
		chakra.addPropertyToObj(obj, L"amount", chakra.toNumber(stack->count));
	}

	return obj;
}
*/

JsValueRef CALLBACK InventoryFunctions::getItems(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);

	JsValueRef inventoryArr;
	chakra.JsCreateArray_(36, &inventoryArr);

	// Stub: Return empty array since getItemStack API doesn't exist
	return inventoryArr;
}

JsValueRef CALLBACK InventoryFunctions::getArmor(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);

	// TODO: createItem function doesn't exist (commented above due to ItemStack API changes)
	/*
	JsValueRef armorArr;
	chakra.JsCreateArray_(4, &armorArr);

	for (int i = 0; i < 4; i++) {
		auto stack = plr->getArmor(i);
		chakra.arraySet(armorArr, i, createItem(stack));
	};

	return armorArr;
	*/
	return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK InventoryFunctions::getHeld(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);

	int slot;

	JsValueType type;
	chakra.JsGetValueType_(arguments[1], &type);
	if (type != JsNumber) {
		THROW(L"Argument 1 not a valid integer");
	}
	if (chakra.JsNumberToInt_(arguments[1], &slot) != JsNoError) {
		THROW(L"Argument 1 not a valid integer");
	}

	// Stub: Return empty object since getItemStack API doesn't exist
	JsValueRef obj;
	chakra.JsCreateObject_(&obj);
	return obj;
}

JsValueRef CALLBACK InventoryFunctions::setSelected(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);

	int slot;

	JsValueType type;
	chakra.JsGetValueType_(arguments[1], &type);
	if (type != JsNumber) {
		THROW(L"Argument 1 not a valid integer");
	}
	if (chakra.JsNumberToInt_(arguments[1], &slot) != JsNoError) {
		THROW(L"Argument 1 not a valid integer");
	}

	slot = std::clamp(slot, 0, 8);

	plr->getSupplies()->selectedHotbarSlot = slot;

	return chakra.trueValue();
}

JsValueRef CALLBACK InventoryFunctions::getSlot(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);
	const int currSlot = plr->getSupplies()->selectedHotbarSlot;

	// Stub: Return empty object since getItemStack API doesn't exist
	JsValueRef obj;
	chakra.JsCreateObject_(&obj);
	return obj;
}

JsValueRef CALLBACK InventoryFunctions::isFull(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);

	// Stub: Always return false since isFull API doesn't exist
	return chakra.toBoolean(false);
}

JsValueRef CALLBACK InventoryFunctions::moveItem(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<LocalPlayer*>(ent);

	int slot[2];

	for (int i = 0; i < 2; i++) {
		JsValueType type;
		chakra.JsGetValueType_(arguments[i + 1], &type);
		if (type != JsNumber) {
			THROW(L"Argument 1 or 2 not valid integers");
		}
		if (chakra.JsNumberToInt_(arguments[i + 1], &slot[i]) != JsNoError) {
			THROW(L"Argument 1 or 2 not valid integers");
		}
	}

	if (slot[0] < 0) slot[0] = 0;
	if (slot[1] < 0) slot[1] = 0;

	// TODO: swapSlots doesn't exist in new API
	// plr->getSupplies()->inventory->swapSlots(slot[0], slot[1]);

	return chakra.trueValue();
}