
#include "pch.h"
#include "Hookz.h"

#include <iostream>

const int MIN_HOOK_SIZE = 5; // The jump instruction requires at least 5 bytes.

Hookz::Hookz(DWORD src, DWORD target, INT size){
	m_relative_delta = target - src - MIN_HOOK_SIZE; // The relative address to jump to.
	m_size = size;
	m_src = src;
}

Hookz::~Hookz(){
	if(m_isActive){
		Disable();
	}
}

bool Hookz::IsActive(){
	return m_isActive;
}

void Hookz::Enable(){
	if(m_isActive){
		std::cout << "-- Hookz Error -- Failed to enable the hook. The hook is already enabled." << '\n';
	}

	if(m_size < MIN_HOOK_SIZE){
		std::cout << "-- Hookz Error -- The hook size is too small. It needs to be at least 5 bytes, but was only " << m_size << "." << '\n';
		return;
	}

	// Set read & write access in preperation to do byte manipulation.
	DWORD oldProtect{};
	if(!VirtualProtect((LPVOID)m_src, m_size, PAGE_EXECUTE_READWRITE, &oldProtect)){
		std::cout << "-- Hookz Error -- Was unable to set read and write access at the hook source." << '\n';
		return;
	}

	// Do byte manipulation here.
	for(int i = 0; i < m_size; i++){
		m_oldBytes[i] = *(BYTE*)(m_src + i);	// Store the bytes for later unhooking.
		*(BYTE*)(m_src + i) = 0x90;				// NOP the target bytes.
	}

	*(BYTE*)m_src = 0xE9;
	*(DWORD*)(m_src + 1) = m_relative_delta;

	if(!VirtualProtect((LPVOID)m_src, m_size, oldProtect, &oldProtect)){
		std::cout << "Hookz warning. Failed to reset the protection flags." << '\n';
	}

	std::cout << "-- Hookz -- Hook successfully enabled." << '\n';
	m_isActive = true;
}

void Hookz::Disable(){
	if(!m_isActive){
		std::cout << "-- Hookz Error -- Failed to disable the hook. It's already disabled." << '\n';
		return;
	}

	bool hasOldBytes{};
	for(int i = 0; i < m_size; i++){
		if(m_oldBytes[i] != 0){
			hasOldBytes = true;
			break;
		}
	}

	if(!hasOldBytes){
		std::cout << "-- Hookz Error -- Failed to disable the hook. It doesn't have any stored bytes." << '\n';
		return;
	}

	DWORD oldProtect{};
	if(!VirtualProtect((LPVOID)m_src, m_size, PAGE_EXECUTE_READWRITE, &oldProtect)){
		std::cout << "-- Hookz Error -- Failed to disable the hook. Was unable to set read write access." << '\n';
		return;
	}

	for(int i = 0; i < m_size; i++){
		*(BYTE*)(m_src + i) = m_oldBytes[i];
		m_oldBytes[i] = 0;
	}

	if(!VirtualProtect((LPVOID)m_src, m_size, oldProtect, &oldProtect)){
		std::cout << "-- Hookz Warning -- Unable to reset the protection flags. Did something go wrong?" << '\n';
	}

	std::cout << "-- Hookz -- Hook successfully disabled." << '\n';
	m_isActive = false;
}

Nop::Nop(DWORD src, INT size){
	m_src = src;
	m_size = size;
}

void Nop::Enable(){
	if(m_isActive){
		std::cout << "-- NOP Error -- Failed to enable the nop, because it's already active." << '\n';
		return;
	}

	DWORD oldProtection{};
	if(!VirtualProtect((LPVOID)m_src, m_size, PAGE_EXECUTE_READWRITE, &oldProtection)){
		std::cout << "-- NOP Error -- Failed to set read write access at the source location." << '\n';
		return;
	}

	for(int i = 0; i < m_size; i++){
		m_oldBytes[i] = *(BYTE*)(m_src + i);
		*(BYTE*)(m_src + i) = 0x90;
	}

	if(!VirtualProtect((LPVOID)m_src, m_size, oldProtection, &oldProtection)){
		std::cout << "-- NOP Warning -- Failed to reset the access. Did something go wrong?" << '\n';
	}

	m_isActive = true;
}

void Nop::Disable(){
	if(!m_isActive){
		std::cout << "-- NOP Error -- Failed to disable the nop, because it's already inactive." << '\n';
		return;
	}

	DWORD oldProtection{};
	if(!VirtualProtect((LPVOID)m_src, m_size, PAGE_EXECUTE_READWRITE, &oldProtection)){
		std::cout << "-- NOP Error -- Failed to set read write access at the source location." << '\n';
		return;
	}

	for(int i = 0; i < m_size; i++){
		*(BYTE*)(m_src + i) = m_oldBytes[i];
		m_oldBytes[i] = 0;
	}

	if(!VirtualProtect((LPVOID)m_src, m_size, oldProtection, &oldProtection)){
		std::cout << "-- NOP Warning -- Failed to reset the access. Did something go wrong?" << '\n';
	}

	m_isActive = false;
}

Nop::~Nop(){
	if(m_isActive){
		Disable();
	}
}

bool Nop::isActive(){
	return m_isActive;
}