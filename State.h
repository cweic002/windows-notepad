#ifndef STATE_H
#define STATE_H

#include <QtWidgets>

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
    using Arg = std::invoke_result_t<FSave,Obj*>;
    Arg _arg;
public:
    StateArg(Obj * obj,FSave fSave,FRestore fRestore):_obj(obj),_fSave(fSave),_fRestore(fRestore),_arg(fSave(obj)){}
    void saveState() override {
        _arg = _fSave(_obj);
    }
    void restoreState() override {
        if constexpr (std::is_invocable_v<FRestore,Obj *,Arg>){
            _fRestore(_obj,_arg);
        }else{
            _fRestore(_arg);
        }
    }
};

class State {
    QList<IState *>stateArg;
public:
    template<typename Obj,typename FSave,typename FRestore,
             typename = std::enable_if_t<
                 std::is_invocable_v<FSave,Obj*>&&(
                 std::is_invocable_v<FRestore,Obj*,std::invoke_result_t<FSave,Obj*>> ||
                 std::is_invocable_v<FRestore,std::invoke_result_t<FSave,Obj*>>)>>
    void addState(Obj * obj, FSave fSave,FRestore fRestore){
        stateArg.push_back(new StateArg<Obj,FSave,FRestore>(obj,fSave,fRestore));
    }

    void saveState() {
        for(auto iter=stateArg.begin(),iterEnd=stateArg.end(); iter!=iterEnd; ++iter){
            (*iter)->saveState();
        }
    }
    void restoreState() {
        for(auto iter=stateArg.begin(), iterEnd=stateArg.end(); iter!=iterEnd; ++iter){
            (*iter)->restoreState();
        }
    }
};


//class State : public IState{
//    QList<std::function<void()>>funsSave;
//    QList<std::function<void()>>funsRestore;
//public:
//    template<typename T,typename FSave,typename FRestore,
//             typename = std::enable_if_t<
//                 std::is_invocable_v<FSave,T*> &&(
//                 std::is_invocable_v<FRestore,T*,std::invoke_result_t<FSave,T*>> ||
//                 std::is_invocable_v<FRestore,std::invoke_result_t<FSave,T*>>) >>
//    void addState(T * obj, FSave fSave,FRestore fRestore){

//        using typeResult = std::invoke_result_t<FSave,T*>;
//        typeResult * save = new typeResult(fSave(obj));
//        funsSave.push_back([fSave,obj,save](){
//            *save = fSave(obj);
//        });
//        if constexpr(std::is_invocable_v<FRestore,T*,std::invoke_result_t<FSave,T*>>){
//            funsRestore.push_back([fRestore,obj,save](){
//                fRestore(obj,*save);
//            });
//        }else{
//            funsRestore.push_back([fRestore,save](){
//                fRestore(*save);
//            });
//        }
//    }

//    void saveState() override {
//        for(auto iter=funsSave.begin(),iterEnd=funsSave.end(); iter!=iterEnd; ++iter){
//            (*iter)();
//        }
//    }
//    void restoreState() override {
//        for(auto iter=funsRestore.begin(), iterEnd=funsRestore.end(); iter!=iterEnd; ++iter){
//            (*iter)();
//        }
//    }
//};



#endif // STATE_H