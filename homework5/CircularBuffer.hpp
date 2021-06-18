#include <cstddef>
#include <iterator>
#include <cstring>


//fixed warnings: no need to get rid of -Wdeprecated-copy warnings, the others are done
template<typename ValueT>
class circular_buffer_iterator;

template<typename ValueT>
using circular_buffer_const_iterator = circular_buffer_iterator<const ValueT>;


//fixed use size_t index and length in exception output
class circular_buffer_exception : public std::exception
{
    virtual const char* what() const noexcept override { return "circular_buffer_exception"; }
};

class out_of_range : public circular_buffer_exception
{
public:
    out_of_range(std::size_t where, int length) : circular_buffer_exception(), _index(where), _length(length) { }

    virtual const char* what() const noexcept override 
    {
        char* message = new char[100];
        std::sprintf(message, "circular_buffer: out of range (index = %lu, length = %d)", _index, _length);
        return message; 
    }

protected:
    std::size_t _index;
    int _length;
};

template<
    typename ValueT,
    typename AllocatorT = std::allocator<ValueT> >
class CircularBuffer
{
public:

    using size_type                 = std::size_t;
    using value_type                = ValueT;
    using allocator_type            = AllocatorT;
    using pointer                   = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer             = typename std::allocator_traits<allocator_type>::const_pointer;
    using reference                 = value_type&;
    using const_reference           = const value_type&;
    using iterator                  = circular_buffer_iterator<ValueT>;
    using const_iterator            = circular_buffer_const_iterator<ValueT>;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;

public:

    explicit CircularBuffer();
    
    CircularBuffer(size_type);
    CircularBuffer(const CircularBuffer&);

    CircularBuffer& operator = (const CircularBuffer&);
    
    void changeCapacity(int);

    ~CircularBuffer();

    void addFirst(const value_type&);
 
    void addLast(const value_type&);
 
    void delFirst();
 
    void delLast();

    value_type first() const;

    value_type last() const;

    // selectors
    size_type size() const;
    bool empty() const;

    inline reference operator [] (int);
    inline const_reference operator [] (int) const;

    // iterators
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator crend() const;

protected:

    inline int _increment(int) const;
    inline int _decrement(int) const;


public:

    pointer buffer;

    int length;

    int start_pos, end_pos;

    int capacity;

};

template<typename ValueT>
class circular_buffer_iterator
{
protected:

    using self = circular_buffer_iterator<ValueT>;
    using self_reference = self&;

public:

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = ValueT;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using reference = value_type&;
    using const_reference = const reference;

    circular_buffer_iterator() : _buffer(nullptr), _length(-1), _pos(-1) { }

    circular_buffer_iterator(pointer buffer, int pos, int length) : _buffer(buffer), _length(length), _pos(pos) { }

    ~circular_buffer_iterator() = default;

    inline pointer data() const { return &_buffer[_pos]; }

    inline reference operator * () const { return _buffer[_pos]; }

    inline bool operator != (const circular_buffer_iterator& another) const { return data() != another.data(); }

    inline bool operator == (const circular_buffer_iterator& another) const { return !(*this != another); }

    inline bool operator < (const circular_buffer_iterator& another) const { return operator*() < *another; }

    self_reference operator = (const self_reference another) { _buffer = another._buffer; _length = another._length; _pos = another._pos; return *this; }

    self_reference operator++() { _pos = (_pos + 1) % _length; return *this; }
    self operator++(int) { self tmp = self(*this); ++(*this); return tmp; }

    self_reference operator--() { _pos = (_pos + _length - 1) % _length; return *this; }
    self operator--(int) { self tmp = self(*this); --(*this); return tmp; }

    self operator + (difference_type n) { return self(*this) += n; }
    self_reference operator += (difference_type n) { _pos = (_pos + n) % _length; return *this; }

    self operator - (difference_type n) { return self(*this) -= n; }
    self_reference operator -= (difference_type n) { for (int i = 0; i < n; ++i) operator--(); return *this; }

    int operator - (const self_reference another) { return data() - another.data(); }
    int operator + (const self_reference another) { return data() + another.data(); }


private:

    pointer _buffer;

    int _length;

    int _pos;

};


template<typename ValueT, typename AllocatorT> int CircularBuffer<ValueT, AllocatorT>::_increment(int pos) const
{
    return (pos + 1) % length;
}

template<typename ValueT, typename AllocatorT> int CircularBuffer<ValueT, AllocatorT>::_decrement(int pos) const
{
    if (pos - 1 < 0) return length - 1;
    return pos - 1;
}

template<typename ValueT, typename AllocatorT> CircularBuffer<ValueT, AllocatorT>::CircularBuffer()
    : start_pos(0), end_pos(1), length(1), capacity(0)
{
    buffer = new value_type[length];
}

template<class ValueT, class AllocatorT> CircularBuffer<ValueT, AllocatorT>::CircularBuffer(size_type size)
    : length(size + 1), start_pos(0), end_pos(1), capacity(0)
{
    buffer = new value_type[length];
}

template<class ValueT, class AllocatorT> CircularBuffer<ValueT, AllocatorT>::CircularBuffer(const CircularBuffer<ValueT, AllocatorT>& another)
    : length(another.length), start_pos(another.start_pos), end_pos(another.end_pos)
{
    buffer = new value_type[length];
    std::memcpy(buffer, another.buffer, length);
}


template<class ValueT, class AllocatorT> CircularBuffer<ValueT, AllocatorT>::~CircularBuffer()
{
    delete[] buffer;
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::reference CircularBuffer<ValueT, AllocatorT>::operator [] (int index)
{ 
    if (index + 1 > capacity) throw out_of_range(index, capacity);
    int where = (start_pos + index + 1) % length;
    return buffer[where]; 
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::const_reference CircularBuffer<ValueT, AllocatorT>::operator [] (int index) const
{ 
    if (index + 1 > capacity) throw out_of_range(index, capacity);
    int where = (start_pos + index + 1) % length;
    return buffer[where]; 
}


template<class ValueT, class AllocatorT> CircularBuffer<ValueT, AllocatorT>& CircularBuffer<ValueT, AllocatorT>::operator = (const CircularBuffer<ValueT, AllocatorT>& another)
{
    if (this == &another) return *this;

    length = another.length;
    start_pos = another.start_pos;
    end_pos = another.end_pos;
    buffer = new value_type[length];
    std::memcpy(buffer, another.buffer, length);

    return *this;
}

template<typename ValueT, typename AllocatorT> void CircularBuffer<ValueT, AllocatorT>::changeCapacity(int new_size)
{
    if (new_size == length - 1) return;
    else if (new_size < length - 1)
    {
        return;
    }
    else
    {
        pointer new_buffer = new value_type[new_size + 1];
        int pos = 1;
        for (auto it = begin(); it != end(); ++it)
        {
            new_buffer[pos] = *it;
            pos = (pos + new_size + 2) % (new_size + 1);
        }
    
        delete[] buffer;
        end_pos = pos;
        start_pos = 0;
        buffer = new_buffer;
        length = new_size + 1;
    }
}

template<class ValueT, class AllocatorT> void CircularBuffer<ValueT, AllocatorT>::addFirst(const value_type& value)
{
    if (capacity >= length - 1) 
    {
        end_pos = _decrement(end_pos);
        --capacity;
    }
    buffer[start_pos] = value;
    start_pos = _decrement(start_pos);
    ++capacity;
}

template<class ValueT, class AllocatorT> void CircularBuffer<ValueT, AllocatorT>::addLast(const value_type& value)
{
    if (capacity >= length - 1) 
    {
        start_pos = _increment(start_pos);
        --capacity;
    }
    buffer[end_pos] = value;
    end_pos = _increment(end_pos);
    ++capacity;
}

template<class ValueT, class AllocatorT> void CircularBuffer<ValueT, AllocatorT>::delFirst()
{
    if (capacity == 0) throw out_of_range(0, 0);
    --capacity;
    start_pos = _increment(start_pos);
}

template<class ValueT, class AllocatorT> void CircularBuffer<ValueT, AllocatorT>::delLast()
{
    if (capacity == 0) throw out_of_range(0, 0);
    --capacity;
    end_pos = _decrement(end_pos);
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::value_type CircularBuffer<ValueT, AllocatorT>::first() const
{
    if (capacity == 0) throw out_of_range(0, 0);
    return buffer[_increment(start_pos)];
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::value_type CircularBuffer<ValueT, AllocatorT>::last() const
{
    if (capacity == 0) throw out_of_range(0, 0);
    return buffer[_decrement(end_pos)];
}


// iterators

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::iterator CircularBuffer<ValueT, AllocatorT>::begin()
{
    return iterator(buffer, _increment(start_pos), length);
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::const_iterator CircularBuffer<ValueT, AllocatorT>::begin() const
{
    return const_iterator(buffer, _increment(start_pos), length);
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::const_iterator CircularBuffer<ValueT, AllocatorT>::cbegin() const
{
    return const_iterator(buffer, _increment(start_pos), length);
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::iterator CircularBuffer<ValueT, AllocatorT>::end()
{
    return iterator(buffer, end_pos, length);       
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::const_iterator CircularBuffer<ValueT, AllocatorT>::end() const
{
    return const_iterator(buffer, end_pos, length);
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::const_iterator CircularBuffer<ValueT, AllocatorT>::cend() const
{
    return const_iterator(buffer, end_pos, length);      
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::reverse_iterator CircularBuffer<ValueT, AllocatorT>::rbegin()
{
    return reverse_iterator(buffer, _increment(start_pos), length);
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::const_reverse_iterator CircularBuffer<ValueT, AllocatorT>::crbegin() const
{
    return const_reverse_iterator(buffer, _increment(start_pos), length);
}


template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::reverse_iterator CircularBuffer<ValueT, AllocatorT>::rend()
{
    return reverse_iterator(buffer, end_pos, length);
}

template<class ValueT, class AllocatorT> typename CircularBuffer<ValueT, AllocatorT>::const_reverse_iterator CircularBuffer<ValueT, AllocatorT>::crend() const
{
    return const_reverse_iterator(buffer, end_pos, length);
}
