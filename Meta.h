#ifndef META_H
#define META_H

template<typename T, typename U>
struct isEqualType
{
    static const bool value = false;
};

template<typename T>
struct isEqualType<T,T>
{
    static const bool value = true;
};

#endif