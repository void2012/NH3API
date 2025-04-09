//===----------------------------------------------------------------------===//
//
// Part of the NH3API, under the Apache License v2.0.
// SPDX-License-Identifier: Apache-2.0
//
//===----------------------------------------------------------------------===//
#pragma once

#include "nh3api_exceptions.hpp"

// NH3API normally implement STL things in its own namespace or use 'using ::std::xyz'
// std::array is an exception

#if !(NH3API_STD_ARRAY)
#include "algorithm.hpp"
namespace std // C++98 <array> implementation
{
    /// @brief C++11 array implementation for pre-C++11 compiler
    /// @tparam T values of type <T> to store
    /// @tparam N number of values to store
    template<typename T, size_t N>
    struct array
    {
        // member type alias
        public:
            typedef T    	    			         value_type;
            typedef value_type*			             pointer;
            typedef const value_type*                const_pointer;
            typedef value_type&                   	 reference;
            typedef const value_type&             	 const_reference;
            typedef value_type*          		     iterator;
            typedef const value_type*			     const_iterator;
            typedef size_t                    	     size_type;
            typedef ptrdiff_t                   	 difference_type;
            typedef reverse_iterator<iterator>	     reverse_iterator;
            typedef reverse_iterator<const_iterator> const_reverse_iterator;

        public:
            /// @brief fills array with values <arg>
            /// @param arg value to fill array with
            void fill(const value_type & arg)
            { nh3api::fill(begin(), end(), arg); }

            /// @brief
            /// @param other
            void swap(array & other)
            { std::swap_ranges(begin(), end(), other.begin()); }

            /// @brief
            /// @return iterator to the beginning
            iterator begin() throw()
            { return iterator(data()); }

            /// @brief const
            /// @return const_iterator to the beginning
            const_iterator begin() const throw()
            { return const_iterator(data()); }

            /// @brief
            /// @return iterator to the back() + 1
            iterator end() throw()
            { return iterator(data() + N); }

            /// @brief const
            /// @return const_iterator to the back() + 1
            const_iterator end() const throw()
            { return const_iterator(data() + N); }

            /// @brief
            /// @return reverse_iterator to the end()
            reverse_iterator rbegin() throw()
            { return reverse_iterator(end()); }

            /// @brief const
            /// @return const_reverse_iterator to the end()
            const_reverse_iterator rbegin() const throw()
            { return const_reverse_iterator(end()); }

            /// @brief
            /// @return reverse_iterator to the begin()
            reverse_iterator rend() throw()
            { return reverse_iterator(begin()); }

            /// @brief const
            /// @return const_reverse_iterator to the begin()
            const_reverse_iterator rend() const throw()
            { return const_reverse_iterator(begin()); }

            /// @brief const
            /// @return
            const_iterator cbegin() const throw()
            { return const_iterator(data()); }

            /// @brief const
            /// @return
            const_iterator cend() const throw()
            { return const_iterator(data() + N); }

            /// @brief const
            /// @return
            const_reverse_iterator crbegin() const throw()
            { return const_reverse_iterator(end()); }

            /// @brief const
            /// @return
            const_reverse_iterator crend() const throw()
            { return const_reverse_iterator(begin()); }

            /// @brief
            /// @return
            size_type size() const throw()
            { return N; }

            /// @brief
            /// @return
            size_type max_size() const  throw()
            { return N; }

            /// @brief
            /// @return
            bool empty() const  throw()
            { return N == 0; }

            /// @brief
            /// @param pos
            /// @return
            reference operator[](size_type pos)
            #if !NH3API_DEBUG
            throw()
            #endif
            {
            #if !NH3API_DEBUG
                return _data[pos];
            #else
                return at(pos);
            #endif
            }

            /// @brief
            /// @param pos
            /// @return
            const_reference operator[](size_type pos) const
            #if !NH3API_DEBUG
            throw()
            #endif
            {
            #if !NH3API_DEBUG
                return _data[pos];
            #else
                return at(pos);
            #endif
            }

            /// @brief
            /// @param pos
            /// @return
            reference at(size_type pos) throw(std::out_of_range)
            {
                return  ( size() < pos ) ? _data[pos] :
                (NH3API_THROW(std::out_of_range, "std::array::at pos out of range."), _data[pos]);
            }

            /// @brief
            /// @param pos
            /// @return
            const_reference at(size_type pos) const throw(std::out_of_range)
            {
                return  ( size() < pos ) ? _data[pos] :
                (NH3API_THROW(std::out_of_range, "std::array::at pos out of range."), _data[pos]);
            }

            /// @brief access the beginning of the array
            /// @return reference to the beginning
            reference front() throw()
            { return _data[0]; }

            /// @brief access the beginning of the array
            /// @return const_reference to the beginning
            const_reference front() const throw()
            { return _data[0]; }

            /// @brief access the end of the array
            /// @return reference to the end
            reference back() throw()
            { return _data[N - 1]; }

            /// @brief access the end of the array
            /// @return const_reference to end
            const_reference back() const throw()
            { return _data[N - 1]; }

            /// @brief
            /// @return underlying data
            pointer data() throw()
            { return _data; }

            /// @brief
            /// @return underlying data
            const_pointer data() const throw()
            { return _data; }

        protected:

            value_type _data[N];
    }; // class std::array<T, N>

    template<typename T, size_t N>
    inline void swap(array<T, N> & left, array<T, N> & right)
    { left.swap(right); }

} // C++98 <array> implementation

template<typename T, size_t N>
inline bool operator==(const std::array<T, N> & left, const std::array<T, N> & right)
{ return std::equal(left.begin(), left.end(), right.begin()); }

template<typename T, size_t N>
inline bool operator!=(const std::array<T, N> & left, const std::array<T, N> & right)
{ return !(left == right); }

namespace nh3api
{
template<class T, size_t N>
size_t size(const T (&array)[N]) throw()
{ return N; }

template<class T>
typename T::size_type size(const T& container)
{ return container.size(); }

template< class T, size_t N >
T* begin(T (&array)[N]) throw()
{ return array; }

template<class T>
typename T::iterator begin(T& container)
{ return container.begin(); }

template<class T>
typename T::const_iterator begin(const T& container)
{ return container.begin(); }

template<class T, size_t N>
T* end(T (&array)[N]) throw()
{ return array + N; }

template<class T>
typename T::iterator end(T& container)
{ return container.end(); }

template<class T>
typename T::const_iterator end(const T& container)
{ return container.end(); }

} // namespace nh3api

#else // !(NH3API_STD_ARRAY)
#include <array>

namespace nh3api
{
using ::std::size;
using ::std::begin;
using ::std::end;

} // namespace nh3api
#endif
