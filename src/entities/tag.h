#ifndef TAG_H
#define TAG_H

#include <QString>
#include <QDateTime>

struct Tag
{
    int id = -1;
    QString name;
    QDateTime createdAt;
};

#endif //TAG_H
