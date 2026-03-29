#include "postrepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// TODO: Implement each method using QSqlQuery
//       Use query.prepare() + query.bindValue() for safe SQL (no SQL injection)
//       Example pattern:
//
//       QSqlQuery query;
//       query.prepare("SELECT ... FROM posts WHERE id = :id");
//       query.bindValue(":id", id);
//       query.exec();

bool PostRepository::save(Post &post)
{
    // TODO: INSERT into posts table, then set post.id from query.lastInsertId()
    return false;
}

bool PostRepository::update(const Post &post)
{
    // TODO: UPDATE posts SET ... WHERE id = :id
    return false;
}

bool PostRepository::remove(int id)
{
    // TODO: DELETE FROM posts WHERE id = :id
    return false;
}

Post PostRepository::findById(int id) const
{
    // TODO: SELECT * FROM posts WHERE id = :id, fill a Post struct
    return Post{};
}

QList<Post> PostRepository::findAll() const
{
    // TODO: SELECT * FROM posts ORDER BY updated_at DESC
    return {};
}
