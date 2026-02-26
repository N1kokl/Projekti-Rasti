# Software Development Application Project – Makeup Assignment

This repository contains a **REST API** implemented with **Node.js** and **Express.js**, a **MySQL** database schema, and a **Qt (C++)** desktop client application. The implemented functionality matches the assignment requirements:

- A MySQL database and a REST API that provides **full CRUD** operations for the **book** table.
- A MySQL table named **car** with an integer primary key and text fields **branch** and **model**.
- A REST API that provides **full CRUD** operations for the **car** table.
- A Qt desktop application that can **view**, **add**, **update**, and **delete** rows in the **car** table.

## Repository Structure

- `rest-api/` – Express + MySQL REST API
- `qt-client/` – Qt Widgets client (C++)

## Database Setup (MySQL)

1. Open a MySQL client (MySQL Workbench, CLI, etc.).
2. Run the SQL script:

`rest-api/sql/schema.sql`

### SQL Used to Create the `car` Table (Required)

```sql
CREATE TABLE car (
  id INT NOT NULL AUTO_INCREMENT,
  branch VARCHAR(100) NOT NULL,
  model VARCHAR(100) NOT NULL,
  PRIMARY KEY (id)
) ENGINE=InnoDB;
```

## REST API Setup (Node.js + Express)

### Prerequisites
- Node.js 18+ (recommended)
- MySQL Server

### Install and Run

```bash
cd rest-api
cp .env.example .env
# edit .env with your MySQL credentials
npm install
npm start
```

The API will start at:
- `http://localhost:3000/`

### Endpoints

**Books**
- `GET /api/books`
- `GET /api/books/:id`
- `POST /api/books`
- `PUT /api/books/:id`
- `DELETE /api/books/:id`

**Cars**
- `GET /api/cars`
- `GET /api/cars/:id`
- `POST /api/cars`
- `PUT /api/cars/:id`
- `DELETE /api/cars/:id`

### Postman Testing

A ready Postman collection is included:
- `rest-api/postman/ATM_Assignment_API.postman_collection.json`

Import it into Postman and verify that all CRUD operations work for both tables.

## Qt Client Setup (C++ / Qt Widgets)

### Prerequisites
- Qt 6 (Widgets + Network)
- Qt Creator (recommended)

### Build and Run

Open the folder `qt-client/` in Qt Creator and build the project.

The client uses the following default API base URL:
- `http://localhost:3000/api`

If the REST API is running locally, the client can:
- fetch and display `car` table rows
- add a new row
- update a row by id
- delete a row by id

## GitHub Submission

Push this repository to GitHub as a **public** repository and submit the repository link to the instructors.

