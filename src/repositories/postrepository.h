#ifndef POSTREPOSITORY_H
#define POSTREPOSITORY_H

#include "../entities/post.h"

#include <QList>

class PostRepository
{
public:
    PostRepository() = default;

    // TODO: Implement these methods in postrepository.cpp
    bool save(Post &post);
    bool update(const Post &post);
    bool remove(int id);
    Post findById(int id) const;
    QList<Post> findAll() const;
};

#endif // POSTREPOSITORY_H
