#ifndef STATE_H
#define STATE_H

#include<list>
#include<memory>

struct IState{
    virtual void saveState() = 0;
    virtual void restoreState() = 0;
    virtual ~IState(){}
};

template<typename Obj,typename FSave,typename FRestore>
class StateArg : public IState{
    Obj * _obj;
    FSave _fSave;
    FRestore _fRestore;
    using Arg=std::invoke_result_t<FSave,Obj*>;
    Arg _arg;
public:
    StateArg(Obj * obj,FSave fSave,FRestore fRestore);
    void saveState() override ;
    void restoreState() override;
    ~StateArg();
};

class State {
    std::list<std::unique_ptr<IState>> stateArg;
public:
    template<typename Obj,typename FSave,typename FRestore,
             typename = std::enable_if_t<
                 std::is_invocable_v<FSave,Obj*>&&(
                 std::is_invocable_v<FRestore,Obj*,std::invoke_result_t<FSave,Obj*>> ||
                 std::is_invocable_v<FRestore,Obj*,std::invoke_result_t<FSave,Obj*>&> ||
                 std::is_invocable_v<FRestore,std::invoke_result_t<FSave,Obj*>> ||
                 std::is_invocable_v<FRestore,std::invoke_result_t<FSave,Obj*>&>)>>
    void addState(Obj * obj, FSave fSave,FRestore fRestore);
    void saveState();
    void restoreState();
};

#include "StateTemplate.cpp"

#endif // STATE_H
