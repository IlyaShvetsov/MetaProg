//
// Created by ilya on 18.11.2020.
//

#ifndef METAPROGA_TYPELIST_H
#define METAPROGA_TYPELIST_H

#include <vector>



class Nulltype {};

template <typename ...Args>
struct TypeList
{
    using Head = Nulltype;
    using Tail = Nulltype;
};

template <typename H, typename ...T>
struct TypeList<H, T...>
{
    using Head = H;
    using Tail = TypeList<T...>;
};

typedef TypeList<> EmptyTypeList;

template <typename H>
struct TypeList<H>
{
    using Head = H;
    using Tail = EmptyTypeList;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <size_t ind, typename ...T>
struct TypeAt {
    using Result = Nulltype;
};

template <size_t ind, typename ...T>
struct TypeAt<ind, TypeList<T...>>
{
    using Result = typename TypeAt<ind - 1, typename TypeList<T...>::Tail>::Result;
};

template <size_t ind>
struct TypeAt<ind, Nulltype>
{
    using Result = Nulltype;
};

template <typename ...T>
struct TypeAt<0, TypeList<T...>>
{
    using Result = typename TypeList<T...>::Head;
};


///////////////////////////////////////////////////////////////////////////////////////////////////

template<class ...T>
struct Join {
    using Result = Nulltype;
};

template<class ...T>
struct Join<TypeList<T...>> {
    using Result = TypeList<T...>;
};

template<class ...T, class ...K, class ... U>
struct Join<TypeList<T...>, TypeList<K...>, U...> {
    using Result = typename Join<TypeList<T..., K...>, U...>::Result;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> struct Reverse;

template <typename H, typename... T>
struct Reverse<TypeList<H, T...>> {
    using reversedTail = typename Reverse<TypeList<T...> >::Result;
    using Result = typename Join<reversedTail, TypeList<H>>::Result;
};

template <>
struct Reverse<EmptyTypeList> {
    using Result = EmptyTypeList;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TList>
struct Contains;

template <typename T>
struct Contains<T, EmptyTypeList> {
    static const bool Result = false;
};

template <typename T, typename... Tail>
struct Contains<T, TypeList<T, Tail...>> {
    static const bool Result = true;
};

template <typename T, typename Head, typename... Tail>
struct Contains<T, TypeList<Head, Tail...>> {
    static const bool Result = Contains<T, TypeList<Tail...>>::Result;
};



#endif //METAPROGA_TYPELIST_H
