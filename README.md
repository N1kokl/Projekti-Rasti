### SQL Used to Create the `car` Table (Required)

```sql
CREATE TABLE car (
  id INT NOT NULL AUTO_INCREMENT,
  branch VARCHAR(100) NOT NULL,
  model VARCHAR(100) NOT NULL,
  PRIMARY KEY (id)
) ENGINE=InnoDB;
```


