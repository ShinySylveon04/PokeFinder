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

#ifndef SEEDTOTIME4_HPP
#define SEEDTOTIME4_HPP

#include <QMainWindow>
#include <QMessageBox>
#include <QModelIndex>
#include <QSettings>
#include <Models/Gen4/SeedtoTime4Model.hpp>
#include <PokeFinderCore/Objects/Utilities.hpp>
#include <Forms/Gen4/SearchCoinFlips.hpp>
#include <Forms/Gen4/SearchCalls.hpp>
#include <Forms/Gen4/RoamerMap.hpp>

namespace Ui
{
    class SeedtoTime4;
}

class SeedtoTime4 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *);

private:
    Ui::SeedtoTime4 *ui;
    SeedtoTime4Model *dppt = new SeedtoTime4Model(this, false);
    SeedtoTime4Model *dpptCalibrate = new SeedtoTime4Model(this, true);
    SeedtoTime4Model *hgss = new SeedtoTime4Model(this, false, HeartGold);
    SeedtoTime4Model *hgssCalibrate = new SeedtoTime4Model(this, true, HeartGold);

    void setupModels();
    void saveSettings();
    void loadSettings();
    vector<DateTime> generate(u32 seed, u32 year, bool forceSecond, int forcedSecond, Game version);
    vector<DateTime> calibrate(int minusDelay, int plusDelay, int minusSecond, int plusSecond, DateTime target);

private slots:
    void on_pushButtonGenerateDPPt_clicked();
    void on_pushButtonGenerateHGSS_clicked();
    void on_pushButtonCalibrateDPPt_clicked();
    void on_pushButtonCalibrateHGSS_clicked();
    void on_pushButtonSearchFlips_clicked();
    void on_pushButtonSearchCalls_clicked();
    void on_checkBoxSecondsHGSS_clicked(bool checked);
    void on_checkBoxSecondsDPPt_clicked(bool checked);

    void on_pushButtonMap_clicked();

public:
    explicit SeedtoTime4(QWidget *parent = 0);
    ~SeedtoTime4();

};

#endif // SEEDTOTIME4_HPP
