#ifndef STATEARG_CPP
#define STATEARG_CPP
#include "StateArg.h"

template<typename Obj, typename FSave, typename FRestore>
bool constexpr isTwoArg(){
    using Arg = std::invoke_result_t<FSave, Obj*>;
    return std::is_invocable_v<FRestore, Obj*, Arg> ||
        std::is_invocable_v<FRestore, Obj*, std::add_lvalue_reference_t<Arg>>;
}

template<typename Obj, typename FSave, typename FRestore>
bool constexpr isLvalueReferenseArg() {
    using Arg = std::invoke_result_t<FSave, Obj*>;
    return std::is_invocable_v<FRestore, Obj*, std::add_lvalue_reference_t<Arg>> ||
        std::is_invocable_v<FRestore, std::add_lvalue_reference_t<Arg>>;
}

template<typename Obj, typename FSave, typename FRestore>
bool constexpr isFRestoreStatus(){
    using Arg = std::invoke_result_t<FSave, Obj*>;
    return std::is_invocable_v<FRestore, Obj*,Arg>||
        std::is_invocable_v<FRestore, Obj*, std::add_lvalue_reference_t<Arg>>||
        std::is_invocable_v<FRestore,Arg>||
        std::is_invocable_v<FRestore, std::add_lvalue_reference_t<Arg>>;
}

template<typename Obj, typename Arg, typename FRestore>
template<typename _Obj, typename _FSave, typename _FRestore>
StateArg<Obj,Arg,FRestore>::StateArg(_Obj* obj, _FSave fSave, _FRestore fRestore):_obj(obj),
_fSave(fSave),_fRestore(fRestore),_arg(fSave(obj)){};

template<typename Obj, typename Arg, typename FRestore>
void StateArg<Obj,Arg,FRestore>::saveState() {
    _arg = _fSave(_obj);
};

template<typename Obj, typename Arg, typename FRestore>
void StateArg<Obj,Arg,FRestore>::restoreState() {
    if constexpr (isTwoArg<Obj,FSave,FRestore>()){
        _fRestore(_obj, _arg);
    }
    else {
        _fRestore(_arg);
    }
};

template<typename Obj, typename Arg, typename FRestore>
StateArg<Obj,Arg,FRestore>::~StateArg(){};

#endif // STATEARG_CPP

