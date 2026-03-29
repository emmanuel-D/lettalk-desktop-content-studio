#ifndef POST_H
#define POST_H

#include <QString>
#include <QDateTime>
// TODO: You may need more Qt includes as you add fields

struct Post
{
    int id = -1;
    QDateTime createdAt;
    // TODO: Add the remaining fields based on the 'posts' table schema
    //       Hint: look at databasemanager.cpp → createTables()
};

#endif // POST_H
