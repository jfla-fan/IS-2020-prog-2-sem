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

//fixed use default template argument
template<class ForwardIt, class Compare = std::less<>>
bool isSorted(ForwardIt first, ForwardIt last, Compare comparator = std::less<>())
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

template<class InputIt, class ValueT, class Compare = std::equal_to<ValueT>>
InputIt findNot(InputIt first, InputIt last, const ValueT& value, Compare comp = std::equal_to<ValueT>())
{
    for (; first != last; ++first)
    {
        if (!comp(*first, value))
            return first;
    }

    return last;
}

template<class ForwardIt, class ValueT, class Compare = std::equal_to<ValueT>>
ForwardIt findBackward(ForwardIt first, ForwardIt last, const ValueT& value, Compare comp = std::equal_to<ValueT>())
{
    ForwardIt result = last;

    for (; first != last; ++first)
    {
        if (comp(*first, value))
            result = first;
    }

    return result;
}

template<class ForwardIt, class Compare = std::equal_to<>>
bool isPalindrome(ForwardIt first, ForwardIt last, Compare comp = std::equal_to<>())
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

#endif  // _PREDICATE_HPP_