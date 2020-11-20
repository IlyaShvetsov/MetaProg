//
// Created by ilya on 18.11.2020.
//

#ifndef METAPROGA_FACTORY_H
#define METAPROGA_FACTORY_H

#include "Hierarchy.h"



template <typename T>
struct TypeManipHelper {
    T makeT() {
        T t;
        return t;
    }
};

template<typename T, typename U>
struct Conversion {
private:
    using Small = char;
    using Big = struct { char chars[2]; };

private:
    static Small Test(U);
    static Big Test(...);

public:
    static constexpr bool exists = sizeof(Small) == \
                sizeof(Test(TypeManipHelper<T>().makeT()));
    static constexpr bool isSame = false;
};

template<typename T>
struct Conversion<T, T> {
    static constexpr bool exists = true;
    static constexpr bool isSame = true;
};

template<bool boolLhs, bool boolRhs>
struct AndNotExpr {
    static constexpr bool Result = false;
};

template<>
struct AndNotExpr</* boolLhs = */true, /* boolRhs = */false> {
    static constexpr bool Result = true;
};

template<typename T, typename U>
struct IsBase {
    static constexpr bool Result = AndNotExpr<
            /* boolLhs = */Conversion<const U*, const T*>::exists,
            /* boolRhs = */Conversion<const U*, const T*>::isSame
    >::Result;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <class H, unsigned int i> struct FieldHelper;

template <class H>
struct FieldHelper<H, 0>
{
    typedef typename H::TL::Head ElementType;
    typedef typename H::template Rebind<ElementType>::Result UnitType;

    static UnitType& Do(H& obj)
    {
        UnitType& leftBase = obj;
        return leftBase;
    }
};

template <class H, unsigned int i>
struct FieldHelper
{
    typedef typename TypeAt<i, typename H::TList>::Result ElementType;
    typedef typename H::template Rebind<ElementType>::Result UnitType;

    typedef typename H::RightBase RightBase;

    static UnitType& Do(H& obj)
    {
        RightBase& rightBase = obj;
        return FieldHelper<RightBase, i - 1>::Do(rightBase);
    }
};

template <int i, class H>
typename FieldHelper<H, i>::UnitType&
Field(H& obj)
{
    return FieldHelper<H, i>::Do(obj);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

template<bool condition, class IfTrueType, class IfFalseType>
struct Conditional;

template<class IfTrueType, class IfFalseType>
struct Conditional<true, IfTrueType, IfFalseType> {
    using Result = IfTrueType;
};

template<class IfTrueType, class IfFalseType>
struct Conditional<false, IfTrueType, IfFalseType> {
    using Result = IfFalseType;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename... TLists>
struct Bases;

template <typename Head, typename... Tail, typename TList, typename... TLists>
struct Bases<TypeList<Head, Tail...>, TList, TLists...> {
private:
    using basesTypeList = typename Bases<TList, TLists...>::Result;
public:
    using Result = typename Conditional<
            IsBase<Head, typename basesTypeList::Head>::Result,
            TypeList<Head, Tail...>,
            basesTypeList>::Result;
};

template <typename TList>
struct Bases<TList> {
    using Result = TList;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename Head, typename... Tail>
struct MultiFind;

template <typename T, typename Head, typename... Tail>
struct MultiFind<T, TypeList<Head, Tail...>> {
    using Result = typename Conditional<
            Contains<T, Head>::Result,
            Head,
            typename MultiFind<T, TypeList<Tail...>>::Result>::Result;
};

template <typename T>
struct MultiFind<T, EmptyTypeList> {
    using Result = EmptyTypeList;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct AbstractUnit {
    virtual T* Get(T)=0;
    virtual ~AbstractUnit()=default;
};

template <typename BaseProductTypeList>
struct AbstractFactory: // scatter hierarchy generator for generating AbstractFactory
        public GenScatterHierarchy<BaseProductTypeList, AbstractUnit> {
    using ProductTypeList = BaseProductTypeList;

    template <typename T>
    T* Get() {
        AbstractUnit<T>& unit = *this;
        return unit.Get(TypeManipHelper<T>().makeT());
    }
};

template <class ConcreteProduct, class Base>
struct ConcreteUnit: public Base { // linear hierarchy
    using BaseProductTypeList = typename Base::ProductTypeList;
    using BaseProductType = typename BaseProductTypeList::Head;
    using ProductTypeList = typename BaseProductTypeList::Tail;

    ConcreteProduct* Get(BaseProductType) override {
        return new ConcreteProduct;
    }
};

template<typename AbstractFactoryType, typename TList>
struct ConcreteFactory: // linear hierarchy generator for generating ConcreteFactory from AbstractFactory
        public GenLinearHierarchy<TList, ConcreteUnit, AbstractFactoryType >
{
    using ProductTypeList = typename AbstractFactoryType::ProductTypeList;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <size_t N, size_t M, typename ...TLists>
struct GetAbstractFactory {
    using Factory = AbstractFactory<
            /* BaseProductTypesList =*/typename Bases<TLists...>::Result
    >;

    template <typename T>
    struct GetConcreteFactory {
        using Result = ConcreteFactory<
                /* AbstractFactoryType = */Factory,
                /* TList               = */typename Reverse<
                        typename MultiFind<
                                /* T     = */T,
                                /* TList = */TypeList<TLists...>
                        >::Result
                >::Result
        >;
    };
};



#endif //METAPROGA_FACTORY_H
