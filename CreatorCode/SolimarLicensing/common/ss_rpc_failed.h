#ifndef __SS_RPC_FAILED_H
#define __SS_RPC_FAILED_H

//*******************************************************************
//
// Macro: SS_RPC_FAILED
//
// Check for RPC failure.
//
//*******************************************************************
#define SS_RPC_FAILED(hr) \
	((hr) == 0x800706BA || /* RPC server is unavailable */ \
	 (hr) == 0x800706BF || /* Remote procedure call failed */ \
	 HRESULT_FACILITY((hr)) == FACILITY_RPC)

#endif
