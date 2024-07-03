#pragma once
#include <concepts>
#include <algorithm>

template<class _Iterator>
concept access_iterator = requires(_Iterator iter_1, _Iterator iter_2) {

    typename _Iterator::value_type;

    { iter_1++ } -> std::same_as<_Iterator>;
    { iter_1-- } -> std::same_as<_Iterator>;

    { ++iter_1 } -> std::same_as<_Iterator&>;
    { --iter_1 } -> std::same_as<_Iterator&>;

    { iter_1 == iter_2 } -> std::convertible_to<bool>;
    { iter_1 != iter_2 } -> std::convertible_to<bool>;

    { *iter_1 } -> std::same_as<typename _Iterator::value_type&>;

};

template <class _InsertIterator>
concept Insert_iterator = requires(_InsertIterator iter, typename _InsertIterator::container_type::value_type value) {

    { iter++ } -> std::same_as<_InsertIterator&>;
    { ++iter } -> std::same_as<_InsertIterator&>;

    { *iter } -> std::same_as<_InsertIterator&>;

    { *iter = value } -> std::same_as<typename _InsertIterator::container_type::iterator&>;

};

template<class _DataType, class _F>
concept Predicate = requires(_DataType value, _F func) {
    { func(value) } -> std::convertible_to<bool>;
};

template<class _Iterator, class _F>
concept Function = requires(_Iterator iter, _F func) {
    func(*iter);
};

template <class _Iterator, class _Predicate>
requires access_iterator<_Iterator> && Predicate<typename _Iterator::value_type, _Predicate>
_Iterator findIf(_Iterator beg, _Iterator end, _Predicate pred) {
    for (_Iterator iter = beg; iter != end; ++iter) {
        if (pred(*iter))
            return iter;
    }
    return end;
}

template <class _Iterator>
requires access_iterator<_Iterator>
_Iterator minElement(_Iterator beg, _Iterator end) {
    _Iterator buffer = beg;
    for (_Iterator iter = beg; iter != end; ++iter) {
        if ((*iter) < (*buffer))
            buffer = iter;
    }
    return buffer;
}

template <class _Iterator>
requires access_iterator<_Iterator>
_Iterator maxElement(_Iterator beg, _Iterator end) {
    _Iterator buffer = beg;
    for (_Iterator iter = beg; iter != end; ++iter) {
        if ((*iter) > (*buffer))
            buffer = iter;
    }
    return buffer;
}

template <class _Iterator, class _F>
requires access_iterator<_Iterator> && Function<_Iterator, _F>
void forEach(_Iterator beg, _Iterator end, _F func) {
    for (_Iterator iter = beg; iter != end; ++iter)
        func(*iter);
}

template <class _Iterator, class _InsertIterator, class _Predicate>
requires access_iterator<_Iterator> && Insert_iterator<_InsertIterator> && Predicate<typename _Iterator::value_type, _Predicate>
void copyIf(_Iterator sourceBeg, _Iterator sourceEnd, _InsertIterator dest, _Predicate pred) {
    for (_Iterator iter = sourceBeg; iter != sourceEnd; ++iter) {
        if (pred(*iter))
            *dest = iter->value;
    }
}