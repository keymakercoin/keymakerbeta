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
    CTxDestination  founderAddr;


    // fill payee with the foundFounderRewardStrcutureFounderRewardStrcutureer address
//    vchVersion == Params().Base58Prefix(CChainParams::SCRIPT_ADDRESS))
//Params().Base58Prefix(CChainParams::PUBKEY_ADDRESS)
 	if (nBlockHeight < newFounderAddressStartBlock) {
		//CBitcoinAddress cbAddress(founderAddress);
		//payee = GetScriptForDestination(cbAddress.Get());
            founderAddr = DecodeDestination(founderAddress);
	}
	else {
		//CBitcoinAddress cbAddress(newFounderAddress);
		//payee = GetScriptForDestination(cbAddress.Get());
           founderAddr = DecodeDestination(newFounderAddress);
	}

    CScript payee = GetScriptForDestination(founderAddr);


    // GET FOUNDER PAYMENT VARIABLES SETUP

    // split reward between miner ...
    txNew.vout[0].nValue -= founderPayment;
    txoutFounderRet = CTxOut(founderPayment, payee);
    txNew.vout.push_back(txoutFounderRet);
//	if (nBlockHeight < newFounderAddressStartBlock) {
//		LogPrintf("FounderPayment::FillFounderPayment -- Founder payment %lld to %s\n", founderPayment, founderAddress.c_str());
//	} else {
//		LogPrintf("FounderPayment::FillFounderPayment -- Founder payment %lld to %s\n", founderPayment, newFounderAddress.c_str());
//	}
}

bool FounderPayment::IsBlockPayeeValid(const CTransaction& txNew, const int height, const CAmount blockReward) {
	CScript payee;
	CScript newPayee;
	// fill payee with the founder address
	//LogPrintf("FounderPayment::IsBlockPayeeValid -- height=%d to %s newFounderAddressStartBlock=%d \n", height, newFounderAddress.c_str(),newFounderAddressStartBlock);
	//payee = GetScriptForDestination(CBitcoinAddress(founderAddress).Get());
	//newPayee = GetScriptForDestination(CBitcoinAddress(newFounderAddress).Get());
    //CScript payee = GetScriptForDestination(DecodeDestination(founderAddress));

	if (height < newFounderAddressStartBlock) {
            payee = GetScriptForDestination(DecodeDestination(founderAddress));
	} else {
            payee = GetScriptForDestination(DecodeDestination(newFounderAddress));
	}

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