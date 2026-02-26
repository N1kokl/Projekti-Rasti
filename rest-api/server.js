'use strict';

require('dotenv').config();

const express = require('express');
const cors = require('cors');

const bookRoutes = require('./routes/books');
const carRoutes = require('./routes/cars');

const app = express();

app.use(cors());
app.use(express.json({ limit: '1mb' }));

app.get('/', (req, res) => {
  res.json({
    name: 'ATM CRUD REST API',
    status: 'ok',
    endpoints: {
      books: ['/api/books', '/api/books/:id'],
      cars: ['/api/cars', '/api/cars/:id']
    }
  });
});

app.use('/api/books', bookRoutes);
app.use('/api/cars', carRoutes);

// Basic 404
app.use((req, res) => {
  res.status(404).json({ error: 'Not Found' });
});

// Basic error handler
// eslint-disable-next-line no-unused-vars
app.use((err, req, res, next) => {
  console.error(err);
  res.status(500).json({ error: 'Internal Server Error' });
});

const port = Number(process.env.PORT || 3000);
app.listen(port, () => {
  console.log(`REST API running on http://localhost:${port}`);
});
