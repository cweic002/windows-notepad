#ifndef GROUP_H
#define GROUP_H

template <typename Base,typename Value>
struct Group: public Base{
    Value value;
    Group(Value _value);
};

#endif // GROUP_H
