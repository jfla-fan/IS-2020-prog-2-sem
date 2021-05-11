#ifndef _PREDICATE_HPP_
#define _PREDICATE_HPP_

#include <functional>
#include <iterator>

template<class InputIt, class UnaryPredicate>
bool allOf(InputIt first, InputIt last, UnaryPredicate predicate)
{
    while (first != last)
    {
        if (!predicate(*first))
        {
            return false;
        }
        ++first;
    }

    return true;
}

template<class InputIt, class UnaryPredicate>
bool anyOf(InputIt first, InputIt last, UnaryPredicate predicate)
{
    for (; first != last; ++first)
    {
        if (predicate(*first))
            return true;
    }

    return false;
}

template<class InputIt, class UnaryPredicate>
bool noneOf(InputIt first, InputIt last, UnaryPredicate predicate)
{
    for (; first != last; ++first)
        if (predicate(*first))
            return false;

    return true;
}

template<class InputIt, class UnaryPredicate>
bool oneOf(InputIt first, InputIt last, UnaryPredicate predicate)
{
    bool exist = false;

    for (; first != last; ++first)
    {
        if (predicate(*first))
        {
            if (!exist)
            {
                exist = true;
                continue;
            }
            else
            {
                return false;
            }
        }
    }
        
    return exist;
}

//todo use default template argument
template<class ForwardIt, class Compare>
bool isSorted(ForwardIt first, ForwardIt last, Compare comparator)
{
    if (first != last)
    {
        ForwardIt next = first;
        
        while (++next != last)
        {
            if (comparator(*next, *first))
                return false;
            
            first = next;
        }

    }

    return true;
}

template<class ForwardIt>
bool isSorted(ForwardIt first, ForwardIt last)
{
    return isSorted(first, last, std::less<>());
}

template<class InputIt, class UnaryPredicate>
bool isPartitioned(InputIt first, InputIt last, UnaryPredicate predicate)
{
    for (; first != last; ++first)
        if (!predicate(*first))
            break;

    for (; first != last; ++first)
        if (predicate(*first))
            return false;

    return true;
}

template<class InputIt, class ValueT, class Compare>
InputIt findNot(InputIt first, InputIt last, const ValueT& value, Compare comp)
{
    for (; first != last; ++first)
    {
        if (!comp(*first, value))
            return first;
    }

    return last;
}


template<class InputIt, class ValueT>
InputIt findNot(InputIt first, InputIt last, const ValueT& value)
{
    return findNot(first, last, value, std::equal_to<ValueT>());
}


template<class ForwardIt, class ValueT, class Compare>
ForwardIt findBackward(ForwardIt first, ForwardIt last, const ValueT& value, Compare comp)
{
    ForwardIt result = last;

    for (; first != last; ++first)
    {
        if (comp(*first, value))
            result = first;
    }

    return result;
}

template<class ForwardIt, class ValueT>
ForwardIt findBackward(ForwardIt first, ForwardIt last, const ValueT& value)
{
    return findBackward(first, last, value, std::equal_to<ValueT>());   
}

template<class ForwardIt, class Compare>
bool isPalindrome(ForwardIt first, ForwardIt last, Compare comp)
{
    int dist = std::distance(first, last);
    ForwardIt firstCopy, lastCopy;
    for (int i = 0; i < dist/2; ++i)
    {
        firstCopy = first;
        lastCopy = last;
        std::advance(firstCopy, i);
        std::advance(lastCopy, -i - 1);
        if (!comp(*firstCopy, *lastCopy))
            return false;
    }

    return true;
}

template<class ForwardIt>
bool isPalindrome(ForwardIt first, ForwardIt last)
{
    return isPalindrome(first, last, std::less<>());
}

#endif  // _PREDICATE_HPP_