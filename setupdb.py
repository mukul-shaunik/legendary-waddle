import sqlite3  
  
con = sqlite3.connect("employee.db")  
print("Database opened successfully")  
  
con.execute("create table Temperature (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, temp REAL default 0.0)")  
  
print("Table created successfully")  
  
con.close()  