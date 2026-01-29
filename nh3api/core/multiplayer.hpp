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

#include "resources/files.hpp" // TAbstractFile

#pragma pack(push, 4)
// Multiplayer player information /
// Информация об игре в мультиплеере.
// size = 0x20 = 32, align = 4
struct CNetPlayerInfo
{
    // Player ID /
    // ID игрока.
    // offset: +0x0 = +0,  size = 0x4 = 4
    uint32_t dpid;

    // Player name /
    // Имя игрока.
    // offset: +0x4 = +4,  size = 0x18 = 24
    std::array<char, 24> cName;

    // Player game version /
    // Версия игры игрока.
    // offset: +0x1C = +28,  size = 0x4 = 4
    int32_t version;

};

// Multiplayer message /
// Сообщение мультиплеера.
// size = 0x14 = 20, align = 4
struct CNetMsg
{
    public:
        enum eRS_Messages : int32_t
        {
            RS_GAME_TRANSMIT_INIT            = 1000, //
            RS_GAME_TRANSMIT_MAIN            = 1001, //
            RS_GAME_TRANSMIT_REQ             = 1002, //
            RS_GAME_TRANSMIT_END             = 1003, //
            RS_CHAT_MSG                      = 1004, //
            RS_COMBAT_INIT                   = 1005, //
            RS_COMBAT_MAIN                   = 1006, //
            RS_COMBAT_CONTROL                = 1007, //
            RS_COMBAT_END_PLACEMENT          = 1008, //
            RS_COMBAT_TYPE                   = 1009, //
            RS_MAP_CHANGE                    = 1010, //
            RS_HERO_LEVEL_UPDATE             = 1011, //
            RS_READY_TO_PLAY                 = 1012, //
            RS_ALL_READY_TO_PLAY             = 1013, //
            RS_PLAYER_DROPPED                = 1014, //
            RS_SET_AS_HOST                   = 1015, //
            RS_TURN_UPDATE                   = 1016, //
            RS_PLAYER_DROP_UPDATE            = 1017, //
            RS_PLAYER_DEAD                   = 1018, //
            RS_PLAYER_WON                    = 1019, //
            RS_PLAYER_LOST                   = 1020, //
            RS_SET_VISIBILITY                = 1021, //
            RS_RESET_VISIBILITY              = 1022, //
            RS_GAME_HEADER_INFO              = 1023, //
            RS_GAME_HEADER_INFO_INIT         = 1024, //
            RS_GAME_HEADER_INFO_END          = 1025, //
            RS_NEW_SETUP_INFO                = 1026, //
            RS_SCROLL                        = 1027, //
            RS_NEW_MAP_HEADER_INFO           = 1028, //
            RS_MAP_HEADER_REQUEST            = 1029, //
            RS_MAP_FILE_NAME                 = 1030, //
            RS_SORT_MAPS                     = 1031, //
            RS_SET_FILTER                    = 1032, //
            RS_NEXT_SCREEN                   = 1033, //
            RS_PREV_SCREEN                   = 1034, //
            RS_REQUEST_HERO_FACE             = 1035, //
            RS_REQUEST_HERO_FACE_REPLY       = 1036, //
            RS_SETAGR                        = 1037, //
            RS_NEW_HOST                      = 1038, //
            RS_UPDATE_PLAYER_POS             = 1039, //
            RS_NEW_PLAYER                    = 1040, //
            RS_REQ_HEADER_CONFIRM            = 1041, //
            RS_HEADER_CONFIRM                = 1042, //
            RS_CLICK                         = 1043, //
            RS_TOWN_UPDATE                   = 1044, //
            RS_LAUNCHING_GAME                = 1045, //
            RS_BAD_VERSION                   = 1046, //
            RS_SETUP_PING                    = 1047, //
            RS_SETUP_PING_RESPONSE           = 1048, //
            RS_MAP_CHANGE_START              = 1049, //
            RS_MOVE_HERO                     = 1050, //
            RS_TELEPORT_HERO                 = 1051, //
            RS_CLAIM_MINE                    = 1052, //
            RS_CLAIM_TOWN                    = 1053, //
            RS_CLAIM_GENERATOR               = 1054, //
            RS_CLAIM_GARRISON                = 1055, //
            RS_CLAIM_SHIPYARD                = 1056, //
            RS_BUILD_BOAT                    = 1057, //
            RS_ERASE_OBJECT                  = 1058, //
            RS_DEAD_HERO                     = 1059, //
            RS_RECRUIT_HERO                  = 1060, //
            RS_DEAD_PLAYER                   = 1061, //
            RS_HIDE_HERO                     = 1062, //
            RS_MAP_CHANGE_END                = 1063, //
            RS_TRADE_REQUEST                 = 1064, //
            RS_TRADE_REQUEST_DONE            = 1065, //
            RS_HERO_UPDATE                   = 1066, //
            RS_ARTIFACT_DROP                 = 1067, //
            RS_BACKPACK_DROP                 = 1068, //
            RS_MONSTER_DROP                  = 1069, //
            RS_GIVE_ME_STUFF                 = 1070, //
            RS_PLAYER_ACTIVE                 = 1071, //
            RS_PING                          = 1072, //
            RS_PING_RESPONSE                 = 1073, //
            RS_GIFT                          = 1074, //
            RS_GIFT_REQUEST                  = 1075, //
            RS_SESSION_LOST                  = 1076, //
            RS_TEAM_WON                      = 1077, //
            RS_NORMAL_WIN                    = 1078, //
            RS_DESTROY_PLAYER                = 1079, //
            RS_GAME_TRANSMIT_ACK             = 1080, //
            RS_GAME_XFER_CONFIRM_END         = 1081, //
            RS_GENERATING_RANDOM_MAP         = 1082, //
            RS_REQUEST_RANDOM_MAPS_LIST_SIZE = 1083, //
            RS_REQUEST_RANDOM_MAPS_LIST      = 1084  //
        };

public:
    // offset: +0x0 = +0,  size = 0x4 = 4
    int32_t m_from;

    // offset: +0x4 = +4,  size = 0x4 = 4
    uint32_t m_dpidFrom;

    // offset: +0x8 = +8,  size = 0x4 = 4
    eRS_Messages m_subType;

    // offset: +0xC = +12,  size = 0x4 = 4
    uint32_t m_size;

    // offset: +0x10 = +16,  size = 0x4 = 4
    uint32_t m_UncompressedSize;

};

// size = 0x18 = 24, align = 4
struct t_complex_net_message
{
    public:
        struct vftable_t
        {
            bool (__thiscall* read)(t_complex_net_message*, TAbstractFile*);
            bool (__thiscall* write)(const t_complex_net_message*, TAbstractFile*);
        };

    public:
        // vftable shift: +0
        virtual bool __thiscall read(TAbstractFile* infile)
        { return get_vftable(this)->read(this, infile); }

        // vftable shift: +4
        virtual bool __thiscall write(TAbstractFile* outfile) const
        { return get_vftable(this)->write(this, outfile); }

    public:
        // offset: +0x4 = +4,  size = 0x14 = 20
        CNetMsg netmsg;

};

// size = 0xC = 12, align = 4
struct CNetMsgHandler
{
    public:
        struct vftable_t
        {
            void     (__thiscall* scalar_deleting_destructor)(CNetMsgHandler*, uint8_t);
            CNetMsg *(__thiscall* CheckHandleNet)(CNetMsgHandler *, bool , bool *);
            CNetMsg *(__thiscall* GetAbortPopupMsg)(CNetMsgHandler *);
            CNetMsg *(__thiscall* HandleNetMsg)(CNetMsgHandler *, CNetMsg *);
            void     (__thiscall* HandleGiftMsg)(CNetMsgHandler *, CNetMsg *);
        };

    public:
        // vftable shift: +0
        NH3API_SCALAR_DELETING_DESTRUCTOR

        // vftable shift: +4
        virtual CNetMsg* __thiscall CheckHandleNet(bool inPopup, bool* msgReceived)
        { return get_vftable(this)->CheckHandleNet(this, inPopup, msgReceived); }

        // vftable shift: +8
        virtual CNetMsg* __thiscall GetAbortPopupMsg()
        { return get_vftable(this)->GetAbortPopupMsg(this); }

        // vftable shift: +12
        virtual CNetMsg* __thiscall HandleNetMsg(CNetMsg* msg)
        { return get_vftable(this)->HandleNetMsg(this, msg); }

        // vftable shift: +16
        virtual void __thiscall HandleGiftMsg(CNetMsg* msg)
        { get_vftable(this)->HandleGiftMsg(this, msg); }

    public:
        // offset: +0x4 = +4,  size = 0x1 = 1
        bool m_inPopup;

        unsigned char : 8;
        unsigned char : 8;
        unsigned char : 8;

        // offset: +0x8 = +8,  size = 0x4 = 4
        CNetMsg* m_pAbortPopupMsg;

} NH3API_MSVC_LAYOUT;
#pragma pack(pop) // 4

// Current game type (single, multiplayer, hotseat, etc.) /
// Тип текущей игры(однопользовательский, многопользовательский, хотсит и др.).
enum eNetGameType : int32_t
{
    MP_SINGLE  = 0, // Однопользовательская игра.
    MP_IPX     = 1, //
    MP_TCP     = 2, // TCP/IP.
    MP_HOTSEAT = 3, // Hotseat.
    MP_SERIAL  = 4, //
    MP_MODEM   = 5  // Модем
};

NH3API_SPECIALIZE_TYPE_VFTABLE(0x640280, t_complex_net_message)
NH3API_SPECIALIZE_TYPE_VFTABLE(0x640F24, CNetMsgHandler)
