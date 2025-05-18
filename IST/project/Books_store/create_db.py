import sqlite3

# Подключение к базе данных (создаст файл, если его нет)
conn = sqlite3.connect("bookstore.db")
cursor = conn.cursor()

# Включаем поддержку внешних ключей
cursor.execute("PRAGMA foreign_keys = ON;")

# Таблица авторов
cursor.execute("""
CREATE TABLE IF NOT EXISTS Authors (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    last_name TEXT NOT NULL,        -- Фамилия автора
    first_name TEXT NOT NULL,       -- Имя автора
    middle_name TEXT                -- Отчество автора
);
""")

# Таблица категорий
cursor.execute("""
CREATE TABLE IF NOT EXISTS Categories (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL -- Наименование категории
);
""")

# Таблица книг
cursor.execute("""
CREATE TABLE IF NOT EXISTS Books (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL, -- Название книги
    author_id INTEGER, -- Айди автора
    category_id INTEGER, -- Айди категории
    publication_year INTEGER, -- Год издания
    price REAL NOT NULL, -- Цена
    FOREIGN KEY (author_id) REFERENCES Authors(id),
    FOREIGN KEY (category_id) REFERENCES Categories(id)
);
""")

# Таблица клиентов
cursor.execute("""
CREATE TABLE IF NOT EXISTS Customers (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    last_name TEXT NOT NULL,        -- Фамилия клиента
    first_name TEXT NOT NULL,       -- Имя клиента
    middle_name TEXT,               -- Отчество клиента
    email TEXT, -- Электронная почта
    phone TEXT -- Номер телефона
);
""")

# Таблица должностей
cursor.execute("""
CREATE TABLE IF NOT EXISTS Positions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL -- Наименование должности
);
""")

# Таблица сотрудников
cursor.execute("""
CREATE TABLE IF NOT EXISTS Employees (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    last_name TEXT NOT NULL,        -- Фамилия сотрудника
    first_name TEXT NOT NULL,       -- Имя сотрудника
    middle_name TEXT,               -- Отчество сотрудника
    position_id INTEGER, -- Айди должности
    FOREIGN KEY (position_id) REFERENCES Positions(id)
);
""")

# Таблица заказов
cursor.execute("""
CREATE TABLE IF NOT EXISTS Orders (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    customer_id INTEGER, -- Айди клиента
    employee_id INTEGER, -- Айди сотрудника
    order_date TEXT NOT NULL, -- Дата заказа
    FOREIGN KEY (customer_id) REFERENCES Customers(id),
    FOREIGN KEY (employee_id) REFERENCES Employees(id)
);
""")

# Таблица позиций заказов
cursor.execute("""
CREATE TABLE IF NOT EXISTS OrderItems (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    order_id INTEGER, -- Айди заказа
    book_id INTEGER, -- Айди книги
    amount INTEGER NOT NULL, -- Кол-во книг в заказе
    unit_price REAL NOT NULL, -- Цена за 1 шт
    FOREIGN KEY (order_id) REFERENCES Orders(id),
    FOREIGN KEY (book_id) REFERENCES Books(id)
);
""")

conn.commit()
conn.close()

