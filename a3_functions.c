#include <stdlib.h>
#include "assert.h"
#include <stdio.h>
#include <string.h>
#include "a3_nodes.h"
#include "a3_functions.h"
#include <stdbool.h>

// Your solution goes here

/*
   Function that creates a new user and adds it to a sorted (ascending order) linked list at
   the proper sorted location. Return the head of the list.
*/
user_t *add_user(user_t *users, const char *username, const char *password)
{
   // using to iterate over linked list
   user_t *temp = malloc(sizeof(user_t));
   assert(temp != NULL);

   // creating new node
   user_t *new_node = malloc(sizeof(user_t));
   assert(new_node != NULL);
   strcpy(new_node->username, username);
   strcpy(new_node->password, password);
   new_node->posts = NULL;
   new_node->friends = NULL;
   new_node->next = NULL;

   // case 1: no users in the linked list
   if (users == NULL)
   {
      users = new_node;
      return users;
   }

   // case 2: adding at the begining of linked list
   else if (strcmp(users->username, username) > 0)
   {
      new_node->next = users;
      users = new_node;
      return users;
   }

   else
   {
      for (temp = users; temp->next != NULL; temp = temp->next)
      {
         // case 3: when adding in the middle of linked
         if (strcmp(temp->next->username, username) > 0)
         {
            new_node->next = temp->next;
            temp->next = new_node;
            return users;
         }
      }

      // case 4: When adding at the end of the linked list
      if (temp->next == NULL)
      {
         temp->next = new_node;
         return users;
      }
   }
}
/*
   Function that searches if the user is available in the database
   Return a pointer to the user if found and NULL if not found.
*/
user_t *find_user(user_t *users, const char *username)
{
   user_t *temp = malloc(sizeof(user_t));
   assert(temp != NULL);

   if (users == NULL)
      temp = NULL;

   else
      for (temp = users; temp != NULL; temp = temp->next)
      {

         // we store in new variable since we dont want to modify original expressio (temp->username)
         //  The username is a const meaning we cant chnage, this copy it elsewhere
         if (strcmp(temp->username, username) == 0)
         {
            return temp;
         }
      }
   temp = NULL;
   return temp;
}
/*
   Function that creates a new friend's node.
   Return the newly created node.
*/

friend_t *create_friend(const char *username)
{
   friend_t *new_friend = malloc(sizeof(friend_t));
   assert(new_friend != NULL);

   strcpy(new_friend->username, username);
   new_friend->next = NULL;

   return new_friend;
}

/*
   Function that links a friend to a user. The friend's name should be added into
   a sorted (ascending order) linked list.
*/
void add_friend(user_t *user, const char *friend, user_t *head)
{
   // checking if friend exists
   user_t *check = find_user(head, friend);

   if (check == NULL)
   {
      return;
   }

   friend_t *temp = malloc(sizeof(friend_t));
   assert(temp != NULL);

   // creating new friend
   friend_t *friend_to_insert = create_friend(friend);

   // Case 1: when we have 0 friends in the friends linked list
   if (user->friends == NULL)
   {
      user->friends = friend_to_insert;
   }

   // case 2: adding at the begining of linked list
   else if (strcmp(user->friends->username, friend) > 0)
   {
      friend_to_insert->next = user->friends;
      user->friends = friend_to_insert;
   }
   else
   {
      // Case 3: Adding at middle of linked list
      for (temp = user->friends; temp->next != NULL; temp = temp->next)
      {
         if (strcmp(temp->next->username, friend) > 0)
         {
            friend_to_insert->next = temp->next;
            temp->next = friend_to_insert;
            return;
         }
      }
      // case 4: Adding at the end of linked list
      if (temp->next == NULL)
      {
         temp->next = friend_to_insert;
      }
   }
   friend_to_insert->posts = &(check->posts);
   return;
}

/*
   Function that removes a friend from a user's friend list.
   Return true of the friend was deleted and false otherwise.
*/
_Bool delete_friend(user_t *user, char *friend_name)
{

   // to iterate over friends linked list
   friend_t *temp = malloc(sizeof(friend_t));
   assert(temp != NULL);

   // to free up the deleted user
   friend_t *free_node = malloc(sizeof(friend_t));
   assert(free_node != NULL);

   // case 1: user has no friends
   if (user->friends == NULL)
   {
      return false;
   }

   // case 2: user is at beginning of list
   if (strcmp(user->friends->username, friend_name) == 0)
   {
      temp = user->friends;
      user->friends = user->friends->next;
      temp->next = NULL;
      free(temp);
      return true;
   }

   // case 3: is user is not at 1st element, i.e. any other position
   for (temp = user->friends; temp->next != NULL; temp = temp->next)
   {
      if (strcmp(temp->next->username, friend_name) == 0)
      {
         free_node = temp->next;
         temp->next = temp->next->next;
         free_node->next = NULL;
         free(free_node);
         return true;
      }
   }

   // case 4: when friend isn't found
   return false;
}

/*
   Function that creates a new user's post.
   Return the newly created post.
*/
post_t *create_post(const char *text)
{
   post_t *new_post = malloc(sizeof(post_t));
   assert(new_post != NULL);

   strcpy(new_post->content, text);
   new_post->next = NULL;

   return new_post;
}

/*
   Function that adds a post to a user's timeline. New posts should be added following
   the stack convention (LIFO) (i.e., to the beginning of the Posts linked list).
*/
void add_post(user_t *user, const char *text)
{

   post_t *new_post = create_post(text);
   new_post->next = user->posts;
   user->posts = new_post;
}

/*
   Function that removes a post from a user's list of posts.
   Return true if the post was deleted and false otherwise.
*/
_Bool delete_post(user_t *user, int number)
{
   // case 1: if user has no friends
   if (user->posts == NULL)
   {
      return false;
   }

   // counter to make sure i'm matching the position of post to delete
   int i = 1;

   // to iterate over friends linked list
   post_t *temp = malloc(sizeof(post_t));
   assert(temp != NULL);

   // to free up the deleted user
   post_t *free_post = malloc(sizeof(post_t));
   assert(free_post != NULL);

   for (temp = user->posts; temp != NULL; i++, temp = temp->next)
   {
      // case 2: deleting first post
      if (number == 1)
      {
         // case 2.1: if there's only 1 post
         if (user->posts->next == NULL)
         {
            free(user->posts);
            user->posts = NULL;
         }
         // case 2.2: many posts
         else
         {
            user->posts = user->posts->next;
            free(temp);
            temp = NULL;
         }
         return true;
      }

      // case 3: removing at rest of list (position 2 and greater)
      else if (number == i + 1)
      {
         free_post = temp->next;
         temp->next = temp->next->next;
         free_post->next = NULL;
         free(free_post);
         return true;
      }
   }
   // case 4: post position isn't found
   return false;
}

/*
   Function that  displays a specific user's posts
*/
void display_user_posts(user_t *user)
{
   post_t *temp = user->posts;
   for (int count = 1; temp != NULL; temp = temp->next, count++)
   {
      printf("\n%d- %s: %s\n", count, user->username, temp->content);
   }
}

/*
   Function that displays a specific user's friends
*/
void display_user_friends(user_t *user)
{
   friend_t *temp = user->friends;
   for (int count = 1; temp != NULL; temp = temp->next, count++)
   {
      printf("\n%d- %s\n", count, temp->username);
   }
}
/*
   Function that displays all the posts of 2 users at a time from the database.
   After displaying 2 users' posts, it prompts if you want to display
   posts of the next 2 users.
   If there are no more post or the user types “n” or “N”, the function returns.
*/
void display_all_posts(user_t *users)
{
   unsigned int n = 1; // counter to iterate per 2 users posts
   char display;
   user_t *temp;

   for (temp = users; temp != NULL; temp = temp->next, n++)
   {
      display_user_posts(temp);
      if (n % 2 == 0)
      {

         printf("\n\nDo you want to display next 2 users posts? (Y/N): ");
         scanf(" %c", &display);

         if (display == 'Y' || display == 'y')
         {
            continue;
         }
         else if (display == 'N' || display == 'n')
         {
            return;
         }
      }
   }
}

/*
   Fucntion that free all users from the database before quitting the application.
*/
void teardown(user_t *users)
{
   user_t *temp;
   friend_t *friend_temp;
   post_t *post_temp;

   // looping over each user
   for (temp = users; users != NULL;)
   {
      // looping over each friend of each user
      for (friend_temp = users->friends; friend_temp != NULL;)
      {
         users->friends = users->friends->next;
         free(friend_temp);
         friend_temp = users->friends;
      }

      // looping over each post of each user
      for (post_temp = users->posts; post_temp != NULL;)
      {

         users->posts = users->posts->next;
         free(post_temp);
         post_temp = users->posts;
      }

      // freeing up current user
      users = users->next;
      free(temp);
      temp = users;

   }

   free(temp);
   free(friend_temp);
   free(post_temp);


}

/*
   Function that prints the main menu with a list of options for the user to choose from
*/
void print_menu()
{
   printf("\n1. Manage a user's profile (change password)\n");
   printf("2. Manage a user's posts (display/add/remove)\n");
   printf("3. Manage a user's friends (display/add/remove)\n");
   printf("4. Display All Posts\n");
   printf("5. Logout\n\n");
}



/*
   Function that reads users from the text file.
   IMPORTANT: This function shouldn't be modified and used as is
*/
user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
   user_t *users = NULL;
   char buffer[500];
   fgets(buffer, sizeof(buffer), file); // Read and discard the header line
   int count = 0;
   for (int i = 0; i < num_users; i++)
   {
      fgets(buffer, sizeof(buffer), file);
      buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

      char *token = strtok(buffer, ",");
      char *token2 = strtok(NULL, ",");
      users = add_user(users, token, token2);

      char *username = token;

      token = strtok(NULL, ",");

      user_t *current_user = users;
      for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
         ;

      while (token != NULL && strcmp(token, ",") != 0 && count < 3)
      {
         if (strcmp(token, " ") != 0)
         {
            add_friend(current_user, token, users);
         }
         token = strtok(NULL, ",");
         count++;
      }
      count = 0;

      // token = strtok(NULL, ",");
      while (token != NULL && strcmp(token, ",") != 0)
      {
         add_post(current_user, token);
         token = strtok(NULL, ",");
      }
   }
   return users;
}


// Extra functions' Prototypes/Declarations go here
/*
   Function that prints the main menu with a list of options for the user to choose from
*/
void main_menu()
{
   printf("\n**************************\n");
   printf("\t MAIN MENU\n");
   printf("**************************\n");
   printf("1. Register new user\n");
   printf("2. Log with existing user's information\n");
   printf("3. Exit\n\n");
}

// checks if a friend exists, returns true if they do, otherwise false
_Bool friend_exist(user_t *user, char *friend)
{

   friend_t *temp = malloc(sizeof(friend_t));
   assert(temp != NULL);

   for (temp = user->friends; temp != NULL; temp = temp->next)
   {
      // if the friend exists
      if (strcmp(temp->username, friend) == 0)
      {
         return true;
      }
   }
   // they don't exist
   return false;
}

// changes character from any case to lower case for comparision in find user
void lower(char *name)
{

   // length of name
   int len = strlen(name);

   // upper case ASCII Table from [65 - 90] inclusive, lower case [97 - 122] inclusive

   for (int i = 0; i < len; i++)
   {
      if (name[i] >= 'A' && name[i] <= 'Z')
         name[i] = name[i] + 'a' - 'A';
   }

   return;
}

/*
   used when looking to add a new user, ensuring no duplicates
   Function that searches if the user is available in the database
   Return a pointer to the user if found and NULL if not found.
   Checks to make sure that there aren't any duplicate users. 
   ex. if username = ginnylupin, NULL is returned
*/
user_t *find_user2(user_t *users, const char *username)
{
   char temp_name[30];
   char main_name[30];

   user_t *temp = malloc(sizeof(user_t));
   assert(temp != NULL);

   if (users == NULL)
      temp = NULL;

   else
      for (temp = users; temp != NULL; temp = temp->next)
      {

         // we store in new variable since we dont want to modify original expressio (temp->username)
         //  The username is a const meaning we cant chnage, this copy it elsewhere
         strcpy(temp_name, temp->username);
         strcpy(main_name, username);

         lower(temp_name);
         lower(main_name);

         if (strcmp(temp_name, main_name) == 0)
         {
            return temp;
         }
      }
   temp = NULL;
   return temp;
}
