#pragma once

template <class _T>
class insert_iterator {

	_T* container_ptr;

public:

	using container_type = _T;

	insert_iterator()
		: container_ptr(nullptr) {}

	insert_iterator(_T* in_container_ptr)
		: container_ptr(in_container_ptr) {}

	insert_iterator<_T>& operator*() {
		return (*this);
	}

	insert_iterator<_T>& operator++(int) const {
		return (*this);
	}

	insert_iterator<_T>& operator++() const {
		return (*this);
	}

	_T::iterator& operator=(const typename _T::value_type& in_value) {
		return container_ptr->insert(in_value);
	}

	_T::iterator& operator=(typename _T::value_type&& in_value) {
		return container_ptr->insert(std::move(in_value));
	}

	bool operator==(const insert_iterator<_T>& in_iter) {
		return (container_ptr == in_iter.container_ptr);
	}

	bool operator!=(const insert_iterator<_T>& in_iter) {
		return (container_ptr != in_iter.container_ptr);
	}
};