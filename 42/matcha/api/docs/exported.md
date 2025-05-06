# 📘 API Documentation

> **Note:** All authenticated endpoints require a valid token via `Authorization: Bearer <token>`.

## `POST` /me/block
**Description**: Block a user by username
**Auth required**: Yes
**Tags**: User, Block

### Parameters
- `username` (String) **(required)** - The username of the user to block

### Responses
- `200`: User blocked
```json
{
  "message": "User blocked",
  "data": {
    "username": "janedoe"
  }
}
```
- `404`: User not found
```json
{
  "error": "User not found"
}
```
- `422`: Cannot block yourself
```json
{
  "error": "You cannot block yourself"
}
```

---
## `DELETE` /me/block
**Description**: Unblock a user by username
**Auth required**: Yes
**Tags**: User, Block

### Parameters
- `username` (String) **(required)** - The username of the user to unblock

### Responses
- `200`: User unblocked
```json
{
  "message": "User unblocked",
  "data": {
    "username": "janedoe"
  }
}
```
- `404`: User not found
```json
{
  "error": "User not found"
}
```

---
## `GET` /me/blocked
**Description**: List users you've blocked
**Auth required**: Yes
**Tags**: User, Block

### Responses
- `200`: Returns a list of blocked users
```json
{
  "data": [
    {
      "username": "janedoe"
    },
    {
      "username": "bobsmith"
    }
  ]
}
```

---
## `GET` /me/blocked_by
**Description**: List users who have blocked you
**Auth required**: Yes
**Tags**: User, Block

### Responses
- `200`: Returns a list of users who blocked you
```json
{
  "data": [
    {
      "username": "alicewonder"
    }
  ]
}
```

---
## `POST` /auth/register
**Description**: Register a new user
**Auth required**: No
**Tags**: Auth

### Parameters
- `username` (String) **(required)** - Unique username (max 20 characters)
- `email` (String) **(required)** - User email address used for login and verification
- `password` (String) **(required)** - User password (will be securely hashed)
- `first_name` (String) **(required)** - User's first name
- `last_name` (String) **(required)** - User's last name

### Responses
- `201`: User created
```json
{
  "message": "User created!"
}
```
- `422`: Validation error
```json
{
  "error": "Validation failed",
  "details": {
    "username": [
      "is required"
    ],
    "email": [
      "is invalid"
    ]
  }
}
```
- `422`: Username or email taken
```json
{
  "error": "Username or email already taken"
}
```

---
## `POST` /auth/login
**Description**: Authenticate an existing user using username and password
**Auth required**: No
**Tags**: Auth

### Parameters
- `username` (String) **(required)** - User's unique username
- `password` (String) **(required)** - User's account password

### Responses
- `200`: Login successful
```json
{
  "token": "jwt.token.here"
}
```
- `401`: Invalid credentials
```json
{
  "error": "Invalid credentials"
}
```
- `403`: Email not confirmed
```json
{
  "error": "Please confirm your email first."
}
```
- `403`: User banned
```json
{
  "error": "User is banned."
}
```

---
## `POST` /auth/social
**Description**: Authenticate or register a user via social login (OAuth provider)
**Auth required**: No
**Tags**: Auth

### Parameters
- `provider` (String) **(required)** - OAuth provider (e.g., 'google', 'github', 'intra')
- `provider_user_id` (String) **(required)** - Unique ID returned by the provider for this user
- `first_name` (String)  - User's first name (optional if new user)
- `last_name` (String)  - User's last name (optional if new user)

### Responses
- `200`: User authenticated
```json
{
  "token": "jwt.token.here"
}
```
- `201`: User created via social login
```json
{
  "token": "jwt.token.here"
}
```
- `422`: Missing required social login fields
```json
{
  "error": "Missing provider or UID"
}
```

---
## `POST` /email/reset-password
**Description**: Send password reset email
**Auth required**: No
**Tags**: User, Email

### Parameters
- `email` (String) **(required)** - Your account email

### Responses
- `200`: Email sent
```json
{
  "message": "Password reset email sent"
}
```

---
## `GET` /email/confirm/:code
**Description**: Validate confirmation code and confrim account
**Auth required**: No
**Tags**: User, Email

### Parameters
- `code` (String) **(required)** - Confirmation code

### Responses
- `200`: Account confirmed
```json
{
  "message": "Account confirmed"
}
```
- `404`: Invalid or expired code
```json
{
  "error": "Invalid or expired code"
}
```

---
## `POST` /email/reset-password/:code
**Description**: Reset your password using a valid reset code
**Auth required**: No
**Tags**: User, Email

### Parameters
- `code` (String) **(required)** - Reset password code received by email
- `new_password` (String) **(required)** - Your new password

### Responses
- `200`: Password reset successfully
```json
{
  "message": "Password reset successfully"
}
```
- `404`: Invalid or expired code
```json
{
  "error": "Invalid or expired code"
}
```
- `422`: Missing or invalid input
```json
{
  "error": "Invalid password"
}
```

---
## `GET` /me/pictures
**Description**: List all pictures uploaded by the current user
**Auth required**: Yes
**Tags**: User, Picture

### Responses
- `200`: Returns list of pictures
```json
{
  "data": [
    {
      "id": 1,
      "url": "https://cdn.example.com/pic1.jpg",
      "is_profile": false
    },
    {
      "id": 2,
      "url": "https://cdn.example.com/pic2.jpg",
      "is_profile": true
    }
  ]
}
```

---
## `POST` /me/pictures
**Description**: Upload a new picture
**Auth required**: Yes
**Tags**: User, Picture

### Parameters
- `url` (String) **(required)** - URL of the picture
- `is_profile` (TrueClass)  - Set as profile picture

### Responses
- `201`: Picture created
```json
{
  "message": "Picture uploaded!",
  "data": {
    "id": 3,
    "url": "https://cdn.example.com/pic3.jpg",
    "is_profile": false
  }
}
```
- `422`: Invalid data
```json
{
  "error": "Validation failed",
  "details": [
    "is not a valid URL"
  ]
}
```
- `422`: Too many pictures
```json
{
  "error": "Too many pictures",
  "details": [
    "<= 5"
  ]
}
```

---
## `PATCH` /me/pictures/:id
**Description**: Edit a picture (e.g., set as profile)
**Auth required**: Yes
**Tags**: User, Picture

### Parameters
- `id` (Integer) **(required)** - 
- `is_profile` (TrueClass)  - 
- `url` (String)  - 

### Responses
- `200`: Picture updated
```json
{
  "message": "Picture updated!",
  "data": {
    "id": 2,
    "url": "https://cdn.example.com/updated.jpg",
    "is_profile": true
  }
}
```
- `404`: Picture not found
```json
{
  "error": "Picture not found"
}
```
- `403`: Not your picture
```json
{
  "error": "Unauthorized"
}
```

---
## `DELETE` /me/pictures/:id
**Description**: Delete a picture
**Auth required**: Yes
**Tags**: User, Picture

### Parameters
- `id` (Integer) **(required)** - 

### Responses
- `200`: Picture deleted
```json
{
  "message": "Picture deleted"
}
```
- `404`: Not found
```json
{
  "error": "Picture not found"
}
```
- `403`: Unauthorized
```json
{
  "error": "Unauthorized"
}
```

---
## `GET` /users/:username/pictures
**Description**: Fetch the pictures of a user by their username
**Auth required**: Yes
**Tags**: User, PublicProfile, Picture

### Parameters
- `username` (String) **(required)** - The unique username of the user

### Responses
- `200`: Public user data
```json
{
  "data": [
    {
      "id": 217,
      "user_id": 2248,
      "url": "https://robohash.org/wallace.png?size=300x300&set=set1",
      "is_profile": "t",
      "created_at": "2025-04-15 07:49:41",
      "updated_at": "2025-04-15 07:49:41"
    }
  ]
}
```
- `404`: User not found or banned
```json
{
  "error": "User not found"
}
```
- `404`: User blocked you
```json
{
  "error": "User blocked you"
}
```
- `404`: User is blocked
```json
{
  "error": "User is blocked"
}
```

---
## `POST` /me/dates
**Description**: Schedule a date with a connected user
**Auth required**: Yes
**Tags**: User, Date

### Parameters
- `username` (String) **(required)** - The other user's username
- `scheduled_at` (String) **(required)** - Scheduled time for the date (ISO 8601)
- `location` (String) **(required)** - Optional location for the date
- `note` (String)  - Oprional note for the date

### Responses
- `201`: Date scheduled
```json
{
  "message": "Date scheduled",
  "data": {
    "id": "42",
    "connection_id": "99",
    "scheduled_at": "2025-04-12T20:00:00Z",
    "location": "Café Luna",
    "created_at": "2025-04-12T18:22:00Z"
  }
}
```
- `404`: User or connection not found
```json
{
  "error": "No connection found with this user"
}
```
- `422`: Validation error
```json
{
  "error": "Invalid date creation",
  "details": [
    "scheduled_at must be a valid datetime string"
  ]
}
```

---
## `GET` /me/dates
**Description**: List all scheduled dates for the current user
**Auth required**: Yes
**Tags**: User, Date

### Responses
- `200`: Array of scheduled dates
```json
{
  "data": [
    {
      "id": "42",
      "connection_id": "99",
      "scheduled_at": "2025-04-12T20:00:00Z",
      "location": "Café Luna",
      "created_at": "2025-04-12T18:22:00Z",
      "username": "bob",
      "first_name": "Bob",
      "last_name": "Smith"
    }
  ]
}
```

---
## `DELETE` /me/dates/:id
**Description**: Cancel or delete a date request
**Auth required**: Yes
**Tags**: Date

### Parameters
- `id` (Integer) **(required)** - ID of the date request

### Responses
- `200`: Date request deleted
```json
{
  "message": "Date deleted"
}
```
- `404`: Date not found
```json
{
  "error": "Date not found"
}
```
- `404`: Connection not found
```json
{
  "error": "Connection not found"
}
```
- `403`: Not authorized to delete this date
```json
{
  "error": "Unauthorized"
}
```

---
## `GET` /me/connections
**Description**: Get all users you are connected with
**Auth required**: Yes
**Tags**: User, Connection

### Responses
- `200`: List of connected users
```json
{
  "data": [
    {
      "username": "janedoe",
      "first_name": "Jane",
      "last_name": "Doe"
    },
    {
      "username": "bobsmith",
      "first_name": "Bob",
      "last_name": "Smith"
    }
  ]
}
```

---
## `POST` /me/connect
**Description**: Create a connection with a matched user
**Auth required**: Yes
**Tags**: User, Connection

### Parameters
- `username` (String) **(required)** - The username of the user to connect with

### Responses
- `200`: Connection created
```json
{
  "message": "Connected with janedoe",
  "data": {
    "id": 42,
    "user_a_id": 1,
    "user_b_id": 2,
    "created_at": "2025-04-12T10:00:00Z",
    "updated_at": "2025-04-12T10:00:00Z"
  }
}
```
- `403`: User is not matched with you
```json
{
  "error": "User is not matched with you"
}
```
- `404`: User not found
```json
{
  "error": "User not found"
}
```
- `422`: Invalid request
```json
{
  "error": "Validation failed",
  "details": [
    "username is required"
  ]
}
```

---
## `DELETE` /me/connect
**Description**: Remove an existing connection
**Auth required**: Yes
**Tags**: User, Connection

### Parameters
- `username` (String) **(required)** - The username of the user to disconnect from

### Responses
- `200`: Connection removed
```json
{
  "message": "Disconnected from janedoe"
}
```
- `403`: You and username are not connected
```json
{
  "error": "You and janedoe are not connected"
}
```
- `404`: User not found
```json
{
  "error": "User not found"
}
```

---
## `GET` /me/visits
**Description**: See who has viewed your profile
**Auth required**: Yes
**Tags**: User, ProfileView

### Responses
- `200`: List of users who viewed you
```json
{
  "data": [
    {
      "username": "janedoe",
      "first_name": "Jane",
      "last_name": "Doe",
      "viewed_at": "2025-04-12T10:00:00Z"
    },
    {
      "username": "bobsmith",
      "first_name": "Bob",
      "last_name": "Smith",
      "viewed_at": "2025-04-11T16:40:00Z"
    }
  ]
}
```

---
## `GET` /me/views
**Description**: See which users you have viewed
**Auth required**: Yes
**Tags**: User, ProfileView

### Responses
- `200`: List of profiles you viewed
```json
{
  "data": [
    {
      "username": "alicewonder",
      "first_name": "Alice",
      "last_name": "Wonder",
      "viewed_at": "2025-04-12T09:00:00Z"
    }
  ]
}
```

---
## `GET` /me/notifications
**Description**: List notifications for the current user
**Auth required**: Yes
**Tags**: User, Notifications

### Responses
- `200`: List of notifications
```json
{
  "data": [
    {
      "id": 1,
      "message": "You have a new like!",
      "read": false,
      "from_username": "bob",
      "created_at": "2025-04-13T10:00:00Z"
    }
  ]
}
```

---
## `PATCH` /me/notifications/:id
**Description**: Mark a notification as read
**Auth required**: Yes
**Tags**: User, Notifications

### Parameters
- `id` (Integer) **(required)** - Notification ID

### Responses
- `200`: Notification marked as read
```json
{
  "message": "Notification marked as read"
}
```
- `404`: Notification not found
```json
{
  "error": "Notification not found"
}
```

---
## `DELETE` /me/notifications/:id
**Description**: Delete a notification
**Auth required**: Yes
**Tags**: User, Notifications

### Parameters
- `id` (Integer) **(required)** - Notification ID

### Responses
- `200`: Notification deleted
```json
{
  "message": "Notification deleted"
}
```
- `404`: Notification not found
```json
{
  "error": "Notification not found"
}
```

---
## `GET` /me
**Description**: Get the currently authenticated user
**Auth required**: Yes
**Tags**: User

### Responses
- `200`: User object
```json
{
  "data": {
    "id": 1,
    "username": "johndoe",
    "first_name": "John",
    "last_name": "Doe",
    "email": "john@example.com",
    "gender": "male",
    "sexual_preferences": "everyone",
    "biography": "Just a regular person.",
    "latitude": 48.8566,
    "longitude": 2.3522
  }
}
```
- `403`: User not confirmed or banned
```json
{
  "error": "Access forbidden"
}
```

---
## `PATCH` /me
**Description**: Update profile fields for the current authenticated user
**Auth required**: Yes
**Tags**: User

### Parameters
- `username` (String)  - New username (must be unique)
- `first_name` (String)  - 
- `last_name` (String)  - 
- `gender` (String)  - One of: male, female, other
- `sexual_preferences` (String)  - One of: male, female, non_binary, everyone
- `biography` (String)  - 
- `birth_year` (Integer)  - 

### Responses
- `200`: Profile updated & user object
```json
{
  "message": "Profile updated!",
  "data": {
    "id": 1,
    "username": "newname",
    "biography": "Updated bio."
  }
}
```
- `422`: Validation failed
```json
{
  "error": "Validation failed",
  "details": {
    "username": [
      "has already been taken"
    ]
  }
}
```

---
## `GET` /users/:username
**Description**: Fetch the public profile of a user by their username
**Auth required**: Yes
**Tags**: User, PublicProfile

### Parameters
- `username` (String) **(required)** - The unique username of the user
- `count_view` (bool)  - Whether to record the view and send notification (default: true)

### Responses
- `200`: Public user data
```json
{
  "data": {
    "username": "janedoe",
    "first_name": "Jane",
    "last_name": "Doe",
    "biography": "Hi there!",
    "gender": "female",
    "sexual_preferences": "male",
    "birth_year": "2000",
    "profile_picture_id": 42,
    "online_status": true,
    "last_seen_at": "2025-04-11T14:53:00Z",
    "tags": [
      "yoga",
      "hiking",
      "coding"
    ],
    "pictures": [
      "https://yourdomain.com/uploads/pictures/12.jpg",
      "https://yourdomain.com/uploads/pictures/13.jpg"
    ],
    "views": 18,
    "visitors": [
      {
        "id": 2,
        "username": "johnsmith",
        "profile_picture_url": "https://yourdomain.com/uploads/pictures/99.jpg",
        "viewed_at": "2025-04-11T14:53:00Z"
      }
    ],
    "total_likes_sent": 7,
    "total_likes_received": 8
  }
}
```
- `404`: User not found or banned
```json
{
  "error": "User not found"
}
```
- `404`: User blocked you
```json
{
  "error": "User blocked you"
}
```
- `404`: User is blocked
```json
{
  "error": "User is blocked"
}
```

---
## `DELETE` /me
**Description**: Delete the current authenticated user account and all related data
**Auth required**: Yes
**Tags**: User

### Responses
- `204`: User deleted

---
## `POST` /me/discover
**Description**: Discover users based on preferences (location, age, fame, tags)
**Auth required**: Yes
**Tags**: User, Discover

### Parameters
- `location` (Hash)  - Latitude, longitude and max_distance_km
- `min_age` (Integer)  - Minimum age filter
- `max_distance_km` (Integer)  - Maximum distance in km
- `max_age` (Integer)  - Maximum age filter
- `min_fame` (Float)  - Minimum fame rating filter
- `tags` (Array)  - Filter by shared tags

### Responses
- `200`: Returns list of recommended users
```json
{
  "data": [
    {
      "user": {
        "id": "247",
        "username": "marquerite",
        "first_name": "Savannah",
        "last_name": "Anderson",
        "biography": "null",
        "gender": "female",
        "sexual_preferences": "everyone",
        "birth_year": "2003",
        "fame_rating": "3.75",
        "profile_picture_id": "null",
        "online_status": false,
        "last_seen_at": "null",
        "tags": [
          {
            "id": "33",
            "name": "yoga"
          },
          {
            "id": "37",
            "name": "dogs"
          },
          {
            "id": "40",
            "name": "dancing"
          }
        ],
        "pictures": [
          {
            "id": "28",
            "user_id": "247",
            "url": "https://robohash.org/marquerite.png?size=300x300&set=set1",
            "is_profile": "t",
            "created_at": "2025-04-15 19:03:26",
            "updated_at": "2025-04-15 19:03:26"
          }
        ],
        "views_count": 5,
        "likes_count": 4
      },
      "score": {
        "location_score": 100.0,
        "tag_score": 100.0,
        "fame_score": 3.75,
        "total": 67.92
      }
    }
  ]
}
```

---
## `POST` /me/messages
**Description**: Send a message to a user (requires existing connection)
**Auth required**: Yes
**Tags**: Message

### Parameters
- `username` (String) **(required)** - Recipient's username
- `content` (String) **(required)** - Message content

### Responses
- `200`: Message sent
```json
{
  "message": "Message sent",
  "data": {
    "id": 40,
    "connection_id": 42,
    "sender_id": 121,
    "content": "Hey there!",
    "created_at": "2025-04-12 18:07:25.031215"
  }
}
```
- `404`: User or connection not found
```json
{
  "error": "No connection found with janedoe"
}
```
- `422`: Validation error
```json
{
  "error": "Invalid request",
  "details": [
    "content must not be empty"
  ]
}
```

---
## `GET` /me/messages/:username
**Description**: Get all messages exchanged with a given user
**Auth required**: Yes
**Tags**: Message

### Parameters
- `username` (String) **(required)** - Other user's username

### Responses
- `200`: Message thread
```json
{
  "data": {
    "user": {
      "id": 127,
      "username": "bob",
      "email": "bob@example.com",
      "first_name": "Bob",
      "last_name": "B",
      "gender": "male",
      "sexual_preferences": "female",
      "biography": null,
      "is_email_verified": true,
      "is_banned": false,
      "fame_rating": 0,
      "latitude": null,
      "longitude": null,
      "online_status": false,
      "last_seen_at": null,
      "created_at": "2025-04-12 18:08:00.699235",
      "updated_at": "2025-04-12 18:08:00.702444",
      "profile_picture_id": null
    },
    "messages": [
      {
        "id": 41,
        "connection_id": 44,
        "sender_id": 127,
        "content": "Hi Bob!",
        "created_at": "2025-04-12 18:08:00.704577",
        "sender_username": "bob"
      }
    ]
  }
}
```
- `404`: User or connection not found
```json
{
  "error": "No connection found with bob"
}
```

---
## `GET` /me/messages
**Description**: Get all conversations grouped by user
**Auth required**: Yes
**Tags**: Message

### Responses
- `200`: List of message threads
```json
{
  "data": [
    {
      "user": {
        "id": 129,
        "username": "bob",
        "first_name": "Bob",
        "last_name": "B",
        "biography": null,
        "gender": "male",
        "sexual_preferences": "female",
        "profile_picture_id": null,
        "online_status": false,
        "last_seen_at": null
      },
      "messages": [
        {
          "id": 42,
          "connection_id": 45,
          "sender_id": 128,
          "content": "Hey again!",
          "created_at": "2025-04-12 18:08:09.476871",
          "sender_username": "alice"
        }
      ]
    }
  ]
}
```

---
## `GET` /tags
**Description**: List all tags
**Auth required**: Yes
**Tags**: Tag

### Responses
- `200`: Returns a list of available tags
```json
{
  "data": [
    {
      "id": 1,
      "name": "travel"
    },
    {
      "id": 2,
      "name": "music"
    }
  ]
}
```

---
## `POST` /tags
**Description**: Create a new tag
**Auth required**: Yes
**Tags**: Tag

### Parameters
- `name` (String) **(required)** - The name of the tag

### Responses
- `201`: Tag created
```json
{
  "message": "Tag created",
  "data": {
    "id": 3,
    "name": "photography"
  }
}
```
- `422`: Missing or invalid name
```json
{
  "error": "Validation failed",
  "details": {
    "name": [
      "is too short"
    ]
  }
}
```
- `422`: Tag name already taken
```json
{
  "error": "Tag name already taken"
}
```

---
## `GET` /me/tags
**Description**: List all tags for the current user
**Auth required**: Yes
**Tags**: User, Tag

### Responses
- `200`: Returns user’s tags
```json
{
  "data": [
    {
      "id": 1,
      "name": "travel"
    },
    {
      "id": 3,
      "name": "photography"
    }
  ]
}
```

---
## `POST` /me/tags
**Description**: Add a tag to the current user
**Auth required**: Yes
**Tags**: User, Tag

### Parameters
- `name` (String) **(required)** - The name of the tag to add, if tag doesn't exist it's created

### Responses
- `200`: Tag added to user
```json
{
  "message": "Tag added",
  "data": {
    "id": 4,
    "name": "sports"
  }
}
```
- `422`: Tag name missing or invalid
```json
{
  "error": "Missing tag name"
}
```

---
## `DELETE` /me/tags
**Description**: Remove a tag from the current user
**Auth required**: Yes
**Tags**: User, Tag

### Parameters
- `name` (String) **(required)** - The name of the tag to remove

### Responses
- `200`: Tag removed
```json
{
  "message": "Tag removed"
}
```
- `422`: Missing or invalid tag
```json
{
  "error": "Tag not found"
}
```

---
## `GET` /users/:username/tags
**Description**: Fetch the tags of a user by their username
**Auth required**: Yes
**Tags**: User, PublicProfile, Tag

### Parameters
- `username` (String) **(required)** - The unique username of the user

### Responses
- `200`: Public user data
```json
{
  "data": [
    {
      "id": 4,
      "name": "sports"
    },
    {
      "id": 5,
      "name": "cycling"
    }
  ]
}
```
- `404`: User not found or banned
```json
{
  "error": "User not found"
}
```
- `404`: User blocked you
```json
{
  "error": "User blocked you"
}
```
- `404`: User is blocked
```json
{
  "error": "User is blocked"
}
```

---
## `GET` /me/location
**Description**: Returns the last known latitude and longitude of the current user
**Auth required**: Yes
**Tags**: User, Location

### Responses
- `200`: Returns current location of the user
```json
{
  "data": {
    "latitude": 48.8566,
    "longitude": 2.3522,
    "city": "Paris",
    "country": "France",
    "recorded_at": "2025-04-12T08:30:00Z"
  }
}
```

---
## `POST` /me/location
**Description**: Record the current user's location (estimated from IP), or provided manually by the user
**Auth required**: Yes
**Tags**: User, Location

### Parameters
- `latitude` (Integer)  - 
- `longitude` (Integer)  - 
- `city` (String)  - 
- `country` (String)  - 

### Responses
- `200`: Location saved
```json
{
  "message": "Location recorded",
  "data": {
    "latitude": 48.8566,
    "longitude": 2.3522,
    "city": "Paris",
    "country": "France",
    "ip_address": "192.0.2.1",
    "recorded_at": "2025-04-12T08:30:00Z"
  }
}
```
- `422`: Geolocation service failed
```json
{
  "error": "Geolocation service failed"
}
```

---
## `GET` /me/location/history
**Description**: Get your full location history
**Auth required**: Yes
**Tags**: User, Location

### Responses
- `200`: List of location records
```json
{
  "data": [
    {
      "latitude": 48.8566,
      "longitude": 2.3522,
      "city": "Paris",
      "country": "France",
      "ip_address": "192.0.2.1",
      "recorded_at": "2025-04-12T08:30:00Z"
    },
    {
      "latitude": 52.52,
      "longitude": 13.405,
      "city": "Berlin",
      "country": "Germany",
      "ip_address": "192.0.2.2",
      "recorded_at": "2025-04-10T16:45:00Z"
    }
  ]
}
```

---
## `POST` /me/report
**Description**: Report a user for inappropriate behavior
**Auth required**: Yes
**Tags**: User, Report

### Parameters
- `username` (String) **(required)** - The username of the user to report
- `reason` (String) **(required)** - enum: [fake_account scam sexual_harassment]
- `description` (String)  - Optional description for the report

### Responses
- `200`: User reported successfully
```json
{
  "message": "User reported successfully",
  "data": {
    "username": "fakeuser"
  }
}
```
- `404`: User not found
```json
{
  "error": "User not found"
}
```
- `422`: Cannot report yourself
```json
{
  "error": "You cannot report yourself"
}
```

---
## `POST` /me/like
**Description**: Like another user
**Auth required**: Yes
**Tags**: User, Like

### Parameters
- `username` (String) **(required)** - The username of the user to like

### Responses
- `200`: User liked
```json
{
  "message": "You liked janedoe"
}
```
- `404`: User not found or unavailable
```json
{
  "error": "User not found"
}
```
- `422`: Invalid request
```json
{
  "error": "You cannot like yourself"
}
```

---
## `DELETE` /me/like
**Description**: Unlike a user
**Auth required**: Yes
**Tags**: User, Like

### Parameters
- `username` (String) **(required)** - The username of the user to unlike

### Responses
- `200`: User unliked
```json
{
  "message": "janedoe has been unliked"
}
```
- `404`: User not found
```json
{
  "error": "User not found"
}
```
- `422`: Like does not exist
```json
{
  "error": "You haven't liked this user yet"
}
```

---
## `GET` /me/likes
**Description**: Get list of users you have liked
**Auth required**: Yes
**Tags**: User, Like

### Responses
- `200`: Array of liked user objects
```json
{
  "data": [
    {
      "username": "janedoe",
      "first_name": "Jane",
      "last_name": "Doe"
    },
    {
      "username": "bobsmith",
      "first_name": "Bob",
      "last_name": "Smith"
    }
  ]
}
```

---
## `GET` /me/liked_by
**Description**: Get list of users who liked you
**Auth required**: Yes
**Tags**: User, Like

### Responses
- `200`: Array of users who liked you
```json
{
  "data": [
    {
      "username": "alicewonder",
      "first_name": "Alice",
      "last_name": "Wonder"
    }
  ]
}
```

---
## `GET` /me/matches
**Description**: Get list of users who liked you back (matches)
**Auth required**: Yes
**Tags**: User, Like

### Responses
- `200`: Array of matched user objects
```json
{
  "data": [
    {
      "username": "janedoe",
      "first_name": "Jane",
      "last_name": "Doe"
    }
  ]
}
```

---
