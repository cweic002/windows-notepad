#ifndef RESTORE_H
#define RESTORE_H

#include <functional>

template<bool status ,bool twoArg,bool lvalueReferenseArg,typename Obj, typename Arg>
struct Restore {};

template<typename Obj, typename Arg>
struct Restore<true, true, true, Obj, Arg> {
    using type = std::function<void(Obj*, std::add_lvalue_reference_t<Arg>)>;
};

template<typename Obj, typename Arg>
struct Restore<true,true,false,Obj,Arg> {
    using type = std::function<void(Obj*, Arg)>;
};

template<typename Obj, typename Arg>
struct Restore<true,false, true, Obj, Arg> {
    using type = std::function<void(std::add_lvalue_reference_t<Arg>)>;
};

template<typename Obj, typename Arg>
struct Restore<true,false, false,Obj, Arg> {
    using type = std::function<void(Arg)>;
};

#endif // RESTORE_H
