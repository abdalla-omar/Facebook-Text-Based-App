/*
 * Text-Based Facebook Application
 * A social network simulation with users, posts, and friendships
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "facebook_types.h"
#include "facebook_functions.h"

int main()
{
    /* Load the initial user database from CSV file */
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }
    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);

    /* Main application variables and logic */
    _Bool main_menu_active = true;
    _Bool posts_menu_active = true;
    _Bool friends_menu_active = true;
    _Bool posts_submenu_active = true;
    _Bool friends_submenu_active = true;
    _Bool user_session_active = true;
    unsigned short int main_choice;
    unsigned short int user_choice;
    unsigned short int posts_choice;
    unsigned short int friends_choice;
    char username[30];
    char password[15];

    printf("\n**********************************************\n");
    printf("\t Welcome to Text-Based Facebook \n");
    printf("**********************************************\n\n");

    // infinite loop to keep regenerating the menu options
    while (main_menu_active)
    {
        user_session_active = true;

        main_menu();
        printf("Enter your choice: ");
        scanf("%d", &main_choice);

        switch (main_choice)
        {
        case 1:
            printf("\nEnter a username: ");
            scanf("%s", username);

            if (find_user2(users, username) == NULL)
            {
                printf("Enter up to 15 characters password: ");
                scanf("%s", password);

                users = add_user(users, username, password);
                printf("\n**** User Added! ****\n");
            }
            else
                printf("\nUser already exits, try a different username!\n");

            break;

        case 2:
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            // look for user
            user_t *check = find_user(users, username);

            if (check == NULL)
                printf("\nUser not found!\n");

            else if (check != NULL && strcmp(check->password, password) != 0)
                printf("\nUser & password don't match!\n");
            else if (check != NULL && strcmp(check->password, password) == 0)
            {
                printf("\n**********************************\n");
                printf("\t Welcome %s \t\n", check->username);
                printf("**********************************\n");

                // User frame
                while (user_session_active)
                {
                    posts_menu_active = true;
                    friends_menu_active = true;
                    posts_submenu_active = true;
                    friends_submenu_active = true;

                    print_menu();
                    printf("Enter your choice: ");
                    scanf("%d", &user_choice);

                    // Insuring that user inputs appropriate choices
                    while (user_choice > 5 || user_choice < 1)
                    {
                        printf("Invalid choice. Please try again.\n");
                        print_menu();
                        printf("Enter your choice: ");
                        scanf("%d", &user_choice);
                    }

                    //  All 5 options
                    switch (user_choice)
                    {
                    case 1:

                        printf("\nEnter a new password that's up to 15 charecters: ");
                        scanf("%s", password);
                        strcpy(check->password, password);
                        printf("\n**** Password changed! ****\n\n");
                        break;

                    case 2:
                        while (posts_submenu_active)
                        {
                            // If user has posts
                            if (check->posts != NULL)
                            {
                                display_user_posts(check);
                            }

                            // if user has no posts
                            else if (check->posts == NULL)
                            {
                                printf("\n---------------------------------\n");
                                printf("\t%s's posts\n", username);
                                printf("No posts available for %s", username);
                                printf("\n---------------------------------\n");
                            }
                            while (posts_menu_active)
                            {

                                printf("\n1. Add a new user post\n");
                                printf("2. Remove a users post\n");
                                printf("3. Return to main menu\n");
                                printf("\nYour choice: ");
                                scanf("%d", &posts_choice);

                                switch (posts_choice)
                                {
                                case 1:
                                    // to store new element
                                    char new_post[250];
                                    printf("Enter your post content: ");
                                    scanf(" %[^\n]s", new_post);
                                    add_post(check, new_post);
                                    printf("Post added to your profile.");
                                    printf("\n-----------------------\n");
                                    display_user_posts(check);

                                    break;

                                case 2:
                                    // Post position looking to delete
                                    unsigned short int p;
                                    printf("\nWhich post did you want to delete? ");
                                    scanf("%d", &p);
                                    _Bool remove = delete_post(check, p);

                                    if (remove == true)
                                    {
                                        printf("Post %d was successfully deleted!", p);
                                        printf("\n------------------------------------\n");
                                        display_user_posts(check);

                                        // if there are no posts remaining
                                        if (check->posts == NULL)
                                        {
                                            printf("\n---------------------------------\n");
                                            printf("\t%s's posts\n", username);
                                            printf("No posts available for %s", username);
                                            printf("\n---------------------------------\n");
                                        }
                                    }

                                    else if (remove == false)
                                    {
                                        printf("Invalid post's number.");
                                        posts_menu_active = false;
                                        posts_submenu_active = false; // to break out of choice 2 loop
                                    }
                                    break;
                                case 3:
                                    posts_menu_active = false;
                                    posts_submenu_active = false; // to break out of choice 2 loop
                                    break;

                                default:
                                    printf("\nPlease enter a valid option between 1 to 3");
                                    scanf("%d", &posts_choice);

                                    break;
                                }
                            }
                        }
                        break;

                    case 3:
                        while (friends_submenu_active)
                        {
                            // Option menu
                            printf("\n1. Display all user's friends\n");
                            printf("2. Add a new friend\n");
                            printf("3. Delete a friend\n");
                            printf("4. Display a friend's posts\n");
                            printf("5. return to main menu\n");
                            printf("\nYour choice: ");
                            scanf("%d", &friends_choice);

                            // sub menu edit friends option
                            switch (friends_choice)
                            {
                            case 1:
                                // case 1: user has no friends
                                if (check->friends == NULL)
                                {
                                    printf("\nList of %s's friends:\n", username);
                                    printf("\nNo friends available for %s.\n", username);
                                }

                                // case 2: user has at least 1 friend
                                else if (check->friends != NULL)
                                {
                                    display_user_friends(check);
                                }
                                break;

                            case 2:
                                char new_friend[30];
                                printf("\n\nEnter a new friends's name: ");
                                scanf("%s", &new_friend);

                                user_t *check_friend = find_user(users, new_friend);
                                if (check_friend == NULL)
                                {
                                    // new friend must exist as a user before being added
                                    printf("\nFriend not found!\n");
                                    printf("\nUser must exist before being added as a friend!\n");
                                }
                                else
                                {
                                    add_friend(check, new_friend, users);
                                    printf("\nFriend added to the list.\n");
                                }

                                break;

                            case 3:

                                printf("\nList of %s's friends:\n", username);
                                display_user_friends(check);
                                char delete_friend_name[30];

                                printf("\nEnter a friend's name to delete: ");
                                scanf("%s", &delete_friend_name);

                                // trying to delete given friend
                                _Bool friend_status = delete_friend(check, delete_friend_name);

                                // case 1: user enters a valid friend
                                if (friend_status == true)
                                {
                                    display_user_friends(check);
                                    printf("\nFriend deleted!\n");
                                }

                                // case 2: user enters a NON-valid friend
                                else if (friend_status == false)
                                {
                                    printf("Invalid friend's name.\n");
                                }
                                break;
                            case 4:

                                if(check->friends == NULL){
                                    // if user has no friends, we send them baack to the manage friends tabs
                                    printf("\nUser %s has no friends, please add an existing user as a friend first!\n", check->username);
                                    break;
                                }

                                char friend_post[30];
                                display_user_friends(check);
                                printf("\nChoose one of your following friends: ");
                                scanf("%s", &friend_post);

                                while (friend_exist(check, friend_post) == false)
                                {
                                    printf("\nFriend not found, please choose one of your following friends: ");
                                    display_user_friends(check);
                                    printf("\nChoose one of your following friends: ");
                                    scanf("%s", &friend_post);
                                }

                                if (friend_exist(check, friend_post) == true)
                                {
                                    printf("\n------------------------\n");
                                    user_t *friend_position = find_user(users, friend_post);
                                    display_user_posts(friend_position);
                                }

                                break;
                            case 5:
                                friends_menu_active = false;
                                friends_submenu_active = false; // to break out of choice 3 loop
                                break;

                            default:
                                printf("\nPlease enter a valid option between 1 to 5: ");
                                scanf("%d", &friends_choice);
                                break;
                            }
                        }
                        break;
                    case 4:
                        display_all_posts(check);
                        break;

                    case 5:
                        user_session_active = false;
                        break;

                    default:
                        break;
                    }
                }
            }
            break;
        case 3:
            main_menu_active = false;
            teardown(users);
            printf("\n*******************************************");
            printf("\nThank you for using Text-Based Facebook");
            printf("\n\t\tGoodbye !");
            printf("\n*******************************************");
            break;

        default:
            break;
        }
    }
}
