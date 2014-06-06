#include "TaskManager.h"

using namespace engiX;

TaskManager::~TaskManager()
{
    ClearAllTaskes();
}

//---------------------------------------------------------------------------------------------------------------------
// The task update tick.  Called every logic tick.  This function returns the number of task chains that 
// succeeded in the upper 32 bits and the number of task chains that failed or were aborted in the lower 32 bits.
//---------------------------------------------------------------------------------------------------------------------
void TaskManager::OnUpdate(_In_ const Timer& time)
{
    unsigned short int successCount = 0;
    unsigned short int failCount = 0;
    StrongTaskPtr pChild;

    TaskList::iterator it = m_taskList.begin();
    while (it != m_taskList.end())
    {
        // grab the next task
        StrongTaskPtr pCurrTask = (*it);

        // save the iterator and increment the old one in case we need to remove this task from the list
        TaskList::iterator thisIt = it;
        ++it;

        // task is uninitialized, so initialize it
        if (pCurrTask->GetState() == Task::STATE_Uninitialized)
        {
            if (pCurrTask->Init())
                pCurrTask->SetState(Task::STATE_Running);
        }

        // give the task an update tick if it's running
        if (pCurrTask->GetState() == Task::STATE_Running)
            pCurrTask->OnUpdate(time);

        // check to see if the task is dead
        if (pCurrTask->IsDead())
        {
            // run the appropriate exit function
            switch (pCurrTask->GetState())
            {
            case Task::STATE_Succeeded:
                pCurrTask->OnSuccess();
                pChild = pCurrTask->RemoveChild();
                if (pChild)
                    AttachTask(pChild);
                else
                    ++successCount;  // only counts if the whole chain completed
                break;

            case Task::STATE_Failed:
                pCurrTask->OnFail();
                ++failCount;
                break;

            case Task::STATE_Aborted:
                pCurrTask->OnAbort();
                ++failCount;
                break;
            }

            // remove the task and destroy it
            m_taskList.erase(thisIt);
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------
// Attaches the task to the task list so it can be run on the next update.
//---------------------------------------------------------------------------------------------------------------------
WeakTaskPtr TaskManager::AttachTask(StrongTaskPtr pTask)
{
    m_taskList.push_front(pTask);
    return WeakTaskPtr(pTask);
}

//---------------------------------------------------------------------------------------------------------------------
// Clears all taskes (and DOESN'T run any exit code)
//---------------------------------------------------------------------------------------------------------------------
void TaskManager::ClearAllTaskes()
{
    m_taskList.clear();
}

//---------------------------------------------------------------------------------------------------------------------
// Aborts all taskes.  If immediate == true, it immediately calls each ones OnAbort() function and destroys all 
// the taskes.
//---------------------------------------------------------------------------------------------------------------------
void TaskManager::AbortAllTaskes(bool immediate)
{
    TaskList::iterator it = m_taskList.begin();
    while (it != m_taskList.end())
    {
        TaskList::iterator tempIt = it;
        ++it;

        StrongTaskPtr pTask = *tempIt;
        if (pTask->IsAlive())
        {
            pTask->SetState(Task::STATE_Aborted);
            if (immediate)
            {
                pTask->OnAbort();
                m_taskList.erase(tempIt);
            }
        }
    }
}