# TODO
from cs50 import SQL
from sys import argv

# validate the commend-line input
if len(argv) < 2:
    print("Usage: ./roster.py house_name")
    exit(1)

# open the database we created in import.py
db = SQL("sqlite:///students.db")
students = db.execute("SELECT * FROM students WHERE house = (?) ORDER BY last", argv[1])

# output the results
for info in students:
    if info['middle'] == None:  # first last, born year
        print(f"{info['first']} {info['last']}, born {info['birth']}")
    else:  # first middle last, born year
        print(f"{info['first']} {info['middle']} {info['last']}, born {info['birth']}")