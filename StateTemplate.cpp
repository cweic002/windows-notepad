#ifndef STATETEMPLATE_CPP
#define STATETEMPLATE_CPP

#include "State.h"

template<typename Obj, typename FSave, typename FRestore,typename>
void State::addState(Obj* obj, FSave fSave, FRestore fRestore){
        stateArg.push_back(
            std::unique_ptr<IStateArg>(
                new decltype(StateArg(obj, fSave, fRestore))(obj, fSave, fRestore)));
}

#endif // STATE_CPP

