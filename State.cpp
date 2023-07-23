#include "State.h"

void State::saveState(){
    for(auto iter=stateArg.begin(),iterEnd=stateArg.end(); iter!=iterEnd; ++iter){
        (*iter)->saveState();
    }
}

void State::restoreState(){
    for(auto iter=stateArg.begin(), iterEnd=stateArg.end(); iter!=iterEnd; ++iter){
        (*iter)->restoreState();
    }
}
