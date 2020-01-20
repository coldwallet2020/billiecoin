// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash and Syscoin Core developers
// Copyright (c) 2018-2020 The Billiecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include "arith_uint256.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"
// BILLIECOIN includes for gen block
#include "uint256.h"
#include "arith_uint256.h"
#include "hash.h"
#include "streams.h"
#include <time.h>
// BILLIECOIN generate block
/**
* Main network
*/
/**
* What makes a good checkpoint block?
* + Is surrounded by blocks with reasonable timestamps
*   (no blocks before with a timestamp after, none after with
*    timestamp before)
* + Contains no strange transactions
*/

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateDevNetGenesisBlock(const uint256 &prevBlockHash, const std::string& devNetName, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    assert(!devNetName.empty());

    CMutableTransaction txNew;
    txNew.nVersion = 4;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    // put height (BIP34) and devnet name into coinbase
    txNew.vin[0].scriptSig = CScript() << 1 << std::vector<unsigned char>(devNetName.begin(), devNetName.end());
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = CScript() << OP_RETURN;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock = prevBlockHash;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "do not compile";
    const CScript genesisOutputScript = CScript() << ParseHex("04747bf3a8d353ba4744795ab06e73b68457b3c490492c0ff6f9660fd4b52b7541ec540bada8231e24777df4c256bd49d46918aca6973872bb4d5456de1de2a130") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}
// This will figure out a valid hash and Nonce if you're
// creating a different genesis block:
static void GenerateGenesisBlock(CBlockHeader &genesisBlock, uint256 &phash)
{
	arith_uint256 bnTarget;
	bnTarget.SetCompact(genesisBlock.nBits);
	uint32_t nOnce = 0;
	while (true) {
		genesisBlock.nNonce = nOnce;
		uint256 hash = genesisBlock.GetHash();
		if (UintToArith256(hash) <= bnTarget) {
			phash = hash;
			break;
		}
		nOnce++;
	}
	printf("genesis.nTime = %u \n", genesisBlock.nTime);
	printf("genesis.nNonce = %u \n", genesisBlock.nNonce);
	printf("Generate hash = %s\n", phash.ToString().c_str());
	printf("genesis.hashMerkleRoot = %s\n", genesisBlock.hashMerkleRoot.ToString().c_str());
}

static CBlock FindDevNetGenesisBlock(const Consensus::Params& params, const CBlock &prevBlock, const CAmount& reward)
{
    std::string devNetName = GetDevNetName();
    assert(!devNetName.empty());

    CBlock block = CreateDevNetGenesisBlock(prevBlock.GetHash(), devNetName.c_str(), prevBlock.nTime + 1, 0, prevBlock.nBits, prevBlock.nVersion, reward);

    arith_uint256 bnTarget;
    bnTarget.SetCompact(block.nBits);

    for (uint32_t nNonce = 0; nNonce < UINT32_MAX; nNonce++) {
        block.nNonce = nNonce;

        uint256 hash = block.GetHash();
        if (UintToArith256(hash) <= bnTarget)
            return block;
    }

    // This is very unlikely to happen as we start the devnet with a very low difficulty. In many cases even the first
    // iteration of the above loop will give a result already
    error("FindDevNetGenesisBlock: could not find devnet genesis block for %s", devNetName);
    assert(false);
}

/**
 * Main network
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 525600;
	consensus.nSeniorityInterval = 43800 * 4;
	consensus.nTotalSeniorityIntervals = 9;
        consensus.nMasternodePaymentsStartBlock = 2;
        consensus.nMasternodePaymentsIncreaseBlock = 2; 
        consensus.nMasternodePaymentsIncreasePeriod = 576*30;
        consensus.nInstantSendConfirmationsRequired = 6;
        consensus.nInstantSendKeepLock = 24;
	consensus.nShareFeeBlock = 175000;
        consensus.nBudgetPaymentsStartBlock = 0; // actual historical value
        consensus.nBudgetPaymentsCycleBlocks = 43800;
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nSuperblockStartBlock = 0;
	consensus.nSuperblockStartHash = uint256();
        consensus.nSuperblockCycle = 43800;
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.DIP0001Height = 0;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20
        consensus.nPowTargetTimespan = 6 * 60 * 60; // 6h retarget
        consensus.nPowTargetSpacing = 1 * 60; // Billiecoin: 1 minute
	consensus.nAuxpowChainId = 0x1000;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 15200;
        consensus.nPowDGWHeight = 0;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
	consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // Feb 5th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;

        // Deployment of BIP147
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 999999999999ULL;


        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000100110");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000000cecf89bb26fe2f2a82369d5cb13cf828ab03a5954ca9e4233999ff");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
	pchMessageStart[0] = 0xbf;
	pchMessageStart[1] = 0xbd;
	pchMessageStart[2] = 0x6b;
	pchMessageStart[3] = 0x0c;
        vAlertPubKey = ParseHex("04505a3e87e79046066fdd0da119da2e5447b9ce66bf1f4ac275dc683ab0639bbdbb168d2600392c062f5b58e066bdf1c312f2ff8cbc0ae4b5494d308072f51430");
	nDefaultPort = 5369;
	nPruneAfterHeight = 1000000;
	uint256 hash;
	genesis = CreateGenesisBlock(1579494135, 1068888, 0x1e0ffff0, 1, 42 * COIN);
	/*CBlockHeader genesisHeader = genesis.GetBlockHeader();
	GenerateGenesisBlock(genesisHeader, &hash);*/

	consensus.hashGenesisBlock = genesis.GetHash();
	assert(consensus.hashGenesisBlock == uint256S("0x000003f57cef61d0cd1012b5f7b4af67969f62f72815d4f5d7ce8eb330eb3214"));
	assert(genesis.hashMerkleRoot == uint256S("0x1a78461f5eae7af5109a4e9ec95b85eb91fa2d96cb558f05ce696250fc2de72b"));

	//vSeeds.push_back(CDNSSeedData("seed1.billiecoin.green", "seed1.billiecoin.green"));
	//vSeeds.push_back(CDNSSeedData("seed2.billiecoin.green", "seed2.billiecoin.green"));
	//vSeeds.push_back(CDNSSeedData("seed3.billiecoin.green", "seed3.billiecoin.green"));
	//vSeeds.push_back(CDNSSeedData("seed4.billiecoin.green", "seed4.billiecoin.green"));

	base58Prefixes[PUBKEY_ADDRESS_SYS] = std::vector<unsigned char>(1, 25);
	base58Prefixes[SCRIPT_ADDRESS_SYS] = std::vector<unsigned char>(1, 70);
	base58Prefixes[SECRET_KEY_SYS] = std::vector<unsigned char>(1, 128);

	base58Prefixes[PUBKEY_ADDRESS_BTC] = std::vector<unsigned char>(1, 0);
	base58Prefixes[SCRIPT_ADDRESS_BTC] = std::vector<unsigned char>(1, 5);
	base58Prefixes[SECRET_KEY_BTC] = std::vector<unsigned char>(1, 128);

	base58Prefixes[PUBKEY_ADDRESS_ZEC] = { 0x1C,0xB8 };
	base58Prefixes[SCRIPT_ADDRESS_ZEC] = { 0x1C,0xBD };
	base58Prefixes[SECRET_KEY_ZEC] = std::vector<unsigned char>(1, 128);
	// Billiecoin BIP32 pubkeys start with 'xpub' (Billiecoin defaults)
	base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
	// Billiecoin BIP32 prvkeys start with 'xprv' (Billiecoin defaults)
	base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // Billiecoin BIP44 coin type is '569'
        nExtCoinType = 569;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fAllowMultipleAddressesFromGroup = false;
        fAllowMultiplePorts = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour

        strSporkAddress = "BN8K2NEAB75kfjvLYLXWXJsV1wDBo83cwP";
		checkpointData = {
			{
				{ 128, uint256S("0x0000000000cecf89bb26fe2f2a82369d5cb13cf828ab03a5954ca9e4233999ff") },
			}
		};

		chainTxData = ChainTxData{
			// Data from rpc: getchaintxstats 128
			/* nTime    */ 1579498859,
			/* nTxCount */ 142,
			/* dTxRate  */ 0.09349890430971512
		};
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 525600;
	consensus.nSeniorityInterval = 60; // seniority increases every hour
	consensus.nTotalSeniorityIntervals = 9;
        consensus.nMasternodePaymentsStartBlock = 2; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 2;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendConfirmationsRequired = 2;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 0;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
	consensus.nShareFeeBlock = 1000;
        consensus.nSuperblockStartBlock = 1; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockStartHash = uint256();
        consensus.nSuperblockCycle = 60; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.DIP0001Height = 5500;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20
	consensus.nPowTargetTimespan = 6 * 60 * 60; // 6h retarget
	consensus.nPowTargetSpacing = 60; // Billiecoin: 1 min
	consensus.nAuxpowChainId = 0x1000;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 4001; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
        consensus.nPowDGWHeight = 0;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;

        // Deployment of BIP147
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 999999999999ULL;


        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xce;
        pchMessageStart[1] = 0xe2;
        pchMessageStart[2] = 0xca;
        pchMessageStart[3] = 0xff;
        vAlertPubKey = ParseHex("04505a3e87e79046066fdd0da119da2e5447b9ce66bf1f4ac275dc683ab0639bbdbb168d2600392c062f5b58e066bdf1c312f2ff8cbc0ae4b5494d308072f51430");
        nDefaultPort = 15369;
        nPruneAfterHeight = 1000;

	genesis = CreateGenesisBlock(1579338001, 200111, 0x1e0ffff0, 1, 42 * COIN);
	/*
	uint256 hash;
	CBlockHeader genesisHeader = genesis.GetBlockHeader();
	GenerateGenesisBlock(genesisHeader, &hash);*/
	consensus.hashGenesisBlock = genesis.GetHash();
	assert(consensus.hashGenesisBlock == uint256S("0x00000101fb5fe4d627a21f57f91d6e4a7d9633850d6d54b8c5abf1c9bea4aa72"));
	assert(genesis.hashMerkleRoot == uint256S("0x1a78461f5eae7af5109a4e9ec95b85eb91fa2d96cb558f05ce696250fc2de72b"));

        vFixedSeeds.clear();
        vSeeds.clear();
  

	base58Prefixes[PUBKEY_ADDRESS_SYS] = std::vector<unsigned char>(1, 63);
	base58Prefixes[SCRIPT_ADDRESS_SYS] = std::vector<unsigned char>(1, 125);
	base58Prefixes[SECRET_KEY_SYS] = std::vector<unsigned char>(1, 9);

	base58Prefixes[PUBKEY_ADDRESS_BTC] = std::vector<unsigned char>(1, 111);
	base58Prefixes[SCRIPT_ADDRESS_BTC] = std::vector<unsigned char>(1, 196);
	base58Prefixes[SECRET_KEY_BTC] = std::vector<unsigned char>(1, 239);

	base58Prefixes[PUBKEY_ADDRESS_ZEC] = std::vector<unsigned char>(0x1C, 0xB8);
	base58Prefixes[SCRIPT_ADDRESS_ZEC] = std::vector<unsigned char>(0x1C, 0xBD);
	base58Prefixes[SECRET_KEY_ZEC] = std::vector<unsigned char>(1, 239);

        // Testnet Billiecoin BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;


        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fAllowMultipleAddressesFromGroup = false;
        fAllowMultiplePorts = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        strSporkAddress = "B4eSBWqawiRKAbpL9kXAMNv649zoMAwDt9";

		checkpointData = {
			{
				{ 0, uint256S("0x00000101fb5fe4d627a21f57f91d6e4a7d9633850d6d54b8c5abf1c9bea4aa72") },
			}
		};

		chainTxData = ChainTxData{
			0,
			0,
			0
		};

		

    }
};
static CTestNetParams testNetParams;

/**
 * Devnet
 */
class CDevNetParams : public CChainParams {
public:
    CDevNetParams() {
        strNetworkID = "dev";
        consensus.nSubsidyHalvingInterval = 210240;
        consensus.nMasternodePaymentsStartBlock = 4010; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 4030;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendConfirmationsRequired = 2;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 4100;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nSuperblockStartBlock = 4200; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockStartHash = uint256(); // do not check this on devnet
        consensus.nSuperblockCycle = 60; // Superblocks can be issued hourly on devnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.BIP34Height = 2; // BIP34 activated immediately on devnet
        consensus.BIP65Height = 2; // BIP65 activated immediately on devnet
        consensus.BIP66Height = 2; // BIP66 activated immediately on devnet
        consensus.DIP0001Height = 2; // DIP0001 activated immediately on devnet
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1
        consensus.nPowTargetTimespan = 6 * 60 * 60; // Billiecoin: 6 hours
        consensus.nPowTargetSpacing = 1 * 60; // Billiecoin: 1 minute
	consensus.nAuxpowChainId = 0x1000;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nPowKGWHeight = 4001; // nPowKGWHeight >= nPowDGWHeight means "no KGW"
        consensus.nPowDGWHeight = 0;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0; // September 28th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL; // September 28th, 2018

        // Deployment of DIP0001
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0; // Sep 18th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL; // Sep 18th, 2018

        // Deployment of BIP147
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 0; // Feb 5th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 999999999999ULL; // Feb 5th, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000000000000000000000000000000000000000000000000000000000000");

        pchMessageStart[0] = 0xe2;
        pchMessageStart[1] = 0xca;
        pchMessageStart[2] = 0xff;
        pchMessageStart[3] = 0xce;
        vAlertPubKey = ParseHex("04505a3e87e79046066fdd0da119da2e5447b9ce66bf1f4ac275dc683ab0639bbdbb168d2600392c062f5b58e066bdf1c312f2ff8cbc0ae4b5494d308072f51430");
        nDefaultPort = 15369;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1417713337, 1096447, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e"));
        assert(genesis.hashMerkleRoot == uint256S("0x3fc1815124d408495fb860705d2188d84fcfeb5efc894f26fefc81a5cbdc49e8"));

        devnetGenesis = FindDevNetGenesisBlock(consensus, genesis, 50 * COIN);
        consensus.hashDevnetGenesisBlock = devnetGenesis.GetHash();

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("billiecoinevo.org",  "devnet-seed.billiecoinevo.org"));

	base58Prefixes[PUBKEY_ADDRESS_SYS] = std::vector<unsigned char>(1, 63);
	base58Prefixes[SCRIPT_ADDRESS_SYS] = std::vector<unsigned char>(1, 125);
	base58Prefixes[SECRET_KEY_SYS] = std::vector<unsigned char>(1, 9);

	base58Prefixes[PUBKEY_ADDRESS_BTC] = std::vector<unsigned char>(1, 111);
	base58Prefixes[SCRIPT_ADDRESS_BTC] = std::vector<unsigned char>(1, 196);
	base58Prefixes[SECRET_KEY_BTC] = std::vector<unsigned char>(1, 239);

	base58Prefixes[PUBKEY_ADDRESS_ZEC] = { 0x1C,0xB8 };
	base58Prefixes[SCRIPT_ADDRESS_ZEC] = { 0x1C,0xBD };
	base58Prefixes[SECRET_KEY_ZEC] = std::vector<unsigned char>(1, 239);
	// Regtest Billiecoin BIP32 pubkeys start with 'tpub' (Billiecoin defaults)
	base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
	// Regtest Billiecoin BIP32 prvkeys start with 'tprv' (Billiecoin defaults)
	base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Testnet Billiecoin BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fAllowMultipleAddressesFromGroup = true;
        fAllowMultiplePorts = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        strSporkAddress = "SN3j3YFP1rZH5wvAjaLKyv1PzM1W6jmk4t";

		checkpointData = {
			{
				{ 0, uint256S("000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e") },
			}
		};

		chainTxData = ChainTxData{
			0,
			0,
			0
		};
    }
};
static CDevNetParams *devNetParams;


/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
	strNetworkID = "regtest";
	consensus.nSeniorityInterval = 60; // seniority increases every hour
	consensus.nTotalSeniorityIntervals = 9;;

        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 2;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendConfirmationsRequired = 2;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 0;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nSuperblockStartBlock = 1;
        consensus.nSuperblockStartHash = uint256(); // do not check this on regtest
        consensus.nSuperblockCycle = 10;
	consensus.nShareFeeBlock = 0;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.BIP34Height = 0; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.DIP0001Height = 2000;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1
        consensus.nPowTargetTimespan = 6 * 60 * 60; // Billiecoin: 6 hour
        consensus.nPowTargetSpacing = 1 * 60; // Billiecoin: 1 minute
	consensus.nAuxpowChainId = 0x1000;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nPowKGWHeight = 15200; // same as mainnet
        consensus.nPowDGWHeight = 0; // same as mainnet
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_DIP0001].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_BIP147].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

	pchMessageStart[0] = 0xfa;
	pchMessageStart[1] = 0xbf;
	pchMessageStart[2] = 0xb5;
	pchMessageStart[3] = 0xda;
        nDefaultPort = 15369;
        nPruneAfterHeight = 1000;

	genesis = CreateGenesisBlock(1579338002, 1749224, 0x1e0ffff0, 1, 42 * COIN);
	/*
	uint256 hash;
	CBlockHeader genesisHeader = genesis.GetBlockHeader();
	GenerateGenesisBlock(genesisHeader, &hash);*/
	consensus.hashGenesisBlock = genesis.GetHash();
	assert(consensus.hashGenesisBlock == uint256S("0x000009246d7eb479231fc53ad4eaf1e13d01a4d29ab4df5170887685c8f31724"));
	assert(genesis.hashMerkleRoot == uint256S("0x1a78461f5eae7af5109a4e9ec95b85eb91fa2d96cb558f05ce696250fc2de72b"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = true;
        fAllowMultipleAddressesFromGroup = true;
        fAllowMultiplePorts = true;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        // privKey: 2LEye61kVbt8yWmxfGswDEZkUWUGidY6vRxz4JbTkuPjec7Dr4cj
        strSporkAddress = "SPpdQkdXHFbqUxQ6FvRW5KD24NcWdiRAuQ";

   

	base58Prefixes[PUBKEY_ADDRESS_SYS] = std::vector<unsigned char>(1, 63);
	base58Prefixes[SCRIPT_ADDRESS_SYS] = std::vector<unsigned char>(1, 125);
	base58Prefixes[SECRET_KEY_SYS] = std::vector<unsigned char>(1, 9);

	base58Prefixes[PUBKEY_ADDRESS_BTC] = std::vector<unsigned char>(1, 111);
	base58Prefixes[SCRIPT_ADDRESS_BTC] = std::vector<unsigned char>(1, 196);
	base58Prefixes[SECRET_KEY_BTC] = std::vector<unsigned char>(1, 239);

	base58Prefixes[PUBKEY_ADDRESS_ZEC] = { 0x1C,0xB8 };
	base58Prefixes[SCRIPT_ADDRESS_ZEC] = { 0x1C,0xBD };
	base58Prefixes[SECRET_KEY_ZEC] = std::vector<unsigned char>(1, 239);
	// Regtest Billiecoin BIP32 pubkeys start with 'tpub' (Billiecoin defaults)
	base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
	// Regtest Billiecoin BIP32 prvkeys start with 'tprv' (Billiecoin defaults)
	base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Regtest Billiecoin BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 1;
		checkpointData = {
			{
				{ 0, uint256S("0x000007311c61c83005d18a9bf8279d524d04a6b2566ea479e7477f347dc16d97") },
			}
		};

		chainTxData = ChainTxData{
			0,
			0,
			0
		};
   }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::DEVNET) {
            assert(devNetParams);
            return *devNetParams;
    } else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    if (network == CBaseChainParams::DEVNET) {
        devNetParams = new CDevNetParams();
    }

    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
