# Copyright (c) 2013-2016 The Bitcoin Core developers
# Copyright (c) 2018-2020 The Billiecoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS) $(ERROR_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

BILLIECOIN_CONFIG_INCLUDES=-I$(builddir)/config
BILLIECOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

BILLIECOIN_INCLUDES += -I$(srcdir)/secp256k1/include
BILLIECOIN_INCLUDES += $(UNIVALUE_CFLAGS)

LIBBILLIECOIN_SERVER=libbilliecoin_server.a
LIBBILLIECOIN_COMMON=libbilliecoin_common.a
LIBBILLIECOIN_CONSENSUS=libbilliecoin_consensus.a
LIBBILLIECOIN_CLI=libbilliecoin_cli.a
LIBBILLIECOIN_UTIL=libbilliecoin_util.a
LIBBILLIECOIN_CRYPTO=crypto/libbilliecoin_crypto.a
LIBBILLIECOINQT=qt/libbilliecoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_ZMQ
LIBBILLIECOIN_ZMQ=libbilliecoin_zmq.a
endif
if BUILD_BILLIECOIN_LIBS
LIBBILLIECOINCONSENSUS=libbilliecoinconsensus.la
endif
if ENABLE_WALLET
LIBBILLIECOIN_WALLET=libbilliecoin_wallet.a
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBBILLIECOIN_CRYPTO) \
  $(LIBBILLIECOIN_UTIL) \
  $(LIBBILLIECOIN_COMMON) \
  $(LIBBILLIECOIN_CONSENSUS) \
  $(LIBBILLIECOIN_SERVER) \
  $(LIBBILLIECOIN_CLI) \
  $(LIBBILLIECOIN_WALLET) \
  $(LIBBILLIECOIN_ZMQ)

lib_LTLIBRARIES = $(LIBBILLIECOINCONSENSUS)

bin_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_BILLIECOIND
  bin_PROGRAMS += billiecoind
endif

if BUILD_BILLIECOIN_UTILS
  bin_PROGRAMS += billiecoin-cli
endif

.PHONY: FORCE check-symbols check-security
# billiecoin core #
BILLIECOIN_CORE_H = \
  alias.h \
  offer.h \
  cert.h \
  ranges.h \
  graph.h \
  asset.h \
  assetallocation.h \
  escrow.h \
  feedback.h \
  auxpow.h \
  thread_pool/fixed_function.hpp \
  thread_pool/mpmc_bounded_queue.hpp \
  thread_pool/thread_pool.hpp \
  thread_pool/thread_pool_options.hpp \
  thread_pool/worker.hpp \
  addrdb.h \
  activemasternode.h \
  addressindex.h \
  spentindex.h \
  addrman.h \
  alert.h \
  base58.h \
  bip39.h \
  bip39_english.h \
  blockencodings.h \
  bloom.h \
  cachemap.h \
  cachemultimap.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coins.h \
  compat.h \
  compat/byteswap.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  core_io.h \
  core_memusage.h \
  cuckoocache.h \
  privatesend.h \
  privatesend-client.h \
  privatesend-server.h \
  privatesend-util.h \
  dsnotificationinterface.h \
  governance.h \
  governance-classes.h \
  governance-exceptions.h \
  governance-object.h \
  governance-validators.h \
  governance-vote.h \
  governance-votedb.h \
  flat-database.h \
  hdchain.h \
  httprpc.h \
  httpserver.h \
  indirectmap.h \
  init.h \
  instantx.h \
  key.h \
  keepass.h \
  keystore.h \
  dbwrapper.h \
  limitedmap.h \
  masternode.h \
  masternode-payments.h \
  masternode-sync.h \
  masternodeman.h \
  masternodeconfig.h \
  memusage.h \
  merkleblock.h \
  messagesigner.h \
  miner.h \
  net.h \
  net_processing.h \
  netaddress.h \
  netbase.h \
  netfulfilledman.h \
  netmessagemaker.h \
  noui.h \
  policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
  protocol.h \
  random.h \
  reverselock.h \
  rpc/client.h \
  rpc/mining.h \
  rpc/protocol.h \
  rpc/server.h \
  rpc/register.h \
  scheduler.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/ismine.h \
  spork.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/events.h \
  support/lockedpool.h \
  sync.h \
  threadsafety.h \
  threadinterrupt.h \
  timedata.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  undo.h \
  util.h \
  utilmoneystr.h \
  utiltime.h \
  validation.h \
  validationinterface.h \
  versionbits.h \
  wallet/coincontrol.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/rpcwallet.h \
  wallet/wallet.h \
  wallet/walletdb.h \
  warnings.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h \
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libbilliecoin_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between billiecoind and billiecoin-qt
libbilliecoin_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(BILLIECOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libbilliecoin_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbilliecoin_server_a_SOURCES = \
  alias.cpp \
  offer.cpp \
  cert.cpp \
  ranges.cpp \
  graph.cpp \
  asset.cpp \
  assetallocation.cpp \
  escrow.cpp \
  auxpow.cpp \
  activemasternode.cpp \
  addrman.cpp \
  addrdb.cpp \
  alert.cpp \
  bloom.cpp \
  blockencodings.cpp \
  chain.cpp \
  checkpoints.cpp \
  dsnotificationinterface.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  instantx.cpp \
  dbwrapper.cpp \
  governance.cpp \
  governance-classes.cpp \
  governance-object.cpp \
  governance-validators.cpp \
  governance-vote.cpp \
  governance-votedb.cpp \
  masternode.cpp \
  masternode-payments.cpp \
  masternode-sync.cpp \
  masternodeconfig.cpp \
  masternodeman.cpp \
  merkleblock.cpp \
  messagesigner.cpp \
  miner.cpp \
  net.cpp \
  netfulfilledman.cpp \
  net_processing.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
  privatesend.cpp \
  privatesend-server.cpp \
  rest.cpp \
  rpc/blockchain.cpp \
  rpc/masternode.cpp \
  rpc/governance.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtransaction.cpp \
  rpc/server.cpp \
  script/sigcache.cpp \
  script/ismine.cpp \
  sendalert.cpp \
  spork.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  ui_interface.cpp \
  validation.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  $(BILLIECOIN_CORE_H)

if ENABLE_ZMQ
libbilliecoin_zmq_a_CPPFLAGS = $(BILLIECOIN_INCLUDES) $(ZMQ_CFLAGS)
libbilliecoin_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbilliecoin_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif


# wallet: shared between billiecoind and billiecoin-qt, but only linked
# when wallet enabled
libbilliecoin_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(BILLIECOIN_INCLUDES)
libbilliecoin_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbilliecoin_wallet_a_SOURCES = \
  keepass.cpp \
  privatesend-client.cpp \
  privatesend-util.cpp \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/walletdb.cpp \
  policy/rbf.cpp \
  $(BILLIECOIN_CORE_H)

# crypto primitives library
crypto_libbilliecoin_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(BILLIECOIN_CONFIG_INCLUDES) $(PIC_FLAGS)
crypto_libbilliecoin_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) $(PIC_FLAGS)
crypto_libbilliecoin_crypto_a_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
  crypto/common.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha512.cpp \
  crypto/sha512.h

if USE_ASM
crypto_libbilliecoin_crypto_a_SOURCES += crypto/sha256_sse4.cpp
endif

# consensus: shared between all executables that validate any consensus rules.
libbilliecoin_consensus_a_CPPFLAGS = $(AM_CPPFLAGS) $(BILLIECOIN_INCLUDES)
libbilliecoin_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbilliecoin_consensus_a_SOURCES = \
  amount.h \
  arith_uint256.cpp \
  arith_uint256.h \
  consensus/merkle.cpp \
  consensus/merkle.h \
  consensus/params.h \
  consensus/validation.h \
  hash.cpp \
  hash.h \
  prevector.h \
  primitives/block.cpp \
  primitives/block.h \
  primitives/pureheader.cpp \
  primitives/pureheader.h \
  primitives/transaction.cpp \
  primitives/transaction.h \
  pubkey.cpp \
  pubkey.h \
  script/billiecoinconsensus.cpp \
  script/interpreter.cpp \
  script/interpreter.h \
  script/script.cpp \
  script/script.h \
  script/script_error.cpp \
  script/script_error.h \
  serialize.h \
  tinyformat.h \
  uint256.cpp \
  uint256.h \
  utilstrencodings.cpp \
  utilstrencodings.h \
  version.h

# common: shared between billiecoind, and billiecoin-qt and non-server tools
libbilliecoin_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(BILLIECOIN_INCLUDES)
libbilliecoin_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbilliecoin_common_a_SOURCES = \
  amount.cpp \
  base58.cpp \
  bip39.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  hdchain.cpp \
  key.cpp \
  keystore.cpp \
  netaddress.cpp \
  netbase.cpp \
  protocol.cpp \
  scheduler.cpp \
  script/sign.cpp \
  script/standard.cpp \
  warnings.cpp \
  $(BILLIECOIN_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libbilliecoin_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(BILLIECOIN_INCLUDES)
libbilliecoin_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbilliecoin_util_a_SOURCES = \
  support/lockedpool.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  random.cpp \
  rpc/protocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  threadinterrupt.cpp \
  util.cpp \
  utilmoneystr.cpp \
  utilstrencodings.cpp \
  utiltime.cpp \
  $(BILLIECOIN_CORE_H)

if GLIBC_BACK_COMPAT
libbilliecoin_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between billiecoin-cli and billiecoin-qt
libbilliecoin_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(BILLIECOIN_INCLUDES)
libbilliecoin_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbilliecoin_cli_a_SOURCES = \
  rpc/client.cpp \
  $(BILLIECOIN_CORE_H)

nodist_libbilliecoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# billiecoind binary #
billiecoind_SOURCES = billiecoind.cpp
billiecoind_CPPFLAGS = $(AM_CPPFLAGS) $(BILLIECOIN_INCLUDES)
billiecoind_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
billiecoind_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
billiecoind_SOURCES += billiecoind-res.rc
endif

billiecoind_LDADD = \
  $(LIBBILLIECOIN_SERVER) \
  $(LIBBILLIECOIN_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBBILLIECOIN_UTIL) \
  $(LIBBILLIECOIN_WALLET) \
  $(LIBBILLIECOIN_ZMQ) \
  $(LIBBILLIECOIN_CONSENSUS) \
  $(LIBBILLIECOIN_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBLEVELDB_SSE42) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

billiecoind_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

# billiecoin-cli binary #
billiecoin_cli_SOURCES = billiecoin-cli.cpp
billiecoin_cli_CPPFLAGS = $(AM_CPPFLAGS) $(BILLIECOIN_INCLUDES) $(EVENT_CFLAGS)
billiecoin_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
billiecoin_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
billiecoin_cli_SOURCES += billiecoin-cli-res.rc
endif

billiecoin_cli_LDADD = \
  $(LIBBILLIECOIN_CLI) \
  $(LIBUNIVALUE) \
  $(LIBBILLIECOIN_UTIL) \
  $(LIBBILLIECOIN_CRYPTO)
billiecoin_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# billiecoin-tx binary #
billiecoin_tx_SOURCES = billiecoin-tx.cpp
billiecoin_tx_CPPFLAGS = $(AM_CPPFLAGS) $(BILLIECOIN_INCLUDES)
billiecoin_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
billiecoin_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
billiecoin_tx_SOURCES += billiecoin-tx-res.rc
endif

billiecoin_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBBILLIECOIN_COMMON) \
  $(LIBBILLIECOIN_UTIL) \
  $(LIBBILLIECOIN_CONSENSUS) \
  $(LIBBILLIECOIN_CRYPTO) \
  $(LIBSECP256K1)

billiecoin_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# billiecoinconsensus library #
if BUILD_BILLIECOIN_LIBS
include_HEADERS = script/billiecoinconsensus.h
libbilliecoinconsensus_la_SOURCES = $(crypto_libbilliecoin_crypto_a_SOURCES) $(libbilliecoin_consensus_a_SOURCES)

if GLIBC_BACK_COMPAT
  libbilliecoinconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libbilliecoinconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libbilliecoinconsensus_la_LIBADD = $(LIBSECP256K1)
libbilliecoinconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_BILLIECOIN_INTERNAL
libbilliecoinconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CTAES_DIST =  crypto/ctaes/bench.c
CTAES_DIST += crypto/ctaes/ctaes.c
CTAES_DIST += crypto/ctaes/ctaes.h
CTAES_DIST += crypto/ctaes/README.md
CTAES_DIST += crypto/ctaes/test.c

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = $(CTAES_DIST)


config/billiecoin-config.h: config/stamp-h1
	@$(MAKE) -C $(top_builddir) $(subdir)/$(@)
config/stamp-h1: $(top_srcdir)/$(subdir)/config/billiecoin-config.h.in $(top_builddir)/config.status
	$(AM_V_at)$(MAKE) -C $(top_builddir) $(subdir)/$(@)
$(top_srcdir)/$(subdir)/config/billiecoin-config.h.in:  $(am__configure_deps)
	$(AM_V_at)$(MAKE) -C $(top_srcdir) $(subdir)/config/billiecoin-config.h.in

clean-local:
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf test/__pycache__

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
