/// \file
/// \brief The rmsd_score class header

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

#ifndef _CATH_TOOLS_SOURCE_UNI_SCORE_ALIGNED_PAIR_SCORE_RMSD_SCORE_HPP
#define _CATH_TOOLS_SOURCE_UNI_SCORE_ALIGNED_PAIR_SCORE_RMSD_SCORE_HPP

#include "score/aligned_pair_score/aligned_pair_score.hpp"
#include "score/aligned_pair_score/detail/score_common_coord_handler.hpp"

#include <memory>

namespace cath { namespace align { class common_atom_selection_policy;    } }
namespace cath { namespace align { class common_residue_selection_policy; } }

namespace cath {
	namespace score {

		/// \brief Calculate (and represent) RMSD, a very widely used measure based on the
		///        root of the mean of the squared deviation between the positions of equivalent atoms.
		class rmsd_score : public aligned_pair_score {
		private:
			friend bool operator<(const rmsd_score &,
			                      const rmsd_score &);

			friend class boost::serialization::access;

			template<class archive> void serialize(archive &ar,
			                                       const size_t /*version*/
			                                       ) {
				ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP( aligned_pair_score );
				ar & BOOST_SERIALIZATION_NVP( the_coord_handler );
			}

			/// \brief TODOCUMENT
			detail::score_common_coord_handler the_coord_handler;

			std::unique_ptr<aligned_pair_score> do_clone() const final;

			boost::logic::tribool do_higher_is_better() const final;
			score_value do_calculate(const align::alignment &,
			                         const protein &,
			                         const protein &) const final;
			std::string do_description() const final;
			std::string do_id_name() const final;
			str_bool_pair_vec do_short_name_suffixes() const final;
			std::string do_long_name() const final;

//			std::unique_ptr<aligned_pair_score> do_build_from_short_name_spec(const std::string &) const final;

			bool do_less_than_with_same_dynamic_type(const aligned_pair_score &) const final;

		public:
			rmsd_score() = default;
			rmsd_score(const align::common_residue_selection_policy &,
			           const align::common_atom_selection_policy &);

			std::string short_suffix_string() const;
			std::string long_suffix_string() const;
			std::string description_brackets_string() const;

			const detail::score_common_coord_handler & get_score_common_coord_handler() const;
		};

		bool operator<(const rmsd_score &,
		               const rmsd_score &);

	} // namespace score
} // namespace cath
#endif
