// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2018-2020 The Billiecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BILLIECOIN_BILLIECOINCONSENSUS_H
#define BILLIECOIN_BILLIECOINCONSENSUS_H

#if defined(BUILD_BILLIECOIN_INTERNAL) && defined(HAVE_CONFIG_H)
#include "config/billiecoin-config.h"
  #if defined(_WIN32)
    #if defined(DLL_EXPORT)
      #if defined(HAVE_FUNC_ATTRIBUTE_DLLEXPORT)
        #define EXPORT_SYMBOL __declspec(dllexport)
      #else
        #define EXPORT_SYMBOL
      #endif
    #endif
  #elif defined(HAVE_FUNC_ATTRIBUTE_VISIBILITY)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBBILLIECOINCONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define BILLIECOINCONSENSUS_API_VER 0

typedef enum billiecoinconsensus_error_t
{
    billiecoinconsensus_ERR_OK = 0,
    billiecoinconsensus_ERR_TX_INDEX,
    billiecoinconsensus_ERR_TX_SIZE_MISMATCH,
    billiecoinconsensus_ERR_TX_DESERIALIZE,
    billiecoinconsensus_ERR_INVALID_FLAGS,
} billiecoinconsensus_error;

/** Script verification flags */
enum
{
    billiecoinconsensus_SCRIPT_FLAGS_VERIFY_NONE                = 0,
    billiecoinconsensus_SCRIPT_FLAGS_VERIFY_P2SH                = (1U << 0), // evaluate P2SH (BIP16) subscripts
    billiecoinconsensus_SCRIPT_FLAGS_VERIFY_DERSIG              = (1U << 2), // enforce strict DER (BIP66) compliance
    billiecoinconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY           = (1U << 4), // enforce NULLDUMMY (BIP147)
    billiecoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9), // enable CHECKLOCKTIMEVERIFY (BIP65)
    billiecoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10), // enable CHECKSEQUENCEVERIFY (BIP112)
    billiecoinconsensus_SCRIPT_FLAGS_VERIFY_ALL                 = billiecoinconsensus_SCRIPT_FLAGS_VERIFY_P2SH | billiecoinconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
                                                            billiecoinconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY | billiecoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
                                                            billiecoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not NULL, err will contain an error/success code for the operation
EXPORT_SYMBOL int billiecoinconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, billiecoinconsensus_error* err);

EXPORT_SYMBOL unsigned int billiecoinconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // BILLIECOIN_BILLIECOINCONSENSUS_H
