#include "manager.h"

manager::manager()
{

}

template<typename T>
T manager::find_in_list(std::list<T> lst, const T &key)
{
    unsigned long int index = 0;
    for (const auto &el : lst){
        if (el == key){
            return index;
        }
        index++;
    }
    return NOT_EXIST;
}
