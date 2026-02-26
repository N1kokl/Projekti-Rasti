'use strict';

const express = require('express');
const { pool } = require('../db');

const router = express.Router();

function toInt(value) {
  const n = Number(value);
  return Number.isInteger(n) && n > 0 ? n : null;
}

// GET /api/cars
router.get('/', async (req, res, next) => {
  try {
    const [rows] = await pool.execute('SELECT id, branch, model FROM car ORDER BY id ASC');
    res.json(rows);
  } catch (err) {
    next(err);
  }
});

// GET /api/cars/:id
router.get('/:id', async (req, res, next) => {
  try {
    const id = toInt(req.params.id);
    if (!id) {
      return res.status(400).json({ error: 'Invalid id' });
    }

    const [rows] = await pool.execute('SELECT id, branch, model FROM car WHERE id = ?', [id]);
    if (rows.length === 0) {
      return res.status(404).json({ error: 'Car not found' });
    }

    res.json(rows[0]);
  } catch (err) {
    next(err);
  }
});

// POST /api/cars
router.post('/', async (req, res, next) => {
  try {
    const { branch, model } = req.body || {};

    if (typeof branch !== 'string' || branch.trim().length === 0) {
      return res.status(400).json({ error: 'branch is required' });
    }
    if (typeof model !== 'string' || model.trim().length === 0) {
      return res.status(400).json({ error: 'model is required' });
    }

    const [result] = await pool.execute(
      'INSERT INTO car (branch, model) VALUES (?, ?)',
      [branch.trim(), model.trim()]
    );

    const [rows] = await pool.execute('SELECT id, branch, model FROM car WHERE id = ?', [result.insertId]);
    res.status(201).json(rows[0]);
  } catch (err) {
    next(err);
  }
});

// PUT /api/cars/:id
router.put('/:id', async (req, res, next) => {
  try {
    const id = toInt(req.params.id);
    if (!id) {
      return res.status(400).json({ error: 'Invalid id' });
    }

    const { branch, model } = req.body || {};

    if (typeof branch !== 'string' || branch.trim().length === 0) {
      return res.status(400).json({ error: 'branch is required' });
    }
    if (typeof model !== 'string' || model.trim().length === 0) {
      return res.status(400).json({ error: 'model is required' });
    }

    const [result] = await pool.execute(
      'UPDATE car SET branch = ?, model = ? WHERE id = ?',
      [branch.trim(), model.trim(), id]
    );

    if (result.affectedRows === 0) {
      return res.status(404).json({ error: 'Car not found' });
    }

    const [rows] = await pool.execute('SELECT id, branch, model FROM car WHERE id = ?', [id]);
    res.json(rows[0]);
  } catch (err) {
    next(err);
  }
});

// DELETE /api/cars/:id
router.delete('/:id', async (req, res, next) => {
  try {
    const id = toInt(req.params.id);
    if (!id) {
      return res.status(400).json({ error: 'Invalid id' });
    }

    const [result] = await pool.execute('DELETE FROM car WHERE id = ?', [id]);
    if (result.affectedRows === 0) {
      return res.status(404).json({ error: 'Car not found' });
    }

    res.status(204).send();
  } catch (err) {
    next(err);
  }
});

module.exports = router;
