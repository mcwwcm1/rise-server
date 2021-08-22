#pragma once

#include <memory>

template <class T>
class CircularBuffer
{
 public:
	explicit CircularBuffer(size_t size)
			: _buf(std::unique_ptr<T[]>(new T[size])), _maxSize(size)
	{
	}

	void Put(T item)
	{
		_buf[_head] = item;
		if (_full) { _tail = (_tail + 1) % _maxSize; }
		_head = (_head + 1) % _maxSize;
		_full = _head == _tail;
	}

	T Get()
	{
		if (Empty()) { return T(); }

		//Read data and advance the tail (we now have a free space)
		auto val = _buf[_tail];
		_full    = false;
		_tail    = (_tail + 1) % _maxSize;

		return val;
	}

	void Reset()
	{
		_head = _tail;
		_full = false;
	}

	bool Empty() const
	{
		//if head and tail are equal, we are empty
		return (!_full && (_head == _tail));
	}

	bool Full() const
	{
		//If tail is ahead the head by 1, we are full
		return _full;
	}

	size_t Capacity() const { return _maxSize; }

	size_t Size() const
	{
		size_t size = _maxSize;

		if (!_full) {
			if (_head >= _tail) {
				size = _head - _tail;
			} else {
				size = _maxSize + _head - _tail;
			}
		}

		return size;
	}

 private:
	std::unique_ptr<T[]> _buf;
	size_t _head = 0;
	size_t _tail = 0;
	const size_t _maxSize;
	bool _full = 0;
};
