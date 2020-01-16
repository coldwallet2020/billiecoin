![Billicoin Logo](https://billiecoin.green/img/core-img/logo.png)
=========================

http://www.billiecoin.green

What is Billiecoin?
----------------

Billiecoin is a merge-minable SHA256 coin which provides an array of useful services
which leverage the bitcoin protocol and blockchain technology.

- 1 minute block targets, diff retarget each block using Dark Gravity Wave(24) 
- 300 million total coins (deflation 5 percent per year, deflation on all payouts)
- Block time: 60 seconds target
- Rewards: 42 Billiecoins per block deflated 5 percent per year. 
  - 10 percent to governance proposals (4.2 Billiecoins / block). 
  - 22.5 percent to miner (9.45 Billiecoins / block)
  - 67.5 percent to masternode (28.35 Billiecoins / block)
- SHA256 Proof of Work
- Mineable either exclusively or via merge-mining any SHA256 PoW coin
- Billiecoin data service fees burned
- Masternode collateral requirement: 25,000 Billiecoins
- Masternode seniority: 3 percent every 4 months until 27 percent over 3 years
- Governance proposals payout schedule: every month
- Governance funding per round (183960 Billiecoins per month)

Services include:

- Hybrid layer 2 PoW/PoS consensus with bonded validator system (masternodes)
- Decentralized governance (blockchain pays for work via proposals and masternode votes)

For more information, as well as an immediately useable, binary version of
the Billiecoin client sofware, see https://www.billiecoin.green.


License
-------

Billiecoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Fast Build Daemon in Ubuntu 16.4 or 18.4
-----------------------------------

```
sudo apt -y update
sudo apt -y install build-essential libssl-dev libdb++-dev libboost-all-dev libcrypto++-dev libqrencode-dev libminiupnpc-dev libgmp-dev libgmp3-dev autoconf autogen automake libtool autotools-dev pkg-config bsdmainutils software-properties-common libzmq3-dev libminiupnpc-dev libssl-dev libevent-dev

sudo add-apt-repository ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install libdb4.8-dev libdb4.8++-dev -y

git clone https://github.com/coldwallet2020/billiecoin.git

cd billiecoin
find . -name "*.sh" -exec sudo chmod 755 {} \;
./autogen.sh
./configure --without-gui
make

cd src

strip billiecoind
strip billiecoin-cli

cp billiecoind /usr/bin
cp billiecoin-cli /usr/bin

billiecoind -daemon
```
