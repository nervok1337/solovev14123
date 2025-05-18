import sqlite3

def execute_query(sql_path, params):
    try:
        with open(sql_path, 'r', encoding='utf-8') as file:
            query = file.read()

        conn = sqlite3.connect('bookstore.db')
        cursor = conn.cursor()
        cursor.execute(query, params)
        conn.commit()
        conn.close()
        return True, None
    except Exception as e:
        return False, str(e)
