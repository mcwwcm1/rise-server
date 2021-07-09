// File: circular_buffer.h
// Purpose: Defines a circular buffer for use in the websocket input stage

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <memory>
#include <mutex>

template <class T>
class CircularBuffer {
private:
	std::mutex mutex_;
	std::unique_ptr<T[]> buf_;
	size_t head_;
	size_t tail_;
	const size_t max_size_;
	bool full_;

public:
	explicit CircularBuffer(size_t size) :
		buf_(std::unique_ptr<T[]>(new T[size])),
		max_size_(size) {};
	void put(T item);
	T get();
	void reset();
	bool empty() const;
	bool full() const;
	size_t capacity();
	size_t size();
};



#endif
