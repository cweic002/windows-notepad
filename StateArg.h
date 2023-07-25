#ifndef STATEARG_H
#define STATEARG_H

#include <functional>
#include <Restore.h>

struct IStateArg {
    virtual void saveState() = 0;
    virtual void restoreState() = 0;
    virtual ~IStateArg() {}
};

template<typename _Obj, typename _FSave, typename _FRestore>
bool constexpr isTwoArg();

template<typename _Obj, typename _FSave, typename _FRestore>
bool constexpr isLvalueReferenseArg();

template<typename _Obj, typename _FSave, typename _FRestore>
bool constexpr isFRestoreStatus();

template<typename Obj, typename Arg, typename FRestore>
class StateArg : public IStateArg{
    Obj* _obj;
    using FSave = std::function<Arg(Obj*)>;
    FSave _fSave;
    FRestore _fRestore;
    Arg _arg;
public:
    template<typename _Obj, typename _FSave, typename _FRestore>
    StateArg(_Obj* obj, _FSave fSave, _FRestore fRestore);
    void saveState() override ;
    void restoreState() override;
    ~StateArg();
};

template<typename _Obj, typename _FSave, typename _FRestore>
StateArg(_Obj* obj, _FSave fSave, _FRestore fRestore) ->
StateArg<
    _Obj,
    std::invoke_result_t<_FSave, _Obj*>,
    typename Restore<
                isFRestoreStatus<_Obj, _FSave, _FRestore>(),
                isTwoArg<_Obj, _FSave, _FRestore>(),
                isLvalueReferenseArg<_Obj, _FSave, _FRestore>(),
                _Obj,
                std::invoke_result_t<_FSave, _Obj*>>::type>;
#include<StateArg.cpp>

#endif // STATEARG_H
