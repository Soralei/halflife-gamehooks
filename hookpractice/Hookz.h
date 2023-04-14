#pragma once

/*
namespace Hookz
{
	const int JUMP_INSTRUCTION_SIZE = 5;

	bool DoHook(DWORD hookAddress, DWORD redirectAddress, int hookSize);
	bool DoUnhook(DWORD hookAddress);

	struct HookData {
		DWORD address{};
		BYTE restorationBytes[16]{};
		int size{};
	};

	static HookData hooks[128]{};
	bool IsHooked(DWORD address);
	bool AddHookData(HookData data);
	bool ClearHookData(DWORD address);
	HookData* GetHookData(DWORD address);
}
*/

class Hookz {
	private:
		BYTE	m_oldBytes[32]{};
		DWORD	m_src{};
		DWORD	m_target{};
		DWORD	m_relative_delta{};
		INT		m_size{};
		BOOL	m_isActive{};
	public:
		Hookz(DWORD src, DWORD target, INT size);
		void Enable();
		void Disable();
		~Hookz();

		bool IsActive();
};

class Nop {
	private:
		BYTE m_oldBytes[32]{};
		DWORD m_src{};
		INT m_size{};
		BOOL m_isActive{};
	public:
		Nop(DWORD src, INT size);
		void Enable();
		void Disable();
		~Nop();

		bool isActive();
};