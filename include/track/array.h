#pragma once

#ifndef TRACK_ARRAY_H
#define TRACK_ARRAY_H

#include "pointer.h"
#include "iterator.h"
#include "index.h"
#include <array>

namespace rsl
{
	namespace track
	{
        template<class T, std::size_t N>
        class array : public std::array<T, N>
        {
        public:
            typedef std::array<T, N> array_type;

            using array_type::array;
            using array_type::operator[];
            using array_type::operator=;

            const auto& get_trackable() const { return m_trackable; }

        private:
            trackable m_trackable;
        };

        template <class T, std::size_t N>
        const auto& get_trackable(const array<T, N>& arr)
        {
            return arr.get_trackable();
        }
	} // end namespace track
} // end namespace rsl

#endif // TRACK_ARRAY_H
