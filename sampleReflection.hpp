#include <string>
#include <utility>
#include <functional>

template<typename T>
struct ReflectionType{
};

template<std::size_t N>
struct FixedString{
    const char data[N];

    constexpr FixedString():data{}{}

    constexpr FixedString(const char(&other)[N]):FixedString(other, std::make_index_sequence<N>()) {
    }

    template<size_t... _Idx>
    constexpr FixedString(const char(&other)[N], std::index_sequence<_Idx...>):data{other[_Idx]...}{
    }

};

template<size_t N, size_t M>
constexpr bool operator==(const FixedString<N>& a, const FixedString<M>& b){
    if constexpr(N != M){
        return false;
    }else{
        for(int i = 0; i < N; ++i){
            if(a.data[i] != b.data[i]){
                return false;
            }
        }
        return true;
    }
}

template<size_t N, size_t M>
constexpr bool operator==(const FixedString<N>& a, const char(&b)[M]){
    return a == FixedString{b};
}

template<size_t N, size_t M>
constexpr bool operator==(const char(&a)[N], const FixedString<M>& b){
    return FixedString{a} == b;
}


#define REFL_TYPE(TYPE_NAME)   							                            \
template<>                        							                        \
struct ReflectionType<TYPE_NAME>{								                    \
	using type = TYPE_NAME;        					                                \
	static constexpr size_t memberIndexBegin = __COUNTER__;		                    \
    template<FixedString, typename _Unused = void> struct _Member{};                \
    template<size_t, typename _Unused = void> struct _MemberIndex{};                \


#define REFL_FIELD(fieldName)                   		                            \
    template<typename _Unused>								                        \
    struct _Member<#fieldName, _Unused>{    	                                    \
        using memberType = decltype(type::fieldName);                               \
        static constexpr auto name{#fieldName};                                     \
        static constexpr auto pointer{ &type::fieldName };                          \
    };                                                                              \
    template<typename _Unused>                                                      \
    struct _MemberIndex<__COUNTER__, _Unused>{                                      \
        using _MemberType = _Member<#fieldName>;                                    \
    };


#define REFL_METHOD(methodName)                                                     \
    template<typename _Unused>                                                      \
    struct _Member<#methodName, _Unused>{                                           \
        using memberType = decltype(&type::methodName);                             \
        using resultType = std::invoke_result_t<memberType, type*>;                 \
        static constexpr auto name{#methodName};                                    \
        static constexpr auto pointer{ &type::methodName };                         \
    };
    

#define REFL_TYPE_END                                                               \
    template<FixedString Name>                                                      \
    using getMember = _Member<Name>;                                                \
                                                                                    \
    template<size_t N>                                                              \
    using getFieldMemberViaIndex = _MemberIndex<memberIndexBegin + N>::_MemberType; \
                                                                                    \
    static constexpr size_t memberCount = __COUNTER__ - memberIndexBegin - 1;		\
};


template<int N, typename T>
struct _travel_type{            
    void operator()(T* p){
        using _Member = typename ReflectionType<T>::getFieldMemberViaIndex<N>;
        std::cout<<_Member::name<< " : "<< p->*_Member::pointer<< std::endl;
        _travel_type<N - 1, T>()(p);
    }
};

template<typename T>
struct _travel_type<0L, T>{
    void operator()(T* p){}
};

template<typename T>
void travelType(T* p){
    _travel_type<ReflectionType<T>::memberCount, T>()(p);
}