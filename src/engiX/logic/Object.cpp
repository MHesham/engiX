#include "Object.h"
#include "Logger.h"
#include "engiXDefs.h"
#include <unordered_set>
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace engiX;
using namespace std;

HANDLE g_hHeap = NULL;
unordered_set<Object*>* g_pAliveObjects = nullptr;
size_t g_AliveObjectsUsedMem = 0;

void* Object::Alloc(std::size_t sz)
{
    if (NULL == g_hHeap)
    {
        g_hHeap = HeapCreate(HEAP_NO_SERIALIZE | HEAP_GENERATE_EXCEPTIONS, 0 , 0);
        _ASSERTE(NULL != g_hHeap);

		if (nullptr == g_pAliveObjects)
			g_pAliveObjects = new unordered_set<Object*>;
    }

    void* pMem = HeapAlloc(g_hHeap, 0, sz);
    g_AliveObjectsUsedMem += HeapSize(g_hHeap, 0, pMem);
	g_pAliveObjects->insert((Object*)pMem);

    return pMem;
}
//////////////////////////////////////////////////////////////////////////
void Object::Free(void* pMem)
{
	g_pAliveObjects->erase((Object*)pMem);
    g_AliveObjectsUsedMem -= HeapSize(g_hHeap, 0, pMem);
    (void)HeapFree(g_hHeap, 0, pMem);
}
//////////////////////////////////////////////////////////////////////////
void Object::FreeMemoryPool()
{
    if (g_hHeap)
    {
        LogInfo("Freeing EngineObject memory pool");

        if (!g_pAliveObjects->empty())
        {
            LogInfo("Found %d alive EngineObject, destroying them...", g_pAliveObjects->size());

            while (!g_pAliveObjects->empty())
            {
                auto pCurrObj = *g_pAliveObjects->begin();
                g_pAliveObjects->erase(pCurrObj);
                // The delete should call the destructor of EngineObject which
                // in turn will remove the object from the alive objects
                delete pCurrObj;
            }

            _ASSERTE(g_pAliveObjects->empty());
        }

        (void)HeapDestroy(g_hHeap);
        g_hHeap = NULL;

        SAFE_DELETE(g_pAliveObjects);

        LogInfo("EngineObject memory pool heap destroyed");
    }
}
//////////////////////////////////////////////////////////////////////////
void Object::DumpAliveObjects()
{
    if (g_pAliveObjects->empty())
    {
        LogInfo("All EngineObject instances are dead, nothing to dump");
    }
    else
    {
        LogInfo("Dumping %d alive EngineObject", g_pAliveObjects->size());
        
        size_t count = 0;
        for (auto pObj : *g_pAliveObjects)
        {
            LogInfo("Alive EngineObject[%d]@0x%x: %s", count, (void*)pObj, pObj->Typename());
            ++count;
        }
    }
}
//////////////////////////////////////////////////////////////////////////
Object::Object()
{
    LogVerbose("EngineObject@0x%x created", (void*)this);
}
//////////////////////////////////////////////////////////////////////////
Object::~Object()
{
    LogVerbose("EngineObject@0x%x destroyed", (void*)this);
}
//////////////////////////////////////////////////////////////////////////