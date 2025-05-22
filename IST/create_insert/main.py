from sqlite3 import connect

connection = connect("base_db")
cur = connection.cursor()

cur.execute("""
    CREATE TABLE IF NOT EXISTS `Employees` (
    `id` INTEGER PRIMARY KEY NOT NULL UNIQUE,
    `name` TEXT NOT NULL,
    `lastname` TEXT NOT NULL,
    `phone_number` TEXT NOT NULL,
    `post_id` INTEGER NOT NULL,
    FOREIGN KEY(`post_id`) REFERENCES `Employee_positions`(`id`)
);
""")

cur.execute("""
    CREATE TABLE IF NOT EXISTS `Employee_positions` (
    `id` INTEGER PRIMARY KEY NOT NULL UNIQUE,
    `name` TEXT NOT NULL
);
""")

cur.execute("""
    CREATE TABLE IF NOT EXISTS `Customers` (
    `id` INTEGER PRIMARY KEY NOT NULL UNIQUE,
    `organization` TEXT NOT NULL,
    `phone_number` TEXT NOT NULL
);
""")

cur.execute("""
    CREATE TABLE IF NOT EXISTS `Orders` (
    `id` INTEGER PRIMARY KEY NOT NULL UNIQUE,
    `customer_id` INTEGER NOT NULL,
    `employee_id` INTEGER NOT NULL,
    `amount` INTEGER NOT NULL,
    `date_of_completion` TEXT NOT NULL,
    `mark_of_completion` TEXT NOT NULL,
    FOREIGN KEY(`customer_id`) REFERENCES `Customers`(`id`),
    FOREIGN KEY(`employee_id`) REFERENCES `Employees`(`id`)
);
""")

positions = [
    (1, "Manager"),
    (2, "Developer"),
    (3, "Designer"),
    (4, "Analyst"),
    (5, "Director")
]
cur.executemany("INSERT OR IGNORE INTO `Employee_positions`(`id`, `name`) VALUES(?, ?)", positions)

employees_data = [
    (1, "Ivan", "Ivanov", "+7(912)345-67-89", 2),
    (2, "Petr", "Petrov", "+7(987)654-32-10", 1),
    (3, "Maria", "Sidorova", "+7(926)123-45-67", 3),
    (4, "Aleksey", "Alekseev", "+7(905)678-90-12", 2),
    (5, "Ekaterina", "Smirnova", "+7(999)888-77-66", 3),
    (6, "Dmitry", "Kuznetsov", "+7(916)555-44-33", 4),
    (7, "Anna", "Petrova", "+7(903)222-11-00", 1),
    (8, "Igor", "Volkov", "+7(925)777-66-55", 5),
    (9, "Olga", "Morozova", "+7(901)234-56-78", 4),
    (10, "Sergey", "Pleskunov", "+7(913)987-65-43", 2)
]

cur.executemany("""
    INSERT OR IGNORE INTO `Employees`(`id`, `name`, `lastname`, `phone_number`, `post_id`)
    VALUES(?, ?, ?, ?, ?)
""", employees_data)

сustomers_data = [
    (1, "TechnoProfi", "+7(915)123-45-67"),
    (2, "StartUp Logistics", "+7(926)987-65-43"),
    (3, "GlobalService", "+7(903)456-78-90"),
    (4, "Innovative Solutions", "+7(999)111-22-33"),
    (5, "EcoTrade", "+7(916)777-88-99")
]

cur.executemany("""
    INSERT OR IGNORE INTO `Customers`(`id`,`organization`,`phone_number`) VALUES(?,?,?)
""", сustomers_data)

orders_data = [
    (1, 1, 10, 90000, "20.11.2024", "completed"),
    (2, 1, 9, 85000, "22.11.2024", "not completed"),
    (3, 5, 6, 90000, "03.01.2025", "completed"),
    (4, 3, 2, 95000, "01.02.2025", "completed"),
    (5, 2, 7, 65000, "04.03.2025", "not completed"),
    (6, 2, 8, 110000, "15.02.2024", "completed"),
    (7, 4, 5, 84000, "25.07.2023", "completed"),
    (8, 4, 10, 115000, "19.06.2024", "completed"),
    (9, 5, 1, 86000, "18.03.2025", "not completed"),
    (10, 2, 4, 100000, "12.12.2024", "not completed")
]

cur.executemany("""
    INSERT OR IGNORE INTO `Orders`(`id`,`customer_id`,`employee_id`,`amount`,`date_of_completion`,`mark_of_completion`)
    VALUES(?,?,?,?,?,?)
""", orders_data)

connection.commit()

cur.execute("""
    SELECT e.name, e.lastname, j.name as Employee_positions
    FROM Employees e
    JOIN Employee_positions j ON e.post_id = j.id
""")

employees_with_job_titles = cur.fetchall()
print("Сотрудники ")
for employees in employees_with_job_titles:
    print(f"{employees[0]} {employees[1]} - {employees[2]}")


cur.execute("""
    SELECT e.name, e.lastname
    FROM Employees e
    JOIN Employee_positions j ON e.post_id = j.id
    WHERE j.name = 'Developer'
""")

developers = cur.fetchall()

print("\nРазработчик")
for developer in developers:
    print(f"{developer[0]} {developer[1]}")

cur.execute("""
    SELECT sum(amount)
    FROM Orders
""")


total = cur.fetchall()
print("\nОбщая сумма")
for t in total:
    print(t[0])

cur.execute("""
    SELECT max(amount)
    FROM Orders
""")

maxi = cur.fetchall()

print("\nМаксимальная денежная выплата")
for t in maxi:
    print(t[0])

cur.execute("""
    SELECT min(amount)
    FROM Orders
""")
mini = cur.fetchall()

print("\nМинимальная денежная выплата")
for t in mini:
    print(t[0])

cur.execute("""
    SELECT avg(amount)
    FROM Orders
""")

avg = cur.fetchall()

print("\nСредняя денежная выплата")
for t in avg:
    print(t[0])

cur.execute("""
    SELECT count(amount)
    FROM Orders
    WHERE customer_id = 2
""")

cnt = cur.fetchall()

print("\nКоличество заказов второй фирмы")
for t in cnt:
    print(t[0])


cur.execute("""
    SELECT o.amount, e.lastname
    FROM Orders o
    JOIN Employees e ON o.employee_id = e.id
    WHERE e.lastname = 'Pleskunov' OR e.lastname = 'Petrova'
""")

sum_by_name = cur.fetchall()

print("\nСумма, полученная двумя определенными сотрудниками")
total = 0
for s in sum_by_name:
    total += s[0]
print(total)


cur.execute("""
    SELECT e.lastname, o.amount, o.mark_of_completion
    FROM Orders o
    JOIN Employees e ON o.employee_id = e.id
    WHERE o.amount > 70000 AND o.mark_of_completion = 'not completed'
""")

result = cur.fetchall()

print("\nЛюди, которые не выполнили заказ, выплата за который больше 70000 рублей")
for r in result:
    print(f"{r[0]}, сумма {r[1]} - {r[2]}")

cur.execute("""
    SELECT c.organization, e.lastname, ep.name, o.amount
    FROM Orders o
    JOIN Employees e ON o.employee_id = e.id
    JOIN Customers c ON o.customer_id = c.id
    JOIN Employee_positions ep ON e.post_id = ep.id
    WHERE ep.name = 'Developer' AND o.amount > 80000
""")

result = cur.fetchall()

print("\nКомпании, которые работают с разработчиками и платят им больше 80000")
for r in result:
    print(f"Компания {r[0]}, работник {r[1]}, выплата {r[3]}")

cur.execute("""
    SELECT c.organization, sum(o.amount)
    FROM Orders o
    JOIN Customers c ON o.customer_id = c.id
    GROUP BY c.organization
""")

result = cur.fetchall()

print("\nОбщие выплаты компаний")
for r in result:
    print(f"Компания {r[0]} - {r[1]}")

cur.execute("""
    SELECT e.lastname, count(o.amount), o.mark_of_completion
    FROM Orders o
    JOIN Employees e ON o.employee_id = e.id
    WHERE o.mark_of_completion = 'completed'
    GROUP BY e.lastname
""")

result = cur.fetchall()

print("\nКоличество заказов, выполненых работниками")
for r in result:
    print(f"Фамилия {r[0]} - {r[1]}")

cur.execute("""
    SELECT c.organization, max(o.amount), o.mark_of_completion
    FROM Orders o
    JOIN Customers c ON o.customer_id = c.id
    WHERE o.mark_of_completion = 'completed'
    GROUP BY c.organization
""")


result = cur.fetchall()

print("\nМаксимальная выплата")
for r in result:
    print(f"Компания {r[0]} - {r[1]}")


connection.close()