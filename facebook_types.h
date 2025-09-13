#ifndef FACEBOOK_TYPES_H
#define FACEBOOK_TYPES_H
/* Data structures for the Text-Based Facebook application */

/* User structure representing a person in the social network */
typedef struct user
{
    char username[30];
    char password[15];
    struct friend *friends;
    struct post *posts;
    struct user *next;
} user_t;

/* Friend structure representing a connection between users */
typedef struct friend
{
    char username[30];
    struct post **posts;
    struct friend *next;
}
friend_t;

/* Post structure representing user-generated content */
typedef struct post
{
    char content[250];
    struct post *next;
} post_t;

#endif /* FACEBOOK_TYPES_H */
