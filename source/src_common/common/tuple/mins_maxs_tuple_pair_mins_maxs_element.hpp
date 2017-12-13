/// \file
/// \brief The tuple_increment header

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

#ifndef _CATH_TOOLS_SOURCE_SRC_COMMON_COMMON_TUPLE_MINS_MAXS_TUPLE_PAIR_MINS_MAXS_ELEMENT_HPP
#define _CATH_TOOLS_SOURCE_SRC_COMMON_COMMON_TUPLE_MINS_MAXS_TUPLE_PAIR_MINS_MAXS_ELEMENT_HPP

#include <boost/core/ignore_unused.hpp>

#include "common/boost_addenda/range/range_concept_type_aliases.hpp"
#include "common/cpp14/cbegin_cend.hpp"
#include "common/detail/make_static_const.hpp"
#include "common/detail/tuple_index_sequence.hpp"
#include "common/type_traits/is_tuple.hpp"

#include <cassert>
#include <tuple>

namespace cath {
	namespace common {
		namespace detail {

			/// \brief Update a min and max with a new value
			template <typename T>
			int update_min_max_with_value_impl(T       &arg_min,       ///< The min value
			                                   T       &arg_max,       ///< The max value
			                                   const T &arg_min_value, ///< The new min value
			                                   const T &arg_max_value  ///< The new max value
			                                   ) {
				if ( arg_min_value < arg_min ) { arg_min = arg_min_value; }
				if ( arg_max_value > arg_max ) { arg_max = arg_max_value; }
				return 0;
			}

			/// \brief Implementation for update_mins_maxs_with_value
			template <typename Tpl, size_t... Index>
			void update_mins_maxs_with_value_impl(Tpl                       &arg_mins,  ///< The min tuple
			                                      Tpl                       &arg_maxs,  ///< The max tuple
			                                      const std::pair<Tpl, Tpl> &arg_value, ///< The new tuple
			                                      std::index_sequence<Index...>         ///< An index_sequence matching the indices of Tpl
			                                      ) {
				auto dummy_list = {
					update_min_max_with_value_impl(
						std::get<Index>( arg_mins         ),
						std::get<Index>( arg_maxs         ),
						std::get<Index>( arg_value.first  ),
						std::get<Index>( arg_value.second )
					)...
				};
				boost::ignore_unused( dummy_list );
			}

			/// \brief Element-wise update a min tuple and a max tuple with a new tuple
			template <typename Tpl>
			void update_mins_maxs_with_value(Tpl                       &arg_mins, ///< The min tuple
			                                 Tpl                       &arg_maxs, ///< The max tuple
			                                 const std::pair<Tpl, Tpl> &arg_value ///< The new tuple
			                                 ) {
				return update_mins_maxs_with_value_impl(
					arg_mins,
					arg_maxs,
					arg_value,
					tuple_index_sequence<Tpl>{}
				);
			}

			/// \brief Function object to find the element-wise mins and maxs tuples of a range of tuples
			struct mins_maxs_tuple_pair_mins_maxs_element_fn final {

				/// \brief Find the element-wise mins and maxs tuples of a range of tuples
				template <typename Rng>
				auto operator()(Rng &&arg_rng ///< A range of tuples
				                ) const {
					using pair_t  = common::range_value_t<Rng>;
					static_assert(
						is_template_of_type<pair_t, std::pair>::value,
						"mins_maxs_tuple_pair_mins_maxs_element_fn() can only be applied to *pairs* of tuples"
					);
					using value_t = typename pair_t::first_type;
					static_assert(
						std::is_same<value_t, typename pair_t::second_type>::value,
						"mins_maxs_tuple_pair_mins_maxs_element_fn() can only be applied to pairs of the *same type* of tuples"
					);
					static_assert(
						is_tuple<value_t>::value,
						"mins_maxs_tuple_pair_mins_maxs_element_fn() can only be applied to pairs of *tuples*"
					);

					auto       begin_itr     = common::cbegin( arg_rng );
					const auto end_itr       = common::cend  ( arg_rng );

					assert( begin_itr != end_itr ); // arg_rng mustn't be empty

					// Set the mins and maxs to the element
					value_t mins = begin_itr->first;
					value_t maxs = begin_itr->second;

					// Update the mins and maxs over the rest of the range
					++begin_itr;
					std::for_each(
						begin_itr,
						end_itr,
						[&] (const pair_t &x) { update_mins_maxs_with_value( mins, maxs, x ); }
					);
					return make_pair( mins, maxs );
				}

				mins_maxs_tuple_pair_mins_maxs_element_fn()                                                  = delete;
				mins_maxs_tuple_pair_mins_maxs_element_fn(const mins_maxs_tuple_pair_mins_maxs_element_fn &) = delete;
				void operator=(const mins_maxs_tuple_pair_mins_maxs_element_fn &)                            = delete;
			};

		} // namespace detail

		MAKE_STATIC_CONST( detail::mins_maxs_tuple_pair_mins_maxs_element_fn, mins_maxs_tuple_pair_mins_maxs_element )

	} // namespace common
} // namespace cath

#endif
