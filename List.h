#pragma once
#include "NoValueException.h"

template <class _T>
struct Node {

	_T value;
	Node* next;
	Node* prev;

	Node(const _T& in_value = _T())
		: value(in_value), next(nullptr), prev(nullptr) {}

	Node(_T&& in_value) 
		: value(std::move(in_value)), next(nullptr), prev(nullptr) {
		in_value = _T();
	}
};

template<class _T>
class List {

private:

	Node<_T>* first;
	Node<_T>* last;

public:
	
	List() 
		: first(nullptr), last(nullptr) {}

	List(const List<_T>& in_list) {
		Node<_T>* buffer = first, * in_buffer = in_list.first;
		if (in_buffer != nullptr) {
			buffer = new Node<_T>(in_buffer->value);
			buffer->prev = nullptr;
			while (in_buffer->next != nullptr) {
				buffer->next = new Node<_T>(in_buffer->value);
				buffer->next->prev = buffer;
				buffer = buffer->next;
				in_buffer = in_buffer->next;
			}
			last = buffer;
		}
	}

	List(List<_T>&& in_list) {
		first = in_list.first;
		last = in_list.last;
		in_list.first = nullptr;
		in_list.last = nullptr;
	}

	~List() { 
		if (this->first != nullptr) {
			while (first->next != nullptr) {
				first = first->next;
				delete first->prev;
			}
			delete first;
		}
	}

	Node<_T>* get_first() const {
		return first;
	}

	List<_T>& operator=(const List<_T>& in_list) {
		if (this == &in_list)
			return (*this);
		clear();
		Node<_T>* buffer = first, * in_buffer = in_list.first;
		if (in_buffer != nullptr) {
			buffer = new Node<_T>(in_buffer->value);
			buffer->prev = nullptr;
			while (in_buffer->next != nullptr) {
				buffer->next = new Node<_T>(in_buffer->value);
				buffer->next->prev = buffer;
				buffer = buffer->next;
				in_buffer = in_buffer->next;
			}
			last = buffer;
		}
		return (*this);
	}

	List<_T>& operator=(List<_T>&& in_list) {
		if (this == &in_list)
			return (*this);
		clear();
		first = in_list.first;
		last = in_list.last;
		in_list.first = nullptr;
		in_list.last = nullptr;
		return (*this);
	}

	Node<_T>* push_forward(const _T& in_value) {
		if (first == nullptr)
			return push_back(in_value);
		Node<_T>* buffer = new Node<_T>(in_value);
		first->prev = buffer;
		buffer->next = first;
		first = buffer;
		return first;
	}

	Node<_T>* push_forward(_T&& in_value) {
		if (first == nullptr)
			return push_back(std::move(in_value));
		Node<_T>* buffer = new Node<_T>(std::move(in_value));
		first->prev = buffer;
		buffer->next = first;
		first = buffer;
		return first;
	}

	Node<_T>* push_back(const _T& in_value) {
		if (last == nullptr) {
			first = new Node<_T>(in_value);
			last = first;
		}
		else {
			Node<_T>* buffer = new Node<_T>(in_value);
			last->next = buffer;
			buffer->prev = last;
			last = buffer;
		}
		return last;
	}

	Node<_T>* push_back(_T&& in_value) {
		if (last == nullptr) {
			first = new Node<_T>(std::move(in_value));
			last = first;
		}
		else {
			Node<_T>* buffer = new Node<_T>(std::move(in_value));
			last->next = buffer;
			buffer->prev = last;
			last = buffer;
		}
		return last;
	}

	Node<_T>* add_after(Node<_T>* in_node, const _T& in_value) {
		if (in_node == last)
			return push_back(in_value);
		Node<_T>* buffer = new Node<_T>(in_value);
		buffer->next = in_node->next;
		buffer->prev = in_node;
		in_node->next->prev = buffer;
		in_node->next = buffer;
		return buffer;
	}

	Node<_T>* add_after(Node<_T>* in_node, _T&& in_value) {
		if (in_node == last)
			return push_back(std::move(in_value));
		Node<_T>* buffer = new Node<_T>(std::move(in_value));
		buffer->next = in_node->next;
		buffer->prev = in_node;
		in_node->next->prev = buffer;
		in_node->next = buffer;
		return buffer;
	}

	void erase(Node<_T>* in_node) {
		if (in_node == first) {
			if (first->next == nullptr) {
				delete first;
				first = nullptr;
			}
			else {
				first = first->next;
				delete first->prev;
				first->prev = nullptr;
			}
			return;
		}
		if (in_node == last) {
			last = last->prev;
			delete last->next;
			last->next = nullptr;
			return;
		}
		in_node->prev->next = in_node->next;
		in_node->next->prev = in_node->prev;
		delete in_node;
	}

	void clear() {
		if (first != nullptr) {
			while (first->next != nullptr) {
				first = first->next;
				delete first->prev;
				first->prev = nullptr;
			}
			delete first;
			first = nullptr;
		}
	}
};