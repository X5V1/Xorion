#include "Killaura.h"

Killaura::Killaura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically.") {
	mode.addEntry(EnumEntry("Single", 0))
		.addEntry(EnumEntry("Multi", 1));
	registerEnumSetting("Mode", &mode, 0);
	rotationMode.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Silent", 1))
		.addEntry(EnumEntry("Strafe", 2))
		.addEntry(EnumEntry("Normal", 3));
	registerEnumSetting("Rotations", &rotationMode, 2);
	registerBoolSetting("MobAura", &isMobAura, isMobAura);
	registerFloatSetting("Range", &range, range, 2.f, 20.f);
	registerIntSetting("Delay", &delay, delay, 0, 20);
	registerBoolSetting("Hurttime", &hurttime, hurttime);
	registerBoolSetting("AutoWeapon", &autoweapon, autoweapon);
}

Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
	return "Killaura";
}

static std::vector<Entity*> targetList;
void findEntity(Entity* currentEntity, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr)
		return;
	
	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
		return;

	if (!Game.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!Game.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 66) // falling block
		return;
	  
	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (killauraMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->isPlayer())
			return;
		if (currentEntity->getAABBShapeComponent()->size.x <= 0.01f || currentEntity->getAABBShapeComponent()->size.y <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if(currentEntity->getEntityTypeId() == 64) // item
			return;
		if (currentEntity->getEntityTypeId() == 80)  // Arrows
			return;
		if (currentEntity->getEntityTypeId() == 51) // NPC
			return;
	} else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetList.push_back(currentEntity);
	}
}

void Killaura::findWeapon() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

void Killaura::onTick(GameMode* gm) {
	targetListEmpty = targetList.empty();
	//Loop through all our players and retrieve their information
	targetList.clear();

	Game.forEachEntity(findEntity);

	Odelay++;
	if (!targetList.empty() && Odelay >= delay) {

		if (autoweapon) findWeapon();

		if (Game.getLocalPlayer()->entityLocation->velocity.squaredxzlen() < 0.01) {
			MovePlayerPacket p(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
		}

		// Attack all entitys in targetList
		if (mode.selected == 1) {
			for (auto& i : targetList) {
				if (!(i->damageTime > 1 && hurttime)) {
					Game.getLocalPlayer()->swing();
					Game.getGameMode()->attack(i);
				}
			}
		} else {
			if (!(targetList[0]->damageTime > 1 && hurttime)) {
				Game.getLocalPlayer()->swing();
				Game.getGameMode()->attack(targetList[0]);
			}
		}
		Odelay = 0;
	}
}

void Killaura::onEnable() {
	if (Game.getLocalPlayer() == nullptr)
		setEnabled(false);
}

void Killaura::onSendPacket(Packet* packet) {
	if (Game.getLocalPlayer() != nullptr && rotationMode.selected == 1 && !targetList.empty()) {
		if (targetList[0] == nullptr)
			return;

		if (packet->isInstanceOf<MovePlayerPacket>()) {
			auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
			Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(targetList[0]->getMovementProxy()->getAttachPos(ActorLocation::Eyes, 1.f));
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}

void Killaura::onPlayerTick(Player* player) {
	if (Game.getLocalPlayer() != nullptr && !targetList.empty() && rotationMode.selected > 1) {
		if (targetList[0] == nullptr)
			return;

		Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(targetList[0]->getMovementProxy()->getAttachPos(ActorLocation::Eyes, 1.f));

		if (rotationMode.selected == 3)
			player->getActorRotationComponent()->rot.x = angle.x;
		else if (rotationMode.selected == 2)
			player->getActorRotationComponent()->rot = angle;

		player->getActorHeadRotationComponent()->rot.y = angle.y;
		player->getMobBodyRotationComponent()->bodyRot = angle.y;
		player->getMovementProxy()->setYHeadRot(angle.y);
	}
}