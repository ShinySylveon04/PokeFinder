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

#include "ProfileManager3.hpp"
#include "ui_ProfileManager3.h"

ProfileManager3::ProfileManager3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileManager3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

ProfileManager3::~ProfileManager3()
{
    delete ui;
    delete model;
}

void ProfileManager3::changeEvent(QEvent *event)
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

void ProfileManager3::setupModels()
{
    model->setModel(Profile3::loadProfileList());
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ProfileManager3::on_pushButtonNew_clicked()
{
    ProfileManager3NewEdit *dialog = new ProfileManager3NewEdit();
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile3 profile = dialog->getNewProfile();
        profile.saveProfile();
        model->addItem(profile);
        emit updateProfiles();
    }
    delete dialog;
}

void ProfileManager3::on_pushButtonOk_clicked()
{
    this->close();
}

void ProfileManager3::on_pushButtonEdit_clicked()
{
    int r = ui->tableView->currentIndex().row();

    if (r == -1)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    ProfileManager3NewEdit *dialog = new ProfileManager3NewEdit(model->getProfile(r));
    if (dialog->exec() == QDialog::Accepted)
    {
        Profile3 profile = dialog->getNewProfile();
        profile.updateProfile(dialog->getOriginal());
        int r = ui->tableView->currentIndex().row();
        model->updateProfile(profile, r);
        emit updateProfiles();
    }
    delete dialog;
}

void ProfileManager3::on_pushButtonDelete_clicked()
{
    int r = ui->tableView->currentIndex().row();

    if (r == -1)
    {
        QMessageBox error;
        error.setText(tr("Please select a profile."));
        error.exec();
        return;
    }

    Profile3 profile = model->getProfile(r);
    profile.deleteProfile();

    model->removeProfile(r);

    emit updateProfiles();
}
