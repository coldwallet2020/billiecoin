// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2020 The Billiecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BILLIECOIN_QT_BILLIECOINADDRESSVALIDATOR_H
#define BILLIECOIN_QT_BILLIECOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BilliecoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BilliecoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Billiecoin address widget validator, checks for a valid billiecoin address.
 */
class BilliecoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BilliecoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // BILLIECOIN_QT_BILLIECOINADDRESSVALIDATOR_H
