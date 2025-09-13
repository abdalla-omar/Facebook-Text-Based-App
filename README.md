# Text-Based Facebook Application

A command-line social network application written in C that simulates basic Facebook functionality with users, posts, and friendships.

## Features

- **User Management**
  - User registration with username and password
  - Secure login system
  - Password modification
  - Case-insensitive username validation to prevent duplicates

- **Social Networking**
  - Add and remove friends
  - View friends list (sorted alphabetically)
  - View friends' posts

- **Content Creation**
  - Create text posts (up to 250 characters)
  - Delete posts by position
  - View personal timeline (newest posts first)
  - Browse all users' posts with pagination

- **Data Persistence**
  - Load initial user data from CSV file
  - Maintain sorted user lists for efficient searching
  - Clean memory management with proper cleanup on exit

## Requirements

- C compiler (gcc recommended)
- Standard C libraries (stdio.h, stdlib.h, string.h, stdbool.h)
- Unix/Linux environment (for assert.h)

## Installation

1. Clone or download the repository:
```bash
git clone <repository-url>
cd Facebook-Text-Based-App
```

2. Compile the application:
```bash
gcc -o facebook main.c facebook_functions.c -Wall
```

Or with debugging symbols:
```bash
gcc -o facebook main.c facebook_functions.c -Wall -g
```

## Usage

1. Ensure you have a `user_details.csv` file in the same directory as the executable (see Data Format section)

2. Run the application:
```bash
./facebook
```

3. Follow the on-screen menu:
   - **Register**: Create a new user account
   - **Login**: Access existing account
   - **Exit**: Save and quit the application

4. Once logged in, you can:
   - Change your password
   - Manage posts (view, add, delete)
   - Manage friends (view, add, remove, view their posts)
   - Browse all posts from all users
   - Logout to return to main menu

## Data Format

The application expects a `user_details.csv` file with the following format:

```csv
Username,Password,Friend1,Friend2,Friend3,Post1,Post2,...
john_doe,pass123,jane_smith,bob_jones,,Hello world!,My first post
jane_smith,secure456,john_doe,,,Welcome to my profile
```

- First row should be headers
- Each subsequent row represents a user
- Friends are comma-separated (maximum 3 friends in initial data)
- Posts are comma-separated after friends
- Empty fields should be left blank

## Project Structure

```
Facebook-Text-Based-App/
├── main.c                  # Main application logic and menu system
├── facebook_functions.c    # Implementation of all functions
├── facebook_functions.h    # Function declarations
├── facebook_types.h        # Data structure definitions
├── user_details.csv       # Initial user database (required)
└── README.md             # This file
```

## Data Structures

The application uses three main linked list structures:

- **User**: Stores username, password, and pointers to friends and posts lists
- **Friend**: Stores friend's username and reference to their posts
- **Post**: Stores post content (up to 250 characters)

All lists maintain sorted order (users and friends alphabetically, posts by recency).

## Memory Management

The application implements proper memory management:
- All dynamically allocated memory is freed on exit
- No memory leaks in normal operation
- Efficient linked list operations without unnecessary allocations

## Limitations

- Maximum username length: 30 characters
- Maximum password length: 15 characters
- Maximum post length: 250 characters
- CSV file must exist with at least header row
- No data persistence beyond initial CSV load (changes are not saved to file)

## Future Enhancements

- Save user changes back to CSV file
- Add post timestamps
- Implement user search functionality
- Add profile information (bio, location, etc.)
- Implement post likes and comments
- Add group functionality
- Enhanced security with password hashing

## Author

This application was developed as a demonstration of C programming concepts including:
- Linked list manipulation
- File I/O operations
- Memory management
- Modular programming
- User interface design in console applications
