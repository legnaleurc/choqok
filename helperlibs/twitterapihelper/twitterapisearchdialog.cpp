/*
    This file is part of Choqok, the KDE micro-blogging client

    Copyright (C) 2008-2009 Mehrdad Momeny <mehrdad.momeny@gmail.com>

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

#include "twitterapisearchdialog.h"
#include "twitterapiaccount.h"
#include <kcombobox.h>
#include <klineedit.h>
#include <QGridLayout>
#include <QLabel>
#include <klocalizedstring.h>
#include "twitterapisearch.h"
#include "twitterapimicroblog.h"

class TwitterApiSearchDialog::Private
{
public:
    Private(TwitterApiAccount* theAccount)
        :account(theAccount)
    {
        mBlog = qobject_cast<TwitterApiMicroBlog*>(account->microblog());
    }
    KComboBox *searchTypes;
    KLineEdit *searchQuery;
    TwitterApiAccount* account;
    TwitterApiMicroBlog *mBlog;
};

TwitterApiSearchDialog::TwitterApiSearchDialog(TwitterApiAccount* theAccount, QWidget* parent)
: KDialog(parent), d(new Private(theAccount))
{
    setAttribute(Qt::WA_DeleteOnClose);
    QWidget *wd = new QWidget(this);
    setMainWidget(wd);
    createUi();
}

TwitterApiSearchDialog::~TwitterApiSearchDialog()
{
    delete d;
}

void TwitterApiSearchDialog::createUi()
{
    QVBoxLayout *layout = new QVBoxLayout(mainWidget());
    d->searchTypes = new KComboBox(mainWidget());
    fillSearchTypes();
    layout->addWidget(d->searchTypes);

    QHBoxLayout *queryLayout = new QHBoxLayout;
    layout->addLayout(queryLayout);
    QLabel *lblType = new QLabel(i18n("Query:"), mainWidget());
    lblType->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    queryLayout->addWidget(lblType);

    d->searchQuery = new KLineEdit(this);
    queryLayout->addWidget(d->searchQuery);

    setButtonText( Ok, i18n("Search") );
}

void TwitterApiSearchDialog::fillSearchTypes()
{
    QMap<int, QPair<QString, bool> > searchTypes = d->mBlog->searchBackend()->getSearchTypes();
    int count = searchTypes.count();
    for(int i = 0; i < count; ++i){
        d->searchTypes->insertItem(i, searchTypes[i].first);
    }
}

void TwitterApiSearchDialog::slotButtonClicked(int button)
{
    if(button == Ok) {
        d->mBlog->searchBackend()->requestSearchResults( d->account, d->searchQuery->text(),
                                                         d->searchTypes->currentIndex());
    } else
        KDialog::slotButtonClicked(button);
}
