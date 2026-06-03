#include "IDManager.h"

IDManager& IDManager::GetInstance()
{
    static IDManager instance;
    return instance;
}

int IDManager::GetNewId()
{
    int id = m_nextId;
    m_nextId++;
    return id;
}
