#ifndef SET_H
#define SET_H

#include "Hash.h"

/**
 * Set template.
 *
 * I ended up never using this one....
 */
template<typename T>
class Set
{
public:
	inline void insert(const T& val)
		{ this->hash.insert(val, DummyHashValue()); }
	inline bool contains(const T& val) const
		{ return this->hash.hasKey(val); }
	inline void remove(const T& val)
		{ this->hash.remove(val); }

private:
	struct DummyHashValue {};

	Hash<T, DummyHashValue> hash;
};

#endif /* SET_H */
