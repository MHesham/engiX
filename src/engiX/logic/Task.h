#pragma once

#include <memory>
#include "engiXDefs.h"
#include "Timer.h"

namespace engiX
{
    class Task;
    typedef std::shared_ptr<Task> StrongTaskPtr;
    typedef std::weak_ptr<Task> WeakTaskPtr;

    //---------------------------------------------------------------------------------------------------------------------
    // Task class
    // 
    // Taskes are ended by one of three methods: Success, Failure, or Aborted.
    //		- Success means the Task completed successfully.  If the Task has a child, it will be attached to 
    //		  the Task mgr.
    //		- Failure means the Task started but failed in some way.  If the Task has a child, it will be 
    //		  aborted.
    //		- Aborted Taskes are Taskes that are canceled while not submitted to the Task mgr.  Depending 
    //		  on the circumstances, they may or may not have gotten an OnInit() call.  For example, a Task can 
    //		  spawn another Task and call AttachToParent() on itself.  If the new Task fails, the child will
    //		  get an Abort() call on it, even though its status is RUNNING.
    //---------------------------------------------------------------------------------------------------------------------
    class Task
    {
    public:
        enum State
        {
            // Tasks that are neither dead nor alive
            STATE_Uninitialized = 0,  // created but not running
            // Living Tasks
            STATE_Running,  // initialized and running
            // Dead Tasks
            STATE_Succeeded,  // completed successfully
            STATE_Failed,  // failed to complete
            STATE_Aborted,  // aborted; may not have started
        };

        // construction
        Task() :
            m_state(STATE_Uninitialized)
        {}
        virtual ~Task();

        // interface; these functions should be overridden by the subclass as needed
        virtual bool Init() { return true; }  // called during the first update; responsible for setting the initial state (typically RUNNING)
        virtual void OnUpdate(_In_ const Timer& time) = 0;  // called every frame
        virtual void OnSuccess() { }  // called if the Task succeeds (see below)
        virtual void OnFail() { }  // called if the Task fails (see below)
        virtual void OnAbort() { }  // called if the Task is aborted (see below)

        // accessors
        State GetState() const { return m_state; }
        bool IsAlive() const { return (m_state == STATE_Running); }
        bool IsDead() const { return (m_state == STATE_Succeeded || m_state == STATE_Failed || m_state == STATE_Aborted); }

        // child functions
        inline void AttachChild(StrongTaskPtr pChild);
        StrongTaskPtr RemoveChild();  // releases ownership of the child
        StrongTaskPtr PeekChild() { return m_pChild; }  // doesn't release ownership of the child

        void SetState(State newState) { m_state = newState; }

    protected:
        // Functions for ending the Task.
        inline void Succeed();
        inline void Fail();

    private:
        State m_state;  // the current state of the Task
        StrongTaskPtr m_pChild;  // the child Task, if any
    };

    inline void Task::Succeed()
    {
        _ASSERTE(m_state == STATE_Running);
        m_state = STATE_Succeeded;
    }

    inline void Task::Fail()
    {
        _ASSERTE(m_state == STATE_Running);
        m_state = STATE_Failed;
    }

    inline void Task::AttachChild(StrongTaskPtr pChild)
    {
        if (m_pChild)
            m_pChild->AttachChild(pChild);
        else
            m_pChild = pChild;
    }
}