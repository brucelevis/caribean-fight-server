#include "room_mgr.h"

uint32_t RoomMgr::g_room_id = 1;

Room* RoomMgr::CreateRoom(Session *creator, unsigned max_player_num)
{
    Room *new_room = new Room(creator, max_player_num);
    new_room->SetId(g_room_id++);

    m_room_map.insert({new_room->GetId(), new_room});

    return new_room;
}

Room* RoomMgr::JoinRoom(Session *player, unsigned rid)
{
    Room *r = NULL;
    auto iter = m_room_map.find(rid);
    if(iter != m_room_map.end())
    {
        r = iter->second;
        if(r != NULL)
        {
            r->AddPlayer(player);
        }
    }
    else
    {
        printf("ERROR: can not find room id: %d\n", rid);
    }

    return r;
}

Game* RoomMgr::StartGame(unsigned rid)
{
    auto iter = m_room_map.find(rid);
    if(iter == m_room_map.end())
    {
        printf("ERROR: can not find room id: %d\n", rid);
        return NULL;
    }

    Room *r = iter->second;
    //room exist and is NOT fighting
    if(r != NULL && !r->IsFighting())
    {
        //let room r start fighting
        Game *new_game = GameMgrSin::instance().CreateGame(r);
        return new_game;
    }
    else
    {
        if(r->IsFighting())
        {
            cout<<"ERROR: Room is Fighting!" <<endl;
        }

        printf("ERROR: can not start a game\n");
        return NULL;
    }

    return NULL;
}
