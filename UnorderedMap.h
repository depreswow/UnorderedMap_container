#pragma once
#include "List.h"
#include <functional>
#include <utility>
#include <initializer_list>
#include "UnorderedMapIterator.h"
#include "BadLoadFactor.h"
#include "WrongSizeException.h"
#include "ValueExistsException.h"

template <class _KeyType, class _DataType>
class UnorderedMapInsertIterator;

template<class _KeyType, class _DataType, class _Hash = std::hash<_KeyType>>
class UnorderedMap {
public:

	using value_type = std::pair<_KeyType, _DataType>;
	using iterator = UnorderedMapIterator<_KeyType, _DataType>;

private:

	using Pair =  std::pair<_KeyType, _DataType>;

	List<Pair> elements;

	std::pair<iterator, iterator>* buckets;

	int size;
	double LoadFactor;
	int used_buckets;

	void check_LoadFactor() {
		if (used_buckets >= (size * LoadFactor))
			reserve(size+10);
	}

public:

	UnorderedMap() 
		: buckets(nullptr), 
		LoadFactor(1.0), 
		used_buckets(0), 
		size(0), 
		elements() {}

	UnorderedMap(int in_size) 
		: LoadFactor(1.0), 
		used_buckets(0), 
		size(in_size),
		elements() {

		buckets = new std::pair<iterator, iterator>[size];
	}

	template<class _Iterator>
	UnorderedMap(const _Iterator& in_beg, const _Iterator& in_end) 
		: size(10), 
		LoadFactor(1.0), 
		used_buckets(0), 
		elements() {

		buckets = new std::pair<iterator, iterator>[size];
		for (_Iterator iter = in_beg; iter != in_end; ++iter)
			insert(*iter);
	}

	template<>
	UnorderedMap(const iterator& in_beg, const iterator& in_end)
		: size(10),
		LoadFactor(1.0),
		used_buckets(0),
		elements() {

		buckets = new std::pair<iterator, iterator>[size];
		for (iterator iter = in_beg; iter != in_end; ++iter)
			insert(iter->value);
	}

	UnorderedMap(const std::initializer_list<Pair>& in_list) 
		: LoadFactor(1.0), 
		size(in_list.size()+1), 
		used_buckets(0), 
		elements() {

		buckets = new std::pair<iterator, iterator>[size];
		for (auto iter = in_list.begin(); iter != in_list.end(); ++iter)
			insert(*iter);
	}

	UnorderedMap(const UnorderedMap<_KeyType, _DataType, _Hash>& in_map) 
		: LoadFactor(in_map.LoadFactor), 
		size(in_map.size), 
		used_buckets(in_map.used_buckets), 
		elements() {

		buckets = new std::pair<iterator, iterator>[size];
		for (iterator iter = in_map.begin(); iter != in_map.end(); ++iter)
			insert(iter->value);
	}

	UnorderedMap(UnorderedMap&& in_map) 
		: LoadFactor(in_map.LoadFactor), 
		size(in_map.size), 
		used_buckets(in_map.used_buckets), 
		buckets(in_map.buckets), 
		elements(std::move(in_map.elements)) {

		in_map.LoadFactor = 1.0;
		in_map.size = 0;
		in_map.used_buckets = 0;
		in_map.buckets = nullptr;
	}

	~UnorderedMap() {
		delete[] buckets;
	}

	UnorderedMap<_KeyType, _DataType, _Hash>& operator=(const UnorderedMap<_KeyType, _DataType, _Hash>& in_map) {
		if (this == &in_map)
			return (*this);
		delete[] buckets;
		elements = in_map.elements;
		size = in_map.size;
		LoadFactor = in_map.LoadFactor;
		used_buckets = in_map.used_buckets;
		buckets = new std::pair<iterator, iterator>[size];
		int hash;
		for (iterator iter = begin(); iter != end(); ++iter) {
			hash = _Hash{}(iter->value->first) % size;
			if (buckets[hash].first == end()) {
				buckets[hash].first.operator->() = iter.operator->();
				buckets[hash].second = buckets[hash].first;
			}
			else
				++buckets[hash].second;
		}
		return (*this);
	}

	UnorderedMap<_KeyType, _DataType, _Hash>& operator=(const UnorderedMap<_KeyType, _DataType, _Hash>&& in_map) {
		if (this == &in_map)
			return (*this);
		delete[] buckets;
		LoadFactor = in_map.LoadFactor;
		size = in_map.size;
		used_buckets = in_map.used_buckets;
		buckets = in_map.buckets;
		elements = std::move(in_map.elements);
		in_map.LoadFactor = 1.0;
		in_map.size = 0;
		in_map.used_buckets = 0;
		in_map.buckets = nullptr;
		return (*this);
	}

	_DataType& operator[](const _KeyType& key) {
		int hash = _Hash{}(key) % size;
		for (iterator iter = buckets[hash].first; iter != (buckets[hash].second); ++iter) {
			if (iter->value.first == key)
				return (*iter);
		}
		if (buckets[hash].second->value.first == key)
			return *buckets[hash].second;
		throw NoValueException;
	}

	const double& maxLoadFactor() const {
		return LoadFactor;
	}
	void max_Load_Factor(double val) {
		if (val <= 0. || val > 1.0)
			throw BadLoadFactor(val);
		LoadFactor = val;
		check_LoadFactor();
	}

	const _DataType& at(const _KeyType& key) const {
		int hash = _Hash{}(key) % size;
		for (iterator iter = buckets[hash].first; iter != (buckets[hash].second); ++iter) {
			if (iter->value.first == key)
				return (*iter);
		}
		if (buckets[hash].second != end() && buckets[hash].second.first == key)
			return (*buckets[hash].second);
		throw NoValueException();
	}

	iterator begin() const {
		iterator iter(elements.get_first());
		return iter;
	}

	iterator end() const {
		iterator iter(nullptr);
		return iter;
	}

	iterator& insert(const Pair& pair) {
		int hash = _Hash{}(pair.first) % size;
		if (buckets[hash].second != end()) { //если в ячейке уже есть элементы
			for (iterator iter = buckets[hash].first; iter != buckets[hash].second; ++iter) {
				if (iter->value.first == pair.first)
					throw ValueExistsException();
			}
			elements.add_after(buckets[hash].second.operator->(), pair);
			++buckets[hash].second;
			return buckets[hash].second;
		}
		if (begin() == end()) { //если таблица пустая
			buckets[hash].first = elements.push_back(pair);
			buckets[hash].second = buckets[hash].first;
			used_buckets++;
			return buckets[hash].second;
		}
		for (int i = hash - 1; i >= 0; i--) { //ищет ближайшую предшествующую ячейку, в которой есть элементы
			if (buckets[i].second != end()) {
				buckets[hash].first = elements.add_after(buckets[i].second.operator->(), pair);
				buckets[hash].second = buckets[hash].first;
				used_buckets++;
				return buckets[hash].second;
			}
		}
		buckets[hash].first = elements.push_forward(pair); //иначе добавляет элемент в начало
		buckets[hash].second = buckets[hash].first;
		used_buckets++;
		return buckets[hash].second;
	}

	iterator& insert(Pair&& pair) {
		int hash = _Hash{}(pair.first) % size;
		if (buckets[hash].second != end()) { //если в ячейке уже есть элементы
			for (iterator iter = buckets[hash].first; iter != buckets[hash].second; ++iter) {
				if (iter->value.first == pair.first)
					throw ValueExistsException();
			}
			elements.add_after(buckets[hash].second.operator->(), std::move(pair));
			++buckets[hash].second;
			return buckets[hash].second;
		}
		if (begin() == end()) { //если таблица пустая
			buckets[hash].first = elements.push_back(std::move(pair));
			buckets[hash].second = buckets[hash].first;
			used_buckets++;
			return buckets[hash].second;
		}
		for (int i = hash - 1; i >= 0; i--) { //ищет ближайшую предшествующую ячейку, в которой есть элементы
			if (buckets[i].second != end()) {
				buckets[hash].first = elements.add_after(buckets[i].second.operator->(), pair);
				buckets[hash].second = buckets[hash].first;
				used_buckets++;
				return buckets[hash].second;
			}
		}
		buckets[hash].first = elements.push_forward(std::move(pair)); //иначе добавляет элемент в начало
		buckets[hash].second = buckets[hash].first;
		used_buckets++;
		return buckets[hash].second;
	}

	void erase(const _KeyType& key) {
		int hash = _Hash{}(key) % size;
		if ((*this)[key]) { //если элемент есть в таблице
			if (buckets[hash].first->value.first == key) { //если элемент первый в ячейке
				if (buckets[hash].second == buckets[hash].first) { //если в ячейке один элемент, очищает ячейку
					elements.erase(buckets[hash].first.operator->());
					buckets[hash].first = end();
					buckets[hash].second = buckets[hash].first;
					used_buckets--;
				}
				else {
					++buckets[hash].first;
					elements.erase(buckets[hash].first->prev);
				}
				return;
			}
			else {
				for (iterator iter = buckets[hash].first; iter != buckets[hash].second; ++iter) {
					if (iter->value.first == key) {
						elements.erase(iter.operator());
						return;
					}
				}
				if (buckets[hash].second->value.first == key) {
					--buckets[hash].second;
					elements.erase(buckets[hash].second->next);
					return;
				}
			}
		}
		throw NoValueException();
	}

	iterator erase(iterator& pos) {
		int hash = _Hash{}(pos->value.first) % size;
		iterator buffer(pos->next);
		if (buckets[hash].first->value.first == pos->value.first) { //если элемент первый в ячейке
			if (buckets[hash].second == buckets[hash].first) { //если в ячейке один элемент, очищает ячейку
				elements.erase(buckets[hash].first.operator->());
				buckets[hash].first = end();
				buckets[hash].second = buckets[hash].first;
				used_buckets--;
			}
			else {
				++buckets[hash].first;
				elements.erase(buckets[hash].first->prev);
			}
		}
		else {
			if (buckets[hash].second->value.first == pos->value.first) {
				--buckets[hash].second;
				elements.erase(buckets[hash].second->next);
			}
			else 
				elements.erase(pos.operator->());
		}
		return buffer;
	}

	void reserve(int num) {
		UnorderedMap<_KeyType, _DataType, _Hash> new_map(num);
		for (iterator iter = begin(); iter != end(); ++iter)
			new_map.insert(iter->value);
		(*this) = new_map;
	}

	bool empty() const {
		return (used_buckets == 0);
	}

	void clear() {
		elements.clear();
	}
};