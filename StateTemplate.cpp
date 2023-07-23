#ifndef STATETEMPLATE_CPP
#define STATETEMPLATE_CPP

#include "State.h"

template<typename Obj,typename FSave,typename FRestore>
StateArg<Obj,FSave,FRestore>::StateArg(Obj * obj,FSave fSave,FRestore fRestore)
    :_obj(obj),_fSave(fSave),_fRestore(fRestore),_arg(fSave(obj)){}

template<typename Obj,typename FSave,typename FRestore>
void StateArg<Obj,FSave,FRestore>::saveState(){
    _arg = _fSave(_obj);
}

template<typename Obj,typename FSave,typename FRestore>
void StateArg<Obj,FSave,FRestore>::restoreState(){
    if constexpr (std::is_invocable_v<FRestore,Obj *,Arg> || std::is_invocable_v<FRestore,Obj *,Arg &>){
        _fRestore(_obj,_arg);
    }else{
        _fRestore(_arg);
    }
};

template<typename Obj,typename FSave,typename FRestore>
StateArg<Obj,FSave,FRestore>::~StateArg(){}

template<typename Obj,typename FSave,typename FRestore,typename >
void State::addState(Obj * obj, FSave fSave,FRestore fRestore){
    stateArg.push_back(std::unique_ptr<IState>(new StateArg<Obj,FSave,FRestore>(obj,fSave,fRestore)));
}

#endif // STATE_CPP

