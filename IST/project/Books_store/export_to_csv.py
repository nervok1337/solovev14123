import sqlite3
import os
import csv
import re

def to_snake_case(name):
    """Преобразует CamelCase в snake_case"""
    return re.sub(r'(?<!^)(?=[A-Z])', '_', name).lower()

def export_all_tables(db_path="bookstore.db", output_dir="data"):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    cursor.execute("SELECT name FROM sqlite_master WHERE type='table'")
    tables = cursor.fetchall()

    errors = []

    for (table_name,) in tables:
        try:
            cursor.execute(f"SELECT * FROM {table_name}")
            rows = cursor.fetchall()

            cursor.execute(f"PRAGMA table_info({table_name})")
            columns = [col[1] for col in cursor.fetchall()]

            # Преобразование названия таблицы в snake_case
            file_name = to_snake_case(table_name) + ".csv"
            file_path = os.path.join(output_dir, file_name)

            with open(file_path, mode="w", encoding="utf-8", newline="") as f:
                writer = csv.writer(f)
                writer.writerow(columns)
                writer.writerows(rows)
        except Exception as e:
            errors.append(f"Ошибка при экспорте {table_name}: {e}")

    conn.close()
    if errors:
        raise Exception("\n".join(errors))
