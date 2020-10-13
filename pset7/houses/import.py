# TODO
from cs50 import SQL
from csv import reader, DictReader
from sys import argv

# open the database
db = SQL("sqlite:///students.db")
# validate commend-line agrument
if len(argv) < 2:
    print("Usage: ./import.py ./characters.csv")
    exit(1)

# open csv file
# copy student info
with open(argv[1], newline="") as student_info:
    student_name = reader(student_info)
    for sn in student_name:
        if sn[0] == 'name':
            continue
        # split the name
        # 1 or 2 space
        fullname = sn[0].split()
        if len(fullname) < 3:  # only first, last name
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       fullname[0], None, fullname[1], sn[1], sn[2])
        else:  # first, middle, last name
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       fullname[0], fullname[1], fullname[2], sn[1], sn[2])