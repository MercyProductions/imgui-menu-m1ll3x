#define IMGUI_DEFINE_MATH_OPERATORS
#include "stdafx.h"
#include "Menu.h"
#include "imgui/imgui.h"
#include "globals.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"
#include "obfuscator.hpp"
#include "xor.hpp"
#include "memory.h"
#include "mem.h"
#include "xorstr.hpp"
#include "style.h"
#include "custom_widgets.hpp"
#include "ImGui_Notify.h"

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

bool dark = true;

char field[45] = { "" };

int page = 0;

static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

int key, m;

bool checkbox_on = true;
bool checkbox_off = false;

static int slider_int = 50;
static float slider_float = 0.5f;

static int select1 = 0;
const char* items[3]{ "Selected", "Nope", "what :/ a y?" };

static int bullet_select = 0;
const char* bullet[2]{ "Disabled", "Enabled" };

static int sound_select = 0;
const char* sound[2]{ "Disabled", "Enabled" };

static int style_select = 0;
const char* stylee[2]{ "Flat", "Back" };

static int style2_select = 0;
const char* stylee2[2]{ "Textured", "3D Mode" };

static bool multi_num[5] = { false, true, true, true, false };
const char* multi_items[5] = { "One", "Two", "Three", "Four", "Five" };

float knob = 1.f;

float col[4] = { 118 / 255.f, 187 / 255.f, 117 / 255.f, 0.5f };

int rotation_start_index;
void ImRotateStart()
{
	rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 ImRotationCenter()
{
	ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

	const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = rotation_start_index; i < buf.Size; i++)
		l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

	return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
}

void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
	float s = sin(rad), c = cos(rad);
	center = ImRotate(center, s, c) - center;

	auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = rotation_start_index; i < buf.Size; i++)
		buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

bool b_menu_open = true;
bool b_debug_open = false;
bool boxcheck;
int Selected_Camo_MW = 0;
int Selected_Camo_CW = 0;
int Selected_Camo_VG = 0;
int gameMode2 = 0;
int i_MenuTab = 0;
uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	if (ptr != 0)
	{
		uintptr_t addr = ptr;
		for (unsigned int i = 0; i < offsets.size(); ++i)
		{
			addr = *(uintptr_t*)addr;
			addr += offsets[i];
		}
		return addr;
	}
	else
		return 0;
}


//uint64_t BASEIMAGE2 = reinterpret_cast<uint64_t>(GetModuleHandleA(NULL));

bool b_fov = false;
float f_fov = 1.20f;
float f_map = 1.0f;
bool b_map = false;
bool b_brightmax = false;
bool b_thirdperson = false;
bool b_heartcheat = false;
bool b_norecoil = false;
bool b_no_flashbang = false;

struct unnamed_type_integer
{
	int min;
	int max;
};
struct unnamed_type_integer64
{
	__int64 min;
	__int64 max;
};
struct unnamed_type_enumeration
{
	int stringCount;
	const char* strings;
};
/* 433 */
struct unnamed_type_unsignedInt64
{
	unsigned __int64 min;
	unsigned __int64 max;
};

/* 434 */
struct unnamed_type_value
{
	float min;
	float max;
	float devguiStep;
};

/* 435 */
struct unnamed_type_vector
{
	float min;
	float max;
	float devguiStep;
};




uintptr_t cbuff1;
uintptr_t cbuff2;
char inputtext[50];
int C_TagMOde = 0;


__int64 find_pattern(__int64 range_start, __int64 range_end, const char* pattern) {
	const char* pat = pattern;
	__int64 firstMatch = NULL;
	__int64 pCur = range_start;
	__int64 region_end;
	MEMORY_BASIC_INFORMATION mbi{};
	while (sizeof(mbi) == VirtualQuery((LPCVOID)pCur, &mbi, sizeof(mbi))) {
		if (pCur >= range_end - strlen(pattern))
			break;
		if (!(mbi.Protect & (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_READWRITE))) {
			pCur += mbi.RegionSize;
			continue;
		}
		region_end = pCur + mbi.RegionSize;
		while (pCur < region_end)
		{
			if (!*pat)
				return firstMatch;
			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
				if (!firstMatch)
					firstMatch = pCur;
				if (!pat[1] || !pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;
				else
					pat += 2;
			}
			else {
				if (firstMatch)
					pCur = firstMatch;
				pat = pattern;
				firstMatch = 0;
			}
			pCur++;
		}
	}
	return NULL;
}


bool init_once = true;
char input[30];
bool Unlock_once = true;

void Visual()
{
	ImGui::Dummy(ImVec2(0.0f, 3.0f));
	ImGui::Spacing();
	ImGui::Checkbox(xorstr_("Check Visibility"), &globals::b_visible);
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
	{
		ImGui::SetTooltip(xorstr_("Can only be turned on INGAME!"));
	}
	ImGui::Checkbox(xorstr_("Show Box"), &globals::b_box);
	/*ImGui::SameLine();
	ImGui::Combo("##", &globals::box_index, globals::box_types, 2);*/

	//ImGui::Checkbox(xorstr_("Show HealthBar"), &globals::b_health);
	ImGui::Checkbox(xorstr_("Show Line"), &globals::b_line);
	ImGui::Checkbox(xorstr_("Show Bones "), &globals::b_skeleton);
	ImGui::Checkbox(xorstr_("Show Names"), &globals::b_names);
	ImGui::Checkbox(xorstr_("Show Distance"), &globals::b_distance);
	ImGui::Checkbox(xorstr_("Show Team"), &globals::b_friendly);
	ImGui::SliderInt(xorstr_("##MAXDISTANCE"), &globals::max_distance, 0, 1000, xorstr_("ESP Distance: %d"));
}
void KeyBindButton(int& key, int width, int height)
{
	static auto b_get = false;
	static std::string sz_text = xorstr_("Click to bind.");

	if (ImGui::Button(sz_text.c_str(), ImVec2(static_cast<float>(width), static_cast<float>(height))))
		b_get = true;

	if (b_get)
	{
		for (auto i = 1; i < 256; i++)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				if (i != 12)
				{
					key = i == VK_ESCAPE ? -1 : i;
					b_get = false;
				}
			}
		}
		sz_text = xorstr_("Press a Key.");
	}
	else if (!b_get && key == -1)
		sz_text = xorstr_("Click to bind.");
	else if (!b_get && key != -1)
	{
		sz_text = xorstr_("Key ~ ") + std::to_string(key);
	}
}
void Aimbot()
{

	ImGui::Dummy(ImVec2(0.0f, 3.0f));
	ImGui::Spacing();
	ImGui::Checkbox(xorstr_("Enable"), &globals::b_lock);
	if (globals::b_lock)
	{
		ImGui::SliderInt(xorstr_("##LOCKSMOOTH"), &globals::aim_smooth, 1, 30, xorstr_("Lock Smooth: %d"));
	}
	ImGui::Checkbox(xorstr_("Crosshair"), &globals::b_crosshair);
	ImGui::Checkbox(xorstr_("Show FOV"), &globals::b_fov);
	if (globals::b_fov)
	{
		ImGui::SliderFloat(xorstr_("##LOCKFOV"), &globals::f_fov_size, 10.f, 800.f, xorstr_("FOV Size: %0.0f"));
	}
	ImGui::Checkbox(xorstr_("Skip Knocked"), &globals::b_skip_knocked);

	
	
	/*ImGui::Checkbox(xorstr_("Prediction"), &globals::b_prediction);*/

	
	

	/*ImGui::Checkbox(xorstr_("Use Bones"), &globals::target_bone);
	if (globals::target_bone)
	{
		ImGui::Combo(xorstr_("Lock Bone"), &globals::bone_index, globals::aim_lock_point, 4);
	}*/

	KeyBindButton(globals::aim_key, 100, 30);
	ImGui::Dummy(ImVec2(0.0f, 1.0f));
	ImGui::Spacing();

	

}
void ColorPicker()
{
	ImGui::Dummy(ImVec2(0.0f, 1.0f));
	ImGui::Text("Fov");
	ImGui::ColorEdit4("##Fov Color7", (float*)&color::bfov);
	ImGui::Text("Crosshair");
	ImGui::ColorEdit4("##cross hair Color9", (float*)&color::draw_crosshair);
	ImGui::Text("Visible Team");
	ImGui::ColorEdit4("##esp Color1", (float*)&color::VisibleColorTeam);
	ImGui::Spacing();
	ImGui::Text("Not Visible Team");
	ImGui::ColorEdit4("##esp Color2", (float*)&color::NotVisibleColorTeam);
	ImGui::Spacing();
	ImGui::Text("Visible Enemy");
	ImGui::ColorEdit4("##esp Color3", (float*)&color::VisibleColorEnemy);
	ImGui::Spacing();
	ImGui::Text("Not Visible Enemy");
	ImGui::ColorEdit4("##esp Color4", (float*)&color::NotVisibleColorEnemy);
	/*ImGui::Spacing();
	ImGui::Text("Names");
	ImGui::ColorEdit4("##esp Color5", (float*)&color::nameColor);
	ImGui::Text("Distance");
	ImGui::ColorEdit4("##esp Color6", (float*)&color::dis_Color);*/
	//ImGui::Text("loot Color");
	//ImGui::ColorEdit4("##esp lootcolor", (float*)&loot::lootcolor);
	//ImGui::EndTabItem();

}
void CL_PlayerData_SetCustomClanTag(int controllerIndex, const char* clanTag) {

}
void ShowToastNotificationAfterUserJoinedParty(const char* message)
{

}
void SetCamo(int Class, int Weapon, int Camo)
{
	

}
void Misc()
{
	ImGui::Dummy(ImVec2(0.0f, 3.0f));
	ImGui::Checkbox(xorstr_("UAV"), &globals::b_UAV);
	/*if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
	{
		ImGui::SetTooltip(xorstr_("Use at your own Risk"));
	}*/
	ImGui::Spacing();

	ImGui::Dummy(ImVec2(0.0f, 3.0f));
	ImGui::Spacing();
	/*if (ImGui::SliderFloat("FOV SLIDER", &globals::f_fov, 0.1f, 4.f, "%.2f"))
	{
		dvar_set2("NSSLSNKPN", globals::f_fov);
	}
	ImGui::Spacing();*/
	if (ImGui::Button(xorstr_("Unlock All"), ImVec2(150, 30)))
	{

		sdk::unlockall();
		//ShowToastNotificationAfterUserJoinedParty("^4Sim^2ple^5Too^2lZ ^3Unlock^1ed ^5Everything! ^1<3");
	}
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
	{
		ImGui::SetTooltip(xorstr_("Use at your own Risk! Use AC Blocker!"));
	}
	ImGui::Dummy(ImVec2(0.0f, 3.0f));
	//ImGui::Checkbox(xorstr_("No Recoil"), &globals::b_recoil);
	
	ImGui::Spacing();
	
	
	static char customtag[20];
	ImGui::Spacing();
	ImGui::InputTextWithHint(xorstr_("##KeyInput"), xorstr_("Custom Clan Tag"), customtag, 20);
	ImGui::SameLine();
	if (ImGui::Button(xorstr_("Set"), ImVec2(50, 20)))
	{
		CL_PlayerData_SetCustomClanTag(0, customtag);
	}




}


namespace g_menu
{
	void menu()
	{
		ImGui::SetNextWindowSizeConstraints(ImVec2(c::bg::size.x, c::bg::size.y), ImGui::GetIO().DisplaySize);
		ImGui::Begin("M1LL3X", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
		{
			ImGuiStyle* style = &ImGui::GetStyle();

			const ImVec2& pos = ImGui::GetWindowPos();
			const ImVec2& region = ImGui::GetContentRegionMax();
			const ImVec2& spacing = style->ItemSpacing;

			style->WindowPadding = ImVec2(0, 0);
			style->ItemSpacing = ImVec2(20, 20);
			style->WindowBorderSize = 0;
			style->ScrollbarSize = 10.f;

			c::bg::background = ImLerp(c::bg::background, dark ? ImColor(15, 15, 15) : ImColor(255, 255, 255), ImGui::GetIO().DeltaTime * 12.f);
			c::separator = ImLerp(c::separator, dark ? ImColor(22, 23, 26) : ImColor(222, 228, 244), ImGui::GetIO().DeltaTime * 12.f);

			c::accent = ImLerp(c::accent, dark ? ImColor(118, 187, 117) : ImColor(121, 131, 207), ImGui::GetIO().DeltaTime * 12.f);

			c::elements::background_hovered = ImLerp(c::elements::background_hovered, dark ? ImColor(31, 33, 38) : ImColor(197, 207, 232), ImGui::GetIO().DeltaTime * 25.f);
			c::elements::background = ImLerp(c::elements::background, dark ? ImColor(22, 23, 25) : ImColor(222, 228, 244), ImGui::GetIO().DeltaTime * 25.f);

			c::checkbox::mark = ImLerp(c::checkbox::mark, dark ? ImColor(0, 0, 0) : ImColor(255, 255, 255), ImGui::GetIO().DeltaTime * 12.f);

			c::child::background = ImLerp(c::child::background, dark ? ImColor(17, 17, 18) : ImColor(241, 243, 249), ImGui::GetIO().DeltaTime * 12.f);
			c::child::cap = ImLerp(c::child::cap, dark ? ImColor(20, 21, 23) : ImColor(228, 235, 248), ImGui::GetIO().DeltaTime * 12.f);

			c::page::text_hov = ImLerp(c::page::text_hov, dark ? ImColor(68, 71, 85) : ImColor(136, 145, 176), ImGui::GetIO().DeltaTime * 12.f);
			c::page::text = ImLerp(c::page::text, dark ? ImColor(68, 71, 85) : ImColor(136, 145, 176), ImGui::GetIO().DeltaTime * 12.f);

			c::page::background_active = ImLerp(c::page::background_active, dark ? ImColor(31, 33, 38) : ImColor(196, 205, 228), ImGui::GetIO().DeltaTime * 25.f);
			c::page::background = ImLerp(c::page::background, dark ? ImColor(22, 23, 25) : ImColor(222, 228, 244), ImGui::GetIO().DeltaTime * 25.f);

			c::text::text_active = ImLerp(c::text::text_active, dark ? ImColor(255, 255, 255) : ImColor(0, 0, 0), ImGui::GetIO().DeltaTime * 12.f);
			c::text::text_hov = ImLerp(c::text::text_hov, dark ? ImColor(68, 71, 85) : ImColor(68, 71, 81), ImGui::GetIO().DeltaTime * 12.f);
			c::text::text = ImLerp(c::text::text, dark ? ImColor(68, 71, 85) : ImColor(68, 71, 81), ImGui::GetIO().DeltaTime * 12.f);

			ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + ImVec2(region), ImGui::GetColorU32(c::bg::background), c::bg::rounding);
			ImGui::GetWindowDrawList()->AddRectFilled(pos + spacing, pos + ImVec2(region.x - spacing.x, 50 + spacing.y), ImGui::GetColorU32(c::child::background), c::child::rounding);

			//   PushFont(font::icomoon_logo);
			//   GetWindowDrawList()->AddText(pos + ImVec2(region.x - (spacing.x + CalcTextSize("m").x) - 14, 50 - (spacing.y + CalcTextSize("m").y) + 31), GetColorU32(c::accent, 1.f), "m");
			//   PopFont();
			//
			SetCursorPos(ImVec2(spacing.x, (50 + (spacing.y * 2))));

			tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (page == active_tab ? 1.f : -1.f)), 0.f, 1.f);
			if (tab_alpha == 0.f && tab_add == 0.f) active_tab = page;

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);

			if (active_tab == 0)
			{

				custom::BeginGroup();
				{

					custom::Child("GENERAL", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 2, (GetContentRegionMax().y - (60 + spacing.y * 2) * 2) + 10), true);
					{

						static bool popup = false;
						if (custom::CheckboxClicked("Checkbox true", &checkbox_on)) popup = !popup;

						if (popup) {
							ImGui::Begin("Popupbox", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
							{
								custom::Child("Popupbox", ImVec2(300, 170), ImGuiWindowFlags_NoBringToFrontOnFocus);
								{
									custom::ColorEdit4("Color Palette", col, picker_flags);

									custom::Separator_line();

									custom::Combo("Combobox", &select1, items, IM_ARRAYSIZE(items), 3);
								}
								custom::EndChild();
							}
							ImGui::End();
						}

						custom::Separator_line();

						custom::Checkbox("Checkbox false", &checkbox_off);

						custom::Separator_line();

						custom::MultiCombo("Multi Combo", multi_num, multi_items, 5);

						custom::Separator_line();

						custom::SliderInt("Slider Integer", &slider_int, 1, 100, "%d%%");

						custom::Separator_line();

						custom::SliderFloat("Slider Float", &slider_float, 0.f, 5.f, "%.3f [FL]");

						custom::Separator_line();

						ImGui::InputTextWithHint("Text Field", "Hello Mate :/", field, 45, NULL);

						SetCursorPosY(2000);

					}
					custom::EndChild();
				}
				custom::EndGroup();

				ImGui::SameLine();

				custom::BeginGroup();
				{
					custom::Child("SETTINGS", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 2, ((GetContentRegionMax().y - (60 + spacing.y * 2) * 2) + 10) / 2 - 10), true);
					{
						custom::ColorEdit4("Color Palette", col, picker_flags);

						custom::Separator_line();

						custom::Combo("Combobox", &select1, items, IM_ARRAYSIZE(items), 3);

					}
					custom::EndChild();

					custom::Child("OTHER", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 2, ((GetContentRegionMax().y - (60 + spacing.y * 2) * 2) + 10) / 2 - 10), true);
					{

						custom::KnobFloat("Dot Slider", &knob, -1.f, 1.f, "%.1f [F]");

						custom::Separator_line();

						custom::Keybind("Keybind", &key, &m);

					}
					custom::EndChild();

				}
				custom::EndGroup();

			}
			else if (active_tab == 1)
			{
				static bool checkboxes[10];

				custom::BeginGroup();
				{

					custom::Child("ESP", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 2, (GetContentRegionMax().y - (60 + spacing.y * 2) * 2) + 10), true);
					{

						custom::Checkbox("Enable ESP", &checkboxes[0]);

						custom::Separator_line();

						custom::Checkbox("Through Walls", &checkboxes[1]);

						custom::Separator_line();

						custom::Combo("Bullet Tracer", &bullet_select, bullet, IM_ARRAYSIZE(bullet), 2);

						custom::Separator_line();

						custom::Checkbox("Dynamic Boxes", &checkboxes[2]);

						custom::Separator_line();

						custom::Checkbox("Dormant", &checkboxes[3]);

						custom::Separator_line();

						custom::Checkbox("Shared ESP", &checkboxes[4]);

						custom::Separator_line();

						custom::Combo("Sound ESP", &sound_select, sound, IM_ARRAYSIZE(sound), 2);


					}
					custom::EndChild();
				}
				custom::EndGroup();

				ImGui::SameLine();

				custom::BeginGroup();
				{
					custom::Child("CHAMS", ImVec2((GetContentRegionMax().x - spacing.x * 3) / 2, (GetContentRegionMax().y - (60 + spacing.y * 2) * 2) + 10), true);
					{

						custom::Checkbox("Enable Chams", &checkboxes[5]);

						custom::Separator_line();

						custom::Checkbox("Through Walls", &checkboxes[6]);

						custom::Separator_line();

						custom::Combo("Style", &style_select, stylee, IM_ARRAYSIZE(stylee), 2);

						custom::Separator_line();

						custom::Checkbox("Backtrack", &checkboxes[7]);

						custom::Separator_line();

						custom::Combo("Style ", &style2_select, stylee2, IM_ARRAYSIZE(stylee2), 2);

						custom::Separator_line();

						custom::Checkbox("One Shot", &checkboxes[8]);

						custom::Separator_line();

						custom::Checkbox("Ragdolls", &checkboxes[9]);

					}
					custom::EndChild();
				}
				custom::EndGroup();
			}

			ImGui::PopStyleVar();

			SetCursorPos(ImVec2(spacing.x, region.y - (60 + spacing.y)));
			custom::BeginGroup();
			{

				custom::Child("Page One", ImVec2((GetContentRegionMax().x - (spacing.x * 6)) / 4 - 2, 60), false);
				{

					if (custom::ThemeButton("0", dark, ImVec2((GetContentRegionMax().x - spacing.x * 2) / 2, GetContentRegionMax().y - spacing.y)))
					{
						dark = !dark;
						ImGui::Notification({ ImGuiToastType_Success, 4000, dark ? "It's black mode! Finally it was added" : "This is the white mode! Finally it was added" });
					}
					SameLine();
					if (custom::Button("b", ImVec2((GetContentRegionMax().x - spacing.x * 2) / 2, GetContentRegionMax().y - spacing.y)));

				}
				custom::EndChild();

				SameLine();

				custom::SeparatorEx(ImGuiSeparatorFlags_Vertical, 2.f);

				SameLine();

				custom::Child("Page Two", ImVec2((GetContentRegionMax().x - (spacing.x * 6)) / 2, 60), false);
				{

					if (custom::Page(0 == page, "c", ImVec2((GetContentRegionMax().x - spacing.x * 4) / 4, GetContentRegionMax().y - spacing.y))) page = 0;
					SameLine();
					if (custom::Page(1 == page, "d", ImVec2((GetContentRegionMax().x - spacing.x * 4) / 4, GetContentRegionMax().y - spacing.y))) page = 1;
					SameLine();
					if (custom::Page(2 == page, "e", ImVec2((GetContentRegionMax().x - spacing.x * 4) / 4, GetContentRegionMax().y - spacing.y))) page = 2;
					SameLine();
					if (custom::Page(3 == page, "f", ImVec2((GetContentRegionMax().x - spacing.x * 4) / 4, GetContentRegionMax().y - spacing.y))) page = 3;

				}
				custom::EndChild();

				SameLine();

				custom::SeparatorEx(ImGuiSeparatorFlags_Vertical, 2.f);

				SameLine();

				custom::Child("Page Three", ImVec2((GetContentRegionMax().x - (spacing.x * 6)) / 4 - 2, 60), false);
				{
					if (custom::Button("g", ImVec2((GetContentRegionMax().x - spacing.x * 2) / 2, GetContentRegionMax().y - spacing.y)));
					SameLine();
					if (custom::Button("h", ImVec2((GetContentRegionMax().x - spacing.x * 2) / 2, GetContentRegionMax().y - spacing.y)));
				}
				custom::EndChild();

			}
			custom::EndGroup();

		}
		ImGui::End();

		ImGui::RenderNotifications();
	}
}