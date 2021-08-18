#ifndef MANAGER_H
#define MANAGER_H
#include <list>

constexpr auto NOT_EXIST = -1;

class manager
{
public:
    manager();

    template <typename T> T find_in_list(std::list<T>lst , const T& key) ;
};

#endif // MANAGER_H
