/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef QTEXTBOX_HPP
#define QTEXTBOX_HPP

#include <QLineEdit>

typedef uint64_t u64;
typedef uint32_t u32;

class QTextBox : public QLineEdit
{
    Q_OBJECT

private:
    u64 maxValue = 0;
    u64 minValue;
    u32 base;
    QRegExp filter;

private slots:
    void onTextChanged(QString string);

public:
    QTextBox(QWidget *parent = 0);
    void setValues(u64 min, u64 shift, bool isDecimal);
    void setFilter(QString string);
    void setValue(u64 value);
    void setBase(u32 base);

};

#endif // QTEXTBOX_HPP
