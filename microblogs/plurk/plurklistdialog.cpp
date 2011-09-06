/*
    This file is part of Choqok, the KDE micro-blogging client

    Copyright (C) 2010-2011 Mehrdad Momeny <mehrdad.momeny@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.


    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, see http://www.gnu.org/licenses/

*/

#include "plurklistdialog.h"
#include <KDebug>
#include <QListWidget>
#include <kcombobox.h>
#include <QLabel>
#include <klineedit.h>
#include <KLocalizedString>
#include "plurkaccount.h"
#include <KMessageBox>
#include "plurkmicroblog.h"
#include <QLayout>

PlurkListDialog::PlurkListDialog(PlurkAccount* theAccount, QWidget* parent)
: KDialog(parent)
{
    if(theAccount){
        account = qobject_cast<PlurkAccount*>(theAccount);
        if(!account){
        kError()<<"PlurkListDialog: ERROR, the provided account is not a valid Plurk account";
        return;
        }
    } else {
        kError()<<"PlurkListDialog: ERROR, theAccount is NULL";
        return;
    }
    setWindowTitle(i18n("Add List"));
    blog = qobject_cast<PlurkMicroBlog*>(account->microblog());
    mainWidget = new QWidget(this);
    ui.setupUi(mainWidget);
    setMainWidget(mainWidget);
    connect(ui.username, SIGNAL(textChanged(QString)), SLOT(slotUsernameChanged(QString)));
    connect(ui.loadUserLists, SIGNAL(clicked(bool)), SLOT(loadUserLists()));
    QRegExp rx("([a-z0-9_]){1,20}(\\/)", Qt::CaseInsensitive);
    QValidator *val = new QRegExpValidator(rx, 0);
    ui.username->setValidator(val);
    ui.username->setFocus();
    setButtonText(Ok, i18n("Add"));
    setButtonGuiItem(Cancel, KStandardGuiItem::close());
    listWidget = new QListWidget(this);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(ui.label, 0, 0);
    layout->addWidget(ui.username, 0, 1);
    layout->addWidget(ui.loadUserLists, 0, 2);
    layout->addWidget(listWidget, 1, 0, 1, 3);
    layout->addWidget(ui.label_2, 2, 0);
    layout->addWidget(ui.listname, 2, 1, 2, 3);
    mainWidget->setLayout(layout);
}

PlurkListDialog::~PlurkListDialog()
{

}

void PlurkListDialog::slotButtonClicked(int button)
{
    if(button == KDialog::Ok){
        if(ui.listname->text().isEmpty() || ui.username->text().isEmpty()){
            KMessageBox::error(this, i18n("You should provide both list author username and list name."));
        } else {
            blog->addListTimeline(account, ui.username->text(), ui.listname->text());
        }
    } else
        KDialog::slotButtonClicked(button);
}

void PlurkListDialog::slotUsernameChanged(const QString& name)
{
    if(name.endsWith('/')){
        QString n = name;
        n.chop(1);
        ui.username->setText(n);
        ui.listname->setFocus();
    }
    listWidget->clear();
    ui.listname->clear();
}

void PlurkListDialog::loadUserLists()
{
    if(ui.username->text().isEmpty()){
        KMessageBox::error(choqokMainWindow, i18n("No User!"));
        return;
    }
    connect( blog, SIGNAL(userLists(Choqok::Account*,QString,QList<Plurk::List>)),
             SLOT(slotLoadUserlists(Choqok::Account*,QString,QList<Plurk::List>)) );
    blog->fetchUserLists(account, ui.username->text());
}

void PlurkListDialog::slotLoadUserlists(Choqok::Account* theAccount, QString username,
                                          QList<Plurk::List> list)
{
    if(theAccount == account && username == ui.username->text() && !list.isEmpty()){
        listWidget->clear();
        QList<Plurk::List>::const_iterator it = list.constBegin();
        QList<Plurk::List>::const_iterator endIt = list.constEnd();
        for(; it != endIt; ++it){
            QListWidgetItem *item = new QListWidgetItem(listWidget);
            QString iText;
            if(it->description.isEmpty())
                iText = it->fullname;
            else
                iText = QString("%1 [%2]").arg(it->fullname).arg(it->description);
            item->setText(iText);
            item->setData(32, it->slug);
            listWidget->addItem(item);
        }
        connect( listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                 SLOT(slotListItemChanged(QListWidgetItem*)) );
    }
}

void PlurkListDialog::slotListItemChanged(QListWidgetItem* item)
{
    ui.listname->setText(item->data(32).toString());
}

#include "plurklistdialog.moc"
