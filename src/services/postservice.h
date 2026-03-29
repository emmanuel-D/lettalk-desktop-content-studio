#ifndef POSTSERVICE_H
#define POSTSERVICE_H

#include "../entities/post.h"
#include "../repositories/postrepository.h"

#include <QList>

class PostService
{
public:
    PostService() = default;

    bool createPost(Post &post);
    bool updatePost(const Post &post);
    bool deletePost(int id);
    Post getPost(int id) const;
    QList<Post> getAllPosts() const;

private:
    PostRepository m_repository;
};

#endif // POSTSERVICE_H
