#pragma once

#include <list>
#include "Task.h"

namespace engiX
{
    class TaskManager
    {
    public:
        virtual ~TaskManager();

        // interface
        void OnUpdate(_In_ const Timer& time);
        WeakTaskPtr AttachTask(StrongTaskPtr pTask);  // attaches a Task to the Task mgr
        void AbortAllTaskes(bool immediate);

        // accessors
        unsigned GetTaskCount(void) const { return m_taskList.size(); }

    private:
        void ClearAllTaskes(void);  // should only be called by the destructor

        typedef std::list<StrongTaskPtr> TaskList;
        TaskList m_taskList;
    };
}