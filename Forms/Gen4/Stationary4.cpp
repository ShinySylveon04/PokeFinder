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

#include "Stationary4.hpp"
#include "ui_Stationary4.h"

Stationary4::Stationary4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Stationary4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();

    qRegisterMetaType<vector<Frame4>>("vector<Frame4>");
    connect(this, &Stationary4::updateView, this, &Stationary4::updateViewSearcher);
    connect(this, &Stationary4::updateProgress, this, &Stationary4::updateProgressBar);
}

Stationary4::~Stationary4()
{
    saveSettings();

    delete ui;
    delete g;
    delete s;
}

void Stationary4::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
            default:
                break;
        }
    }
}

void Stationary4::loadSettings()
{
    QSettings setting;
    if (setting.contains("stationary4MinDelay")) ui->minDelay->setText(setting.value("stationary4MinDelay").toString());
    if (setting.contains("stationary4MaxDelay")) ui->maxDelay->setText(setting.value("stationary4MaxDelay").toString());
}

void Stationary4::saveSettings()
{
    QSettings setting;
    setting.setValue("stationary4Profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("stationary4MinDelay", ui->minDelay->text().toInt());
    setting.setValue("stationary4MaxDelay", ui->maxDelay->text().toInt());
}

void Stationary4::setupModels()
{
    ui->tableViewGenerator->setModel(g);
    ui->tableViewGenerator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableViewSearcher->setModel(s);
    ui->tableViewSearcher->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->initialSeedGenerator->setValues(0, 32, false);
    ui->idGenerator->setValues(0, 48, true);
    ui->sidGenerator->setValues(0, 48, true);
    ui->startingFrameGenerator->setValues(1, 32, true);
    ui->maxResultsGenerator->setValues(1, 32, true);
    ui->delayGenerator->setValues(1, 32, true);

    ui->idSearcher->setValues(0, 48, true);
    ui->sidSearcher->setValues(0, 48, true);
    ui->minDelay->setValues(0, 48, true);
    ui->maxDelay->setValues(0, 48, true);
    ui->minFrame->setValues(0, 48, true);
    ui->maxFrame->setValues(0, 48, true);

    ui->comboBoxMethodGenerator->setItemData(0, Method1);
    ui->comboBoxMethodGenerator->setItemData(1, MethodJ);
    ui->comboBoxMethodGenerator->setItemData(2, MethodK);
    ui->comboBoxMethodGenerator->setItemData(3, WondercardIVs);

    ui->comboBoxMethodSearcher->setItemData(0, Method1);
    ui->comboBoxMethodSearcher->setItemData(1, MethodJ);
    ui->comboBoxMethodSearcher->setItemData(2, MethodK);
    ui->comboBoxMethodSearcher->setItemData(3, WondercardIVs);

    ui->comboBoxLeadSearcher->setItemData(0, Search);
    ui->comboBoxLeadSearcher->setItemData(1, Synchronize);
    ui->comboBoxLeadSearcher->setItemData(2, CuteCharm);
    ui->comboBoxLeadSearcher->setItemData(3, None);

    ui->comboBoxLeadGenerator->addItem(tr("None"));
    ui->comboBoxLeadGenerator->addItems(Nature::getNatures());

    ui->comboBoxNatureGenerator->setup();
    ui->comboBoxNatureSearcher->setup();

    ui->comboBoxHiddenPowerGenerator->setup();
    ui->comboBoxHiddenPowerSearcher->setup();

    loadSettings();
}

void Stationary4::updateProfiles()
{
    profiles = Profile4::loadProfileList();
    profiles.insert(profiles.begin(), Profile4());

    ui->comboBoxProfiles->clear();

    for (int i = 0; i < (int)profiles.size(); i++)
        ui->comboBoxProfiles->addItem(profiles.at(i).profileName);

    QSettings setting;
    int val = setting.value("stationary4Profile").toInt();
    if (val < ui->comboBoxProfiles->count())
        ui->comboBoxProfiles->setCurrentIndex(val >= 0 ? val : 0);
}

void Stationary4::refreshProfiles()
{
    emit alertProfiles(4);
}

void Stationary4::on_comboBoxProfiles_currentIndexChanged(int index)
{
    auto profile = profiles[index >= 0 ? index : 0];

    ui->idGenerator->setText(QString::number(profile.tid));
    ui->sidGenerator->setText(QString::number(profile.sid));
    ui->idSearcher->setText(QString::number(profile.tid));
    ui->sidSearcher->setText(QString::number(profile.sid));
    ui->profileTID->setText(QString::number(profile.tid));
    ui->profileSID->setText(QString::number(profile.sid));
    ui->profileGame->setText(profile.getVersion());
}

void Stationary4::on_anyNatureGenerator_clicked()
{
    ui->comboBoxNatureGenerator->uncheckAll();
}

void Stationary4::on_anyHiddenPowerGenerator_clicked()
{
    ui->comboBoxHiddenPowerGenerator->uncheckAll();
}

void Stationary4::on_anyNatureSearcher_clicked()
{
    ui->comboBoxNatureSearcher->uncheckAll();
}

void Stationary4::on_anyHiddenPowerSearcher_clicked()
{
    ui->comboBoxHiddenPowerSearcher->uncheckAll();
}

void Stationary4::on_generate_clicked()
{
    g->clear();
    g->setMethod((Method)ui->comboBoxMethodGenerator->currentData().toInt(NULL));

    u32 seed = ui->initialSeedGenerator->text().toUInt(NULL, 16);
    u32 startingFrame = ui->startingFrameGenerator->text().toUInt(NULL, 10);
    u32 maxResults = ui->maxResultsGenerator->text().toUInt(NULL, 10);
    u32 tid = ui->idGenerator->text().toUInt(NULL, 10);
    u32 sid = ui->sidGenerator->text().toUInt(NULL, 10);
    u32 offset = 0;
    if (ui->checkBoxDelayGenerator->isChecked())
        offset = ui->delayGenerator->text().toUInt(NULL, 10);

    int genderRatioIndex = ui->comboBoxGenderRatioGenerator->currentIndex();
    Generator4 generator = Generator4(maxResults, startingFrame, seed, tid, sid, offset, (Method)ui->comboBoxMethodGenerator->currentData().toInt(NULL));
    FrameCompare compare = FrameCompare(ui->ivFilterGenerator->getEvals(), ui->ivFilterGenerator->getValues(),
                                        ui->comboBoxGenderGenerator->currentIndex(), genderRatioIndex, ui->comboBoxAbilityGenerator->currentIndex(),
                                        ui->comboBoxNatureGenerator->getChecked(), ui->comboBoxHiddenPowerGenerator->getChecked(),
                                        ui->checkBoxShinyGenerator->isChecked(), ui->checkBoxDisableGenerator->isChecked());

    generator.encounterType = Stationary;
    if (ui->pushButtonLeadGenerator->text() == tr("Cute Charm"))
        generator.leadType = (Lead)ui->comboBoxLeadGenerator->currentData().toInt();
    else
    {
        int num = ui->comboBoxLeadGenerator->currentIndex();
        if (num == 0)
        {
            generator.leadType = None;
        }
        else
        {
            generator.leadType = Synchronize;
            generator.synchNature = Nature::getAdjustedNature(ui->comboBoxLeadGenerator->currentIndex() - 1);
        }
    }

    vector<Frame4> frames = generator.generate(compare);
    g->setModel(frames);
}

void Stationary4::search()
{
    u32 tid = ui->idSearcher->text().toUInt(NULL, 10);
    u32 sid = ui->sidSearcher->text().toUInt(NULL, 10);

    int genderRatioIndex = ui->comboBoxGenderRatioSearcher->currentIndex();
    FrameCompare compare = FrameCompare(ui->ivFilterSearcher->getEvals(), ui->ivFilterSearcher->getValues(), ui->comboBoxGenderSearcher->currentIndex(),
                                        genderRatioIndex, ui->comboBoxAbilitySearcher->currentIndex(), ui->comboBoxNatureSearcher->getChecked(),
                                        ui->comboBoxHiddenPowerSearcher->getChecked(), ui->checkBoxShinySearcher->isChecked(), false);
    Searcher4 searcher = Searcher4(tid, sid, genderRatioIndex, ui->minDelay->text().toUInt(), ui->maxDelay->text().toUInt(), ui->minFrame->text().toUInt(), ui->maxFrame->text().toUInt(), compare, (Method)ui->comboBoxMethodSearcher->currentData().toInt());
    searcher.leadType = (Lead)ui->comboBoxLeadSearcher->currentData().toInt();

    vector<u32> min = ui->ivFilterSearcher->getLower();
    vector<u32> max = ui->ivFilterSearcher->getUpper();

    ui->progressBar->setMaximum((max[0] - min[0] + 1) * (max[1] - min[1] + 1) * (max[2] - min[2] + 1) * (max[3] - min[3] + 1) * (max[4] - min[4] + 1) * (max[5] - min[5] + 1));

    for (u32 a = min[0]; a <= max[0]; a++)
    {
        for (u32 b = min[1]; b <= max[1]; b++)
        {
            for (u32 c = min[2]; c <= max[2]; c++)
            {
                for (u32 d = min[3]; d <= max[3]; d++)
                {
                    for (u32 e = min[4]; e <= max[4]; e++)
                    {
                        for (u32 f = min[5]; f <= max[5]; f++)
                        {
                            vector<Frame4> frames = searcher.search(a, b, c, d, e, f);

                            if (!frames.empty())
                                emit updateView(frames);

                            progress++;

                            if (cancel)
                            {
                                isSearching = false;
                                ui->search->setText(tr("Search"));
                                emit updateProgress();
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
    isSearching = false;
    ui->search->setText(tr("Search"));
    emit updateProgress();
}

void Stationary4::updateSearch()
{
    while (isSearching && !cancel)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        emit updateProgress();
    }
}

void Stationary4::on_search_clicked()
{
    if (isSearching)
    {
        cancel = true;
    }
    else
    {
        s->clear();
        s->setMethod((Method)ui->comboBoxMethodSearcher->currentData().toInt(NULL));

        ui->progressBar->setValue(0);
        progress = 0;

        isSearching = true;
        cancel = false;
        ui->search->setText(tr("Cancel"));

        std::thread job(&Stationary4::search, this);
        job.detach();

        std::thread update(&Stationary4::updateSearch, this);
        update.detach();
    }
}

void Stationary4::on_pushButtonProfileManager_clicked()
{
    ProfileManager4 *manager = new ProfileManager4();
    connect(manager, SIGNAL(updateProfiles()), this, SLOT(refreshProfiles()));
    manager->show();
}

void Stationary4::updateProgressBar()
{
    ui->progressBar->setValue(progress);
}

void Stationary4::updateViewSearcher(vector<Frame4> frames)
{
    s->addItems(frames);
}

void Stationary4::on_pushButtonLeadGenerator_clicked()
{
    ui->comboBoxLeadGenerator->clear();
    QString text = ui->pushButtonLeadGenerator->text();
    if (text == tr("Synchronize"))
    {
        ui->pushButtonLeadGenerator->setText(tr("Cute Charm"));
        ui->comboBoxLeadGenerator->setEnabled(true);

        ui->comboBoxLeadGenerator->addItem(tr("♂ Lead"), Lead::CuteCharmFemale);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (50% ♂ Target)"), Lead::CuteCharm50M);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (75% ♂ Target)"), Lead::CuteCharm75M);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (25% ♂ Target)"), Lead::CuteCharm25M);
        ui->comboBoxLeadGenerator->addItem(tr("♀ Lead (87.5% ♂ Target)"), Lead::CuteCharm875M);
    }
    else
    {
        ui->pushButtonLeadGenerator->setText(tr("Synchronize"));

        ui->comboBoxLeadGenerator->addItem("None");
        ui->comboBoxLeadGenerator->addItems(Nature::getNatures());
    }
}
