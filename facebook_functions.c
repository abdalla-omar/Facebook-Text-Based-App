#include <stdlib.h>
#include "assert.h"
#include <stdio.h>
#include <string.h>
#include "facebook_types.h"
#include "facebook_functions.h"
#include <stdbool.h>

/* Function implementations for the Text-Based Facebook application */

/*
   Function that creates a new user and adds it to a sorted (ascending order) linked list at
   the proper sorted location. Return the head of the list.
*/
user_t *add_user(user_t *users, const char *username, const char *password)
{
   user_t *temp;
   
   // Create new user node
   user_t *new_node = malloc(sizeof(user_t));
   assert(new_node != NULL);
   strcpy(new_node->username, username);
   strcpy(new_node->password, password);
   new_node->posts = NULL;
   new_node->friends = NULL;
   new_node->next = NULL;

   // Case 1: Empty list
   if (users == NULL)
   {
      return new_node;
   }

   // Case 2: Insert at beginning
   if (strcmp(users->username, username) > 0)
   {
      new_node->next = users;
      return new_node;
   }

   // Case 3: Insert in middle or at end
   for (temp = users; temp->next != NULL; temp = temp->next)
   {
      if (strcmp(temp->next->username, username) > 0)
      {
         new_node->next = temp->next;
         temp->next = new_node;
         return users;
      }
   }

   // Case 4: Insert at end
   temp->next = new_node;
   return users;
}
/*
   Function that searches if the user is available in the database
   Return a pointer to the user if found and NULL if not found.
*/
user_t *find_user(user_t *users, const char *username)
{
   user_t *temp;

   for (temp = users; temp != NULL; temp = temp->next)
   {
      if (strcmp(temp->username, username) == 0)
      {
         return temp;
      }
   }
   
   return NULL;
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
   // Check if friend exists in the user database
   user_t *check = find_user(head, friend);
   if (check == NULL)
   {
      return;
   }

   friend_t *temp;
   
   // Create new friend node
   friend_t *friend_to_insert = create_friend(friend);
   friend_to_insert->posts = &(check->posts);

   // Case 1: Empty friends list
   if (user->friends == NULL)
   {
      user->friends = friend_to_insert;
      return;
   }

   // Case 2: Insert at beginning
   if (strcmp(user->friends->username, friend) > 0)
   {
      friend_to_insert->next = user->friends;
      user->friends = friend_to_insert;
      return;
   }
   
   // Case 3: Insert in middle or at end
   for (temp = user->friends; temp->next != NULL; temp = temp->next)
   {
      if (strcmp(temp->next->username, friend) > 0)
      {
         friend_to_insert->next = temp->next;
         temp->next = friend_to_insert;
         return;
      }
   }
   
   // Case 4: Insert at end
   temp->next = friend_to_insert;
}

/*
   Function that removes a friend from a user's friend list.
   Return true of the friend was deleted and false otherwise.
*/
_Bool delete_friend(user_t *user, char *friend_name)
{
   friend_t *temp;
   friend_t *to_delete;

   // Case 1: Empty friends list
   if (user->friends == NULL)
   {
      return false;
   }

   // Case 2: Delete first friend
   if (strcmp(user->friends->username, friend_name) == 0)
   {
      to_delete = user->friends;
      user->friends = user->friends->next;
      free(to_delete);
      return true;
   }

   // Case 3: Delete friend in middle or at end
   for (temp = user->friends; temp->next != NULL; temp = temp->next)
   {
      if (strcmp(temp->next->username, friend_name) == 0)
      {
         to_delete = temp->next;
         temp->next = temp->next->next;
         free(to_delete);
         return true;
      }
   }

   // Case 4: Friend not found
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
   post_t *temp;
   post_t *to_delete;
   int i;

   // Case 1: Empty posts list
   if (user->posts == NULL)
   {
      return false;
   }

   // Case 2: Delete first post
   if (number == 1)
   {
      to_delete = user->posts;
      user->posts = user->posts->next;
      free(to_delete);
      return true;
   }

   // Case 3: Delete post at position 2 or greater
   i = 1;
   for (temp = user->posts; temp->next != NULL; temp = temp->next, i++)
   {
      if (number == i + 1)
      {
         to_delete = temp->next;
         temp->next = temp->next->next;
         free(to_delete);
         return true;
      }
   }
   
   // Case 4: Invalid position
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
   Function that frees all users from the database before quitting the application.
*/
void teardown(user_t *users)
{
   user_t *current_user;
   user_t *next_user;
   friend_t *current_friend;
   friend_t *next_friend;
   post_t *current_post;
   post_t *next_post;

   current_user = users;
   while (current_user != NULL)
   {
      // Free all friends of current user
      current_friend = current_user->friends;
      while (current_friend != NULL)
      {
         next_friend = current_friend->next;
         free(current_friend);
         current_friend = next_friend;
      }

      // Free all posts of current user
      current_post = current_user->posts;
      while (current_post != NULL)
      {
         next_post = current_post->next;
         free(current_post);
         current_post = next_post;
      }

      // Move to next user and free current user
      next_user = current_user->next;
      free(current_user);
      current_user = next_user;
   }
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

/* Function that checks if a friend exists in a user's friend list */
_Bool friend_exist(user_t *user, char *friend)
{
   friend_t *temp;

   for (temp = user->friends; temp != NULL; temp = temp->next)
   {
      if (strcmp(temp->username, friend) == 0)
      {
         return true;
      }
   }
   
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
   char search_name[30];
   user_t *temp;

   // Copy search username to avoid modifying the const parameter
   strcpy(search_name, username);
   lower(search_name);

   for (temp = users; temp != NULL; temp = temp->next)
   {
      // Copy current username for case-insensitive comparison
      strcpy(temp_name, temp->username);
      lower(temp_name);

      if (strcmp(temp_name, search_name) == 0)
      {
         return temp;
      }
   }
   
   return NULL;
}
