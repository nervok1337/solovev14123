import sqlite3
import csv

# Конфигурация: CSV-файл -> {table_name, schema (dict: column -> converter)}
CONFIG = {
    'authors.csv': {
        'table': 'Authors',
        'schema': {
            'id': int,
            'last_name': str,
            'first_name': str,
            'middle_name': str,
        }
    },
    'books.csv': {
        'table': 'Books',
        'schema': {
            'id': int,
            'title': str,
            'author_id': int,
            'category_id': int,
            'publication_year': int,
            'price':float,
        }
    },
    'categories.csv': {
        'table': 'Categories',
        'schema': {
            'id': int,
            'name': str,
        }
    },
    'customers.csv': {
        'table': 'Customers',
        'schema': {
            'id': int,
            'last_name': str,
            'first_name': str,
            'middle_name': str,
            'email': str,
            'phone': str,
        }
    },
    'employees.csv': {
        'table': 'Employees',
        'schema': {
            'id': int,
            'last_name': str,
            'first_name': str,
            'middle_name': str,
            'position_id': str,
        }
    },
    'orders.csv': {
        'table': 'Orders',
        'schema': {
            'id': int,
            'customer_id': int,
            'employee_id': int,
            'order_date': str,
        }
    },
    'order_items.csv': {
        'table': 'OrderItems',
        'schema': {
            'id': int,
            'order_id': int,
            'book_id': int,
            'amount': int,
            'unit_price': float,
        }
    },
    'positions.csv': {
        'table': 'Positions',
        'schema': {
            'id': int,
            'title': str,
        }
    },
    # Добавляй новые файлы и схемы здесь
}

def convert_row(row_dict, schema):
    converted = []
    for column, converter in schema.items():
        try:
            value = row_dict[column]
            converted.append(converter(value))
        except Exception as e:
            print(f"Ошибка преобразования в колонке '{column}': '{value}' — {e}")
            return None
    return tuple(converted)

def run_import():
    errors = []
    conn = sqlite3.connect('bookstore.db')
    cursor = conn.cursor()

    for csv_file, config in CONFIG.items():
        table = config['table']
        schema = config['schema']
        path = f"data/{csv_file}"

        print(f"Импорт из файла: {csv_file} → таблица: {table}")

        try:
            with open(path, newline='', encoding='utf-8') as f:
                reader = csv.DictReader(f)
                columns = ', '.join(schema.keys())
                placeholders = ', '.join(['?'] * len(schema))
                insert_query = f'INSERT OR REPLACE INTO {table} ({columns}) VALUES ({placeholders})'

                for row in reader:
                    converted = convert_row(row, schema)
                    if converted:
                        cursor.execute(insert_query, converted)
                    else:
                        errors.append(f"{csv_file}: некорректная строка {row}")
        except Exception as e:
            errors.append(f"{csv_file}: ошибка открытия или чтения — {e}")

    conn.commit()
    conn.close()

    return errors  # вернём список ошибок


# Опционально: запуск в одиночку
if __name__ == "__main__":
    result = run_import()
    if result:
        print("Импорт завершен с ошибками:")
        for err in result:
            print(err)
    else:
        print("Импорт завершен успешно.")
