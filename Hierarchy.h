//
// Created by ilya on 18.11.2020.
//

#ifndef METAPROGA_HIERARCHY_H
#define METAPROGA_HIERARCHY_H

#include "TypeList.h"



template <class TList, template <class> class Unit>
struct GenScatterHierarchy;

template <class ...TList, template <class> class Unit>
struct GenScatterHierarchy<TypeList<TList...>, Unit>:
        public Unit<typename TypeList<TList...>::Head>,
        public GenScatterHierarchy<typename TypeList<TList...>::Tail, Unit>
{
    using TL = TypeList<TList...>;
    using LeftBase = Unit<typename TypeList<TList...>::Head>;
    using RightBase = GenScatterHierarchy<typename TypeList<TList...>::Tail, Unit>;
    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

template <class T, template <class> class Unit>
struct GenScatterHierarchy<TypeList<T>, Unit> :
        public Unit<T>
{
    using LeftBase = T;
    using RightBase = Nulltype;
    template <typename U> struct Rebind
    {
        typedef Unit<U> Result;
    };
};

template <template <class> class Unit>
struct GenScatterHierarchy<TypeList<Nulltype>, Unit>
{
    using LeftBase = Nulltype;
    using RightBase = Nulltype;
    template <typename T> struct Rebind
    {
        typedef Unit<T> Result;
    };
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template<class TList, template <class T, class Base> class Unit, class Root = Nulltype>
struct GenLinearHierarchy;

template<class ...TList, template <class T, class Base> class Unit, class Root>
struct GenLinearHierarchy<TypeList<TList...>, Unit, Root> :
        public Unit< typename TypeList<TList...>::Head,
        GenLinearHierarchy<typename TypeList<TList...>::Tail, Unit, Root>>
{

};

template<class TList, template <class T, class Base> class Unit, class Root>
struct GenLinearHierarchy<TypeList<TList>, Unit, Root> :
        public Unit<TList, Root>
{

};



#endif //METAPROGA_HIERARCHY_H
