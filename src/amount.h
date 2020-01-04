// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2018-2020 The Billiecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BILLIECOIN_AMOUNT_H
#define BILLIECOIN_AMOUNT_H

#include "serialize.h"

#include <stdlib.h>
#include <string>

/** Amount in finneas (Can be negative) */
typedef int64_t CAmount;

static const CAmount COIN = 100000000;
static const CAmount CENT = 1000000;

extern const std::string CURRENCY_UNIT;

/** No amount larger than this (in finnea) is valid.
 *
 * Note that this constant is *not* the total money supply, which in Billiecoin
 * currently happens to be less than 21,000,000 BTC for various reasons, but
 * rather a sanity check. As this sanity check is used by consensus-critical
 * validation code, the exact value of the MAX_MONEY constant is consensus
 * critical; in unusual circumstances like a(nother) overflow bug that allowed
 * for the creation of coins out of thin air modification could lead to a fork.
 * */
static const CAmount MAX_MONEY = 300000000 * COIN;
inline bool MoneyRange(const CAmount& nValue) { return (nValue >= 0 && nValue <= MAX_MONEY); }

/**
 * Fee rate in finneas per kilobyte: CAmount / kB
 */
class CFeeRate
{
private:
    CAmount nFinneasPerK; // unit is finneas-per-1,000-bytes
public:
    /** Fee rate of 0 finneas per kB */
    CFeeRate() : nFinneasPerK(0) { }
    explicit CFeeRate(const CAmount& _nFinneasPerK): nFinneasPerK(_nFinneasPerK) { }
    /** Constructor for a fee rate in finneas per kB. The size in bytes must not exceed (2^63 - 1)*/
    CFeeRate(const CAmount& nFeePaid, size_t nBytes);
    CFeeRate(const CFeeRate& other) { nFinneasPerK = other.nFinneasPerK; }
    /**
     * Return the fee in finneas for the given size in bytes.
     */
    CAmount GetFee(size_t nBytes) const;
    /**
     * Return the fee in finneas for a size of 1000 bytes
     */
    CAmount GetFeePerK() const { return GetFee(1000); }
    friend bool operator<(const CFeeRate& a, const CFeeRate& b) { return a.nFinneasPerK < b.nFinneasPerK; }
    friend bool operator>(const CFeeRate& a, const CFeeRate& b) { return a.nFinneasPerK > b.nFinneasPerK; }
    friend bool operator==(const CFeeRate& a, const CFeeRate& b) { return a.nFinneasPerK == b.nFinneasPerK; }
    friend bool operator<=(const CFeeRate& a, const CFeeRate& b) { return a.nFinneasPerK <= b.nFinneasPerK; }
    friend bool operator>=(const CFeeRate& a, const CFeeRate& b) { return a.nFinneasPerK >= b.nFinneasPerK; }
    CFeeRate& operator+=(const CFeeRate& a) { nFinneasPerK += a.nFinneasPerK; return *this; }
    std::string ToString() const;

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(nFinneasPerK);
    }
};

#endif //  BILLIECOIN_AMOUNT_H
