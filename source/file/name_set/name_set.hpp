/// \file
/// \brief The name_set class header

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

#ifndef _CATH_TOOLS_SOURCE_FILE_NAME_SET_NAME_SET_H
#define _CATH_TOOLS_SOURCE_FILE_NAME_SET_NAME_SET_H

#include <boost/operators.hpp>
#include <boost/optional.hpp>

#include "common/type_aliases.hpp"

namespace cath {
	namespace file {

		/// \brief Store the set of names that can be attached to a structure being processed
		class name_set final : private boost::equality_comparable<name_set> {
		private:
			/// \brief The name that came from the acquirer
			std::string name_from_acq;

			/// \brief An optional name that may have been explicitly specified
			str_opt     specified_id;

			/// \brief An optional (domain) name that may have come from regions (eg D[2zkmX03]308-465:X,516-669:X)
			str_opt     domain_name_from_regions;

		public:
			explicit name_set(std::string,
			                  str_opt = boost::none,
			                  str_opt = boost::none);

			const std::string & get_name_from_acq() const;
			const str_opt & get_specified_id() const;
			const str_opt & get_domain_name_from_regions() const;
		};


		/// \brief Ctor
		inline name_set::name_set(std::string arg_name_from_acq,           ///< The name that came from the acquirer
		                          str_opt     arg_specified_id,            ///< An optional name that may have been explicitly specified
		                          str_opt     arg_domain_name_from_regions ///< An optional (domain) name that may have come from regions (eg D[2zkmX03]308-465:X,516-669:X)
		                          ) : name_from_acq            { std::move( arg_name_from_acq            ) },
		                              specified_id             { std::move( arg_specified_id             ) },
		                              domain_name_from_regions { std::move( arg_domain_name_from_regions ) } {
		}

		/// \brief Getter for the name that came from the acquirer
		inline const std::string & name_set::get_name_from_acq() const {
			return name_from_acq;
		}

		/// \brief Getter for an optional name that may have been explicitly specified
		inline const str_opt & name_set::get_specified_id() const {
			return specified_id;
		}

		/// \brief Getter for an optional (domain) name that may have come from regions (eg D[2zkmX03]308-465:X,516-669:X)
		inline const str_opt & name_set::get_domain_name_from_regions() const {
			return domain_name_from_regions;
		}

		/// \brief Return whether the two specified name_sets are identical
		///
		/// \relates name_set
		inline bool operator==(const name_set &arg_lhs, ///< The first  name_set to compare
		                       const name_set &arg_rhs  ///< The second name_set to compare
		                       ) {
			return (
				arg_lhs.get_name_from_acq()            == arg_rhs.get_name_from_acq()
				&&
				arg_lhs.get_specified_id()             == arg_rhs.get_specified_id()
				&&
				arg_lhs.get_domain_name_from_regions() == arg_rhs.get_domain_name_from_regions()
			);
		}

		std::string get_domain_or_specified_or_name_from_acq(const name_set &);

		std::string to_string(const name_set &);

		std::ostream & operator<<(std::ostream &,
		                          const name_set &);

	} // namespace file
} // namespace cath

#endif