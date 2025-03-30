import sqlite3

def read_line(line, table_name):
    fields = line.strip().split()

    if table_name == "movement":
        return (int(fields[0]), fields[1], fields[2], int(fields[3]), int(fields[4]), fields[5], int(fields[6]))
    
    elif table_name == "product":
        return (int(fields[0]), fields[1], fields[2], fields[3], float(fields[4].replace(',', '.')), fields[5])
    
    elif table_name == "store":
        return (fields[0], fields[1], fields[2])
    
    return None

conn = sqlite3.connect("database.db")
cursor = conn.cursor()

cursor.execute("""
    CREATE TABLE IF NOT EXISTS movement (
        id_operation INTEGER PRIMARY KEY,
        date TEXT,
        store_id TEXT,
        product_id INTEGER,
        quantity INTEGER,
        operation_type TEXT,
        price_per_unit INTEGER
    )
""")

cursor.execute("""
    CREATE TABLE IF NOT EXISTS product (
        product_id INTEGER PRIMARY KEY,  -- Артикул
        department TEXT,           -- Отдел
        product_name TEXT,         -- Наименование товара
        unit TEXT,                 -- Единица измерения
        quantity_per_pack REAL,   -- Количество в упаковке
        supplier TEXT             -- Поставщик
    )
""")

# Создание таблицы "store"
cursor.execute("""
    CREATE TABLE IF NOT EXISTS store (
        store_id TEXT PRIMARY KEY,
        district TEXT,
        address TEXT
    )
""")

# Список файлов для обработки
files = ["movement.txt", "product.txt", "store.txt"]

# Чтение данных из файлов и вставка в соответствующие таблицы
for file_path in files:
    if 'movement' in file_path.lower():
        table_name = "movement"
    elif 'product' in file_path.lower():
        table_name = "product"
    elif 'store' in file_path.lower():
        table_name = "store"
    else:
        continue

    # Открытие файла и обработка строк
    with open(file_path, encoding='utf-8') as file:
        file.readline()  # Пропускаем заголовок
        
        for line in file:
            processed_data = read_line(line, table_name)
            
            if processed_data:
                if table_name == "movement":
                    cursor.execute("""
                        INSERT OR IGNORE INTO movement 
                        (id_operation, date, store_id, product_id, quantity, operation_type, price_per_unit)
                        VALUES (?, ?, ?, ?, ?, ?, ?)
                    """, processed_data)
                elif table_name == "product":
                    cursor.execute("""
                        INSERT OR IGNORE INTO product 
                        (product_id, department, product_name, unit, quantity_per_pack, supplier)
                        VALUES (?, ?, ?, ?, ?, ?)
                    """, processed_data)
                elif table_name == "store":
                    cursor.execute("""
                        INSERT OR IGNORE INTO store 
                        (store_id, district, address)
                        VALUES (?, ?, ?)
                    """, processed_data)

        conn.commit()


cursor.execute("""
SELECT 
    (SUM(CASE WHEN m.operation_type = 'Поступление' THEN m.quantity ELSE 0 END) - 
    SUM(CASE WHEN m.operation_type = 'Продажа' THEN m.quantity ELSE 0 END)) AS difference
FROM movement m
JOIN product p ON m.product_id = p.product_id  -- связываем по артикулу (product_id заменили на article)
JOIN store s ON m.store_id = s.store_id
WHERE p.product_name = 'Яйцо_диетическое'
  AND s.district = 'Заречный'
  AND m.date BETWEEN '01.06.2021' AND '10.06.2021';
""")

difference = cursor.fetchone()[0]

print(f"Разница между поступлениями и продажами яиц диетических: {difference}")

conn.close()
