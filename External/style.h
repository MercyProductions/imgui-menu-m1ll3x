#include <imgui.h>
#include <corecrt_math.h>
class EditorColorScheme
{
	// 0xRRGGBBAA
	inline static int BackGroundColor = 0x2424230F;
	inline static int TextColor = 0xf5cb5c0F;
	inline static int MainColor = 0x3335330F;
	inline static int MainAccentColor = 0x403d390F;
	inline static int HighlightColor = 0xADB5BD0F;

	inline static int Black = 0x00000000;
	inline static int White = 0xFFFFFF00;

	inline static int AlphaTransparent = 0x00;
	inline static int Alpha20 = 0x33;
	inline static int Alpha40 = 0x66;
	inline static int Alpha50 = 0x80;
	inline static int Alpha60 = 0x99;
	inline static int Alpha80 = 0xCC;
	inline static int Alpha90 = 0xE6;
	inline static int AlphaFull = 0xFF;

	static float GetR(int colorCode) { return (float)((colorCode & 0xFF000000) >> 24) / (float)(0xFF); }
	static float GetG(int colorCode) { return (float)((colorCode & 0x00FF0000) >> 16) / (float)(0xFF); }
	static float GetB(int colorCode) { return (float)((colorCode & 0x0000FF00) >> 8) / (float)(0xFF); }
	static float GetA(int alphaCode) { return ((float)alphaCode / (float)0xFF); }

	static ImVec4 GetColor(int c, int a = Alpha90) { return ImVec4(GetR(c), GetG(c), GetB(c), GetA(a)); }
	static ImVec4 Darken(ImVec4 c, float p) { return ImVec4(fmax(0.f, c.x - 1.0f * p), fmax(0.f, c.y - 1.0f * p), fmax(0.f, c.z - 1.0f * p), c.w); }
	static ImVec4 Lighten(ImVec4 c, float p) { return ImVec4(fmax(0.f, c.x + 1.0f * p), fmax(0.f, c.y + 1.0f * p), fmax(0.f, c.z + 1.0f * p), c.w); }

	static ImVec4 Disabled(ImVec4 c) { return Darken(c, 0.6f); }
	static ImVec4 Hovered(ImVec4 c) { return Lighten(c, 0.2f); }
	static ImVec4 Active(ImVec4 c) { return Lighten(ImVec4(c.x, c.y, c.z, 1.0f), 0.1f); }
	static ImVec4 Collapsed(ImVec4 c) { return Darken(c, 0.2f); }

public:

	static void SetColors(int backGroundColor, int textColor, int mainColor, int mainAccentColor, int highlightColor)
	{
		BackGroundColor = backGroundColor;
		TextColor = textColor;
		MainColor = mainColor;
		MainAccentColor = mainAccentColor;
		HighlightColor = highlightColor;
	}
	static void ApplyTheme()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.33f, 0.86f, 0.22f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.90f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.09f, 0.10f, 0.50f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
		//colors[ImGuiCol_BorderShadow] = ImVec4(0.12f, 0.12f, 0.12f, 0.70f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.37f, 0.37f, 0.37f, 0.93f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.60f, 0.60f, 0.60f, 0.78f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 0.80f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.57f, 0.57f, 0.57f, 0.59f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.60f, 0.60f, 0.60f, 0.78f);
		colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.93f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.38f, 0.38f, 0.96f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.69f, 0.58f, 0.58f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.65f, 0.65f, 0.50f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.65f, 0.65f, 0.65f, 0.50f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.52f, 0.52f, 0.52f, 0.73f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.88f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.27f, 0.27f, 0.79f);
		colors[ImGuiCol_TabActive] = ImVec4(0.90f, 1.00f, 0.00f, 0.59f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.65f, 0.65f, 0.65f, 0.50f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);

		colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.73f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		ImGuiStyle* style = &ImGui::GetStyle();
		style->FramePadding = ImVec2(4, 2);
		style->ItemSpacing = ImVec2(10, 2);
		style->IndentSpacing = 12;
		style->ScrollbarSize = 10;

		style->WindowRounding = 4;
		style->FrameRounding = 4;
		style->PopupRounding = 4;
		style->ScrollbarRounding = 6;
		style->GrabRounding = 4;
		style->TabRounding = 4;

		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style->WindowMenuButtonPosition = ImGuiDir_Right;
		style->DisplaySafeAreaPadding = ImVec2(4, 4);
	}
	static void ApplyTheme2()
	{
		static float rainbow;
		rainbow += 0.0010f; //0.0001f
		if (rainbow > 1.f)
			rainbow = 0.f;

		auto rgb = ImColor::HSV(rainbow, 1.f, 1.f, 0.8f);     // Rainbow Items
		auto rgb_a = ImColor::HSV(rainbow, 1.f, 1.f, 0.9f);   // Selected / Hovered Items
		auto rgb_b = ImColor::HSV(rainbow, 1.f, 1.f, 0.7f);   // Pressed Items


		float color_menu[4]{ 167 / 255.f, 0 / 255.f , 253 / 255.f, 1.0f };
		ImVec4* colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_FrameBg] = ImColor(37, 40, 47, 255);
		colors[ImGuiCol_CheckMark] = rgb;
		colors[ImGuiCol_Header] = rgb;
		colors[ImGuiCol_HeaderHovered] = rgb;
		colors[ImGuiCol_HeaderActive] = rgb;
		colors[ImGuiCol_SliderGrab] = rgb;
		colors[ImGuiCol_SliderGrabActive] = rgb;
		colors[ImGuiCol_ScrollbarGrab] = rgb;
		colors[ImGuiCol_ScrollbarGrabHovered] = rgb;
		colors[ImGuiCol_ScrollbarGrabActive] = rgb;
		//colors[ImGuiCol_ChildBg] = rgb;
		//colors[ImGuiCol_Button] = rgb;
		colors[ImGuiCol_Text] = rgb;
		colors[ImGuiCol_TextDisabled] = rgb;
		//colors[ImGuiCol_WindowBg] = rgb;
		ImGui::SetNextWindowSize({ 490,393 });
		ImGuiStyle* style = &ImGui::GetStyle();
		style->FramePadding = ImVec2(4, 2);
		style->ItemSpacing = ImVec2(10, 2);
		style->IndentSpacing = 12;
		style->ScrollbarSize = 10;

		style->WindowRounding = 4;
		style->FrameRounding = 4;
		style->PopupRounding = 4;
		style->ScrollbarRounding = 6;
		style->GrabRounding = 4;
		style->TabRounding = 4;

		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style->WindowMenuButtonPosition = ImGuiDir_Right;
		style->DisplaySafeAreaPadding = ImVec2(4, 4);
	}
};