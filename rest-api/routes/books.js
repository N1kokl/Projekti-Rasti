'use strict';

const express = require('express');
const { pool } = require('../db');

const router = express.Router();

function toInt(value) {
  const n = Number(value);
  return Number.isInteger(n) && n > 0 ? n : null;
}

// GET /api/books
router.get('/', async (req, res, next) => {
  try {
    const [rows] = await pool.execute('SELECT idbook, name, author, isbn FROM book ORDER BY idbook ASC');
    res.json(rows);
  } catch (err) {
    next(err);
  }
});

// GET /api/books/:id
router.get('/:id', async (req, res, next) => {
  try {
    const id = toInt(req.params.id);
    if (!id) {
      return res.status(400).json({ error: 'Invalid id' });
    }

    const [rows] = await pool.execute('SELECT idbook, name, author, isbn FROM book WHERE idbook = ?', [id]);
    if (rows.length === 0) {
      return res.status(404).json({ error: 'Book not found' });
    }

    res.json(rows[0]);
  } catch (err) {
    next(err);
  }
});

// POST /api/books
router.post('/', async (req, res, next) => {
  try {
    const { name, author, isbn } = req.body || {};

    if (typeof name !== 'string' || name.trim().length === 0) {
      return res.status(400).json({ error: 'name is required' });
    }
    if (typeof author !== 'string' || author.trim().length === 0) {
      return res.status(400).json({ error: 'author is required' });
    }

    const safeIsbn = typeof isbn === 'string' ? isbn.trim() : null;

    const [result] = await pool.execute(
      'INSERT INTO book (name, author, isbn) VALUES (?, ?, ?)',
      [name.trim(), author.trim(), safeIsbn]
    );

    const [rows] = await pool.execute('SELECT idbook, name, author, isbn FROM book WHERE idbook = ?', [result.insertId]);
    res.status(201).json(rows[0]);
  } catch (err) {
    next(err);
  }
});

// PUT /api/books/:id
router.put('/:id', async (req, res, next) => {
  try {
    const id = toInt(req.params.id);
    if (!id) {
      return res.status(400).json({ error: 'Invalid id' });
    }

    const { name, author, isbn } = req.body || {};

    if (typeof name !== 'string' || name.trim().length === 0) {
      return res.status(400).json({ error: 'name is required' });
    }
    if (typeof author !== 'string' || author.trim().length === 0) {
      return res.status(400).json({ error: 'author is required' });
    }

    const safeIsbn = typeof isbn === 'string' ? isbn.trim() : null;

    const [result] = await pool.execute(
      'UPDATE book SET name = ?, author = ?, isbn = ? WHERE idbook = ?',
      [name.trim(), author.trim(), safeIsbn, id]
    );

    if (result.affectedRows === 0) {
      return res.status(404).json({ error: 'Book not found' });
    }

    const [rows] = await pool.execute('SELECT idbook, name, author, isbn FROM book WHERE idbook = ?', [id]);
    res.json(rows[0]);
  } catch (err) {
    next(err);
  }
});

// DELETE /api/books/:id
router.delete('/:id', async (req, res, next) => {
  try {
    const id = toInt(req.params.id);
    if (!id) {
      return res.status(400).json({ error: 'Invalid id' });
    }

    const [result] = await pool.execute('DELETE FROM book WHERE idbook = ?', [id]);
    if (result.affectedRows === 0) {
      return res.status(404).json({ error: 'Book not found' });
    }

    res.status(204).send();
  } catch (err) {
    next(err);
  }
});

module.exports = router;
