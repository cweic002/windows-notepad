#ifndef STATE_H
#define STATE_H

#include<list>
#include<memory>
#include<StateArg.h>

class State{
    std::list<std::unique_ptr<IStateArg>> stateArg;
public:
    template<typename Obj, typename FSave, typename FRestore,
        typename = std::enable_if_t<isFRestoreStatus<Obj, FSave,FRestore>()>>
    void addState(Obj* obj, FSave fSave, FRestore fRestore);
    void saveState();
    void restoreState();
};

#include "StateTemplate.cpp"

#endif // STATE_H
