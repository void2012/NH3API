//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// Copyright (C) devoider17 (aka void_17), 2024-2025
// You may use this file freely as long as you list the author and the license
// In the source code files of your project
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "core/CAutoArray.hpp" // CAutoArray<T>
#include "core/nh3api_std/exe_deque.hpp" // exe_deque<T>
#include "core/multiplayer.hpp" // CNetMsg, CNetMsgHandler

// avoid including dplay.h
// which is also not always available on modern Windows SDK
// shipped with Visual Studio.
// MinGW still provides it, though.
#pragma pack(push, 4)
// size = 0x28 = 40, align = 4
struct __DPCAPS
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    uint32_t dwSize;
    // offset: +0x4 = +4,  size = 0x4 = 4
    uint32_t dwFlags;
    // offset: +0x8 = +8,  size = 0x4 = 4
    uint32_t dwMaxBufferSize;
    // offset: +0xC = +12,  size = 0x4 = 4
    uint32_t dwMaxQueueSize;
    // offset: +0x10 = +16,  size = 0x4 = 4
    uint32_t dwMaxPlayers;
    // offset: +0x14 = +20,  size = 0x4 = 4
    uint32_t dwHundredBaud;
    // offset: +0x18 = +24,  size = 0x4 = 4
    uint32_t dwLatency;
    // offset: +0x1C = +28,  size = 0x4 = 4
    uint32_t dwMaxLocalPlayers;
    // offset: +0x20 = +32,  size = 0x4 = 4
    uint32_t dwHeaderLength;
    // offset: +0x24 = +36,  size = 0x4 = 4
    uint32_t dwTimeout;

};
#pragma pack(pop)

#pragma pack(push, 4)
// size = 0x8 = 8, align = 4
struct CDPlayMsg
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    void* pData;

    // offset: +0x4 = +4,  size = 0x4 = 4
    uint32_t dataSize;
};

// size = 0x104 = 260, align = 4
struct CDPlayPlayer
{
    // offset: +0x0 = +0,  size = 0x100 = 256
    std::array<char, 256> m_sName;

    // offset: +0x100 = +256,  size = 0x4 = 4
    uint32_t m_dpid;
};

// size = 0x104 = 260, align = 4, baseclass: CDPlayPlayer
struct CDPlayGroup : CDPlayPlayer
{
};

// size = 0x98 = 152, align = 4
struct CDPlayConnection
{
    // offset: +0x0 = +0,  size = 0x10 = 16
    GUID guidSP;

    // offset: +0x10 = +16,  size = 0x4 = 4
    void* pConnection;

    // offset: +0x14 = +20,  size = 0x80 = 128
    std::array<char, 128> sName;

    // offset: +0x94 = +148,  size = 0x4 = 4
    uint32_t size;
};

// size = 0x10C = 268, align = 4
struct CDPlaySession
{
    // offset: +0x0 = +0,  size = 0x4 = 4
    uint32_t dwFlags;

    // offset: +0x4 = +4,  size = 0x10 = 16
    GUID guidInstance;

    // offset: +0x14 = +20,  size = 0x10 = 16
    GUID guidApp;

    // offset: +0x24 = +36,  size = 0x4 = 4
    uint32_t maxPlayers;

    // offset: +0x28 = +40,  size = 0x4 = 4
    uint32_t playerCount;

    // offset: +0x2C = +44,  size = 0x80 = 128
    std::array<char, 128> sessionName;

    // offset: +0xAC = +172,  size = 0x50 = 80
    std::array<char, 80> password;

    // offset: +0xFC = +252,  size = 0x4 = 4
    uint32_t dwUser1;
    // offset: +0x100 = +256,  size = 0x4 = 4
    uint32_t dwUser2;
    // offset: +0x104 = +260,  size = 0x4 = 4
    uint32_t dwUser3;
    // offset: +0x108 = +264,  size = 0x4 = 4
    uint32_t dwUser4;

};

// size = 0x18 = 24, align = 4
struct CDPlayAddressElement
{
    // offset: +0x0 = +0,  size = 0x10 = 16
    GUID m_guid;

    // offset: +0x10 = +16,  size = 0x4 = 4
    void* m_pData;

    // offset: +0x14 = +20,  size = 0x4 = 4
    uint32_t m_size;

};
#pragma pack(pop) // align 4

#pragma pack(push, 4)
// size = 0x58 = 88, align = 4
class CDPlay
{
    public:
        struct vftable_t
        {
          void(__thiscall* scalar_deleting_destructor)(CDPlay*, uint8_t);
          bool(__thiscall* Init)(CDPlay*);
          bool(__thiscall* InitConnection)(CDPlay*, CDPlayConnection*);
          bool(__thiscall* HostSession)(CDPlay*, char*, uint32_t, uint32_t,
                                        char*);
          bool(__thiscall* JoinSession)(CDPlay*, GUID*, char*);
          bool(__thiscall* StartSession)(CDPlay*, uint32_t);
          bool(__thiscall* CloseSession)(CDPlay*);
          uint32_t(__thiscall* CreatePlayer)(CDPlay*, char*, void*, uint32_t,
                                             void*);
          bool(__thiscall* DestroyPlayer)(CDPlay*, uint32_t);
          uint32_t(__thiscall* CreateGroup)(CDPlay*, char*, void*, uint32_t,
                                            bool);
          bool(__thiscall* DestroyGroup)(CDPlay*, uint32_t);
          bool(__thiscall* DeleteGroupFromGroup)(CDPlay*, uint32_t, uint32_t);
          bool(__thiscall* SetGroupName)(CDPlay*, uint32_t, char*, char*,
                                         uint32_t);
          bool(__thiscall* SetGroupData)(CDPlay*, uint32_t, void*, uint32_t,
                                         uint32_t);
          bool(__thiscall* SetPlayerName)(CDPlay*, uint32_t, char*, char*,
                                          uint32_t);
          bool(__thiscall* SetPlayerData)(CDPlay*, uint32_t, void*, uint32_t,
                                          uint32_t);
          void*(__thiscall* GetGroupData)(CDPlay*, uint32_t, uint32_t*,
                                          uint32_t);
          bool(__thiscall* GetGroupName)(CDPlay*, uint32_t, char*, int, char*,
                                         int);
          void*(__thiscall* GetPlayerData)(CDPlay*, uint32_t, uint32_t*,
                                           uint32_t);
          bool(__thiscall* GetPlayerName)(CDPlay*, uint32_t, char*, int, char*,
                                          int);
          uint32_t(__thiscall* CreateGroupInGroup)(CDPlay*, uint32_t, char*,
                                                   void*, uint32_t, bool);
          bool(__thiscall* AddPlayerToGroup)(CDPlay*, uint32_t, uint32_t);
          bool(__thiscall* DeletePlayerFromGroup)(CDPlay*, uint32_t, uint32_t);
          bool(__thiscall* UpdateSessionDesc)(CDPlay*, void*);
          void*(__thiscall* GetCurrSession)(CDPlay*);
          bool(__thiscall* EnumConnections)(CDPlay*,
                                            CAutoArray<CDPlayConnection>*);
          bool(__thiscall* EnumSessions)(CDPlay*, CAutoArray<CDPlaySession>*,
                                         uint32_t, uint32_t);
          bool(__thiscall* EnumGroups)(CDPlay*, /*CAutoArray<CDPlayGroup>*/void*,
                                       GUID*, uint32_t);
          bool(__thiscall* EnumPlayers)(CDPlay*, CAutoArray<CDPlayPlayer>*,
                                        GUID*, uint32_t);
          bool(__thiscall* EnumGroupPlayers)(CDPlay*, CAutoArray<CDPlayPlayer>*,
                                             uint32_t, GUID*, uint32_t);
          void(__thiscall* SetGuid)(CDPlay*, GUID);
          GUID* (__thiscall* GetGuid)(CDPlay*);
          bool(__thiscall* Send)(CDPlay*, void*, uint32_t, uint32_t, uint32_t,
                                 bool);
          bool(__thiscall* SendChat)(CDPlay*, char*, uint32_t, uint32_t);
          bool(__thiscall* Receive)(CDPlay*, uint32_t*, uint32_t*, CDPlayMsg*,
                                    uint32_t);
          void(__thiscall* GetErrorDesc)(CDPlay*, int, char*);
          bool(__thiscall* IsHost)(CDPlay*);
          bool(__thiscall* FlushReceiveQueue)(CDPlay*);
          void* (__thiscall* GetPlayerAddress)(CDPlay*, uint32_t, uint32_t*);
          bool(__thiscall* GetCaps)(CDPlay*, __DPCAPS*, bool);
          bool(__thiscall* GetSendQueueSize)(CDPlay*, uint32_t, uint32_t,
                                             uint32_t*, uint32_t*);
          bool(__thiscall* GetReceiveQueueSize)(CDPlay*, uint32_t, uint32_t,
                                                uint32_t*, uint32_t*);
          bool(__thiscall* ReceiveMsg)(CDPlay*, uint32_t, uint32_t, CDPlayMsg*);
          bool(__thiscall* ReceiveSystemMsg)(CDPlay*, uint32_t, CDPlayMsg*);
          bool(__thiscall* SysMsgAddGroupToGroup)(CDPlay*, void*, uint32_t);
          bool(__thiscall* SysMsgAddPlayerToGroup)(CDPlay*, void*, uint32_t);
          bool(__thiscall* SysMsgChat)(CDPlay*, void*, uint32_t);
          bool(__thiscall* SysMsgDeleteGroupFromGroup)(CDPlay*, void*,
                                                       uint32_t);
          bool(__thiscall* SysMsgDeletePlayerFromGroup)(CDPlay*, void*,
                                                        uint32_t);
          bool(__thiscall* SysMsgSecureMessage)(CDPlay*, void*, uint32_t);
          bool(__thiscall* SysMsgSessionLost)(CDPlay*, void*, uint32_t);
          bool(__thiscall* SysMsgSetPlayerOrGroupData)(CDPlay*, void*,
                                                       uint32_t);
          bool(__thiscall* SysMsgSetPlayerOrGroupName)(CDPlay*, void*,
                                                       uint32_t);
          bool(__thiscall* SysMsgSetSessionDesc)(CDPlay*, void*, uint32_t);
          bool(__thiscall* SysMsgStartSession)(CDPlay*, void*, uint32_t);
          bool(__thiscall* SysMsgHost)(CDPlay*, void*, uint32_t);
          bool(__thiscall* SysMsgCreatePlayerOrGroup)(CDPlay*, void*, uint32_t);
          bool(__thiscall* SysMsgDestroyPlayerOrGroup)(CDPlay*, void*,
                                                       uint32_t);
          bool(__thiscall* AddGroupEnum)(CDPlay*, uint32_t, const void*,
                                         uint32_t);
          bool(__thiscall* AddPlayerEnum)(CDPlay*, uint32_t, const void*,
                                          uint32_t);
          bool(__thiscall* AddSessionEnum)(CDPlay*, const void*, uint32_t);
          bool(__thiscall* AddConnectionEnum)(CDPlay*, const GUID*, void*,
                                              uint32_t, const void*, uint32_t);
        };
    public:

        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual bool __thiscall Init()
        { return get_vftable(this)->Init(this); }

        // vftable shift: +8
        virtual bool __thiscall InitConnection(CDPlayConnection* a1)
        { return get_vftable(this)->InitConnection(this, a1); }

        // vftable shift: +12
        virtual bool __thiscall HostSession(char* a1, uint32_t a2, uint32_t a3, char* a4)
        { return get_vftable(this)->HostSession(this, a1, a2, a3, a4); }

        // vftable shift: +16
        virtual bool __thiscall JoinSession(GUID* a1, char* a2)
        { return get_vftable(this)->JoinSession(this, a1, a2); }

        // vftable shift: +20
        virtual bool __thiscall StartSession(uint32_t a1)
        { return get_vftable(this)->StartSession(this, a1); }

        // vftable shift: +24
        virtual bool __thiscall CloseSession()
        { return get_vftable(this)->CloseSession(this); }

        // vftable shift: +28
        virtual uint32_t __thiscall CreatePlayer(char* a1, void* a2, uint32_t a3, void* a4)
        { return get_vftable(this)->CreatePlayer(this, a1, a2, a3, a4); }

        // vftable shift: +32
        virtual bool __thiscall DestroyPlayer(uint32_t a1)
        { return get_vftable(this)->DestroyPlayer(this, a1); }

        // vftable shift: +36
        virtual uint32_t __thiscall CreateGroup(char* a1, void* a2, uint32_t a3, bool a4)
        { return get_vftable(this)->CreateGroup(this, a1, a2, a3, a4); }

        // vftable shift: +40
        virtual bool __thiscall DestroyGroup(uint32_t a1)
        { return get_vftable(this)->DestroyGroup(this, a1); }

        // vftable shift: +44
        virtual bool __thiscall DeleteGroupFromGroup(uint32_t a1, uint32_t a2)
        { return get_vftable(this)->DeleteGroupFromGroup(this, a1, a2); }

        // vftable shift: +48
        virtual bool __thiscall SetGroupName(uint32_t a1, char* a2, char* a3, uint32_t a4)
        { return get_vftable(this)->SetGroupName(this, a1, a2, a3, a4); }

        // vftable shift: +52
        virtual bool __thiscall SetGroupData(uint32_t a1, void* a2, uint32_t a3, uint32_t a4)
        { return get_vftable(this)->SetGroupData(this, a1, a2, a3, a4); }

        // vftable shift: +56
        virtual bool __thiscall SetPlayerName(uint32_t a1, char* a2, char* a3, uint32_t a4)
        { return get_vftable(this)->SetPlayerName(this, a1, a2, a3, a4); }

        // vftable shift: +60
        virtual bool __thiscall SetPlayerData(uint32_t a1, void* a2, uint32_t a3, uint32_t a4)
        { return get_vftable(this)->SetPlayerData(this, a1, a2, a3, a4); }

        // vftable shift: +64
        virtual void* __thiscall GetGroupData(uint32_t a1, uint32_t* a2, uint32_t a3)
        { return get_vftable(this)->GetGroupData(this, a1, a2, a3); }

        // vftable shift: +68
        virtual bool __thiscall GetGroupName(uint32_t a1, char* a2, int32_t a3, char* a4, int32_t a5)
        { return get_vftable(this)->GetGroupName(this, a1, a2, a3, a4, a5); }

        // vftable shift: +72
        virtual void* __thiscall GetPlayerData(uint32_t a1, uint32_t* a2, uint32_t a3)
        { return get_vftable(this)->GetPlayerData(this, a1, a2, a3); }

        // vftable shift: +76
        virtual bool __thiscall GetPlayerName(uint32_t a1, char* a2, int32_t a3, char* a4, int32_t a5)
        { return get_vftable(this)->GetPlayerName(this, a1, a2, a3, a4, a5); }

        // vftable shift: +80
        virtual uint32_t __thiscall CreateGroupInGroup(uint32_t a1, char* a2, void* a3, uint32_t a4, bool a5)
        { return get_vftable(this)->CreateGroupInGroup(this, a1, a2, a3, a4, a5); }

        // vftable shift: +84
        virtual bool __thiscall AddPlayerToGroup(uint32_t a1, uint32_t a2)
        { return get_vftable(this)->AddPlayerToGroup(this, a1, a2); }

        // vftable shift: +88
        virtual bool __thiscall DeletePlayerFromGroup(uint32_t a1, uint32_t a2)
        { return get_vftable(this)->DeletePlayerFromGroup(this, a1, a2); }

        // vftable shift: +92
        virtual bool __thiscall UpdateSessionDesc(void* a1)
        { return get_vftable(this)->UpdateSessionDesc(this, a1); }

        // vftable shift: +96
        virtual void* __thiscall GetCurrSession()
        { return get_vftable(this)->GetCurrSession(this); }

        // vftable shift: +100
        virtual bool __thiscall EnumConnections(CAutoArray<CDPlayConnection>* a1)
        { return get_vftable(this)->EnumConnections(this, a1); }

        // vftable shift: +104
        virtual bool __thiscall EnumSessions(CAutoArray<CDPlaySession>* a1, uint32_t a2, uint32_t a3)
        { return get_vftable(this)->EnumSessions(this, a1, a2, a3); }

        // vftable shift: +108
        virtual bool __thiscall EnumGroups(/*CAutoArray<CDPlayGroup>*/ void* a1, GUID* a2, uint32_t a3)
        { return get_vftable(this)->EnumGroups(this, a1, a2, a3); }

        // vftable shift: +112
        virtual bool __thiscall EnumPlayers(CAutoArray<CDPlayPlayer>* a1, GUID* a2, uint32_t a3)
        { return get_vftable(this)->EnumPlayers(this, a1, a2, a3); }

        // vftable shift: +116
        virtual bool __thiscall EnumGroupPlayers(CAutoArray<CDPlayPlayer>* a1, uint32_t a2, GUID* a3, uint32_t a4)
        { return get_vftable(this)->EnumGroupPlayers(this, a1, a2, a3, a4); }

        // vftable shift: +120
        virtual void __thiscall SetGuid(GUID a1)
        { get_vftable(this)->SetGuid(this, a1); }

        // vftable shift: +124
        virtual GUID* __thiscall GetGuid()
        { return get_vftable(this)->GetGuid(this); }

        // vftable shift: +128
        virtual bool __thiscall Send(void* a1, uint32_t a2, uint32_t a3, uint32_t a4, bool a5)
        { return get_vftable(this)->Send(this, a1, a2, a3, a4, a5); }

        // vftable shift: +132
        virtual bool __thiscall SendChat(char* a1, uint32_t a2, uint32_t a3)
        { return get_vftable(this)->SendChat(this, a1, a2, a3); }

        // vftable shift: +136
        virtual bool __thiscall Receive(uint32_t* a1, uint32_t* a2, CDPlayMsg* a3, uint32_t a4)
        { return get_vftable(this)->Receive(this, a1, a2, a3, a4); }

        // vftable shift: +140
        virtual void __thiscall GetErrorDesc(int32_t a1, char* a2)
        { get_vftable(this)->GetErrorDesc(this, a1, a2); }

        // vftable shift: +144
        virtual bool __thiscall IsHost()
        { return get_vftable(this)->IsHost(this); }

        // vftable shift: +148
        virtual bool __thiscall FlushReceiveQueue()
        { return get_vftable(this)->FlushReceiveQueue(this); }

        // vftable shift: +152
        virtual void* __thiscall GetPlayerAddress(uint32_t a1, uint32_t* a2)
        { return get_vftable(this)->GetPlayerAddress(this, a1, a2); }

        // vftable shift: +156
        virtual bool __thiscall GetCaps(__DPCAPS* a1, bool a2)
        { return get_vftable(this)->GetCaps(this, a1, a2); }

        // vftable shift: +160
        virtual bool __thiscall GetSendQueueSize(uint32_t a1, uint32_t a2, uint32_t* a3, uint32_t* a4)
        { return get_vftable(this)->GetSendQueueSize(this, a1, a2, a3, a4); }

        // vftable shift: +164
        virtual bool __thiscall GetReceiveQueueSize(uint32_t a1, uint32_t a2, uint32_t* a3, uint32_t* a4)
        { return get_vftable(this)->GetReceiveQueueSize(this, a1, a2, a3, a4); }

        // vftable shift: +168
        virtual bool __thiscall ReceiveMsg(uint32_t a1, uint32_t a2, CDPlayMsg* a3)
        { return get_vftable(this)->ReceiveMsg(this, a1, a2, a3); }

        // vftable shift: +172
        virtual bool __thiscall ReceiveSystemMsg(uint32_t a1, CDPlayMsg* a2)
        { return get_vftable(this)->ReceiveSystemMsg(this, a1, a2); }

        // vftable shift: +176
        virtual bool __thiscall SysMsgAddGroupToGroup(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgAddGroupToGroup(this, a1, a2); }

        // vftable shift: +180
        virtual bool __thiscall SysMsgAddPlayerToGroup(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgAddPlayerToGroup(this, a1, a2); }

        // vftable shift: +184
        virtual bool __thiscall SysMsgChat(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgChat(this, a1, a2); }

        // vftable shift: +188
        virtual bool __thiscall SysMsgDeleteGroupFromGroup(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgDeleteGroupFromGroup(this, a1, a2); }

        // vftable shift: +192
        virtual bool __thiscall SysMsgDeletePlayerFromGroup(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgDeletePlayerFromGroup(this, a1, a2); }

        // vftable shift: +196
        virtual bool __thiscall SysMsgSecureMessage(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgSecureMessage(this, a1, a2); }

        // vftable shift: +200
        virtual bool __thiscall SysMsgSessionLost(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgSessionLost(this, a1, a2); }

        // vftable shift: +204
        virtual bool __thiscall SysMsgSetPlayerOrGroupData(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgSetPlayerOrGroupData(this, a1, a2); }

        // vftable shift: +208
        virtual bool __thiscall SysMsgSetPlayerOrGroupName(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgSetPlayerOrGroupName(this, a1, a2); }

        // vftable shift: +212
        virtual bool __thiscall SysMsgSetSessionDesc(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgSetSessionDesc(this, a1, a2); }

        // vftable shift: +216
        virtual bool __thiscall SysMsgStartSession(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgStartSession(this, a1, a2); }

        // vftable shift: +220
        virtual bool __thiscall SysMsgHost(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgHost(this, a1, a2); }

        // vftable shift: +224
        virtual bool __thiscall SysMsgCreatePlayerOrGroup(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgCreatePlayerOrGroup(this, a1, a2); }

        // vftable shift: +228
        virtual bool __thiscall SysMsgDestroyPlayerOrGroup(void* a1, uint32_t a2)
        { return get_vftable(this)->SysMsgDestroyPlayerOrGroup(this, a1, a2); }

        // vftable shift: +232
        virtual bool __thiscall AddGroupEnum(uint32_t a1, const void* a2, uint32_t a3)
        { return get_vftable(this)->AddGroupEnum(this, a1, a2, a3); }

        // vftable shift: +236
        virtual bool __thiscall AddPlayerEnum(uint32_t a1, const void* a2, uint32_t a3)
        { return get_vftable(this)->AddPlayerEnum(this, a1, a2, a3); }

        // vftable shift: +240
        virtual bool __thiscall AddSessionEnum(const void* a1, uint32_t a2)
        { return get_vftable(this)->AddSessionEnum(this, a1, a2); }

        // vftable shift: +244
        virtual bool __thiscall AddConnectionEnum(const GUID* a1, void* a2, uint32_t a3, const void* a4, uint32_t a5)
        { return get_vftable(this)->AddConnectionEnum(this, a1, a2, a3, a4, a5); }

    public:
        // DirectPlay object capabilites /
        // Информация о возможностях текущего сеанса.
        // offset: +0x4 = +4,  size = 0x28 = 40
        __DPCAPS m_caps;

        // IDirectPlay4 interface /
        // Интерфейс IDirectPlay4.
        // offset: +0x2C = +44,  size = 0x4 = 4
        void* m_lpDP;

        // GUID of the current game /
        // GUID текущей игры.
        // offset: +0x30 = +48,  size = 0x10 = 16
        GUID m_guid;

        // offset: +0x40 = +64,  size = 0x4 = 4
        int32_t m_hRes;

        // offset: +0x44 = +68,  size = 0x4 = 4
        CAutoArray<CDPlaySession>* m_pSessionArray;

        // offset: +0x48 = +72,  size = 0x4 = 4
        CAutoArray<CDPlayConnection>* m_pConnectionArray;

        // offset: +0x4C = +76,  size = 0x4 = 4
        /*CAutoArray<CDPlayGroup>*/void* m_pGroupArray;

        // offset: +0x50 = +80,  size = 0x4 = 4
        CAutoArray<CDPlayPlayer>* m_pPlayerArray;

        // Connected /
        // Подключено?
        // offset: +0x54 = +84,  size = 0x1 = 1
        bool m_connected;

        // offset: +0x55 = +85,  size = 0x1 = 1
        bool m_inSession;

        // Current player is host /
        // Текущий игрок является хостом.
        // offset: +0x56 = +86,  size = 0x1 = 1
        bool m_isHost;
};
#pragma pack(pop) // align 4

#pragma pack(push, 4)
//
// size = 0x60 = 96, align = 4, baseclass: CDPlay
class CDPlayLobby : public CDPlay
{
    public:
        struct vftable_t : public CDPlay::vftable_t
        {
          bool(__thiscall* RegisterApp)(CDPlayLobby*, char*, char*, char*,
                                        GUID);
          bool(__thiscall* EnumLobbyConnections)(CDPlayLobby*,
                                                 CAutoArray<CDPlayConnection>*);
          bool(__thiscall* SetGroupConnectionSettings)(CDPlayLobby*, uint32_t,
                                                       void*);
          void*(__thiscall* GetGroupConnectionSettings)(CDPlayLobby*, uint32_t);
          bool(__thiscall* EnumGroupsInGroup)(CDPlayLobby*,
                                              /*CAutoArray<CDPlayGroup>*/ void*,
                                              uint32_t, uint32_t);
          bool(__thiscall* CDPlayLobby_EnumGroupPlayers)(CDPlayLobby*,
                                             CAutoArray<CDPlayPlayer>*,
                                             uint32_t, uint32_t);
          bool(__thiscall* EnumGroupPlayersRemote)(CDPlayLobby*,
                                                   CAutoArray<CDPlayPlayer>*,
                                                   uint32_t, GUID*, uint32_t);
          bool(__thiscall* EnumAddress)(CDPlayLobby*, void*, uint32_t,
                                        CAutoArray<CDPlayAddressElement>*);
          bool(__thiscall* GetIPAddress)(CDPlayLobby*, uint32_t, char*);
          bool(__thiscall* HandleSystemLobbyMsg)(CDPlayLobby*, uint32_t,
                                                 CDPlayMsg*);
          bool(__thiscall* AddAddressEnum)(CDPlayLobby*, const GUID*, uint32_t,
                                           const void*);
        };

    public:
        // vftable shift: +248
        virtual bool __thiscall RegisterApp(char* a1, char* a2, char* a3, GUID a4)
        { return get_vftable(this)->RegisterApp(this, a1, a2, a3, a4); }

        // vftable shift: +252
        virtual bool __thiscall EnumLobbyConnections(CAutoArray<CDPlayConnection>* a1)
        { return get_vftable(this)->EnumLobbyConnections(this, a1); }

        // vftable shift: +256
        virtual bool __thiscall SetGroupConnectionSettings(uint32_t a1, void* a2)
        { return get_vftable(this)->SetGroupConnectionSettings(this, a1, a2); }

        // vftable shift: +260
        virtual void* __thiscall GetGroupConnectionSettings(uint32_t a1)
        { return get_vftable(this)->GetGroupConnectionSettings(this, a1); }

        // vftable shift: +264
        virtual bool __thiscall EnumGroupsInGroup(/*CAutoArray<CDPlayGroup>*/ void* a1, uint32_t a2, uint32_t a3)
        { return get_vftable(this)->EnumGroupsInGroup(this, a1, a2, a3); }

        // vftable shift: +268
        virtual bool __thiscall CDPlayLobby_EnumGroupPlayers(CAutoArray<CDPlayPlayer>* a1, uint32_t a2, uint32_t a3)
        { return get_vftable(this)->CDPlayLobby_EnumGroupPlayers(this, a1, a2, a3); }

        // vftable shift: +272
        virtual bool __thiscall EnumGroupPlayersRemote(CAutoArray<CDPlayPlayer>* a1, uint32_t a2, GUID* a3, uint32_t a4)
        { return get_vftable(this)->EnumGroupPlayersRemote(this, a1, a2, a3, a4); }

        // vftable shift: +276
        virtual bool __thiscall EnumAddress(void* a1, uint32_t a2, CAutoArray<CDPlayAddressElement>* a3)
        { return get_vftable(this)->EnumAddress(this, a1, a2, a3); }

        // vftable shift: +280
        virtual bool __thiscall GetIPAddress(uint32_t a1, char* a2)
        { return get_vftable(this)->GetIPAddress(this, a1, a2); }

        // vftable shift: +284
        virtual bool __thiscall HandleSystemLobbyMsg(uint32_t a1, CDPlayMsg* a2)
        { return get_vftable(this)->HandleSystemLobbyMsg(this, a1, a2); }

        // vftable shift: +288
        virtual bool __thiscall AddAddressEnum(const GUID* a1, uint32_t a2, const void* a3)
        { return get_vftable(this)->AddAddressEnum(this, a1, a2, a3); }

    public:
        // IDirectPlayLobby3 lobby /
        // Лобби IDirectPlayLobby3.
        // offset: +0x58 = +88,  size = 0x4 = 4
        void* m_lpLobby;

        // offset: +0x5C = +92,  size = 0x4 = 4
        CAutoArray<CDPlayAddressElement>* m_pAddressArray;

};
#pragma pack(pop) // align 4

#pragma pack(push, 4)
//
// size = 0xF4 = 244, align = 4, baseclass: CDPlayLobby
class CDPlayHeroes : public CDPlayLobby
{
    public:
        struct vftable_t : public CDPlayLobby::vftable_t
        {};

    public:
        // offset: +0x60 = +96,  size = 0x8 = 8
        CDPlayMsg dpMsg;

        // offset: +0x68 = +104,  size = 0x30 = 48
        exe_deque<CNetMsg*> msgQueue;

        // offset: +0x98 = +152,  size = 0x50 = 80
        std::array<char, 80> sLocalIPAddress;

        // offset: +0xE8 = +232,  size = 0x4 = 4
        uint32_t confirmId;

        // offset: +0xEC = +236,  size = 0x4 = 4
        uint32_t currMessageId;

        // offset: +0xF0 = +240,  size = 0x4 = 4
        CNetMsgHandler* m_pNetMsgHandler;
};
#pragma pack(pop) // align 4

NH3API_SPECIALIZE_TYPE_VFTABLE(0x63DE44, CAutoArray<CDPlayAddressElement>)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x6400E8, CAutoArray<CDPlaySession>)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x640F34, CAutoArray<CDPlayPlayer>)
