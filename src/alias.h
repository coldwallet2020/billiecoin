// Copyright (c) 2015-2020 The Billiecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ALIAS_H
#define ALIAS_H

#include "rpc/server.h"
#include "dbwrapper.h"
#include "consensus/params.h"
#include "sync.h"
#include "script/script.h"
#include "serialize.h"

class CTransaction;
class CTxOut;
class COutPoint;
class CBilliecoinAddress;
class CCoinsViewCache;
struct CRecipient;

static const unsigned int MAX_GUID_LENGTH = 20;
static const unsigned int MAX_NAME_LENGTH = 256;
static const unsigned int MAX_VALUE_LENGTH = 512;
static const unsigned int MAX_SYMBOL_LENGTH = 8;
static const unsigned int MIN_SYMBOL_LENGTH = 1;
static const unsigned int MAX_ENCRYPTED_GUID_LENGTH = MAX_NAME_LENGTH;
static const uint64_t ONE_YEAR_IN_SECONDS = 31536000;
enum {
	ALIAS=0,
	OFFER, 
	CERT,
	ESCROW,
	ASSET,
	ASSETALLOCATION
};
enum {
	ACCEPT_TRANSFER_NONE=0,
	ACCEPT_TRANSFER_CERTIFICATES,
	ACCEPT_TRANSFER_ALL,
};
class CAliasUnprunable
{
	public:
	std::vector<unsigned char> vchGUID;
    uint64_t nExpireTime;
	CAliasUnprunable() {
        SetNull();
    }

	ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
		READWRITE(vchGUID);
		READWRITE(VARINT(nExpireTime));
	}

    inline friend bool operator==(const CAliasUnprunable &a, const CAliasUnprunable &b) {
        return (
		a.vchGUID == b.vchGUID
		&& a.nExpireTime == b.nExpireTime
        );
    }

    inline CAliasUnprunable operator=(const CAliasUnprunable &b) {
		vchGUID = b.vchGUID;
		nExpireTime = b.nExpireTime;
        return *this;
    }

    inline friend bool operator!=(const CAliasUnprunable &a, const CAliasUnprunable &b) {
        return !(a == b);
    }

	inline void SetNull() { vchGUID.clear(); nExpireTime = 0; }
    inline bool IsNull() const { return (vchGUID.empty()); }
};
class COfferLinkWhitelistEntry {
public:
	std::vector<unsigned char> aliasLinkVchRand;
	unsigned char nDiscountPct;
	COfferLinkWhitelistEntry() {
		SetNull();
	}

	ADD_SERIALIZE_METHODS;
	template <typename Stream, typename Operation>
	inline void SerializationOp(Stream& s, Operation ser_action) {
		READWRITE(aliasLinkVchRand);
		READWRITE(VARINT(nDiscountPct));
	}

	inline friend bool operator==(const COfferLinkWhitelistEntry &a, const COfferLinkWhitelistEntry &b) {
		return (
			a.aliasLinkVchRand == b.aliasLinkVchRand
			&& a.nDiscountPct == b.nDiscountPct
			);
	}

	inline COfferLinkWhitelistEntry operator=(const COfferLinkWhitelistEntry &b) {
		aliasLinkVchRand = b.aliasLinkVchRand;
		nDiscountPct = b.nDiscountPct;
		return *this;
	}

	inline friend bool operator!=(const COfferLinkWhitelistEntry &a, const COfferLinkWhitelistEntry &b) {
		return !(a == b);
	}

	inline void SetNull() {
		aliasLinkVchRand.clear(); 
		nDiscountPct = 0;
	}
	inline bool IsNull() const { return (aliasLinkVchRand.empty()); }

};
typedef std::map<std::vector<unsigned char>, COfferLinkWhitelistEntry> whitelistMap_t;
class COfferLinkWhitelist {
public:
	whitelistMap_t entries;
	COfferLinkWhitelist() {
		SetNull();
	}

	ADD_SERIALIZE_METHODS;
	template <typename Stream, typename Operation>
	inline void SerializationOp(Stream& s, Operation ser_action) {
		READWRITE(entries);
	}
	bool GetLinkEntryByHash(const std::vector<unsigned char> &ahash, COfferLinkWhitelistEntry &entry) const;

	inline void RemoveWhitelistEntry(const std::vector<unsigned char> &ahash) {
		entries.erase(ahash);
	}
	inline void PutWhitelistEntry(const COfferLinkWhitelistEntry &theEntry) {
		if (entries.count(theEntry.aliasLinkVchRand) > 0) {
			entries[theEntry.aliasLinkVchRand] = theEntry;
			return;
		}
		entries[theEntry.aliasLinkVchRand] = theEntry;
	}
	inline friend bool operator==(const COfferLinkWhitelist &a, const COfferLinkWhitelist &b) {
		return (
			a.entries == b.entries
			);
	}

	inline COfferLinkWhitelist operator=(const COfferLinkWhitelist &b) {
		entries = b.entries;
		return *this;
	}

	inline friend bool operator!=(const COfferLinkWhitelist &a, const COfferLinkWhitelist &b) {
		return !(a == b);
	}

	inline void SetNull() { entries.clear(); }
	inline bool IsNull() const { return (entries.empty()); }

};
class CAliasIndex {
public:
	std::vector<unsigned char> vchAlias;
	std::vector<unsigned char> vchGUID;
    uint256 txHash;
    unsigned int nHeight;
	uint64_t nExpireTime;
	std::vector<unsigned char> vchAddress;
	std::vector<unsigned char> vchEncryptionPublicKey;
	std::vector<unsigned char> vchEncryptionPrivateKey;
	// 1 accepts certs, 2 accepts assets, 3 accepts both (default)
	unsigned char nAcceptTransferFlags;
	// 1 can edit, 2 can edit/transfer
	unsigned char nAccessFlags;
	std::vector<unsigned char> vchPublicValue;
	// to control reseller access + wholesaler discounts
	COfferLinkWhitelist offerWhitelist;
    CAliasIndex() { 
        SetNull();
    }
    CAliasIndex(const CTransaction &tx) {
        SetNull();
        UnserializeFromTx(tx);
    }
	inline void ClearAlias()
	{
		vchEncryptionPublicKey.clear();
		vchEncryptionPrivateKey.clear();
		vchPublicValue.clear();
		vchAddress.clear();
		offerWhitelist.SetNull();
	}

	ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
	inline void SerializationOp(Stream& s, Operation ser_action) {        
		READWRITE(txHash);
		READWRITE(VARINT(nHeight));
		READWRITE(vchPublicValue);
		READWRITE(vchEncryptionPublicKey);
		READWRITE(vchEncryptionPrivateKey);
		READWRITE(vchAlias);
		READWRITE(vchGUID);
		READWRITE(VARINT(nExpireTime));
		READWRITE(VARINT(nAcceptTransferFlags));
		READWRITE(VARINT(nAccessFlags));
		READWRITE(vchAddress);	
		READWRITE(offerWhitelist);
	}
    inline friend bool operator==(const CAliasIndex &a, const CAliasIndex &b) {
		return (a.vchGUID == b.vchGUID && a.vchAlias == b.vchAlias);
    }

    inline friend bool operator!=(const CAliasIndex &a, const CAliasIndex &b) {
        return !(a == b);
    }
    inline CAliasIndex operator=(const CAliasIndex &b) {
		vchGUID = b.vchGUID;
		nExpireTime = b.nExpireTime;
		vchAlias = b.vchAlias;
        txHash = b.txHash;
        nHeight = b.nHeight;
        vchPublicValue = b.vchPublicValue;
		vchAddress = b.vchAddress;
		nAcceptTransferFlags = b.nAcceptTransferFlags;
		vchEncryptionPrivateKey = b.vchEncryptionPrivateKey;
		vchEncryptionPublicKey = b.vchEncryptionPublicKey;
		nAccessFlags = b.nAccessFlags;
		offerWhitelist = b.offerWhitelist;
        return *this;
    }   
	inline void SetNull() { offerWhitelist.SetNull(); nAccessFlags = 2; vchAddress.clear(); vchEncryptionPublicKey.clear(); vchEncryptionPrivateKey.clear(); nAcceptTransferFlags = 3; nExpireTime = 0; vchGUID.clear(); vchAlias.clear(); txHash.SetNull(); nHeight = 0; vchPublicValue.clear(); }
    inline bool IsNull() const { return (vchAlias.empty()); }
	bool UnserializeFromTx(const CTransaction &tx);
	bool UnserializeFromData(const std::vector<unsigned char> &vchData, const std::vector<unsigned char> &vchHash);
	void Serialize(std::vector<unsigned char>& vchData);
};

class CAliasDB : public CDBWrapper {
public:
    CAliasDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "aliases", nCacheSize, fMemory, fWipe) {
    }
	bool WriteAlias(const CAliasUnprunable &aliasUnprunable, const std::vector<unsigned char>& address, const CAliasIndex& alias, const int &op) {
		if(address.empty())
			return false;	
		bool writeState = false;
		writeState = Write(make_pair(std::string("namei"), alias.vchAlias), alias) && Write(make_pair(std::string("namea"), address), alias.vchAlias) && Write(make_pair(std::string("nameu"), alias.vchAlias), aliasUnprunable);
		if(writeState)
			WriteAliasIndex(alias, op);
		return writeState;
	}

	bool EraseAlias(const std::vector<unsigned char>& vchAlias, bool cleanup = false) {
		return Erase(make_pair(std::string("namei"), vchAlias));
	}
	bool ReadAlias(const std::vector<unsigned char>& vchAlias, CAliasIndex& alias) {
		return Read(make_pair(std::string("namei"), vchAlias), alias);
	}
	bool ReadAddress(const std::vector<unsigned char>& address, std::vector<unsigned char>& name) {
		return Read(make_pair(std::string("namea"), address), name);
	}
	bool ReadAliasUnprunable(const std::vector<unsigned char>& alias, CAliasUnprunable& aliasUnprunable) {
		return Read(make_pair(std::string("nameu"), alias), aliasUnprunable);
	}
	bool EraseAddress(const std::vector<unsigned char>& address) {
	    return Erase(make_pair(std::string("namea"), address));
	}
	bool ExistsAddress(const std::vector<unsigned char>& address) {
	    return Exists(make_pair(std::string("namea"), address));
	}
	bool CleanupDatabase(int &servicesCleaned);
	void WriteAliasIndex(const CAliasIndex& alias, const int &op);
	void WriteAliasIndexHistory(const CAliasIndex& alias, const int &op);
	void WriteAliasIndexTxHistory(const std::string &user1, const std::string &user2, const std::string &user3, const uint256 &txHash, const unsigned int& nHeight, const std::string &type, const std::string &guid);
	bool ScanAliases(const int count, const int from, const UniValue& oOptions, UniValue& oRes);
};

class COfferDB;
class CCertDB;
class CEscrowDB;
class CAssetDB;
class CAssetAllocationDB;
class CAssetAllocationTransactionsDB;
extern CAliasDB *paliasdb;
extern COfferDB *pofferdb;
extern CCertDB *pcertdb;
extern CEscrowDB *pescrowdb;
extern CAssetDB *passetdb;
extern CAssetAllocationDB *passetallocationdb;
extern CAssetAllocationTransactionsDB *passetallocationtransactionsdb;

std::string stringFromVch(const std::vector<unsigned char> &vch);
std::vector<unsigned char> vchFromValue(const UniValue& value);
std::vector<unsigned char> vchFromString(const std::string &str);
std::string stringFromValue(const UniValue& value);
const int BILLIECOIN_TX_VERSION = 0x7400;
bool IsValidAliasName(const std::vector<unsigned char> &vchAlias);
bool CheckAliasInputs(const CCoinsViewCache &inputs, const CTransaction &tx, int op, const std::vector<std::vector<unsigned char> > &vvchArgs, bool fJustCheck, int nHeight, std::string &errorMessage,bool bSanityCheck=false);
void CreateRecipient(const CScript& scriptPubKey, CRecipient& recipient);
void CreateAliasRecipient(const CScript& scriptPubKey, CRecipient& recipient);
void CreateFeeRecipient(CScript& scriptPubKey, const std::vector<unsigned char>& data, CRecipient& recipient);
void CreateAliasRecipient(const CScript& scriptPubKey, CRecipient& recipient);
CAmount GetDataFee(const CScript& scriptPubKey, bool bRequired=false);
bool IsAliasOp(int op);
bool GetAlias(const std::vector<unsigned char> &vchAlias, CAliasIndex& alias);
bool CheckParam(const UniValue& params, const unsigned int index);
bool GetAliasOfTx(const CTransaction& tx, std::vector<unsigned char>& name);
bool DecodeAliasTx(const CTransaction& tx, int& op, std::vector<std::vector<unsigned char> >& vvch);
bool DecodeAndParseAliasTx(const CTransaction& tx, int& op, std::vector<std::vector<unsigned char> >& vvch, char &type);
bool DecodeAndParseBilliecoinTx(const CTransaction& tx, int& op, std::vector<std::vector<unsigned char> >& vvch, char &type);
bool DecodeAliasScript(const CScript& script, int& op,
		std::vector<std::vector<unsigned char> > &vvch);
bool FindAliasInTx(const CCoinsViewCache &inputs, const CTransaction& tx, std::vector<std::vector<unsigned char> >& vvch);
unsigned int aliasunspent(const std::vector<unsigned char> &vchAlias, COutPoint& outPoint);
bool GetAddressFromAlias(const std::string& strAlias, std::string& strAddress, std::vector<unsigned char> &vchPubKey);
bool GetAliasFromAddress(const std::string& strAddress, std::string& strAlias, std::vector<unsigned char> &vchPubKey);
int getFeePerByte(const uint64_t &paymentOptionMask);
float getEscrowFee();
std::string aliasFromOp(int op);
std::string GenerateBilliecoinGuid();
bool RemoveAliasScriptPrefix(const CScript& scriptIn, CScript& scriptOut);
int GetBilliecoinDataOutput(const CTransaction& tx);
bool IsBilliecoinDataOutput(const CTxOut& out);
bool GetBilliecoinData(const CTransaction &tx, std::vector<unsigned char> &vchData, std::vector<unsigned char> &vchHash, int& nOut);
bool GetBilliecoinData(const CScript &scriptPubKey, std::vector<unsigned char> &vchData, std::vector<unsigned char> &vchHash);
bool IsSysServiceExpired(const uint64_t &nTime);
bool GetTimeToPrune(const CScript& scriptPubKey, uint64_t &nTime);
bool IsBilliecoinScript(const CScript& scriptPubKey, int &op, std::vector<std::vector<unsigned char> > &vvchArgs);
bool RemoveBilliecoinScript(const CScript& scriptPubKeyIn, CScript& scriptPubKeyOut);
void SysTxToJSON(const int op, const std::vector<unsigned char> &vchData, const std::vector<unsigned char> &vchHash, UniValue &entry, const char& type);
void AliasTxToJSON(const int op, const std::vector<unsigned char> &vchData, const std::vector<unsigned char> &vchHash, UniValue &entry);
bool BuildAliasJson(const CAliasIndex& alias, UniValue& oName);
void CleanupBilliecoinServiceDatabases(int &servicesCleaned);
void GetAddress(const CAliasIndex &alias, CBilliecoinAddress* address, CScript& script, const uint32_t nPaymentOption=1);
std::string GetBilliecoinTransactionDescription(const CTransaction& tx, const int op, std::string& responseEnglish, const char &type, std::string& responseGUID);
bool BuildAliasIndexerHistoryJson(const CAliasIndex& alias, UniValue& oName);
bool DoesAliasExist(const std::string &strAddress);
bool IsOutpointMature(const COutPoint& outpoint, bool fUseInstantSend = false);
UniValue billiecointxfund_helper(const std::vector<unsigned char> &vchAlias, const std::vector<unsigned char> &vchWitness, const CRecipient &aliasRecipient, std::vector<CRecipient> &vecSend);
bool FlushBilliecoinDBs();
void ToLowerCase(std::vector<unsigned char>& vchValue);
bool FindAssetOwnerInTx(const CCoinsViewCache &inputs, const CTransaction& tx, const std::string& ownerAddressToMatch);
#endif // ALIAS_H
