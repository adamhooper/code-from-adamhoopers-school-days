#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <cstring>

/**
 * A generic vector.
 *
 * The methods need no explanation. at() and operator[] are accessors (at() is
 * faster because it doesn't need a bounds check). ensureCapacity() allocates
 * space manually (it will be allocated as needed, anyway). contains(),
 * indexOf(), append(), clear(), and getSize() hold no surprises.
 */
template<typename T>
class List
{
public:
	List<T>() : size(0), capacity(0), data(0) {}
	List<T>(const List<T>& rhs);
	List<T>& operator=(const List<T>& rhs);
	~List<T>() { delete [] this->data; }

	inline void ensureCapacity(int capacity);

	inline T at(int i) const { return this->data[i]; }

	inline bool contains(const T& val) const
	{
		return this->indexOf(val) != -1;
	}

	inline int indexOf(const T& val) const
	{
		for (int i = 0; i < this->size; i++)
		{
			if (this->at(i) == val)
			{
				return i;
			}
		}
		return -1;
	}

	inline T& operator[](int i)
	{
		this->ensureCapacity(i + 1);
		if (this->size <= i) this->size = i + 1;
		return this->data[i];
	}

	inline void append(const T& v) { this->operator[](this->size) = v; }
	inline void clear()
	{
		delete [] this->data;
		this->size = 0;
		this->capacity = 0;
		this->data = 0;
	}
	inline int getSize() const { return this->size; }

private:
	template<typename U> friend std::ostream& operator<<
		(std::ostream& os, const List<U> &list);
inline int nextPower(int n) const;

	int size;
	int capacity;
	T *data;
};

template<typename T>
List<T>::List(const List<T>& rhs)
	: size(rhs.size)
	, capacity(rhs.capacity)
{
	if (rhs.data != 0)
	{
		this->data = new T[this->capacity];
		memcpy(this->data, rhs.data, this->size * sizeof(T));
	}
	else
	{
		this->data = 0;
	}
}

template<typename T>
List<T>&
List<T>::operator=(const List<T>& rhs)
{
	if (this->data != 0 && this->data == rhs.data) return *this;

	this->size = rhs.size;
	this->capacity = rhs.capacity;

	if (this->data != 0) delete [] this->data;

	if (rhs.data != 0)
	{
		this->data = new T[this->capacity];
		memcpy(this->data, rhs.data,
		       this->size * sizeof(T));
	}
	else
	{
		this->data = 0;
	}

	return *this;
}

template<typename T>
int
List<T>::nextPower(int n) const
{
	int i;

	for (i = 1 << 4; i < n; i <<= 1)
	{
		// no-op
	}

	return i;
}

template<typename T>
void
List<T>::ensureCapacity(int capacity)
{
	if (this->capacity >= capacity) return;

	this->capacity = this->nextPower(capacity);

	T *oldData(this->data);
	this->data = new T[this->capacity];

	if (oldData != 0)
	{
		memcpy(this->data, oldData, this->size * sizeof(T));
		delete [] oldData;
	}
}

template<typename T>
std::ostream&
operator<<(std::ostream& os,
	   const List<T>& list)
{
	os << "List [";

	for (int i = 0; i < list.getSize(); i++)
	{
		os << " " << list.at(i);
	}

	os << " ]";

	return os;
}

#endif /* LIST_H */
