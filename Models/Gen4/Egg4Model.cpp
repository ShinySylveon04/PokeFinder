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

#include "Egg4Model.hpp"

Egg4GeneratorModel::Egg4GeneratorModel(QObject *parent, Method method)
    : QAbstractTableModel(parent)
{
    this->method = method;
}

void Egg4GeneratorModel::setModel(vector<Frame4> frames)
{
    if (frames.empty())
        return;
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.insert(model.end(), frames.begin(), frames.end());
    emit endInsertRows();
}

void Egg4GeneratorModel::clear()
{
    int i = rowCount();
    emit beginRemoveRows(QModelIndex(), 0, i == 0 ? 0 : i - 1);
    model.clear();
    model.shrink_to_fit();
    emit endRemoveRows();
}

void Egg4GeneratorModel::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int Egg4GeneratorModel::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return (int)model.size();
}

int Egg4GeneratorModel::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case Gen4Normal:
        case Gen4Masuada:
            return 6;
        case DPPtIVs:
            return 10;
        case HGSSIVs:
            return 11;
    }
    return 0;
}

QVariant Egg4GeneratorModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    int row = index.row();
    int column = index.column();
    Frame4 frame = model[row];

    switch (method)
    {
        case Gen4Normal:
        case Gen4Masuada:
            switch (column)
            {
                case 0:
                    return frame.frame;
                case 1:
                    return QString::number(frame.pid, 16).toUpper().rightJustified(8, '0');
                case 2:
                    return frame.getShiny();
                case 3:
                    return frame.getNature();
                case 4:
                    return frame.ability;
                case 5:
                    return frame.getGender();
            }
        case DPPtIVs:
            switch (column)
            {
                case 0:
                    return frame.frame;
                case 1:
                    return frame.chatotPitch();
                case 2:
                    return frame.ivs[0];
                case 3:
                    return frame.ivs[1];
                case 4:
                    return frame.ivs[2];
                case 5:
                    return frame.ivs[3];
                case 6:
                    return frame.ivs[4];
                case 7:
                    return frame.ivs[5];
                case 8:
                    return frame.getPower();
                case 9:
                    return frame.power;
            }
        case HGSSIVs:
            switch (column)
            {
                case 0:
                    return frame.frame;
                case 1:
                    return frame.getCall();
                case 2:
                    return frame.chatotPitch();
                case 3:
                    return frame.ivs[0];
                case 4:
                    return frame.ivs[1];
                case 5:
                    return frame.ivs[2];
                case 6:
                    return frame.ivs[3];
                case 7:
                    return frame.ivs[4];
                case 8:
                    return frame.ivs[5];
                case 9:
                    return frame.getPower();
                case 10:
                    return frame.power;
            }
    }

    return QVariant();
}

QVariant Egg4GeneratorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (method)
    {
        case Gen4Normal:
        case Gen4Masuada:
            switch (section)
            {
                case 0:
                    return tr("Frame");
                case 1:
                    return tr("PID");
                case 2:
                    return "!!!";
                case 3:
                    return tr("Nature");
                case 4:
                    return tr("Ability");
                case 5:
                    return tr("Gender");
            }
        case DPPtIVs:
            switch (section)
            {
                case 0:
                    return tr("Frame");
                case 1:
                    return tr("Chatot");
                case 2:
                    return tr("HP");
                case 3:
                    return tr("Atk");
                case 4:
                    return tr("Def");
                case 5:
                    return tr("SpA");
                case 6:
                    return tr("SpD");
                case 7:
                    return tr("Spe");
                case 8:
                    return tr("Hidden");
                case 9:
                    return tr("Power");
            }
        case HGSSIVs:
            switch (section)
            {
                case 0:
                    return tr("Frame");
                case 1:
                    return tr("Call");
                case 2:
                    return tr("Chatot");
                case 3:
                    return tr("HP");
                case 4:
                    return tr("Atk");
                case 5:
                    return tr("Def");
                case 6:
                    return tr("SpA");
                case 7:
                    return tr("SpD");
                case 8:
                    return tr("Spe");
                case 9:
                    return tr("Hidden");
                case 10:
                    return tr("Power");
            }
    }

    return QVariant();
}


Egg4SearcherModel::Egg4SearcherModel(QObject *parent, Method method)
    : QAbstractTableModel(parent)
{
    this->method = method;
}

void Egg4SearcherModel::setModel(vector<Frame4> frames)
{
    if (frames.empty())
        return;
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.insert(model.end(), frames.begin(), frames.end());
    emit endInsertRows();
}

void Egg4SearcherModel::addItems(vector<Frame4> frames)
{
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.insert(model.end(), frames.begin(), frames.end());
    emit endInsertRows();
}

void Egg4SearcherModel::clear()
{
    int i = rowCount();
    emit beginRemoveRows(QModelIndex(), 0, i == 0 ? 0 : i - 1);
    model.clear();
    model.shrink_to_fit();
    emit endRemoveRows();
}

void Egg4SearcherModel::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int Egg4SearcherModel::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return (int)model.size();
}

int Egg4SearcherModel::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case Gen4Normal:
        case Gen4Masuada:
            return 7;
        case DPPtIVs:
        case HGSSIVs:
            return 10;
    }
    return 0;
}

QVariant Egg4SearcherModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    int row = index.row();
    int column = index.column();
    Frame4 frame = model[row];

    switch (method)
    {
        case Gen4Normal:
        case Gen4Masuada:
            switch (column)
            {
                case 0:
                    return QString::number(frame.initialSeed, 16).toUpper().rightJustified(8, '0');
                case 1:
                    return frame.frame;
                case 2:
                    return QString::number(frame.pid, 16).toUpper().rightJustified(8, '0');
                case 3:
                    return frame.getShiny();
                case 4:
                    return frame.getNature();
                case 5:
                    return frame.ability;
                case 6:
                    return frame.getGender();
            }
        case DPPtIVs:
        case HGSSIVs:
            switch (column)
            {
                case 0:
                    return QString::number(frame.initialSeed, 16).toUpper().rightJustified(8, '0');
                case 1:
                    return frame.frame;
                case 2:
                    return frame.ivs[0];
                case 3:
                    return frame.ivs[1];
                case 4:
                    return frame.ivs[2];
                case 5:
                    return frame.ivs[3];
                case 6:
                    return frame.ivs[4];
                case 7:
                    return frame.ivs[5];
                case 8:
                    return frame.getPower();
                case 9:
                    return frame.power;
            }
    }

    return QVariant();
}

QVariant Egg4SearcherModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (method)
    {
        case Gen4Normal:
        case Gen4Masuada:
            switch (section)
            {
                case 0:
                    return tr("Seed");
                case 1:
                    return tr("Frame");
                case 2:
                    return tr("PID");
                case 3:
                    return "!!!";
                case 4:
                    return tr("Nature");
                case 5:
                    return tr("Ability");
                case 6:
                    return tr("Gender");
            }
        case DPPtIVs:
        case HGSSIVs:
            switch (section)
            {
                case 0:
                    return tr("Seed");
                case 1:
                    return tr("Frame");
                case 2:
                    return tr("HP");
                case 3:
                    return tr("Atk");
                case 4:
                    return tr("Def");
                case 5:
                    return tr("SpA");
                case 6:
                    return tr("SpD");
                case 7:
                    return tr("Spe");
                case 8:
                    return tr("Hidden");
                case 9:
                    return tr("Power");
            }
    }

    return QVariant();
}
