#include "postservice.h"

// TODO: Implement each method by calling the corresponding m_repository method
//       Example:
//
//       bool PostService::createPost(Post &post)
//       {
//           return m_repository.save(post);
//       }

bool PostService::createPost(Post &post)
{
    if (post.title.isEmpty()) {
        qWarning() << "Title is empty";
        return false;
    }

    const QDateTime now = QDateTime::currentDateTimeUtc();
    post.createdAt = now;
    post.createdAt = now;

    qInfo() << "A new post created successfully.";
    return m_repository.save(post);
}

bool PostService::updatePost(const Post &post)
{
    if (post.id <= 0) {
        qWarning() << "Id is invalid.";
        return false;
    }

    Post updatedPost = post;
    updatedPost.updatedAt = QDateTime::currentDateTimeUtc();

    qInfo() << "Post updated successfully.";
    return m_repository.update(updatedPost);
}

bool PostService::deletePost(int id)
{
    if (id <= 0) {
        qWarning() << "Id is invalid.";
        return false;
    }

    qInfo() << "Post deleted successfully.";
    return m_repository.remove(id);
}

Post PostService::getPost(int id) const
{
    if (id <= 0) {
        qWarning() << "Id is invalid.";
        return Post{};
    }

    qInfo() << "Post fond successfully.";
    return m_repository.findById(id);
}

QList<Post> PostService::getAllPosts() const
{
    return m_repository.findAll();
}
