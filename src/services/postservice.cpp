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
    return false;
}

bool PostService::updatePost(const Post &post)
{
    return false;
}

bool PostService::deletePost(int id)
{
    return false;
}

Post PostService::getPost(int id) const
{
    return Post{};
}

QList<Post> PostService::getAllPosts() const
{
    return {};
}
