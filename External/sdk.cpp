#include "stdafx.h"
#include "sdk.h"
#include "xor.hpp"
#include "lazyimporter.h"
#include "memory.h"
#include <map>
#include "defs.h"
#include "globals.h"
#include "xorstr.hpp"
#pragma comment(lib, "user32.lib")
#define DEBASE(a) ((size_t)a - (size_t)(unsigned long long)GetModuleHandleA(NULL))

uintptr_t dwProcessBase;
uint64_t backup = 0, Online_Loot__GetItemQuantity = 0, stackFix = 0;
NTSTATUS(*NtContinue)(PCONTEXT threadContext, BOOLEAN raiseAlert) = nullptr;

DWORD64 resolveRelativeAddress(DWORD64 instr, DWORD offset, DWORD instrSize) {
	return instr == 0ui64 ? 0ui64 : (instr + instrSize + *(int*)(instr + offset));
}

bool compareByte(const char* pData, const char* bMask, const char* szMask) {
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	return (*szMask) == NULL;
}

DWORD64 findPattern(DWORD64 dwAddress, DWORD64 dwLen, const char* bMask, const char* szMask) {
	DWORD length = (DWORD)strlen(szMask);
	for (DWORD i = 0; i < dwLen - length; i++)
		if (compareByte((const char*)(dwAddress + i), bMask, szMask))
			return (DWORD64)(dwAddress + i);
	return 0ui64;
}

LONG WINAPI TopLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	if (pExceptionInfo && pExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION)
	{
		if (pExceptionInfo->ContextRecord->R11 == 0xDEEDBEEF89898989)
		{
			pExceptionInfo->ContextRecord->R11 = backup;

			if (pExceptionInfo->ContextRecord->Rip > Online_Loot__GetItemQuantity && pExceptionInfo->ContextRecord->Rip < (Online_Loot__GetItemQuantity + 0x1000))
			{
				pExceptionInfo->ContextRecord->Rip = stackFix;
				pExceptionInfo->ContextRecord->Rax = 1;
			}
			NtContinue(pExceptionInfo->ContextRecord, 0);
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}


namespace process
{
	HWND hwnd;

	BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)
	{
		DWORD dwPid = 0;
		GetWindowThreadProcessId(hWnd, &dwPid);
		if (dwPid == lParam)
		{
			hwnd = hWnd;
			return FALSE;
		}
		return TRUE;
	}

	HWND get_process_window()
	{
		if (hwnd)
			return hwnd;

		EnumWindows(EnumWindowCallBack, GetCurrentProcessId());

		if (hwnd == NULL)
			Exit();

		return hwnd;
	}
}

namespace g_data
{
	uintptr_t base;
	uintptr_t peb;
	HWND hWind;
	uintptr_t unlocker;
	uintptr_t ddl_loadasset;
	uintptr_t ddl_getrootstate;
	uintptr_t ddl_getdllbuffer;
	uintptr_t ddl_movetoname;
	uintptr_t ddl_setint;
	uintptr_t Dvar_FindVarByName;
	uintptr_t Dvar_SetBoolInternal;
	uintptr_t Dvar_SetInt_Internal;
	uintptr_t Dvar_SetFloat_Internal;
	uintptr_t Camo_Offset_Auto_Test;
	

	uintptr_t Clantag_auto;

	uintptr_t a_parse;
	uintptr_t ddl_setstring;
	uintptr_t ddl_movetopath;
	uintptr_t ddlgetInth;
	uintptr_t visible_base;
	QWORD current_visible_offset;
	QWORD cached_visible_base;
	QWORD last_visible_offset;
	uintptr_t cached_client_t = 0;

	bool MemCompare(const BYTE* bData, const BYTE* bMask, const char* szMask) {
		for (; *szMask; ++szMask, ++bData, ++bMask) {
			if (*szMask == 'x' && *bData != *bMask) {
				return false;
			}
		}
		return (*szMask == NULL);
	}
	uintptr_t PatternScanEx(uintptr_t start, uintptr_t size, const char* sig, const char* mask)
	{
		BYTE* data = new BYTE[size];
		SIZE_T bytesRead;

		(iat(memcpy).get()(data, (LPVOID)start, size));
		//ReadProcessMemory(hProcess, (LPVOID)start, data, size, &bytesRead);

		for (uintptr_t i = 0; i < size; i++)
		{
			if (MemCompare((const BYTE*)(data + i), (const BYTE*)sig, mask)) {
				return start + i;
			}
		}
		delete[] data;
		return NULL;
	}

	uintptr_t FindOffset(uintptr_t start, uintptr_t size, const char* sig, const char* mask, uintptr_t base_offset, uintptr_t pre_base_offset, uintptr_t rindex, bool addRip)
	{
		auto address = PatternScanEx(start, size, sig, mask) + rindex;
		if (!address)
			return 0;
		auto ret = pre_base_offset + *reinterpret_cast<int32_t*>(address + base_offset);

		if (addRip)
		{
			ret = ret + address;
			if (ret)
				return (ret - base);
		}

		return ret;
	}
	
	void init()
	{
		base = (uintptr_t)(iat(GetModuleHandleA).get()("cod.exe"));
	
		
		hWind = process::get_process_window();
		
		peb = __readgsqword(0x60);
	}
}

namespace sdk
{
	const DWORD nTickTime = 64;//64 ms
	bool bUpdateTick = false;
	std::map<DWORD, velocityInfo_t> velocityMap;

	void enable_uav()
	{

		

	
	}
	void no_recoil()
	{
		
	}
	void unlockall()
	{
		
	}
	uintptr_t _get_player(int i)
	{
		auto cl_info_base = get_client_info_base();

		if (is_bad_ptr(cl_info_base))return 0;
		
		
			auto base_address = *(uintptr_t*)(cl_info_base);
			if (is_bad_ptr(base_address))return 0;

				return sdk::get_client_info_base() + (i * player_info::size);

	}
	bool in_game()
	{
		auto gameMode = *(int*)(g_data::base + game_mode);
		return  gameMode > 1;
	}

	int get_game_mode()
	{
		return *(int*)(g_data::base + game_mode + 0x4);
	}

	int get_max_player_count()
	{
		return *(int*)(g_data::base + game_mode);
	}

	Vector3 _get_pos(uintptr_t address)
	{
		auto local_pos_ptr = *(uintptr_t*)((uintptr_t)address + player_info::position_ptr);

		if (local_pos_ptr)
		{
			return *(Vector3*)(local_pos_ptr + 0x40);
		}
		return Vector3{};
	}

	uint32_t _get_index(uintptr_t address)
	{
		auto cl_info_base = get_client_info_base();

		if (is_bad_ptr(cl_info_base))return 0;

		return ((uintptr_t)address - cl_info_base) / player_info::size;
	
		
	}

	int _team_id(uintptr_t address)    {

		return *(int*)((uintptr_t)address + player_info::team_id);
	}




	bool _is_visible(uintptr_t address)
	{
		if (IsValidPtr<uintptr_t>(&g_data::visible_base))
		{
			uint64_t VisibleList = *(uint64_t*)(g_data::visible_base + 0x108);
			if (is_bad_ptr( VisibleList))
				return false;

			uint64_t rdx = VisibleList + (_get_index(address) * 9 + 0x14E) * 8;
			if (is_bad_ptr(rdx))
				return false;

			DWORD VisibleFlags = (rdx + 0x10) ^ (*(DWORD*)(rdx + 0x14));
			if (is_bad_ptr(VisibleFlags))
				return false;

			DWORD v511 = VisibleFlags * (VisibleFlags + 2);
			if (!v511)
				return false;

			BYTE VisibleFlags1 = *(DWORD*)(rdx + 0x10) ^ v511 ^ BYTE1(v511);
			if (VisibleFlags1 == 3) {
				return true;
			}
		}
		return false;
	}


	uint64_t get_client_info()
	{
		
	
	}
	uint64_t get_client_info_base()
	{
		

	}
	uint64_t get_bone_ptr()
	{
		
	}
	uint16_t get_bone_index(uint32_t bone_index)
	{
		
	}

	player_t get_player(int i)
	{
		uint64_t decryptedPtr = get_client_info();

		if (is_valid_ptr (decryptedPtr))
		{
			uint64_t client_info = get_client_info_base();

			if (is_valid_ptr(client_info))
			{
				return player_t(client_info + (i * player_info::size));
			}
		}
		return player_t(NULL);
	}

	//player_t player_t
	
	//player_t get_local_player()
	//{
	//	auto addr = sdk::get_client_info_base() + (get_local_index() * player_info::size);
	//	if (is_bad_ptr(addr)) return 0;
	//	return addr;


	//}

	player_t get_local_player()
	{
		uint64_t decryptedPtr = get_client_info();

		if (is_bad_ptr(decryptedPtr))return player_t(NULL);

			auto local_index = *(uintptr_t*)(decryptedPtr + player_info::local_index);
			if (is_bad_ptr(local_index))return player_t(NULL);
			auto index = *(int*)(local_index + player_info::local_index_pos);
			return get_player(index);
		
		
	}

	/*name_t* get_name_ptr(int i)
	{
		uint64_t bgs = *(uint64_t*)(g_data::base + client::name_array);

		if (bgs)
		{
			name_t* pClientInfo = (name_t*)(bgs + client::name_array_padding + ((i + i * 8) << 4));

			if (is_bad_ptr(pClientInfo))return 0;
			else
			return pClientInfo;
			
		}
		return 0;
	}*/

	refdef_t* get_refdef()
	{
		uint32_t crypt_0 = *(uint32_t*)(g_data::base + view_port::refdef_ptr);
		uint32_t crypt_1 = *(uint32_t*)(g_data::base + view_port::refdef_ptr + 0x4);
		uint32_t crypt_2 = *(uint32_t*)(g_data::base + view_port::refdef_ptr + 0x8);
		// lower 32 bits
		uint32_t entry_1 = (uint32_t)(g_data::base + view_port::refdef_ptr);
		uint32_t entry_2 = (uint32_t)(g_data::base + view_port::refdef_ptr + 0x4);
		// decryption
		uint32_t _low = entry_1 ^ crypt_2;
		uint32_t _high = entry_2 ^ crypt_2;
		uint32_t low_bit = crypt_0 ^ _low * (_low + 2);
		uint32_t high_bit = crypt_1 ^ _high * (_high + 2);
		auto ret = (refdef_t*)(((QWORD)high_bit << 32) + low_bit);
		if (is_bad_ptr(ret)) return 0;
		else
			return ret;
	}

	Vector3 get_camera_pos()
	{
		Vector3 pos = Vector3{};

		auto camera_ptr = *(uint64_t*)(g_data::base + view_port::camera_ptr);

		if (is_bad_ptr(camera_ptr))return pos;
		
		
		pos = *(Vector3*)(camera_ptr + view_port::camera_pos);
		if (pos.IsZero())return {};
		else
		return pos;
	}

	/*std::string get_player_name(int i)
	{
		uint64_t bgs = *(uint64_t*)(g_data::base + client::name_array);

		if (is_bad_ptr(bgs))return NULL;


		if (bgs)
		{
			name_t* clientInfo_ptr = (name_t*)(bgs + client::name_array_padding + (i * 0xD0));
			if (is_bad_ptr(clientInfo_ptr))return NULL;

			int length = strlen(clientInfo_ptr->name);
			for (int j = 0; j < length; ++j)
			{
				char ch = clientInfo_ptr->name[j];
				bool is_english = ch >= 0 && ch <= 127;
				if (!is_english)
					return xorstr_("Player");
			}
			return clientInfo_ptr->name;
		}
		return xorstr_("Player");
	}*/

	
    //bool bones_to_screen(Vector3* BonePosArray, Vector2* ScreenPosArray, const long Count)
    //{
    //    for (long i = 0; i < Count; ++i)
    //    {
    //        if (!world(BonePosArray[i], &ScreenPosArray[i]))
    //            return false;
    //    }
    //    return true;
    //}



	bool get_bone_by_player_index(int i, int bone_id, Vector3* Out_bone_pos)
	{
		uint64_t decrypted_ptr = get_bone_ptr();

		if (is_bad_ptr(decrypted_ptr))return false;
		
			unsigned short index = get_bone_index(i);
			if (index != 0)
			{
				uint64_t bone_ptr = *(uint64_t*)(decrypted_ptr + (index * bones::size) + 0xD8);

				if (is_bad_ptr(bone_ptr))return false;
				
					Vector3 bone_pos = *(Vector3*)(bone_ptr + (bone_id * 0x20) + 0x10);

					if (bone_pos.IsZero())return false;

					uint64_t client_info = get_client_info();

					if (is_bad_ptr(client_info))return false;

					
					
						Vector3 BasePos = *(Vector3*)(client_info + bones::bone_base_pos);

						if (BasePos.IsZero())return false;

						bone_pos.x += BasePos.x;
						bone_pos.y += BasePos.y;
						bone_pos.z += BasePos.z;

						*Out_bone_pos = bone_pos;
						return true;
					
				
			}
		
		return false;
	}
	
	int get_player_health(int i)
	{
		uint64_t bgs = *(uint64_t*)(g_data::base + client::name_array);

		if (bgs)
		{
			name_t* pClientInfo = (name_t*)(bgs + client::name_array_padding  +(i * 0xD0));

			if (pClientInfo)
			{
				return pClientInfo->get_health();
			}
		}
		return 0;
	}

	std::string get_player_name(int entityNum)
	{
		uint64_t bgs = *(uint64_t*)(g_data::base + client::name_array);
		if (is_bad_ptr(bgs)) return "";

		if (bgs)
		{
			name_t* clientInfo_ptr = (name_t*)(bgs + client::name_array_padding + (entityNum * 0xD0));
			if (is_bad_ptr(clientInfo_ptr)) return "";

			int length = strlen(clientInfo_ptr->name);
			for (int j = 0; j < length; ++j)
			{
				char ch = clientInfo_ptr->name[j];
				bool is_english = ch >= 0 && ch <= 127;
				if (!is_english)
					return xorstr_("Player");
			}
			return clientInfo_ptr->name;
		}
		return xorstr_("Player");
	}

	void start_tick()
	{
		static DWORD lastTick = 0;
		DWORD t = GetTickCount();
		bUpdateTick = lastTick < t;

		if (bUpdateTick)
			lastTick = t + nTickTime;
	}

	void update_vel_map(int index, Vector3 vPos)
	{
		if (!bUpdateTick)
			return;

		velocityMap[index].delta = vPos - velocityMap[index].lastPos;
		velocityMap[index].lastPos = vPos;
	}

	void clear_map()
	{
		if (!velocityMap.empty()) { velocityMap.clear(); }
	}

	Vector3 get_speed(int index)
	{
		return velocityMap[index].delta;
	}

	Vector3 get_prediction(int index, Vector3 source, Vector3 destination)
	{
		auto local_velocity = get_speed(local_index());
		auto target_velocity = get_speed(index);

		const auto distance = source.distance_to(destination);
		const auto travel_time = distance / globals::bullet_speed;
		auto pred_destination = destination + (target_velocity - local_velocity) * travel_time;
		/*position.x += travel_time * final_speed.x;
		position.y += travel_time * final_speed.y;
		position.z += 0.5 * globals::bullet_gravity * travel_time * travel_time;
		return position;*/

		pred_destination.z += 0.5f * std::fabsf(globals::bullet_gravity) * travel_time;

		return pred_destination;
	}


	Result MidnightSolver(float a, float b, float c)
	{
		Result res;

		double subsquare = b * b - 4 * a * c;

		if (subsquare < 0)
		{
			res.hasResult = false;
			return res;
		}
		else
		{
			res.hasResult = true,
			res.a = (float)((-b + sqrt(subsquare)) / (2 * a));
			res.b = (float)((-b - sqrt(subsquare)) / (2 * a));
		}
		return res;
	}

	Vector3 prediction_solver(Vector3 local_pos, Vector3 position, int index, float bullet_speed)
	{
		Vector3 aimPosition = Vector3().Zero();
		auto target_speed = get_speed(index);

		local_pos -= position; 

		float a = (target_speed.x * target_speed.x) + (target_speed.y * target_speed.y) + (target_speed.z * target_speed.z) - ((bullet_speed * bullet_speed) * 100);
		float b = (-2 * local_pos.x * target_speed.x) + (-2 * local_pos.y * target_speed.y) + (-2 * local_pos.z * target_speed.z);
		float c = (local_pos.x * local_pos.x) + (local_pos.y * local_pos.y) + (local_pos.z * local_pos.z);

		local_pos += position; 

		Result r = MidnightSolver(a, b, c);

		if (r.a >= 0 && !(r.b >= 0 && r.b < r.a))
		{
			aimPosition = position + target_speed * r.a;
		}
		else if (r.b >= 0)
		{
			aimPosition = position + target_speed * r.b;
		}

		return aimPosition;
	
	}
	int decrypt_visible_flag(int i, QWORD valid_list)
	{
		auto ptr = valid_list + ((static_cast<unsigned long long>(i) * 9 + 0x152) * 8) + 0x10;
		DWORD dw1 = (*(DWORD*)(ptr + 4) ^ (DWORD)ptr);
		DWORD dw2 = ((dw1 + 2) * dw1);
		BYTE dec_visible_flag = *(BYTE*)(ptr) ^ BYTE1(dw2) ^ (BYTE)dw2;

		return (int)dec_visible_flag;
	}

	uint64_t get_visible_base()
	{
		
		for (int32_t j{}; j <= 0x1770; ++j)
		{
			
			uint64_t vis_base_ptr = *(uint64_t*)(g_data::base + bones::distribute) + (j * 0x190);
			uint64_t cmp_function = *(uint64_t*)(vis_base_ptr + 0x38);

			if (!cmp_function)
				continue;

			//LOGS_ADDR(cmp_function);

			uint64_t about_visible = g_data::base + bones::visible;

			if (cmp_function == about_visible)
			{

				g_data::current_visible_offset = vis_base_ptr;
				return g_data::current_visible_offset;
			}

		}
		return NULL;
	}
	
	bool is_visible(int entityNum) {

		if (!g_data::last_visible_offset)
			return false;

		uint64_t VisibleList = *(uint64_t*)(g_data::last_visible_offset + 0x80);
		if (!VisibleList)
			return false;
		uint64_t v421 = VisibleList + (entityNum * 9 + 0x152) * 8;
		if (!v421)
			return false;
		DWORD VisibleFlags = (v421 + 0x10) ^ *(DWORD*)(v421 + 0x14);
		if (!VisibleFlags)
			return false;
		DWORD v1630 = VisibleFlags * (VisibleFlags + 2);
		if (!v1630)
			return false;
		BYTE VisibleFlags1 = *(DWORD*)(v421 + 0x10) ^ v1630 ^ BYTE1(v1630);
		if (VisibleFlags1 == 3) {
			return true;
		}
		return false;
	}

	void update_last_visible()
	{
		g_data::last_visible_offset = g_data::current_visible_offset;
	}

	// player class methods
	bool player_t::is_valid() {
		if (is_bad_ptr(address))return 0;

		return *(bool*)((uintptr_t)address + player_info::valid);
	}

	bool player_t::is_dead() {
		if (is_bad_ptr(address))return 0;

		auto dead1 = *(bool*)((uintptr_t)address + player_info::dead_1);
		auto dead2 = *(bool*)((uintptr_t)address + player_info::dead_2);
		return !(!dead1 && !dead2 && is_valid());
	}

	int player_t::team_id() {

		if (is_bad_ptr(address))return 0;
		return *(int*)((uintptr_t)address + player_info::team_id);
	}

	int player_t::get_stance() {
		
		if (is_bad_ptr(address))return 4;
		auto ret = *(int*)((uintptr_t)address + player_info::stance);
	

		return ret;
	}


	float player_t::get_rotation()
	{
		if (is_bad_ptr(address))return 0;
		auto local_pos_ptr = *(uintptr_t*)((uintptr_t)address + player_info::position_ptr);

		if (is_bad_ptr(local_pos_ptr))return 0;

		auto rotation = *(float*)(local_pos_ptr + 0x58);

		if (rotation < 0)
			rotation = 360.0f - (rotation * -1);

		rotation += 90.0f;

		if (rotation >= 360.0f)
			rotation = rotation - 360.0f;

		return rotation;
	}

	Vector3 player_t::get_pos() 
	{
		if (is_bad_ptr(address))return {};
		auto local_pos_ptr = *(uintptr_t*)((uintptr_t)address + player_info::position_ptr);

		if (is_bad_ptr(local_pos_ptr))return{};
		else
			return *(Vector3*)(local_pos_ptr + 0x48);
		return Vector3{}; 


	}

	uint32_t player_t::get_index()
	{
		if (is_bad_ptr(this->address))return 0;

		auto cl_info_base = get_client_info_base();
		if (is_bad_ptr(cl_info_base))return 0;
		
		
	return ((uintptr_t)this->address - cl_info_base) / player_info::size;
		
	
	}

	bool player_t::is_visible()
	{
		if (is_bad_ptr(g_data::visible_base))return false;

		if (is_bad_ptr(this->address))return false;
		
			uint64_t VisibleList =*(uint64_t*)(g_data::visible_base + 0x108);
			if (is_bad_ptr(VisibleList))
				return false;

			uint64_t rdx = VisibleList + (player_t::get_index() * 9 + 0x14E) * 8;
			if (is_bad_ptr(rdx))
				return false;

			DWORD VisibleFlags = (rdx + 0x10) ^ *(DWORD*)(rdx + 0x14);
			if (!VisibleFlags)
				return false;

			DWORD v511 = VisibleFlags * (VisibleFlags + 2);
			if (!v511)
				return false;

			BYTE VisibleFlags1 = *(DWORD*)(rdx + 0x10) ^ v511 ^ BYTE1(v511);
			if (VisibleFlags1 == 3) {
				return true;
			}
		
		return false;
	}
	



}

