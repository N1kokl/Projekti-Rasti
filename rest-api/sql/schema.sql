-- Database and tables for the REST API assignment

CREATE DATABASE IF NOT EXISTS atm_assignment_db CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE atm_assignment_db;

DROP TABLE IF EXISTS book;
DROP TABLE IF EXISTS car;

-- Book table (CRUD required)
CREATE TABLE book (
  idbook INT NOT NULL AUTO_INCREMENT,
  name VARCHAR(200) NOT NULL,
  author VARCHAR(200) NOT NULL,
  isbn VARCHAR(50) NULL,
  PRIMARY KEY (idbook)
) ENGINE=InnoDB;

-- Car table (CRUD required)
CREATE TABLE car (
  id INT NOT NULL AUTO_INCREMENT,
  branch VARCHAR(100) NOT NULL,
  model VARCHAR(100) NOT NULL,
  PRIMARY KEY (id)
) ENGINE=InnoDB;

-- Optional seed data
INSERT INTO book (name, author, isbn) VALUES
  ('Clean Code', 'Robert C. Martin', '9780132350884'),
  ('Design Patterns', 'Erich Gamma et al.', '9780201633610');

INSERT INTO car (branch, model) VALUES
  ('Toyota', 'Corolla'),
  ('Volkswagen', 'Golf');
