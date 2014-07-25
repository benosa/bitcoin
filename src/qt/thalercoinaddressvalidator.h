// Copyright (c) 2011-2014 The Thalercoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef THALERCOINADDRESSVALIDATOR_H
#define THALERCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class ThalercoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ThalercoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Thalercoin address widget validator, checks for a valid thalercoin address.
 */
class ThalercoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ThalercoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // THALERCOINADDRESSVALIDATOR_H
