#pragma once
#include "UnorderedMap.h"

template <class _KeyType, class _DataType>
class UnorderedMapIterator {
private:

	using Pair = std::pair<_KeyType, _DataType>;
	
	Node<Pair>* elem;

public:

	using value_type = _DataType;

	UnorderedMapIterator() 
		: elem(nullptr) {}

	UnorderedMapIterator(Node<Pair>* in_node)
		: elem(in_node) {}

	UnorderedMapIterator(const UnorderedMapIterator<_KeyType, _DataType>& in_iter) 
		: elem(in_iter.elem) {}

	~UnorderedMapIterator() = default;

	UnorderedMapIterator<_KeyType, _DataType>& operator++() {
		if (elem != nullptr)
			elem = elem->next;
		return (*this);
	}

	UnorderedMapIterator<_KeyType, _DataType> operator++(int) {
		if (elem != nullptr)
			elem = elem->next;
		return (*this);
	}

	UnorderedMapIterator<_KeyType, _DataType>& operator--() {
		if (elem != nullptr)
			elem = elem->prev;
		return (*this);
	}

	UnorderedMapIterator<_KeyType, _DataType> operator--(int) {
		if (elem != nullptr)
			elem = elem->prev;
		return (*this);
	}

	_DataType& operator*() {
		if (elem == nullptr)
			throw NoValueException();
		return elem->value.second;
	}

	const UnorderedMapIterator<_KeyType, _DataType>& operator=(const UnorderedMapIterator<_KeyType, _DataType>& in_iter) {
		elem = in_iter.elem;
		return (*this);
	}

	bool operator==(const UnorderedMapIterator<_KeyType, _DataType>& in_iter) {
		return (elem == in_iter.elem);
	}

	bool operator!=(const UnorderedMapIterator<_KeyType, _DataType>& in_iter) {
		return (elem != in_iter.elem);
	}

	Node<Pair>* operator->() {
		return elem;
	}
};

