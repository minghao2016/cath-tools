/// \file
/// \brief The batch_functions header

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

#ifndef _CATH_TOOLS_SOURCE_COMMON_BATCH_BATCH_FUNCTIONS_H
#define _CATH_TOOLS_SOURCE_COMMON_BATCH_BATCH_FUNCTIONS_H

#include <boost/range/irange.hpp>

#include "common/batch/broken_batch_tol.hpp"
#include "common/type_aliases.hpp"

namespace cath {
	namespace common {

		size_t batch_size(const size_t &,
		                  const size_t &,
		                  const broken_batch_tol &);

		size_t num_batches(const size_t &,
		                   const size_t &,
		                   const broken_batch_tol &);

		void check_batch_index(const size_t &,
		                       const size_t &,
		                       const size_t &,
		                       const broken_batch_tol &);

		size_t batch_begin(const size_t &,
		                   const size_t &,
		                   const size_t &,
		                   const broken_batch_tol &);

		size_t batch_end(const size_t &,
		                 const size_t &,
		                 const size_t &,
		                 const broken_batch_tol &);

		size_size_pair batch_begin_and_end(const size_t &,
		                                   const size_t &,
		                                   const size_t &,
		                                   const broken_batch_tol &);

		size_size_pair batch_start_and_stop(const size_t &,
		                                    const size_t &,
		                                    const size_t &,
		                                    const broken_batch_tol &);

		boost::integer_range<size_t> batch_irange(const size_t &,
		                                          const size_t &,
		                                          const size_t &,
		                                          const broken_batch_tol &);

		/// \brief Return a batch sub-range of the specified range for the specified batch settings
		///
		/// \tparam Rng must be a random-access range
		template <typename Rng>
		boost::sub_range<Rng> batch_subrange(Rng                    &arg_rng,             ///< The range to batch up
		                                     const size_t           &arg_batch_size,      ///< The size of the batch
		                                     const size_t           &arg_batch_index,     ///< The index of the batch to return
		                                     const broken_batch_tol &arg_broken_batch_tol ///< Whether to tolerate a broken batch at the end
		                                     ) {
			const auto begin_and_end = batch_begin_and_end(
				boost::size( arg_rng ),
				arg_batch_size,
				arg_batch_index,
				arg_broken_batch_tol
			);
			return {
				std::begin( arg_rng ) + static_cast<ptrdiff_t>( begin_and_end.first  ),
				std::begin( arg_rng ) + static_cast<ptrdiff_t>( begin_and_end.second )
			};
		}

	} // namespace common
} // namespace cath

#endif