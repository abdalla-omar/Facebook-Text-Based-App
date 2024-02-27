/********* main.c ********
    Student Name 	= Abdalla Omar
    Student Number	= 101261926
*/

// Includes go here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a3_nodes.h"
#include "a3_functions.h"

int main()
{
    /******** DONT MODIFY THIS PART OF THE CODE ********/
    /* THIS CODE WILL LOAD THE DATABASE OF USERS FROM THE FILE
       AND GENERATE THE STARTING LINKED LIST.
    */
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }
    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);
    /******** DONT MODIFY THIS PART OF THE CODE ********/

    /* IMPORTANT: You must use the users linked list created in the code above.
                  Any new users should be added to that linked list.
    */

    // Your solution goes here
    _Bool condition = true;         // Keepign the main menu alive
    _Bool condition2 = true;        // Keepign the sub menu of choice 2 alive
    _Bool condition3 = true;        // Keepign the sub menu of choice 3 alive
    _Bool subc2_terminate = true;   // to reset termination of sub choices 2 & 3 in option 3
    _Bool subc3_terminate = true;   // to reset termination of sub choice 4 in option 3
    _Bool manage_user_alive = true; // to keep user manage account alive
    unsigned short int main_option; // Main frame selection
    unsigned short int option;      // user ediditing sub menu
    unsigned short int option2;     // for sub menu choice is choice 2
    unsigned short int option3;     // for sub menu choice is choice 3
    char name[30];
    char pass[15];

    printf("\n**********************************************\n");
    printf("\t Welcome to Text-Based Facebook \n");
    printf("**********************************************\n\n");

    // infinite loop to keep regenerating the menu options
    while (condition)
    {
        manage_user_alive = true;

        main_menu();
        printf("Enter your choice: ");
        scanf("%d", &main_option);

        switch (main_option)
        {
        case 1:
            printf("\nEnter a username: ");
            scanf("%s", &name);

            if (find_user2(users, name) == NULL)
            {
                printf("Enter up to 15 characters password: ");
                scanf("%s", &pass);

                users = add_user(users, name, pass);
                printf("\n**** User Added! ****\n");
            }
            else
                printf("\nUser already exits, try a different username!\n");

            break;

        case 2:
            printf("Enter username: ");
            scanf("%s", &name);
            printf("Enter password: ");
            scanf("%s", &pass);

            // look for user
            user_t *check = find_user(users, name);

            if (check == NULL)
                printf("\nUser not found!\n");

            else if (check != NULL && strcmp(check->password, pass) != 0)
                printf("\nUser & password don't match!\n");
            else if (check != NULL && strcmp(check->password, pass) == 0)
            {
                printf("\n**********************************\n");
                printf("\t Welcome %s \t\n", check->username);
                printf("**********************************\n");

                // User frame
                while (manage_user_alive)
                {
                    condition2 = true;
                    condition3 = true;
                    subc2_terminate = true;
                    subc3_terminate = true;

                    print_menu();
                    printf("Enter your choice: ");
                    scanf("%d", &option);

                    // Insuring that user inputs appropriate choices
                    while (option > 5 || option < 1)
                    {
                        printf("Invalid choice. Please try again.\n");
                        print_menu();
                        printf("Enter your choice: ");
                        scanf("%d", &option);
                    }

                    //  All 5 options
                    switch (option)
                    {
                    case 1:

                        printf("\nEnter a new password that's up to 15 charecters: ");
                        scanf("%s", &pass);
                        strcpy(check->password, pass);
                        printf("\n**** Password changed! ****\n\n");
                        break;

                    case 2:
                        while (subc2_terminate)
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
                                printf("\t%s's posts\n", name);
                                printf("No posts available for %s", name);
                                printf("\n---------------------------------\n");
                            }
                            while (condition2)
                            {

                                printf("\n1. Add a new user post\n");
                                printf("2. Remove a users post\n");
                                printf("3. Return to main menu\n");
                                printf("\nYour choice: ");
                                scanf("%d", &option2);

                                switch (option2)
                                {
                                case 1:
                                    // to store new element
                                    char new_post[250];
                                    printf("Enter your post content: ");
                                    scanf(" %[^\n]s", new_post); // Don't use & operand bc we have an array
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
                                            printf("\t%s's posts\n", name);
                                            printf("No posts available for %s", name);
                                            printf("\n---------------------------------\n");
                                        }
                                    }

                                    else if (remove == false)
                                    {
                                        printf("Invalid post's number.");
                                        condition2 = false;
                                        subc2_terminate = false; // to break out of choice 2 loop
                                    }
                                    break;
                                case 3:
                                    condition2 = false;
                                    subc2_terminate = false; // to break out of choice 2 loop
                                    break;

                                default:
                                    printf("\nPlease enter a valid option between 1 to 3");
                                    scanf("%d", &option2);

                                    break;
                                }
                            }
                        }
                        break;

                    case 3:
                        while (subc3_terminate)
                        {
                            // Option menu
                            printf("\n1. Display all user's friends\n");
                            printf("2. Add a new friend\n");
                            printf("3. Delete a friend\n");
                            printf("4. Display a friend's posts\n");
                            printf("5. return to main menu\n");
                            printf("\nYour choice: ");
                            scanf("%d", &option3);

                            // sub menu edit friends option
                            switch (option3)
                            {
                            case 1:
                                // case 1: user has no friends
                                if (check->friends == NULL)
                                {
                                    printf("\nList of %s's friends:\n", name);
                                    printf("\nNo friends available for %s.\n", name);
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

                                printf("\nList of %s's friends:\n", name);
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
                                condition3 = false;
                                subc3_terminate = false; // to break out of choice 3 loop
                                break;

                            default:
                                printf("\nPlease enter a valid option between 1 to 5: ");
                                scanf("%d", &option3);
                                break;
                            }
                        }
                        break;
                    case 4:
                        display_all_posts(check);
                        break;

                    case 5:
                        manage_user_alive = false;
                        break;

                    default:
                        break;
                    }
                }
            }
            break;
        case 3:
            condition = false;
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