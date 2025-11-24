cmake_minimum_required(VERSION 3.12)
project(Horion)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wno-dynamic-class-memaccess -Wno-microsoft-goto -Wno-delete-incomplete /EHsc /Ob1)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/MP /Ob1)
endif()

add_compile_definitions(WIN32_LEAN_AND_MEAN)
add_compile_definitions(NOMINMAX)
if(CMAKE_BUILD_TYPE MATCHES Release)

elseif(CMAKE_BUILD_TYPE MATCHES BetaConf)
    add_compile_definitions(_BETA)
endif()

include_directories(.)
include_directories(include)
include_directories(include/glm/)

link_directories(minhook)

add_library(Horion SHARED
        Horion/Command/Commands/BindCommand.cpp
        Horion/Command/Commands/BindCommand.h
        Horion/Command/Commands/BruhCommand.cpp
        Horion/Command/Commands/BruhCommand.h
        Horion/Command/Commands/CommandBlockExploitCommand.cpp
        Horion/Command/Commands/CommandBlockExploitCommand.h
        Horion/Command/Commands/ConfigCommand.cpp
        Horion/Command/Commands/ConfigCommand.h
        Horion/Command/Commands/CoordsCommand.cpp
        Horion/Command/Commands/CoordsCommand.h
        Horion/Command/Commands/DamageCommand.cpp
        Horion/Command/Commands/DamageCommand.h
        Horion/Command/Commands/DeviceIDCommand.cpp
        Horion/Command/Commands/DeviceIDCommand.h
        Horion/Command/Commands/DupeCommand.cpp
        Horion/Command/Commands/DupeCommand.h
        Horion/Command/Commands/EjectCommand.cpp
        Horion/Command/Commands/EjectCommand.h
        Horion/Command/Commands/EnchantCommand.cpp
        Horion/Command/Commands/EnchantCommand.h
        Horion/Command/Commands/ExecuteCommand.cpp
        Horion/Command/Commands/ExecuteCommand.h
        Horion/Command/Commands/FriendListCommand.cpp
        Horion/Command/Commands/FriendListCommand.h
        Horion/Command/Commands/GameModeCommand.cpp
        Horion/Command/Commands/GameModeCommand.h
        Horion/Command/Commands/GiveCommand.cpp
        Horion/Command/Commands/GiveCommand.h
        Horion/Command/Commands/HelpCommand.cpp
        Horion/Command/Commands/HelpCommand.h
        Horion/Command/Commands/HideCommand.cpp
        Horion/Command/Commands/HideCommand.h
        Horion/Command/Commands/ICommand.cpp
        Horion/Command/Commands/ICommand.h
        Horion/Command/Commands/ModulesCommand.cpp
        Horion/Command/Commands/ModulesCommand.h
        Horion/Command/Commands/NameSpoofCommand.cpp
        Horion/Command/Commands/NameSpoofCommand.h
        Horion/Command/Commands/NbtCommand.cpp
        Horion/Command/Commands/NbtCommand.h
        Horion/Command/Commands/PanicCommand.cpp
        Horion/Command/Commands/PanicCommand.h
        Horion/Command/Commands/PathCommand.cpp
        Horion/Command/Commands/PathCommand.h
        Horion/Command/Commands/PlayerTeleportCommand.cpp
        Horion/Command/Commands/PlayerTeleportCommand.h
        Horion/Command/Commands/RelativeTeleportCommand.cpp
        Horion/Command/Commands/RelativeTeleportCommand.h
        Horion/Command/Commands/RenameCommand.cpp
        Horion/Command/Commands/RenameCommand.h
        Horion/Command/Commands/RepairCommand.cpp
        Horion/Command/Commands/RepairCommand.h
        Horion/Command/Commands/SayCommand.cpp
        Horion/Command/Commands/SayCommand.h
        Horion/Command/Commands/ScriptCommand.cpp
        Horion/Command/Commands/ScriptCommand.h
        Horion/Command/Commands/SeedCommand.cpp
        Horion/Command/Commands/SeedCommand.h
        Horion/Command/Commands/ServerCommand.cpp
        Horion/Command/Commands/ServerCommand.h
        Horion/Command/Commands/SetLoreCommand.cpp
        Horion/Command/Commands/SetLoreCommand.h
        Horion/Command/Commands/SetOffhandCommand.cpp
        Horion/Command/Commands/SetOffhandCommand.h
        Horion/Command/Commands/SetPrefixCommand.cpp
        Horion/Command/Commands/SetPrefixCommand.h
        Horion/Command/Commands/SpammerCommand.cpp
        Horion/Command/Commands/SpammerCommand.h
        Horion/Command/Commands/TeleportCommand.cpp
        Horion/Command/Commands/TeleportCommand.h
        Horion/Command/Commands/TestCommand.cpp
        Horion/Command/Commands/TestCommand.h
        Horion/Command/Commands/ToggleCommand.cpp
        Horion/Command/Commands/ToggleCommand.h
        Horion/Command/Commands/TopCommand.cpp
        Horion/Command/Commands/TopCommand.h
        Horion/Command/Commands/UnbindCommand.cpp
        Horion/Command/Commands/UnbindCommand.h
        Horion/Command/Commands/WaypointCommand.cpp
        Horion/Command/Commands/WaypointCommand.h
        Horion/Command/Commands/XpCommand.cpp
        Horion/Command/Commands/XpCommand.h
        Horion/Command/CommandMgr.cpp
        Horion/Command/CommandMgr.h
        Horion/Config/AccountInformation.cpp
        Horion/Config/AccountInformation.h
        Horion/Config/ConfigManager.cpp
        Horion/Config/ConfigManager.h
        Horion/FriendList/FriendList.cpp
        Horion/FriendList/FriendList.h
        Horion/Menu/ClickGui.cpp
        Horion/Menu/ClickGui.h
        Horion/Menu/TabGui.cpp
        Horion/Menu/TabGui.h
        Horion/Module/Modules/Aimbot.cpp
        Horion/Module/Modules/Aimbot.h
        Horion/Module/Modules/AirJump.cpp
        Horion/Module/Modules/AirJump.h
        Horion/Module/Modules/AirStuck.cpp
        Horion/Module/Modules/AirStuck.h
        Horion/Module/Modules/AirSwim.cpp
        Horion/Module/Modules/AirSwim.h
        Horion/Module/Modules/AntiBot.cpp
        Horion/Module/Modules/AntiBot.h
        Horion/Module/Modules/AntiCrystal.cpp
        Horion/Module/Modules/AntiCrystal.h
        Horion/Module/Modules/AntiEffect.cpp
        Horion/Module/Modules/AntiEffect.h
        Horion/Module/Modules/AntiVoid.cpp
        Horion/Module/Modules/AntiVoid.h
        Horion/Module/Modules/AutoArmor.cpp
        Horion/Module/Modules/AutoArmor.h
        Horion/Module/Modules/AutoClicker.cpp
        Horion/Module/Modules/AutoClicker.h
        Horion/Module/Modules/AutoGapple.cpp
        Horion/Module/Modules/AutoGapple.h
        Horion/Module/Modules/AutoSneak.cpp
        Horion/Module/Modules/AutoSneak.h
        Horion/Module/Modules/AutoSprint.cpp
        Horion/Module/Modules/AutoSprint.h
        Horion/Module/Modules/AutoTotem.cpp
        Horion/Module/Modules/AutoTotem.h
        Horion/Module/Modules/Bhop.cpp
        Horion/Module/Modules/Bhop.h
        Horion/Module/Modules/Blink.cpp
        Horion/Module/Modules/Blink.h
        Horion/Module/Modules/BowAimbot.cpp
        Horion/Module/Modules/BowAimbot.h
        Horion/Module/Modules/Breadcrumbs.cpp
        Horion/Module/Modules/Breadcrumbs.h
        Horion/Module/Modules/Breaker.cpp
        Horion/Module/Modules/Breaker.h
        Horion/Module/Modules/ChestAura.cpp
        Horion/Module/Modules/ChestAura.h
        Horion/Module/Modules/ChestESP.cpp
        Horion/Module/Modules/ChestESP.h
        Horion/Module/Modules/ChestStealer.cpp
        Horion/Module/Modules/ChestStealer.h
        Horion/Module/Modules/ClickGuiMod.cpp
        Horion/Module/Modules/ClickGuiMod.h
        Horion/Module/Modules/Crasher.cpp
        Horion/Module/Modules/Crasher.h
        Horion/Module/Modules/CrystalAura.cpp
        Horion/Module/Modules/CrystalAura.h
        Horion/Module/Modules/CubeGlide.cpp
        Horion/Module/Modules/CubeGlide.h
        Horion/Module/Modules/Derp.cpp
        Horion/Module/Modules/Derp.h
        Horion/Module/Modules/Dicker.cpp
        Horion/Module/Modules/Dicker.h
        Horion/Module/Modules/DVDLogo.cpp
        Horion/Module/Modules/DVDLogo.h
        Horion/Module/Modules/EditionFaker.cpp
        Horion/Module/Modules/EditionFaker.h
        Horion/Module/Modules/ESP.cpp
        Horion/Module/Modules/ESP.h
        Horion/Module/Modules/FastEat.cpp
        Horion/Module/Modules/FastEat.h
        Horion/Module/Modules/FastLadder.cpp
        Horion/Module/Modules/FastLadder.h
        Horion/Module/Modules/FeetPlace.cpp
        Horion/Module/Modules/FeetPlace.h
        Horion/Module/Modules/Fly.cpp
        Horion/Module/Modules/Fly.h
        Horion/Module/Modules/Freecam.cpp
        Horion/Module/Modules/Freecam.h
        Horion/Module/Modules/Freelook.cpp
        Horion/Module/Modules/Freelook.h
        Horion/Module/Modules/Fucker.cpp
        Horion/Module/Modules/Fucker.h
        Horion/Module/Modules/FullBright.cpp
        Horion/Module/Modules/FullBright.h
        Horion/Module/Modules/Glide.cpp
        Horion/Module/Modules/Glide.h
        Horion/Module/Modules/Godmode.cpp
        Horion/Module/Modules/Godmode.h
        Horion/Module/Modules/HighJump.cpp
        Horion/Module/Modules/HighJump.h
        Horion/Module/Modules/Hitbox.cpp
        Horion/Module/Modules/Hitbox.h
        Horion/Module/Modules/HudEditor.cpp
        Horion/Module/Modules/HudEditor.h
        Horion/Module/Modules/HudModule.cpp
        Horion/Module/Modules/HudModule.h
        Horion/Module/Modules/InfiniteAura.cpp
        Horion/Module/Modules/InfiniteAura.h
        Horion/Module/Modules/InfiniteTreasureAura.cpp
        Horion/Module/Modules/InfiniteTreasureAura.h
        Horion/Module/Modules/InstaBreak.cpp
        Horion/Module/Modules/InstaBreak.h
        Horion/Module/Modules/InventoryCleaner.cpp
        Horion/Module/Modules/InventoryCleaner.h
        Horion/Module/Modules/InventoryMove.cpp
        Horion/Module/Modules/InventoryMove.h
        Horion/Module/Modules/InventoryViewer.cpp
        Horion/Module/Modules/InventoryViewer.h
        Horion/Module/Modules/JavascriptModule.cpp
        Horion/Module/Modules/JavascriptModule.h
        Horion/Module/Modules/Jesus.cpp
        Horion/Module/Modules/Jesus.h
        Horion/Module/Modules/Jetpack.cpp
        Horion/Module/Modules/Jetpack.h
        Horion/Module/Modules/Killaura.cpp
        Horion/Module/Modules/Killaura.h
        Horion/Module/Modules/LigmaNuts.cpp
        Horion/Module/Modules/LigmaNuts.h
        Horion/Module/Modules/MaceHit.cpp
        Horion/Module/Modules/MaceHit.h
        Horion/Module/Modules/MidClick.cpp
        Horion/Module/Modules/MidClick.h
        Horion/Module/Modules/Module.cpp
        Horion/Module/Modules/Module.h
        Horion/Module/Modules/NameTags.cpp
        Horion/Module/Modules/NameTags.h
        Horion/Module/Modules/Nbt.cpp
        Horion/Module/Modules/Nbt.h
        Horion/Module/Modules/NightMode.cpp
        Horion/Module/Modules/NightMode.h
        Horion/Module/Modules/NoClip.cpp
        Horion/Module/Modules/NoClip.h
        Horion/Module/Modules/NoFall.cpp
        Horion/Module/Modules/NoFall.h
        Horion/Module/Modules/NoFriends.cpp
        Horion/Module/Modules/NoFriends.h
        Horion/Module/Modules/NoHurtcam.cpp
        Horion/Module/Modules/NoHurtcam.h
        Horion/Module/Modules/NoPacket.cpp
        Horion/Module/Modules/NoPacket.h
        Horion/Module/Modules/NoPaintingCrash.cpp
        Horion/Module/Modules/NoPaintingCrash.h
        Horion/Module/Modules/NoSlowDown.cpp
        Horion/Module/Modules/NoSlowDown.h
        Horion/Module/Modules/NoSwing.cpp
        Horion/Module/Modules/NoSwing.h
        Horion/Module/Modules/NoWeb.cpp
        Horion/Module/Modules/NoWeb.h
        Horion/Module/Modules/Notifications.cpp
        Horion/Module/Modules/Notifications.h
        Horion/Module/Modules/Nuker.cpp
        Horion/Module/Modules/Nuker.h
        Horion/Module/Modules/PacketLogger.cpp
        Horion/Module/Modules/PacketLogger.h
        Horion/Module/Modules/Phase.cpp
        Horion/Module/Modules/Phase.h
        Horion/Module/Modules/RainbowSky.cpp
        Horion/Module/Modules/RainbowSky.h
        Horion/Module/Modules/Reach.cpp
        Horion/Module/Modules/Reach.h
        Horion/Module/Modules/Scaffold.cpp
        Horion/Module/Modules/Scaffold.h
        Horion/Module/Modules/SeeCrystals.cpp
        Horion/Module/Modules/SeeCrystals.h
        Horion/Module/Modules/Spammer.cpp
        Horion/Module/Modules/Spammer.h
        Horion/Module/Modules/Speed.cpp
        Horion/Module/Modules/Speed.h
        Horion/Module/Modules/StackableItem.cpp
        Horion/Module/Modules/StackableItem.h
        Horion/Module/Modules/Step.cpp
        Horion/Module/Modules/Step.h
        Horion/Module/Modules/TargetHUD.cpp
        Horion/Module/Modules/TargetHUD.h
        Horion/Module/Modules/Teams.cpp
        Horion/Module/Modules/Teams.h
        Horion/Module/Modules/Teleport.cpp
        Horion/Module/Modules/Teleport.h
        Horion/Module/Modules/TestModule.cpp
        Horion/Module/Modules/TestModule.h
        Horion/Module/Modules/Timer.cpp
        Horion/Module/Modules/Timer.h
        Horion/Module/Modules/Tower.cpp
        Horion/Module/Modules/Tower.h
        Horion/Module/Modules/Tracer.cpp
        Horion/Module/Modules/Tracer.h
        Horion/Module/Modules/TriggerBot.cpp
        Horion/Module/Modules/TriggerBot.h
        Horion/Module/Modules/Velocity.cpp
        Horion/Module/Modules/Velocity.h
        Horion/Module/Modules/Watermark.cpp
        Horion/Module/Modules/Watermark.h
        Horion/Module/Modules/Xray.cpp
        Horion/Module/Modules/Xray.h
        Horion/Module/Modules/Zoom.cpp
        Horion/Module/Modules/Zoom.h
        Horion/Module/ModuleManager.cpp
        Horion/Module/ModuleManager.h

        Horion/Render/RenderGlobals.cpp
        Horion/Render/RenderGlobals.h
        Horion/Render/RenderInit.cpp
        Horion/Render/RenderUtils.cpp
        Horion/Render/RenderUtils.h
        Horion/Render/Signatures.cpp
        Horion/Render/Signatures.h

        Horion/Scripting/Functions/CommandManagerFunctions.cpp
        Horion/Scripting/Functions/CommandManagerFunctions.h
        Horion/Scripting/Functions/DrawFunctions.cpp
        Horion/Scripting/Functions/DrawFunctions.h
        Horion/Scripting/Functions/EntityFunctions.cpp
        Horion/Scripting/Functions/EntityFunctions.h
        Horion/Scripting/Functions/GameFunctions.cpp
        Horion/Scripting/Functions/GameFunctions.h
        Horion/Scripting/Functions/GlobalFunctions.cpp
        Horion/Scripting/Functions/GlobalFunctions.h
        Horion/Scripting/Functions/HorionFunctions.cpp
        Horion/Scripting/Functions/HorionFunctions.h
        Horion/Scripting/Functions/InventoryFunctions.cpp
        Horion/Scripting/Functions/InventoryFunctions.h
        Horion/Scripting/Functions/LevelFunctions.cpp
        Horion/Scripting/Functions/LevelFunctions.h
        Horion/Scripting/Functions/LocalPlayerFunctions.cpp
        Horion/Scripting/Functions/LocalPlayerFunctions.h
        Horion/Scripting/Functions/ModuleManagerFunctions.cpp
        Horion/Scripting/Functions/ModuleManagerFunctions.h
        Horion/Scripting/Functions/Vector3Functions.cpp
        Horion/Scripting/Functions/Vector3Functions.h
        Horion/Scripting/JsScriptModule.cpp
        Horion/Scripting/JsScriptModule.h
        Horion/Scripting/ScriptInstance.cpp
        Horion/Scripting/ScriptInstance.h
        Horion/Scripting/ScriptManager.cpp
        Horion/Scripting/ScriptManager.h

        Horion/DrawUtils.cpp
        Horion/DrawUtils.h
        Horion/GuiUtils.cpp
        Horion/GuiUtils.h
        Horion/ImmediateGui.cpp
        Horion/ImmediateGui.h
        Horion/Loader.cpp
        Horion/Loader.h

        include/chakra/ChakraCommon.h
        include/chakra/ChakraCommonWindows.h
        include/chakra/ChakraCore.h
        include/chakra/ChakraCoreWindows.h
        include/chakra/ChakraDebug.h
        include/atlrx.h
        include/d3dx11.h
        include/d3dx11async.h
        include/d3dx11core.h
        include/d3dx11effect.h
        include/RegExp.h
        include/StringProcess.h
        include/WinHttpClient.h

        Memory/GameData.cpp
        Memory/GameData.h
        Memory/Hooks.cpp
        Memory/Hooks.h
        Memory/MinHook.h
        Memory/SlimMem.cpp
        Memory/SlimMem.h

        # SDK (alphabetical, corrected)
        SDK/AABB.h
        SDK/ActorFlags.h
        SDK/Attribute.h
        SDK/BlockLegacy.cpp
        SDK/BlockLegacy.h
        SDK/Camera.cpp
        SDK/Camera.h
        SDK/ChestBlockActor.h
        SDK/ClientInstance.cpp
        SDK/ClientInstance.h
        SDK/ClientInstanceScreenModel.h
        SDK/ComplexInventoryTransaction.cpp
        SDK/ComplexInventoryTransaction.h
        SDK/Components.h
        SDK/Dimension.h
        SDK/DrawUtils.cpp
        SDK/DrawUtils.h
        SDK/Entity.cpp
        SDK/Entity.h
        SDK/EntityContext.h
        SDK/EntityList.h
        SDK/EntityRefs.h
        SDK/GameMode.cpp
        SDK/GameMode.h
        SDK/GameSettingsInput.h
        SDK/GameType.h
        SDK/HIDController.h
        SDK/HitResult.h
        SDK/HorionNameTag.cpp
        SDK/HorionNameTag.h
        SDK/InitSDK.cpp
        SDK/InitSDK.h
        SDK/Inventory.cpp
        SDK/Inventory.h
        SDK/InventoryTransaction.cpp
        SDK/InventoryTransaction.h
        SDK/Item.cpp
        SDK/Item.h
        SDK/ItemStack.h
        SDK/Level.cpp
        SDK/Level.h
        SDK/LocalPlayer.cpp
        SDK/LocalPlayer.h
        SDK/LoopbackPacketSender.h
        SDK/MC_Color.h
        SDK/MathUtils.h
        SDK/MatrixStack.cpp
        SDK/MatrixStack.h
        SDK/MinecraftUIRenderContext.h
        SDK/MojangsonToken.h
        SDK/MoveInputHandler.h
        SDK/Offsets.cpp
        SDK/Offsets.h
        SDK/Packet.cpp
        SDK/Packet.h
        SDK/Player.cpp
        SDK/Player.h
        SDK/PlayerMovementProxy.h
        SDK/PlayerSignatures.h
        SDK/PlayerSupplies.h
        SDK/RakNetConnector.h
        SDK/RayTracer.cpp
        SDK/RayTracer.h
        SDK/Ref.h
        SDK/ScreenContext.h
        SDK/Tag.cpp
        SDK/Tag.h
        SDK/Tessellator.cpp
        SDK/Tessellator.h
        SDK/TextHolder.h
        SDK/TimerUtil.cpp
        SDK/TimerUtil.h
        SDK/UIScene.h
        SDK/Vec2.h
        SDK/Vec3.h
        SDK/Vec3i.h
        SDK/VersionTag.h
        SDK/Weather.h

        Utils/ChakraHelper.h
        Utils/DllHelper.h
        Utils/HMath.h
        Utils/Json.hpp
        Utils/Logger.cpp
        Utils/Logger.h
        Utils/SkinUtil.cpp
        Utils/SkinUtil.h
        Utils/Target.cpp
        Utils/Target.h
        Utils/TextFormat.cpp
        Utils/TextFormat.h
        Utils/Utils.cpp
        Utils/Utils.h
        Utils/xorstr.h

        Utils/keys.h

        resource.h

        Horion/Module/Modules/AntiImmobile.cpp
        Horion/Module/Modules/AntiImmobile.h

        Horion/path/JoeConstants.cpp
        Horion/path/JoeConstants.h
        Horion/path/JoeMovementController.cpp
        Horion/path/JoeMovementController.h
        Horion/path/JoePath.cpp
        Horion/path/JoePath.h
        Horion/path/JoePathFinder.cpp
        Horion/path/JoePathFinder.h
        Horion/path/JoeSegment.cpp
        Horion/path/JoeSegment.h
        Horion/path/goals/JoeGoal.cpp
        Horion/path/goals/JoeGoal.h
        Horion/path/goals/JoeGoalXYZ.cpp
        Horion/path/goals/JoeGoalXYZ.h
        Horion/path/goals/JoeGoalXZ.cpp
        Horion/path/goals/JoeGoalXZ.h
        Horion/path/goals/JoeGoalY.cpp
        Horion/path/goals/JoeGoalY.h

        Horion/Module/Modules/FollowPathModule.cpp
        Horion/Module/Modules/FollowPathModule.h
        Horion/Command/Commands/PathCommand.cpp
        Horion/Command/Commands/PathCommand.h

        "Horion/Module/Modules/Twerk.cpp"
        "Horion/Module/Modules/Twerk.h"

        # Added modules
        Horion/Module/Modules/BedAura.cpp
        Horion/Module/Modules/BedAura.h
        Horion/Module/Modules/AutoAnchor.cpp
        Horion/Module/Modules/AutoAnchor.h
)

# --- Shader compile rules ---
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/Horion/Shaders)

# Function to compile a shader
function(compile_shader SHADER_FILE PROFILE)
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
    set(OUTPUT ${CMAKE_BINARY_DIR}/${SHADER_NAME}.cso)

    add_custom_command(
        OUTPUT ${OUTPUT}
        COMMAND fxc /T ${PROFILE} /E main /Fo ${OUTPUT} ${SHADER_FILE}
        DEPENDS ${SHADER_FILE}
        COMMENT "Compiling ${SHADER_FILE} -> ${OUTPUT}"
    )

    set(COMPILED_SHADERS ${COMPILED_SHADERS} ${OUTPUT} PARENT_SCOPE)
endfunction()

# Compile each shader with the right profile
compile_shader(${SHADER_DIR}/UnlitTintPS.hlsl ps_5_0)
compile_shader(${SHADER_DIR}/UnlitTintVS.hlsl vs_5_0)

# Group them into a target
add_custom_target(Shaders ALL DEPENDS ${COMPILED_SHADERS})

# Make Horion depend on shaders
add_dependencies(Horion Shaders)