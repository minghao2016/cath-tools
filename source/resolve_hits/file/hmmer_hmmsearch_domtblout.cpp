/// \file
/// \brief The hmmer_hmmsearch_domtblout definitions

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

#include "hmmer_hmmsearch_domtblout.h"

#include "common/boost_addenda/make_string_ref.h"
#include "common/c++14/cbegin_cend.h"
#include "common/file/open_fstream.h"
#include "common/string/string_parse_tools.h"
#include "resolve_hits/read_and_resolve_mgr.h"
#include "resolve_hits/resolve_hits_type_aliases.h"

#include <fstream>
#include <iostream>

using namespace cath;
using namespace cath::common;
using namespace cath::rslv;

using boost::filesystem::path;
using std::ifstream;
using std::istream;
using std::string;

/// \brief Parse a HMMER domain hits table file (as produced by the --domtblout option to a HMMER program)
///        from the specified file and pass them to the specified read_and_resolve_mgr
void cath::rslv::parse_domain_hits_table_file(read_and_resolve_mgr &arg_read_and_resolve_mgr,   ///< The read_and_resolve manager to which the hits should be passed for processing
                                              const path           &arg_domain_hits_table_file, ///< The file from which the HMMER domain hits table data should be parsed
                                              const bool           &arg_apply_cath_policies     ///< Whether to apply CATH-specific policies
                                              ) {
	ifstream the_ifstream;
	open_ifstream( the_ifstream, arg_domain_hits_table_file );

	parse_domain_hits_table( arg_read_and_resolve_mgr, the_ifstream, arg_apply_cath_policies );

	the_ifstream.close();
}

static_assert( jon_score_of_hmmer_scores(  25.5f, static_cast<resscr_t>( 5.9e-09 ), static_cast<resscr_t>(  0.0065  ), cath_id_score_category::LATER_ROUND ) ==  0.00207267166115343570709228515625f,   "" ); // 000771f5eee8a65ca49345b0dc640279 vs 1ckmA01_round_3,                     should be  0.002072671875 not  0.016581375';
static_assert( jon_score_of_hmmer_scores( 222.9f, static_cast<resscr_t>( 1.5e-69 ), static_cast<resscr_t>(  1.7e-63 ), cath_id_score_category::DC_TYPE     ) == 88.59722900390625f,                     "" ); // 000771f5eee8a65ca49345b0dc640279 vs dc_6e2322c9011262997009db9e093174a7, should be 88.597239912000 not 11.074654989';
static_assert( jon_score_of_hmmer_scores(  12.9f, static_cast<resscr_t>( 0.001   ), static_cast<resscr_t>( 61       ), cath_id_score_category::LATER_ROUND ) ==  0.0002683360944502055644989013671875f, "" ); // 01318e774c79badf7bd085845cfa953b vs 1rhfA02_round_3,                     should be  0.000268336125 not  0.002146689';
static_assert( jon_score_of_hmmer_scores(  28.0f, static_cast<resscr_t>( 7.5e-09 ), static_cast<resscr_t>(  0.001   ), cath_id_score_category::NORMAL      ) ==  0.1756159961223602294921875f,          "" ); // 0d820694a8578e09915738ac9586747f vs 3a1sB01_round_1,                     should be  0.175616';


/// \brief Parse HMMER domain hits table data (as produced by the --domtblout option to a HMMER program)
///        from the specified istream and pass them to the specified read_and_resolve_mgr
void cath::rslv::parse_domain_hits_table(read_and_resolve_mgr &arg_read_and_resolve_mgr, ///< The read_and_resolve manager to which the hits should be passed for processing
                                         istream              &arg_input_stream,         ///< The istream from which the HMMER domain hits table data should be parsed
                                         const bool           &arg_apply_cath_policies   ///< Whether to apply CATH-specific policies
                                         ) {
	string line_string;

	arg_read_and_resolve_mgr.process_all_outstanding();

	while ( getline( arg_input_stream, line_string ) ) {
		// Skip comment lines
		if ( line_string.front() == '#' ) {
			continue;
		}

		constexpr size_t TARGET_FIELD_IDX        =  0;
		constexpr size_t QUERY_FIELD_IDX         =  3;
		constexpr size_t COND_EVALUE_FIELD_IDX   = 11;
		constexpr size_t INDP_EVALUE_FIELD_IDX   = 12;
		constexpr size_t BITSCORE_FIELD_IDX      = 13;
		constexpr size_t ALI_START_RES_FIELD_IDX = 17;
		constexpr size_t ALI_STOP_RES_FIELD_IDX  = 18;
		constexpr size_t ENV_START_RES_FIELD_IDX = 19;
		constexpr size_t ENV_STOP_RES_FIELD_IDX  = 20;

		const auto     target_field_itrs      = find_field_itrs( line_string, TARGET_FIELD_IDX                                                                );
		const auto     query_field_itrs       = find_field_itrs( line_string, QUERY_FIELD_IDX,       1 + TARGET_FIELD_IDX,      target_field_itrs.second      );

		const auto     target_id_str_ref      = make_string_ref( target_field_itrs.first, target_field_itrs.second );
		const auto     query_id_str_ref       = make_string_ref( query_field_itrs.first,  query_field_itrs.second  );

		const auto     id_score_category      = cath_score_category_of_id( query_id_str_ref, arg_apply_cath_policies );

		const size_t   start_field_idx        = ( id_score_category == cath_id_score_category::DC_TYPE ) ? ALI_START_RES_FIELD_IDX : ENV_START_RES_FIELD_IDX;
		const size_t   stop_field_idx         = ( id_score_category == cath_id_score_category::DC_TYPE ) ? ALI_STOP_RES_FIELD_IDX  : ENV_STOP_RES_FIELD_IDX;

		const auto     cond_evalue_field_itrs = find_field_itrs( line_string, COND_EVALUE_FIELD_IDX, 1 + QUERY_FIELD_IDX,       query_field_itrs.second       );
		const auto     indp_evalue_field_itrs = find_field_itrs( line_string, INDP_EVALUE_FIELD_IDX, 1 + COND_EVALUE_FIELD_IDX, cond_evalue_field_itrs.second );
		const auto     bitscore_field_itrs    = find_field_itrs( line_string, BITSCORE_FIELD_IDX,    1 + INDP_EVALUE_FIELD_IDX, indp_evalue_field_itrs.second );
		const auto     start_res_field_itrs   = find_field_itrs( line_string, start_field_idx,       1 + BITSCORE_FIELD_IDX,    bitscore_field_itrs.second    );
		const auto     stop_res_field_itrs    = find_field_itrs( line_string, stop_field_idx,        1 + start_field_idx,       start_res_field_itrs.second   );

		const resscr_t cond_evalue            = parse_float_from_field( cond_evalue_field_itrs.first, cond_evalue_field_itrs.second );
		const resscr_t indp_evalue            = parse_float_from_field( indp_evalue_field_itrs.first, indp_evalue_field_itrs.second );
		const resscr_t bitscore               = parse_float_from_field( bitscore_field_itrs.first,    bitscore_field_itrs.second    );
		const residx_t start                  = parse_uint_from_field ( start_res_field_itrs.first,   start_res_field_itrs.second   );
		const residx_t stop                   = parse_uint_from_field ( stop_res_field_itrs.first,    stop_res_field_itrs.second    );

		if ( bitscore < 0 ) {
			BOOST_THROW_EXCEPTION(runtime_error_exception(
				"Cannot currently handle a negative bitscore value ("
				+ ::std::to_string( bitscore )
				+ ")"
			));
		}

		arg_read_and_resolve_mgr.add_hit(
			target_id_str_ref,
			arrow_before_res( start ),
			arrow_after_res ( stop  ),
			hit_seg_vec{},
			jon_score_of_hmmer_scores( bitscore, cond_evalue, indp_evalue, id_score_category ),
			string{ query_field_itrs.first, query_field_itrs.second }
		);
	}

	arg_read_and_resolve_mgr.process_all_outstanding();
}