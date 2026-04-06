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

 QMap<QString, QVariant> PostRepository::toMap(const Post &post, bool isUpdate) {
     QMap<QString, QVariant> map = {
         {":title", post.title},
         {":content", post.content},
         {":category", post.category},
         {":created_at", post.createdAt},
         {":updated_at", post.updated_at}
     };

     if (isUpdate) {
         map[":id"] = post.id;
     }
     return map;
 }

 Post PostRepository::mapToPost(const QSqlQuery &query) const {
     Post post;

     post.id = query.value("id").toInt();
     post.title = query.value("title").toString();
     post.content = query.value("content").toString();
     post.category = query.value("category").toString();
     post.createdAt = query.value("created_at").toDateTime();
     post.updated_at = query.value("updated_at").toDateTime();

     return post;
 }

 bool PostRepository::save(Post &post)
{
    // TODO: INSERT into posts table, then set post.id from query.lastInsertId()
    QSqlQuery query;
    query.prepare(
    "INSERT INTO posts(id, title, content, category, created_at, updated_at)"
        "VALUES (:id, :title, :content, :category, :created_at, :updated_at)"
    );

    auto values = toMap(post, false);
    for (auto it = values.begin(); it != values.end(); ++it) {
        query.bindValue(it.key(), it.value());
    }

    if (!query.exec()) {
        qWarning() << "Failed to insert a new post in the 'posts' table:" << query.lastError().text();
        return false;
    }

    post.id = query.lastInsertId().toInt();

    qInfo() << "A new post inserted successfully.";
    return true;
}

bool PostRepository::update(const Post &post)
{
    // TODO: UPDATE posts SET ... WHERE id = :id
     QSqlQuery query;
     query.prepare("UPDATE posts SET"
                    "title = :title, "
                    "content = :content, "
                    "category = :category, "
                    "created_at = :created_at, "
                    "updated_at = :updated_at "
                    "WHERE id = :id");

    auto values = toMap(post, true);
     for (auto it = values.begin(); it != values.end(); ++it) {
         query.bindValue(it.key(), it.value());
     }

     if (!query.exec()) {
         qWarning() << "Failed to update the post with the id: " << post.id
                    << " in the 'posts' table:" << query.lastError().text();
         return false;
     }

     qInfo() << "The post updated successfully.";
     return true;
}

bool PostRepository::remove(int id)
{
    // TODO: DELETE FROM posts WHERE id = :id
     QSqlQuery query;
     query.prepare("DELETE FROM posts WHERE id = :id");
     query.bindValue(":id", id);

     if (!query.exec()) {
         qWarning() << "Failed to delete the post with the id: " << id
                    << " in the 'posts' table:" << query.lastError().text();
         return false;
     }
     if (query.numRowsAffected() == 0) {
         qWarning() << "No post found with id:" << id;
         return false;
     }

     qInfo() << "The post deleted successfully.";
     return true;
}

Post PostRepository::findById(int id) const
{
    // TODO: SELECT * FROM posts WHERE id = :id, fill a Post struct
     QSqlQuery query;
     query.prepare("SELECT * FROM posts WHERE id = :id");
     query.bindValue(":id", id);

     if (!query.exec()) {
         qWarning() << "Failed to select the post with the id: " << id
                    << " in the 'posts' table:" << query.lastError().text();
         return Post{};
     }

     if (!query.next()) {
         qWarning() << "No post found with id:" << id;
         return Post{};
     }

     qInfo() << "Post with id:" << id << " found successfully";
     return mapToPost(query);
}

QList<Post> PostRepository::findAll() const
{
    // TODO: SELECT * FROM posts ORDER BY updated_at DESC
     QSqlQuery query;
     query.prepare("SELECT * FROM posts ORDER BY update_at DESC");

     if (!query.exec()) {
         qWarning() << "Failed to select all the posts in the 'posts' table:" << query.lastError().text();
         return {};
     }

     QList<Post> posts;
     while (query.next()) {
         posts.append(mapToPost(query));
     }

     qInfo() << "All post found successfully";
     return posts;
}
