/**************************************************************************
 * Copyright(c) 1998-2007, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id$ */

/**
 * @file   AliHLTMUONMansoTracksBlockStruct.cxx
 * @author Artur Szostak <artursz@iafrica.com>
 * @date   
 * @brief  Implementation of useful stream and comparison operators.
 */

#include "AliHLTMUONMansoTracksBlockStruct.h"
#include "AliHLTMUONUtils.h"
#include <cassert>


std::ostream& operator << (
		std::ostream& stream, const AliHLTMUONMansoTrackStruct& track
	)
{
	stream	<< "{fId = " << track.fFlags
		<< ", fTrigRec = " << track.fTrigRec
		<< ", fFlags = " << std::showbase << std::hex
		<< track.fFlags << std::dec
		<< ", fPx = " << track.fPx
		<< ", fPy = " << track.fPy
		<< ", fPz = " << track.fPz
		<< ", fChi2 = " << track.fChi2
		<< ", fHit[0] = " << track.fHit[0]
		<< ", fHit[1] = " << track.fHit[1]
		<< ", fHit[2] = " << track.fHit[2]
		<< ", fHit[3] = " << track.fHit[3]
		<< "}";
	return stream;
}


std::ostream& operator << (
		std::ostream& stream,
		const AliHLTMUONMansoTracksBlockStruct& block
	)
{
	assert( AliHLTMUONUtils::IntegrityOk(block) );

	stream 	<< "{fHeader = " << block.fHeader << ", fTrack[] = [";
	if (block.fHeader.fNrecords > 0) stream << block.fTrack[0];
	for (AliHLTUInt32_t i = 1; i < block.fHeader.fNrecords; i++)
		stream << ", " << block.fTrack[i];
	stream << "]}";
	return stream;
}


bool operator == (
		const AliHLTMUONMansoTracksBlockStruct& a,
		const AliHLTMUONMansoTracksBlockStruct& b
	)
{
	assert( AliHLTMUONUtils::IntegrityOk(a) );
	assert( AliHLTMUONUtils::IntegrityOk(b) );

	// First check if the blocks have the same header. If they do then check
	// if every track is the same. In either case if we find a difference
	// return false.
	if (a.fHeader != b.fHeader) return false;
	for (AliHLTUInt32_t i = 0; i < a.fHeader.fNrecords; i++)
		if (a.fTrack[i] != b.fTrack[i]) return false;
	return true;
}
