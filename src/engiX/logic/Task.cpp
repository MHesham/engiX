#include "Task.h"

using namespace engiX;

Task::~Task(void)
{
    if (m_pChild)
    {
        m_pChild->OnAbort();
    }
}
//---------------------------------------------------------------------------------------------------------------------
// Removes the child from this Task.  This releases ownership of the child to the caller and completely removes it
// from the Task chain.
//---------------------------------------------------------------------------------------------------------------------
StrongTaskPtr Task::RemoveChild()
{
    if (m_pChild)
    {
        StrongTaskPtr pChild = m_pChild;  // this keeps the child from getting destroyed when we clear it
        m_pChild.reset();
        return pChild;
    }

    return StrongTaskPtr();
}