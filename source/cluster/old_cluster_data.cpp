/// \file
/// \brief The old_cluster_data class definitions

/// \copyright
/// CATH Tools - Protein structure comparison tools such as SSAP and SNAP
/// Copyright (C) 2011, Orengo Group, University College London
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

#include "old_cluster_data.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>

using boost::adaptors::transformed;
using boost::algorithm::join;
using boost::irange;

/// \brief Generate a string describing the specified old_cluster_data
///
/// \relates old_cluster_data
std::string cath::clust::to_string(const old_cluster_data &arg_old_cluster_data ///< The old_cluster_data to describe
                                   ) {
	using std::to_string;

	// Return a string summarising the old_cluster_data
	return "old_cluster_data["
		+ to_string( get_num_clusters( arg_old_cluster_data ) )
		+ " clusters, clusters{ "
		+ join(
			irange( 0_z, get_num_clusters( arg_old_cluster_data ) )
				| transformed( [&] (const size_t &x) {
					return to_string( x )
						+ R"((")"
						+ get_name_of_cluster_of_id( arg_old_cluster_data, x )
						+ R"("): )"
						+ to_string(
							arg_old_cluster_data[ x ],
							arg_old_cluster_data.get_id_of_seq_name()
						);
				} ),
			", "
		)
		+ " } ]";
}