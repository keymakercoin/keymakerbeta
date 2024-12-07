 /*
 * Copyright (c) 2018 The Pigeon Core developers
 * Distributed under the MIT software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 * 
 * FounderPayment.cpp
 *
 *  Created on: Jun 24, 2018
 *      Author: Tri Nguyen
 */

#include <founder_payment.h>
#include <rpc/server.h>

#include <util/system.h>
#include <chainparams.h>
#include <boost/foreach.hpp>
#include <key_io.h>
#include "base58.h"

CAmount FounderPayment::getFounderPaymentAmount(int blockHeight, CAmount blockReward) {
	 if (blockHeight <= startBlock){
		 return 0;
	 }
	 for(int i = 0; i < rewardStructures.size(); i++) {
		 FounderRewardStructure rewardStructure = rewardStructures[i];
		 if(rewardStructure.blockHeight == INT_MAX || blockHeight <= rewardStructure.blockHeight) {
			 return blockReward * rewardStructure.rewardPercentage / 100;
		 }
	 }
	 return 0;
}

void FounderPayment::FillFounderPayment(CMutableTransaction& txNew, int nBlockHeight, CAmount blockReward, CTxOut& txoutFounderRet) {
    // make sure it's not filled yet
	CAmount founderPayment = getFounderPaymentAmount(nBlockHeight, blockReward);
//	if(founderPayment == 0) {
//	    LogPrintf("FounderPayment::FillFounderPayment -- Founder payment has not started\n");
//	    return;
//
//	}
	txoutFounderRet = CTxOut();
    CScript payee;
    // fill payee with the foundFounderRewardStrcutureFounderRewardStrcutureer address
	if (nBlockHeight < newFounderAddressStartBlock) {
    CTxDestination founderAddr = DecodeDestination(founderAddress);
    if (!IsValidDestination(founderAddr))
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY,
                           strprintf("Invalid Keymaker Founder Address: %s", founderAddress.c_str()));

		payee = GetScriptForDestination(founderAddr);
	}
	else {
        if (!IsValidDestination(newFounderAddress))
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY,
                           strprintf("Invalid Keymaker Founder Address: %s", founderAddress.c_str()));
		payee = GetScriptForDestination(newFounderAddress);

	}
    // GET FOUNDER PAYMENT VARIABLES SETUP

    // split reward between miner ...
    txNew.vout[0].nValue -= founderPayment;
    txoutFounderRet = CTxOut(founderPayment, payee);
    txNew.vout.push_back(txoutFounderRet);
	if (nBlockHeight < newFounderAddressStartBlock) {
		LogPrintf("FounderPayment::FillFounderPayment -- Founder payment %lld to %s\n", founderPayment, founderAddress.c_str());
	} else {
		LogPrintf("FounderPayment::FillFounderPayment -- Founder payment %lld to %s\n", founderPayment, newFounderAddress.c_str());
	}
}

bool FounderPayment::IsBlockPayeeValid(const CTransaction& txNew, const int height, const CAmount blockReward) {
	CScript payee;
	CScript newPayee;
	// fill payee with the founder address
	LogPrintf("FounderPayment::IsBlockPayeeValid -- height=%d to %s newFounderAddressStartBlock=%d \n", height, newFounderAddress.c_str(),newFounderAddressStartBlock);
	payee = GetScriptForDestination(founderAddress);
	newPayee = GetScriptForDestination(newFounderAddress);

	const CAmount founderReward = getFounderPaymentAmount(height, blockReward);
	//std::cout << "founderReward = " << founderReward << endl;
	BOOST_FOREACH(const CTxOut& out, txNew.vout) {
		LogPrintf("FounderPayment::IsBlockPayeeValid -- CTX=%s ", out.ToString());
		if((out.scriptPubKey == payee || out.scriptPubKey == newPayee)&& out.nValue >= founderReward) {
			return true;
		}
	}

	return false;
}