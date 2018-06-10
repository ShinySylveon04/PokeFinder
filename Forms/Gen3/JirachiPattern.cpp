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

#include "JirachiPattern.hpp"
#include "ui_JirachiPattern.h"

JirachiPattern::JirachiPattern(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JirachiPattern)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

JirachiPattern::~JirachiPattern()
{
    delete ui;
    delete model;
}

void JirachiPattern::changeEvent(QEvent *event)
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

void JirachiPattern::setupModels()
{
    ui->jirachiPatternSeed->setValues(0, 32, false);

    model->setHorizontalHeaderLabels(QStringList() << tr("Pattern"));
    ui->tableViewGenerator->setModel(model);
}

void JirachiPattern::on_pushButtonGenerate_clicked()
{
    model->removeRows(0, model->rowCount());
    data.clear();
    generate(ui->jirachiPatternSeed->text().toUInt(NULL, 16));
}

void JirachiPattern::generate(u32 seed)
{
    QStringList result = getPatterns(seed).toSet().toList();

    if (result.count() == 0)
    {
        model->appendRow(new QStandardItem(tr("Spread Impossible")));
    }
    else
    {
        for (auto str : result)
            model->appendRow(new QStandardItem(str));
    }
}

QStringList JirachiPattern::getPatterns(u32 seed)
{
    QStringList results;

    QString pattern = QString::number(seed >> 30);
    data.push_back(seed);

    LCRNG rng = XDRNGR(seed);

    // Populate backwards data
    for (u32 m = 0; m < 35; m++)
    {
        seed = rng.nextUInt();
        data.push_back(seed);
        pattern = QString::number(seed >> 30, 16) + "|" + pattern;
    }

    // Loop through 4 possible cases that would make a valid pattern
    for (int i = 0; i < 4; i++)
    {
        // Modify pattern with target, skip if invalid
        QString copy = getTarget(QString(pattern), i);
        if (copy.isEmpty())
            continue;

        int index = copy.indexOf(QChar(':')) + 1;

        // Menu advances can't stop on 0 so skip
        int target = QString(copy.at(index)).toInt();
        if (target == 0)
            continue;

        // From start, game advances frames until (prng >> 30) gives a 1, 2, and 3
        // (prng >> 30) being 0 just acts as a filler
        // Map remaining numbers from 1, 2, 3 without target for later use
        vector<u32> remain = { 1, 2, 3 };
        remain.erase(std::remove(remain.begin(), remain.end(), target), remain.end());

        u32 obtain[] = { 0, 0, 0, 0};
        obtain[target] = 1;

        // Determine if spread is possible
        // Need to work backwards to see if going forward with 1, 2, and 3 lands on our target
        bool valid = true;
        for (int x = ((copy.length() - index + 3) / 2); x < data.size(); x++)
        {
            int temp = data[x] >> 30;
            if (temp == 1 || temp == 2 || temp == 3)
            {
                if (temp == target)
                {
                    // Check if remaining numbers haven't occured yet
                    if (obtain[remain[0]] == 0 || obtain[remain[1]] == 0)
                    {
                        // Spread impossible
                        valid = false;
                        break;
                    }
                }
                else
                {
                    obtain[temp] = 1;
                }
            }

            // Check to see if pattern passes
            if (obtain[1] == 1 && obtain[2] == 1 && obtain[3] == 1)
                break;
        }

        // This part actually skips when a pattern is impossible
        if (!valid)
            continue;

        results.push_back(copy.replace("|", " | "));
    }

    return results;
}

QString JirachiPattern::getTarget(QString in, int index)
{
    // (prng >> 30 == 0) then advance 1
    // (prng >> 30 == 2) then advance 1, (next prng >> 25 > 41) then advance 1 more
    // (prng >> 30 == 1) or (prng >> 30 == 3) then advance 3

    // Return an empty string if math doesn't fit
    switch (index)
    {
        case 0:
            if ((data[1] >> 30) == 0)
                in = in.mid(0, in.length() - 13) + "T:" + in.mid(in.length() - 13);
            else
                return QString();
            break;
        case 1:
            if ((data[3] >> 30) == 1 || (data[3] >> 30) == 3)
                in = in.mid(0, in.length() - 17) + "T:" + in.mid(in.length() - 17);
            else
                return QString();
            break;
        case 2:
            if ((data[2] >> 30) == 2 && (data[1] >> 25) <= 41)
                in = in.mid(0, in.length() - 13) + "T:" + in.mid(in.length() - 13);
            else
                return QString();
            break;
        case 3:
        default:
            if ((data[2] >> 30) == 2 && (data[1] >> 25) > 41)
                in = in.mid(0, in.length() - 15) + "T:" + in.mid(in.length() - 15);
            else
                return QString();
            break;
    }
    return in;
}
