#include <string>
#include <thread>
#include <future>
#include <iostream>

#include "Cheats.h"
#include "Render.hpp"
#include "MenuConfig.hpp"

#include "Utils/Initial/Init.h"

#include "Features/ESP.h"
#include "Features/GUI.h"
#include "Features/RCS.H"
#include "Features/BombTimer.h"
#include "Features/SpectatorList.h"

int PreviousTotalHits = 0;

void Cheats::KeyCheckThread()
{
	try
	{
		if ((GetAsyncKeyState(VK_INSERT) & 0x8000))
		{
			MenuConfig::ShowMenu = !MenuConfig::ShowMenu;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Cheats::RadarSetting(Base_Radar& Radar)
{
	// Radar window
	ImGui::SetNextWindowBgAlpha(RadarCFG::RadarBgAlpha);
	ImGui::Begin("Radar", 0, ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize({ RadarCFG::RadarRange * 2,RadarCFG::RadarRange * 2 });

	if (!RadarCFG::customRadar)
	{
		ImGui::SetWindowPos(ImVec2(0, 0));
		RadarCFG::ShowRadarCrossLine = false;
		RadarCFG::Proportion = 3300.f;
		RadarCFG::RadarPointSizeProportion = 1.f;
		RadarCFG::RadarRange = 150.f;
		RadarCFG::RadarBgAlpha = 0.1f;
	}

	Radar.SetDrawList(ImGui::GetWindowDrawList());
	Radar.SetPos({ ImGui::GetWindowPos().x + RadarCFG::RadarRange, ImGui::GetWindowPos().y + RadarCFG::RadarRange });
	Radar.SetProportion(RadarCFG::Proportion);
	Radar.SetRange(RadarCFG::RadarRange);
	Radar.SetSize(RadarCFG::RadarRange * 2);
	Radar.SetCrossColor(RadarCFG::RadarCrossLineColor);

	Radar.ArcArrowSize *= RadarCFG::RadarPointSizeProportion;
	Radar.ArrowSize *= RadarCFG::RadarPointSizeProportion;
	Radar.CircleSize *= RadarCFG::RadarPointSizeProportion;

	Radar.ShowCrossLine = RadarCFG::ShowRadarCrossLine;
	Radar.Opened = true;
}

void Cheats::RenderCrossHair(ImDrawList* drawList) noexcept
{
	if (!CrosshairsCFG::ShowCrossHair)
		return;

	if (CrosshairsCFG::isAim && MenuConfig::TargetingCrosshairs)
		Render::DrawCrossHair(drawList, ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGui::ColorConvertFloat4ToU32(CrosshairsCFG::TargetedColor));
	else
		Render::DrawCrossHair(drawList, ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGui::ColorConvertFloat4ToU32(CrosshairsCFG::CrossHairColor));
}

void Cheats::Run()
{
	// Show menu only - modified version
	static DWORD lastTick = 0;
	DWORD currentTick = GetTickCount();
	if ((GetAsyncKeyState(VK_INSERT) & 0x8000) && currentTick - lastTick >= 150) {
		MenuConfig::ShowMenu = !MenuConfig::ShowMenu;
		lastTick = currentTick;
	}

	// Only show menu, disable all cheat features
	if (MenuConfig::ShowMenu)
	{
		GUI::NewGui();
	}

	// Comment out all cheat functionality below
	/*
	// Disable game window check
	if (!Init::Client::isGameWindowActive() && !MenuConfig::ShowMenu)
		return;

	// Disable night mode
	// Misc::NightMode();

	// Disable matrix reading
	// if (!ProcessMgr.ReadMemory(gGame.GetMatrixAddress(), gGame.View.Matrix, 64))
	//	return;

	// Disable entity list update
	// gGame.UpdateEntityListEntry();

	DWORD64 LocalControllerAddress = 0;
	DWORD64 LocalPawnAddress = 0;

	// Disable local entity reading
	// if (!ProcessMgr.ReadMemory(gGame.GetLocalControllerAddress(), LocalControllerAddress))
	//	return;
	// if (!ProcessMgr.ReadMemory(gGame.GetLocalPawnAddress(), LocalPawnAddress))
	//	return;

	// Disable local entity updates
	CEntity LocalEntity, ServerEntity;
	static int LocalPlayerControllerIndex = 1;
	// LocalEntity.UpdateClientData();
	// if (!LocalEntity.UpdateController(LocalControllerAddress))
	//	return;
	// if (!LocalEntity.UpdatePawn(LocalPawnAddress) && !MiscCFG::WorkInSpec)
	//	return;

	// Disable health bar map
	// static std::map<DWORD64, Render::HealthBar> HealthBarMap;

	// Disable aimbot data
	float DistanceToSight = 0;
	float MaxAimDistance = 100000;
	Vec3  HeadPos{ 0,0,0 };
	Vec2  Angles{ 0,0 };
	std::vector<Vec3> AimPosList;

	// Disable radar
	Base_Radar Radar;
	// if (RadarCFG::ShowRadar)
	//	RadarSetting(Radar);

	// Disable entity loop
	for (int i = 0; i < 64; i++)
	{
		CEntity Entity;
		DWORD64 EntityAddress = 0;
		// if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListEntry() + (i + 1) * 0x78, EntityAddress))
		//	continue;
		if (EntityAddress == LocalEntity.Controller.Address)
		{
			LocalPlayerControllerIndex = i;
			continue;
		}
		// if (!Entity.UpdateController(EntityAddress))
		//	continue;
		// if (!Entity.UpdatePawn(Entity.Pawn.Address))
		//	continue;

		// Disable spectator list
		std::vector<std::string> spectators;
		// if (MiscCFG::SpecList && !MenuConfig::ShowMenu)
		// {
		//	... spectator code ...
		// }

		// Disable team check
		// if (MenuConfig::TeamCheck && Entity.Controller.TeamID == LocalEntity.Controller.TeamID)
		//	continue;

		// Disable money service
		// Misc::MoneyService(Entity);

		// Disable alive check
		// if (!Entity.IsAlive())
		//	continue;

		// Disable radar hack
		// Misc::RadarHack(Entity);

		// Disable screen check
		// if (!Entity.IsInScreen())
		//	continue;

		// Disable bone processing for aimbot
		// if (AimControl::HitboxList.size() != 0)
		// {
		//	... aimbot code ...
		// }

		// Disable ESP rendering
		// if (ESPConfig::ESPenabled)
		// {
		//	... ESP rendering code ...
		// }

		// Disable glow
		// Glow::Run(Entity);
	}

	// Disable radar render
	// if (RadarCFG::ShowRadar)
	// {
	//	Radar.Render();
	//	ImGui::End();
	// }

	// Disable trigger bot
	// if (MenuConfig::TriggerBot && (GetAsyncKeyState(TriggerBot::HotKey) || MenuConfig::TriggerAlways))
	//	TriggerBot::Run(LocalEntity);

	// Disable misc features
	// Misc::HitSound(LocalEntity, PreviousTotalHits);
	// Misc::FlashImmunity(LocalEntity);
	// Misc::FastStop();
	// Misc::NadeManager(gGame);
	// Misc::FovChanger(LocalEntity);
	// Misc::Watermark(LocalEntity);
	// Misc::FakeDuck(LocalEntity);
	// Misc::BunnyHop(LocalEntity);
	// Misc::CheatList();
	// Misc::ForceScope(LocalEntity);

	// Disable FOV line
	// Render::DrawFov(LocalEntity, MenuConfig::FovLineSize, MenuConfig::FovLineColor, 1);

	// Disable headshot line
	// Render::HeadShootLine(LocalEntity, MenuConfig::HeadShootLineColor);

	// Disable crosshair
	// TriggerBot::TargetCheck(LocalEntity);
	// Misc::AirCheck(LocalEntity);
	// RenderCrossHair(ImGui::GetBackgroundDrawList());

	// Disable bomb timer
	// bmb::RenderWindow();

	// Disable aimbot completely
	// if (MenuConfig::AimBot) {
	//	... aimbot code ...
	// }
	// else {
	//	RCS::RecoilControl(LocalEntity);
	// }
	*/
}