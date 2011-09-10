/*
    This file is part of Choqok, the KDE micro-blogging client

    Copyright (C) 2008-2011 Mehrdad Momeny <mehrdad.momeny@gmail.com>

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

#ifndef PLURKAPISEARCHTIMELINEWIDGET_H
#define PLURKAPISEARCHTIMELINEWIDGET_H

#include "timelinewidget.h"
#include "plurkapisearch.h"


class CHOQOK_HELPER_EXPORT PlurkApiSearchTimelineWidget : public Choqok::UI::TimelineWidget
{
    Q_OBJECT
public:
    PlurkApiSearchTimelineWidget(Choqok::Account* account, const QString& timelineName,
                                   const SearchInfo &info, QWidget* parent = 0);
    ~PlurkApiSearchTimelineWidget();
    virtual void addNewPosts(QList< Choqok::Post* >& postList);
    void removeAllPosts();
    SearchInfo &searchInfo() const;

protected Q_SLOTS:
    virtual void saveTimeline();
    virtual void loadTimeline();
    void slotUpdateSearchResults();

    void reloadList();
    void loadNextPage();
    void loadPreviousPage();
    void loadCustomPage(const QString&);

private:
    void addFooter();
    class Private;
    Private * const d;
};

#endif // PLURKAPISEARCHTIMELINEWIDGET_H