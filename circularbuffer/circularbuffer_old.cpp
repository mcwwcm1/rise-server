// File: circular_buffer.cpp
// Purpose: Implements circular_buffer.h

#include "circularbuffer.h"

explicit CircularBuffer::CircularBuffer(size_t size) : buf_(std::unique_ptr<T[]>(new T[size])), max_size_(size)
{
	head_ = 0;
	tail_ = 0;
	full_ = 0;
};

void CircularBuffer::put(T item)
{
	std::lock_guard<std::mutex> lock(mutex_);

	buf_[head_] = item;

	if(full_)
	{
		tail_ = (tail_ + 1) % max_size_;
	}

	head_ = (head_ + 1) % max_size_;

	full_ = head_ == tail_;
};

T CircularBuffer::get()
{
	std::lock_guard<std::mutex> lock(mutex_);

	if(empty())
	{
		return T();
	}

	//Read data and advance the tail (we now have a free space)
	auto val = buf_[tail_];
	full_ = false;
	tail_ = (tail_ + 1) % max_size_;

	return val;
};

void CircularBuffer::reset()
{
	std::lock_guard<std::mutex> lock(mutex_);
	head_ = tail_;
	full_ = false;
};

bool CircularBuffer::empty()
{
	//if head and tail are equal, we are empty
	return (!full_ && (head_ == tail_));
};

bool CircularBuffer::full()
{
	//If tail is ahead the head by 1, we are full
	return full_;
};

size_t CircularBuffer::capacity() const
{
	return max_size_;
};

size_t CircularBuffer::size() const
{
	size_t size = max_size_;

	if(!full_)
	{
		if(head_ >= tail_)
		{
			size = head_ - tail_;
		}
		else
		{
			size = max_size_ + head_ - tail_;
		}
	}

	return size;
};
