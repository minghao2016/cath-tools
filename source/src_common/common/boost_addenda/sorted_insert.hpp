/// \file
/// \brief The sorted_insert header

/// \copyright
/// Tony Lewis's Common C++ Library Code (here imported into the CATH Tools project and then tweaked, eg namespaced in cath)
/// Copyright (C) 2007, Tony Lewis
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _CATH_TOOLS_SOURCE_SRC_COMMON_COMMON_BOOST_ADDENDA_SORTED_INSERT_HPP
#define _CATH_TOOLS_SOURCE_SRC_COMMON_COMMON_BOOST_ADDENDA_SORTED_INSERT_HPP

#include <boost/range/algorithm/lower_bound.hpp>

namespace cath {
	namespace common {

		/// \brief Insert a value into the correct place in a container that's already sorted
		template <typename CTR>
		inline void sorted_insert(CTR                            &arg_container, ///< The sorted container into which a value should be inserted
		                          const typename CTR::value_type &arg_value      ///< The value to insert
		                          ) {
			arg_container.insert(
				boost::range::lower_bound(
					arg_container,
					arg_value
				),
				arg_value
			);
    	}

		/// \brief Insert a value into the correct place in a container that's already sorted with the specified less-than predicate
		template <typename CTR, typename PRED>
		inline void sorted_insert(CTR                            &arg_container, ///< The sorted container into which a value should be inserted
		                          const typename CTR::value_type &arg_value,     ///< The value to insert
		                          PRED                           arg_predicate   ///< The less-than predicate used for the sorting of the elements
		                          ) {
			arg_container.insert(
				boost::range::lower_bound(
					arg_container,
					arg_value,
					arg_predicate
				),
				arg_value
			);
    	}
	} // namespace common
} // namespace cath

#endif
