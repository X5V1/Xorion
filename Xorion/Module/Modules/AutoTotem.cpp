#include "AutoTotem.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/GameMode.h"
#include "../../../SDK/ItemStack.h"
#include "../../../SDK\Inventory.h"



AutoTotem::AutoTotem() : IModule(0x0, Category::PLAYER)
{
}


AutoTotem::~AutoTotem()
{
}

const char* AutoTotem::getModuleName()
{
	return ("AutoTotem");
}

void AutoTotem::onTick(C_GameMode* gm) {
	
	if (g_Data.getLocalPlayer() != nullptr) 
	{
		ItemStack* i = g_Data.getLocalPlayer()->getEquippedTotem();

		if (i->item == NULL && delay > 3) 
		{
			PlayerSupplies* supplies = g_Data.getLocalPlayer()->getSupplies();
			PlayerInventory* a = supplies->inventory;
			for (int i = 0; i < 36; i++) {
				ItemStack* test = a->getByGlobalIndex(i);
				if (test->item != NULL) {
					Item* yikes = test->item;
					if (yikes->itemId == 450) {
						g_Data.getLocalPlayer()->consumeTotem();
						g_Data.getLocalPlayer()->setOffhandSlot(test);
					}

				}
			}
			delay = 0;
		}

		delay++;
	}
}


void AutoTotem::onEnable() {
}