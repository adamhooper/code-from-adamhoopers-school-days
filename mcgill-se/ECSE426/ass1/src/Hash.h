#ifndef HASH_H
#define HASH_H

#include <cstring>
#include <exception>

/**
 * Hash table template.
 *
 * I ended up never using this one.... It seemed to have tempting potential for
 * optimizations, but none of them worked.
 */
template<typename K, typename V>
class Hash
{
public:
	Hash() : data(0), capacity(0), nNodes(0) {}
	~Hash();

	inline bool hasKey(const K& key) const;
	inline V at(const K& key) const;
	inline V& insert(const K& key, const V& val);
	inline V& operator[](const K& key);
	inline V remove(const K& key);

private:
	inline bool needToGrow() const;
	inline int getNextPrime() const;
	void grow();

	struct Node {
		Node(K key, V val, Node *next = 0)
			: key(key), val(val), next(next) {}

		K key;
		V val;
		Node *next;
	};

	Node **data;
	int capacity;
	int nNodes;
};

template<typename K, typename V>
Hash<K,V>::~Hash()
{
	for (int i = 0; i < this->capacity; i++)
	{
		Node *n = data[i];
		while (n != 0)
		{
			Node *t = n;
			n = n->next;
			delete t;
		}
	}

	delete [] data;
}

template<typename K, typename V>
bool
Hash<K,V>::hasKey(const K& key) const
{
	if (this->capacity == 0) return false;

	unsigned int hash = key.hash() % this->capacity;

	for (Node *n = this->data[hash]; n != 0; n = n->next)
	{
		if (n->key == key)
		{
			return true;
		}
	}

	return false;
}

template<typename K, typename V>
V
Hash<K,V>::at(const K& key) const
{
	if (this->capacity == 0) return false;

	unsigned int hash = key.hash() % this->capacity;

	for (Node *n = this->data[hash]; n != 0; n = n->next)
	{
		if (n->key == key)
		{
			return n->val;
		}
	}

	return V();
}

template<typename K, typename V>
V&
Hash<K,V>::insert(const K& key,
		  const V& val)
{
	if (this->capacity == 0)
	{
		this->grow();
	}

	unsigned int hash = key.hash() % this->capacity;

	Node *prevNode = 0;
	for (Node *n = this->data[hash]; n != 0; n = n->next)
	{
		if (n->key == key)
		{
			return n->val;
		}

		prevNode = n;
	}

	if (this->needToGrow())
	{
		this->grow();
		// recurse
		return this->operator[](key);
	}

	this->nNodes++;

	if (prevNode == 0)
	{
		this->data[hash] = new Node(key, val);
		return this->data[hash]->val;
	}
	else
	{
		prevNode->next = new Node(key, val);
		return prevNode->next->val;
	}
}

template<typename K, typename V>
V&
Hash<K,V>::operator[](const K& key)
{
	return this->insert(key, V());
}

template<class K, class V>
bool
Hash<K,V>::needToGrow() const
{
	return (this->nNodes * 3 > this->capacity);
}

template<class K, class V>
int
Hash<K,V>::getNextPrime() const
{
	// Copied from GLib's gprimes.c, http://gtk.org
	static const int primes[] =
	{
		11,
		19,
		37,
		73,
		109,
		163,
		251,
		367,
		557,
		823,
		1237,
		1861,
		2777,
		4177,
		6247,
		9371,
		14057,
		21089,
		31627,
		47431,
		71143,
		106721,
		160073,
		240101,
		360163,
		540217,
		810343,
		1215497,
		1823231,
		2734867,
		4102283,
		6153409,
		9230113,
		13845163,
	};
	static const int nPrimes = sizeof(primes) / sizeof(primes[0]);

	for (int i = 0; i < nPrimes; i++)
	{
		if (primes[i] > this->capacity)
		{
			return primes[i];
		}
	}

	throw std::exception(); // out of space
}

template<class K, class V>
void
Hash<K,V>::grow()
{
	Node **oldData = this->data;
	int oldCapacity = this->capacity;

	this->nNodes = 0;
	this->capacity = this->getNextPrime();
	this->data = new Node*[this->capacity];
	memset(this->data, 0, this->capacity * sizeof(Node*));

	for (int i = 0; i < oldCapacity; i++)
	{
		Node *n = oldData[i];
		while (n != 0)
		{
			this->insert(n->key, n->val);
			Node *t = n;
			n = n->next;
			delete t;
		}
	}

	delete [] oldData;
}

template<typename K, typename V>
V
Hash<K,V>::remove(const K& key)
{
	if (this->capacity == 0) return V();

	unsigned int hash = key.hash() % this->capacity;

	for (Node *n = this->data[hash]; n != 0; n = n->next)
	{
		if (n->key == key)
		{
			V ret(n->val);

			if (n->next == 0)
			{
				this->data[hash] = 0;
			}
			else
			{
				n->next = n->next->next;
			}

			delete n;
			this->nNodes--;
			return ret;
		}
	}

	return V();
}

#endif /* HASH_H */
