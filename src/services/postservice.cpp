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

    if (post.content.isEmpty()) {
        qWarning() << "Content is empty";
        return false;
    }

    const QDateTime now = QDateTime::currentDateTimeUtc();
    post.createdAt = now;
    post.updatedAt = now;

    bool const result = m_repository.save(post);
    if (!result) {
        qWarning() << "Failed to save the post.";
    }
    return result;
}

bool PostService::updatePost(const Post &post)
{
    if (post.id <= 0) {
        qWarning() << "Id is invalid.";
        return false;
    }

    Post updatedPost = post;
    updatedPost.updatedAt = QDateTime::currentDateTimeUtc();

    bool const result = m_repository.update(updatedPost);
    if (!result) {
        qWarning() << "Failed to update the post.";
    }

    return result;
}

bool PostService::deletePost(int id)
{
    if (id <= 0) {
        qWarning() << "Id is invalid.";
        return false;
    }

    bool const result = m_repository.remove(id);
    if (!result) {
        qWarning() << "Failed to delete the post.";
    }

    return result;
}

Post PostService::getPost(int id) const
{
    if (id <= 0) {
        qWarning() << "Id is invalid.";
        return Post{};
    }

    qInfo() << "Post found successfully.";
    return m_repository.findById(id);
}

QList<Post> PostService::getAllPosts() const
{
    return m_repository.findAll();
}
