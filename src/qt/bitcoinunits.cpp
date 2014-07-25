// Copyright (c) 2011-2013 The Thalercoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "thalercoinunits.h"

#include "core.h"

#include <QStringList>

ThalercoinUnits::ThalercoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<ThalercoinUnits::Unit> ThalercoinUnits::availableUnits()
{
    QList<ThalercoinUnits::Unit> unitlist;
    unitlist.append(TLR);
    unitlist.append(mTLR);
    unitlist.append(uTLR);
    return unitlist;
}

bool ThalercoinUnits::valid(int unit)
{
    switch(unit)
    {
    case TLR:
    case mTLR:
    case uTLR:
        return true;
    default:
        return false;
    }
}

QString ThalercoinUnits::id(int unit)
{
    switch(unit)
    {
    case TLR: return QString("tlr");
    case mTLR: return QString("mtlr");
    case uTLR: return QString("utlr");
    default: return QString("???");
    }
}

QString ThalercoinUnits::name(int unit)
{
    switch(unit)
    {
    case TLR: return QString("TLR");
    case mTLR: return QString("mTLR");
    case uTLR: return QString::fromUtf8("μTLR");
    default: return QString("???");
    }
}

QString ThalercoinUnits::description(int unit)
{
    switch(unit)
    {
    case TLR: return QString("Thalercoins");
    case mTLR: return QString("Milli-Thalercoins (1 / 1" THIN_SP_UTF8 "000)");
    case uTLR: return QString("Micro-Thalercoins (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
    default: return QString("???");
    }
}

qint64 ThalercoinUnits::factor(int unit)
{
    switch(unit)
    {
    case TLR:  return 100000000;
    case mTLR: return 100000;
    case uTLR: return 100;
    default:   return 100000000;
    }
}

int ThalercoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case TLR: return 8;
    case mTLR: return 5;
    case uTLR: return 2;
    default: return 0;
    }
}

QString ThalercoinUnits::format(int unit, qint64 n, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Use SI-stule separators as these are locale indendent and can't be
    // confused with the decimal marker.  Rule is to use a thin space every
    // three digits on *both* sides of the decimal point - but only if there
    // are five or more digits
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    int r_size = remainder_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && r_size > 4))
        for (int i = 3, adj = 0; i < r_size ; i += 3, adj++)
            remainder_str.insert(i + adj, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');
    return quotient_str + QString(".") + remainder_str;
}


// TODO: Review all remaining calls to ThalercoinUnits::formatWithUnit to
// TODO: determine whether the output is used in a plain text context
// TODO: or an HTML context (and replace with
// TODO: TlroinUnits::formatHtmlWithUnit in the latter case). Hopefully
// TODO: there aren't instances where the result could be used in
// TODO: either context.

// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString ThalercoinUnits::formatWithUnit(int unit, qint64 amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + name(unit);
}

QString ThalercoinUnits::formatHtmlWithUnit(int unit, qint64 amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}


bool ThalercoinUnits::parse(int unit, const QString &value, qint64 *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    qint64 retvalue = str.toLongLong(&ok);
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

QString ThalercoinUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (ThalercoinUnits::valid(unit))
    {
        amountTitle += " ("+ThalercoinUnits::name(unit) + ")";
    }
    return amountTitle;
}

int ThalercoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant ThalercoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

qint64 ThalercoinUnits::maxMoney()
{
    return MAX_MONEY;
}
