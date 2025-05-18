from imports import *

class TabApp(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.title("Информационная система книжный магазин")
        self.geometry("1080x720")
        ctk.set_appearance_mode("dark")
        ctk.set_default_color_theme("blue")

        self.main_frame = ctk.CTkFrame(self)
        self.main_frame.pack(fill="both", expand=True)

        self.left_panel = ContextMenu(self.main_frame, width=240, app=self)
        self.left_panel.pack(side="left", fill="y")

        self.right_panel = TabArea(self.main_frame)
        self.right_panel.pack(side="right", fill="both", expand=True)

        self.create_tabs()

    def create_tabs(self):
        for name in ["Управление", "Запросы"]:
            self.right_panel.add_tab(name, f"{name}")
            self.left_panel.add_button(name)

        self.left_panel.add_button("Просмотр", command=self.view_tables)
        self.left_panel.add_button("Импорт данных", command=self.import_data)
        self.left_panel.add_button("Экспорт данных", command=self.export_data)
        self.left_panel.add_button("Выйти", command=self.quit)

    def import_data(self):
        errors = load_data.run_import()
        if errors:
            messagebox.showerror("Импорт завершен с ошибками", "\n".join(errors))
        else:
            messagebox.showinfo("Импорт завершен", "Все данные успешно импортированы.")
    def export_data(self):
        try:
            export_all_tables()
            messagebox.showinfo("Экспорт выполнен", "Все таблицы экспортированы в папку 'data'.")
        except Exception as e:
            messagebox.showerror("Ошибка экспорта", str(e))

    def view_tables(self):
        self.right_panel.show_table_view_tab()


class ContextMenu(ctk.CTkFrame):
    def __init__(self, master, width, app):
        super().__init__(master, width=width)
        self.app = app
        self.buttons = []

        title = ctk.CTkLabel(self, text="Навигация", font=ctk.CTkFont(size=20, weight="bold"))
        title.pack(pady=(20, 10), padx=10)

    def add_button(self, tab_name, command=None):
        if command is None:
            command = lambda: self.app.right_panel.show_tab(tab_name)

        button = ctk.CTkButton(self, text=tab_name, command=command)
        button.pack(pady=10, padx=10, fill="x")
        self.buttons.append(button)


class TabArea(ctk.CTkFrame):
    def __init__(self, master):
        super().__init__(master)
        self.tabs = {}
        self.overlay_frame = None

    def add_tab(self, name, content):
        frame = ctk.CTkFrame(self)

        title = ctk.CTkLabel(frame, text=content, font=ctk.CTkFont(size=20, weight="bold"))
        title.pack(pady=10)

        if name == "Управление":
            self._build_management_tab(frame)
        elif name == "Запросы":
            self._build_queries_tab(frame)

        self.tabs[name] = frame

        if len(self.tabs) == 1:
            self.show_tab(name)

    def _build_management_tab(self, frame):
        ctk.CTkButton(frame, text="Добавить книгу", command=self.show_add_book_overlay).pack(pady=10)
        ctk.CTkButton(frame, text="Добавить автора", command=self.show_add_author_overlay).pack(pady=10)
        ctk.CTkButton(frame, text="Добавить клиента", command=self.show_add_customer_overlay).pack(pady=10)
        ctk.CTkButton(frame, text="Добавить сотрудника", command=self.show_add_employee_overlay).pack(pady=10)
        ctk.CTkButton(frame, text="Добавить заказ", command=self.show_add_order_overlay).pack(pady=10)

    def show_tab(self, name):
        if self.overlay_frame:
            self.overlay_frame.destroy()
            self.overlay_frame = None

        for frame in self.tabs.values():
            frame.pack_forget()

        self.tabs[name].pack(fill="both", expand=True)
    def show_add_book_overlay(self):
        self._create_overlay("Добавление книги", self._build_add_book_form)
    def show_add_author_overlay(self):
        self._create_overlay("Добавление автора", self._build_add_author_form)
    def show_add_customer_overlay(self):
        self._create_overlay("Добавление клиента", self._build_add_customer_form)
    def show_add_employee_overlay(self):
        self._create_overlay("Добавление сотрудника", self._build_add_employee_form)
    def show_add_order_overlay(self):
        self._create_overlay("Добавление заказа", self._build_add_order_form)
    def show_table_view_tab(self):
        if "Просмотр" not in self.tabs:
            self.add_tab("Просмотр", "Просмотр")
        self.show_tab("Просмотр")
        self._build_table_view(self.tabs["Просмотр"])

    def _create_overlay(self, title_text, build_form_func):
        if self.overlay_frame:
            self.overlay_frame.destroy()

        self.overlay_frame = ctk.CTkFrame(self)
        self.overlay_frame.pack(fill="both", expand=True)

        title = ctk.CTkLabel(self.overlay_frame, text=title_text, font=ctk.CTkFont(size=20, weight="bold"))
        title.pack(pady=10)

        build_form_func(self.overlay_frame)
    def _open_overlay(self, builder_function):
        if hasattr(self, "_active_overlay") and self._active_overlay and self._active_overlay.winfo_exists():
            self._active_overlay.destroy()

        self._active_overlay = builder_function()
    def close_active_overlay(self):
        if hasattr(self, "active_overlay") and self.active_overlay.winfo_exists():
            self.active_overlay.destroy()
            del self.active_overlay

    def show_revenue_overlay(self, total_revenue: float = 0.0):
        self.close_active_overlay()  # Закрываем, если уже есть активное всплывающее окно

        self.active_overlay = ctk.CTkFrame(self, height=100, corner_radius=10, fg_color="#1f1f1f")
        self.active_overlay.place(relx=0.5, rely=1.0, anchor="s", relwidth=0.9)

        revenue_label = ctk.CTkLabel(
            self.active_overlay,
            text=f"Общая выручка: {total_revenue:,.2f} руб.",
            font=("Arial", 18, "bold")
        )
        revenue_label.pack(pady=(10, 0))

        close_button = ctk.CTkButton(
            self.active_overlay,
            text="Закрыть",
            command=self.close_active_overlay
        )
        close_button.pack(pady=(5, 10))
    def show_top_books_overlay(self, books: list[tuple]):
        self.close_active_overlay()  # Закрыть предыдущее активное окно, если есть

        self.active_overlay = ctk.CTkFrame(self, height=200, corner_radius=10, fg_color="#1f1f1f")
        self.active_overlay.place(relx=0.5, rely=1.0, anchor="s", relwidth=0.9)

        ctk.CTkLabel(
            self.active_overlay,
            text="Топ-5 продаваемых книг:",
            font=("Arial", 16, "bold")
        ).pack(pady=(10, 5))

        for i, (title, total_sold) in enumerate(books, 1):
            formatted_title = title.replace("_", " ")
            ctk.CTkLabel(
                self.active_overlay,
                text=f"{i}. {formatted_title} — {total_sold} шт.",
                font=("Arial", 14)
            ).pack()

        ctk.CTkButton(
            self.active_overlay,
            text="Закрыть",
            command=self.close_active_overlay
        ).pack(pady=10)
    def show_top_customers_overlay(self, customers: list[tuple]):
        self.close_active_overlay()  # Закрыть предыдущее окно, если есть

        self.active_overlay = ctk.CTkFrame(self, height=200, corner_radius=10, fg_color="#1f1f1f")
        self.active_overlay.place(relx=0.5, rely=1.0, anchor="s", relwidth=0.9)

        ctk.CTkLabel(
            self.active_overlay,
            text="Топ-5 клиентов по сумме заказов:",
            font=("Arial", 16, "bold")
        ).pack(pady=(10, 5))

        for i, (name, total_spent) in enumerate(customers, 1):
            formatted_name = name.replace("_", " ")
            ctk.CTkLabel(
                self.active_overlay,
                text=f"{i}. {formatted_name} — {total_spent:.2f} руб.",
                font=("Arial", 14)
            ).pack()

        ctk.CTkButton(
            self.active_overlay,
            text="Закрыть",
            command=self.close_active_overlay
        ).pack(pady=10)
    def show_employee_efficiency_overlay(self, employees: list[tuple]):
        self.close_active_overlay()  # Закрыть предыдущее окно, если есть

        self.active_overlay = ctk.CTkFrame(self, height=200, corner_radius=10, fg_color="#1f1f1f")
        self.active_overlay.place(relx=0.5, rely=1.0, anchor="s", relwidth=0.9)

        ctk.CTkLabel(
            self.active_overlay,
            text="Лучшие сотрудники по числу заказов:",
            font=("Arial", 16, "bold")
        ).pack(pady=(10, 5))

        for i, (name, order_count) in enumerate(employees, 1):
            formatted_name = name.replace("_", " ")
            ctk.CTkLabel(
                self.active_overlay,
                text=f"{i}. {formatted_name} — {order_count} заказов",
                font=("Arial", 14)
            ).pack()

        ctk.CTkButton(
            self.active_overlay,
            text="Закрыть",
            command=self.close_active_overlay
        ).pack(pady=10)
    def show_revenue_by_employee_overlay(self, data: list[tuple]):
        self.close_active_overlay()  # Закрываем предыдущее окно, если есть

        self.active_overlay = ctk.CTkFrame(self, height=250, corner_radius=10, fg_color="#1f1f1f")
        self.active_overlay.place(relx=0.5, rely=1.0, anchor="s", relwidth=0.9)

        ctk.CTkLabel(
            self.active_overlay,
            text="Выручка по сотрудникам:",
            font=("Arial", 16, "bold")
        ).pack(pady=(10, 5))

        for name, revenue in data:
            clean_name = name.replace("_", " ")
            ctk.CTkLabel(
                self.active_overlay,
                text=f"{clean_name} — {revenue:.2f} руб.",
                font=("Arial", 14)
            ).pack()

        ctk.CTkButton(
            self.active_overlay,
            text="Закрыть",
            command=self.close_active_overlay
        ).pack(pady=10)
    def show_monthly_revenue_overlay(self, revenue: float, year_month: str):
        self.close_active_overlay()

        self.active_overlay = ctk.CTkFrame(self, height=120, corner_radius=10, fg_color="#1f1f1f")
        self.active_overlay.place(relx=0.5, rely=1.0, anchor="s", relwidth=0.9)

        ctk.CTkLabel(
            self.active_overlay,
            text=f"Выручка за {year_month}:",
            font=("Arial", 16, "bold")
        ).pack(pady=(10, 5))

        ctk.CTkLabel(
            self.active_overlay,
            text=f"{revenue:,.2f} руб.",
            font=("Arial", 18)
        ).pack(pady=(0, 10))

        ctk.CTkButton(
            self.active_overlay,
            text="Закрыть",
            command=self.close_active_overlay
        ).pack(pady=(0, 10))
    def show_monthly_revenue_selector(self):
        self.close_active_overlay()

        self.active_overlay = ctk.CTkFrame(self, height=180, corner_radius=10, fg_color="#1f1f1f")
        self.active_overlay.place(relx=0.5, rely=1.0, anchor="s", relwidth=0.9)

        ctk.CTkLabel(
            self.active_overlay,
            text="Выберите месяц и год:",
            font=("Arial", 16, "bold")
        ).pack(pady=(10, 5))

        # Списки месяцев и лет
        months = [
            "01 - Январь", "02 - Февраль", "03 - Март", "04 - Апрель",
            "05 - Май", "06 - Июнь", "07 - Июль", "08 - Август",
            "09 - Сентябрь", "10 - Октябрь", "11 - Ноябрь", "12 - Декабрь"
        ]
        years = [str(y) for y in range(2020, date.today().year + 1)]

        # Меню выбора
        month_menu = ctk.CTkOptionMenu(self.active_overlay, values=months)
        year_menu = ctk.CTkOptionMenu(self.active_overlay, values=years)
        month_menu.set(months[date.today().month - 1])
        year_menu.set(str(date.today().year))
        month_menu.pack(pady=5)
        year_menu.pack(pady=5)

        def run_query():
            selected_month = month_menu.get().split(" ")[0]
            selected_year = year_menu.get()
            ym = f"{selected_year}-{selected_month}"
            self.query_monthly_revenue_by_value(ym)

        ctk.CTkButton(self.active_overlay, text="Показать выручку", command=run_query).pack(pady=(5, 10))
        ctk.CTkButton(self.active_overlay, text="Закрыть", command=self.close_active_overlay).pack(pady=10)


    def _build_add_book_form(self, parent):
        def is_valid_year(year):
            return year.isdigit() and 0 < int(year) <= 2100

        def is_valid_price(price):
            try:
                float(price)
                return True
            except ValueError:
                return False

        conn = sqlite3.connect("bookstore.db")
        cursor = conn.cursor()

        cursor.execute("SELECT id, last_name, first_name, middle_name FROM Authors")
        authors = cursor.fetchall()
        author_options = [f"{a[1]} {a[2]} {a[3]}" for a in authors]

        cursor.execute("SELECT id, name FROM Categories")
        categories = cursor.fetchall()
        category_options = [c[1] for c in categories]

        conn.close()

        ctk.CTkLabel(parent, text="Название книги: ").pack()
        title_entry = ctk.CTkEntry(parent)
        title_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Автор: ").pack()
        author_menu = ctk.CTkOptionMenu(parent, values=author_options)
        author_menu.pack(pady=5)

        ctk.CTkLabel(parent, text="Категория: ").pack()
        category_menu = ctk.CTkOptionMenu(parent, values=category_options)
        category_menu.pack(pady=5)

        ctk.CTkLabel(parent, text="Год публикации: ").pack()
        year_entry = ctk.CTkEntry(parent)
        year_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Цена: ").pack()
        price_entry = ctk.CTkEntry(parent)
        price_entry.pack(pady=5)

        def add_book():
            title = title_entry.get().strip().replace(" ", "_")
            author_name = author_menu.get()
            category_name = category_menu.get()
            year = year_entry.get()
            price = price_entry.get()

            if not title:
                messagebox.showerror("Ошибка", "Название книги не может быть пустым")
                return

            if not is_valid_year(year):
                messagebox.showerror("Ошибка", "Введите корректный год")
                return

            if not is_valid_price(price):
                messagebox.showerror("Ошибка", "Введите корректную цену")
                return

            conn = sqlite3.connect("bookstore.db")
            cursor = conn.cursor()

            cursor.execute("SELECT MAX(id) FROM Books")
            max_id = cursor.fetchone()[0] or 0
            new_id = max_id + 1

            author_id = [a[0] for a in authors if f"{a[1]} {a[2]} {a[3]}" == author_name][0]
            category_id = [c[0] for c in categories if c[1] == category_name][0]

            with open("sql/insert_book.sql", "r", encoding="utf-8") as file:
                query = file.read()

            cursor.execute(query, (new_id, title, author_id, category_id, int(year), float(price)))
            conn.commit()
            conn.close()

            messagebox.showinfo("Успех", "Книга успешно добавлена")

        ctk.CTkButton(parent, text="Добавить книгу", command=add_book).pack(pady=10)
    def _build_add_author_form(self, parent):
        def is_valid_name(name):
            return name.isalpha()

        ctk.CTkLabel(parent, text="Фамилия: ").pack()
        last_name_entry = ctk.CTkEntry(parent)
        last_name_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Имя: ").pack()
        first_name_entry = ctk.CTkEntry(parent)
        first_name_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Отчество: ").pack()
        middle_name_entry = ctk.CTkEntry(parent)
        middle_name_entry.pack(pady=5)

        def add_author():
            last_name = last_name_entry.get()
            first_name = first_name_entry.get()
            middle_name = middle_name_entry.get() or "отсутствует"

            if not (is_valid_name(last_name) and is_valid_name(first_name) and is_valid_name(middle_name)):
                messagebox.showerror("Ошибка", "Имя, фамилия и отчество должны содержать только буквы")
                return

            try:
                conn = sqlite3.connect("bookstore.db")
                cursor = conn.cursor()
                cursor.execute("SELECT MAX(id) FROM Authors")
                max_id = cursor.fetchone()[0] or 0
                new_id = max_id + 1
                with open("sql/add_author.sql", "r", encoding="utf-8") as file:
                    query = file.read()
                cursor.execute(query, (new_id, last_name, first_name, middle_name))
                conn.commit()
                conn.close()
                messagebox.showinfo("Успех", "Автор успешно добавлен")
            except Exception as e:
                messagebox.showerror("Ошибка", str(e))

        ctk.CTkButton(parent, text="Добавить автора", command=add_author).pack(pady=10)
    def _build_add_customer_form(self, parent):
        def is_valid_name(name):
            return name.isalpha()

        def is_valid_email(email):
            return '@' in email

        def is_valid_phone(phone):
            return phone.startswith('+') and len(phone) == 12 and phone[1:].isdigit()

        ctk.CTkLabel(parent, text="Фамилия: ").pack()
        last_name_entry = ctk.CTkEntry(parent)
        last_name_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Имя: ").pack()
        first_name_entry = ctk.CTkEntry(parent)
        first_name_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Отчество: ").pack()
        middle_name_entry = ctk.CTkEntry(parent)
        middle_name_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Email: ").pack()
        email_entry = ctk.CTkEntry(parent)
        email_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Телефон: ").pack()
        phone_entry = ctk.CTkEntry(parent)
        phone_entry.pack(pady=5)

        def add_customer():
            last_name = last_name_entry.get()
            first_name = first_name_entry.get()
            middle_name = middle_name_entry.get() or "отсутствует"
            email = email_entry.get()
            phone = phone_entry.get()

            if not (is_valid_name(last_name) and is_valid_name(first_name) and is_valid_name(middle_name)):
                messagebox.showerror("Ошибка", "Имя, фамилия и отчество должны содержать только буквы")
                return

            if not is_valid_email(email):
                messagebox.showerror("Ошибка", "Некорректный email")
                return

            if not is_valid_phone(phone):
                messagebox.showerror("Ошибка", "Телефон должен начинаться с '+' и содержать 12 цифр")
                return

            try:
                conn = sqlite3.connect("bookstore.db")
                cursor = conn.cursor()
                cursor.execute("SELECT MAX(id) FROM Customers")
                max_id = cursor.fetchone()[0] or 0
                new_id = max_id + 1
                with open("sql/add_customer.sql", "r", encoding="utf-8") as file:
                    query = file.read()
                cursor.execute(query, (new_id, last_name, first_name, middle_name, email, phone))
                conn.commit()
                conn.close()
                messagebox.showinfo("Успех", "Клиент успешно добавлен")
            except Exception as e:
                messagebox.showerror("Ошибка", str(e))

        ctk.CTkButton(parent, text="Добавить клиента", command=add_customer).pack(pady=10)
    def _build_add_employee_form(self, parent):
        def is_valid_name(name):
            return re.fullmatch(r"[А-Яа-яЁё-]+", name) is not None

        conn = sqlite3.connect("bookstore.db")
        cursor = conn.cursor()

        cursor.execute("SELECT id, title FROM Positions")
        positions = cursor.fetchall()
        position_titles = [p[1] for p in positions]

        conn.close()

        ctk.CTkLabel(parent, text="Фамилия: ").pack()
        last_name_entry = ctk.CTkEntry(parent)
        last_name_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Имя: ").pack()
        first_name_entry = ctk.CTkEntry(parent)
        first_name_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Отчество: ").pack()
        middle_name_entry = ctk.CTkEntry(parent)
        middle_name_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Должность: ").pack()
        position_menu = ctk.CTkOptionMenu(parent, values=position_titles)
        position_menu.pack(pady=5)

        def add_employee():
            last_name = last_name_entry.get().strip()
            first_name = first_name_entry.get().strip()
            middle_name = middle_name_entry.get().strip() or "отсутствует"
            position_title = position_menu.get()

            if not (is_valid_name(last_name) and is_valid_name(first_name)):
                messagebox.showerror("Ошибка", "Фамилия и имя должны содержать только буквы")
                return

            if middle_name != "отсутствует" and not is_valid_name(middle_name):
                messagebox.showerror("Ошибка", "Отчество должно содержать только буквы или быть пустым")
                return

            conn = sqlite3.connect("bookstore.db")
            cursor = conn.cursor()

            cursor.execute("SELECT MAX(id) FROM Employees")
            max_id = cursor.fetchone()[0] or 0
            new_id = max_id + 1

            position_id = [p[0] for p in positions if p[1] == position_title][0]

            with open("sql/insert_employee.sql", "r", encoding="utf-8") as file:
                query = file.read()

            cursor.execute(query, (new_id, last_name, first_name, middle_name, position_id))
            conn.commit()
            conn.close()

            messagebox.showinfo("Успех", "Сотрудник успешно добавлен")

        ctk.CTkButton(parent, text="Добавить сотрудника", command=add_employee).pack(pady=10)
    def _build_add_order_form(self, parent):
        def is_valid_amount(val):
            return val.isdigit() and int(val) > 0

        conn = sqlite3.connect("bookstore.db")
        cursor = conn.cursor()

        cursor.execute("SELECT id, title, price FROM Books")
        books = cursor.fetchall()
        book_options = [f"{b[1].replace('_', ' ')} ({b[2]}₽)" for b in books]

        cursor.execute("SELECT id, last_name, first_name, middle_name FROM Customers")
        customers = cursor.fetchall()
        customer_options = [f"{c[1]} {c[2]} {c[3]}" for c in customers]

        cursor.execute("SELECT id, last_name, first_name, middle_name FROM Employees")
        employees = cursor.fetchall()
        employee_options = [f"{e[1]} {e[2]} {e[3]}" for e in employees]

        conn.close()

        ctk.CTkLabel(parent, text="Книга: ").pack()
        book_menu = ctk.CTkOptionMenu(parent, values=book_options)
        book_menu.pack(pady=5)

        ctk.CTkLabel(parent, text="Количество: ").pack()
        amount_entry = ctk.CTkEntry(parent)
        amount_entry.pack(pady=5)

        ctk.CTkLabel(parent, text="Клиент: ").pack()
        customer_menu = ctk.CTkOptionMenu(parent, values=customer_options)
        customer_menu.pack(pady=5)

        ctk.CTkLabel(parent, text="Сотрудник: ").pack()
        employee_menu = ctk.CTkOptionMenu(parent, values=employee_options)
        employee_menu.pack(pady=5)

        total_sum_label = ctk.CTkLabel(parent, text="Итоговая сумма: 0₽")
        total_sum_label.pack(pady=5)

        def update_total_sum(*_):
            try:
                book_index = book_options.index(book_menu.get())
                price = books[book_index][2]
                amount = int(amount_entry.get()) if amount_entry.get().isdigit() else 0
                total = price * amount
                total_sum_label.configure(text=f"Итоговая сумма: {total:.2f}₽")
            except:
                total_sum_label.configure(text="Итоговая сумма: 0₽")

        book_menu.configure(command=update_total_sum)
        amount_entry.bind("<KeyRelease>", update_total_sum)

        def add_order():
            book_text = book_menu.get()
            customer_text = customer_menu.get()
            employee_text = employee_menu.get()
            amount = amount_entry.get()

            if not is_valid_amount(amount):
                messagebox.showerror("Ошибка", "Введите корректное количество")
                return

            conn = sqlite3.connect("bookstore.db")
            cursor = conn.cursor()

            cursor.execute("SELECT MAX(id) FROM Orders")
            order_id = (cursor.fetchone()[0] or 0) + 1

            cursor.execute("SELECT MAX(id) FROM OrderItems")
            order_item_id = (cursor.fetchone()[0] or 0) + 1

            book_id = books[book_options.index(book_text)][0]
            price = books[book_options.index(book_text)][2]
            customer_id = [c[0] for c in customers if f"{c[1]} {c[2]} {c[3]}" == customer_text][0]
            employee_id = [e[0] for e in employees if f"{e[1]} {e[2]} {e[3]}" == employee_text][0]
            today = date.today().isoformat()

            with open("sql/insert_orders.sql", "r", encoding="utf-8") as f:
                insert_order_query = f.read()
            with open("sql/insert_order_items.sql", "r", encoding="utf-8") as f:
                insert_order_items_query = f.read()

            cursor.execute(insert_order_query, (order_id, customer_id, employee_id, today))
            cursor.execute(insert_order_items_query, (order_item_id, order_id, book_id, int(amount), float(price)))
            conn.commit()
            conn.close()

            messagebox.showinfo("Успех", "Заказ успешно добавлен")

        ctk.CTkButton(parent, text="Добавить заказ", command=add_order).pack(pady=10)
    def _build_queries_tab(self, frame):
        ctk.CTkButton(frame, text="Общая выручка", command=self.query_total_revenue).pack(pady=10)
        ctk.CTkButton(frame, text="Лучшие книги", command=self.query_top_5_books).pack(pady=10)
        ctk.CTkButton(frame, text="Лучшие клиенты", command=self.query_top_customers).pack(pady=10)
        ctk.CTkButton(frame, text="Эффективность сотрудников", command=self.query_employee_efficiency).pack(pady=10)
        ctk.CTkButton(frame, text="Выручка по сотрудникам", command=self.query_revenue_by_employee).pack(pady=10)
        ctk.CTkButton(frame, text="Выручка за выбранный месяц", command=self.show_monthly_revenue_selector).pack(pady=10)


    def query_total_revenue(self):
        try:
            with open("sql/total_revenue.sql", "r", encoding="utf-8") as file:
                sql = file.read()
            conn = sqlite3.connect("bookstore.db")
            cursor = conn.cursor()
            cursor.execute(sql)
            result = cursor.fetchone()
            total = result[0] if result[0] is not None else 0
            conn.close()
            self.show_revenue_overlay(total)
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось выполнить запрос: {e}")
    def query_top_5_books(self):
        try:
            with open("sql/top_5_books.sql", "r", encoding="utf-8") as file:
                sql = file.read()
            conn = sqlite3.connect("bookstore.db")
            cursor = conn.cursor()
            cursor.execute(sql)
            results = cursor.fetchall()
            conn.close()
            self.show_top_books_overlay(results)

        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось получить топ-5 книг: {e}")
    def query_top_customers(self):
        try:
            with open("sql/top_customers.sql", "r", encoding="utf-8") as file:
                sql = file.read()
            conn = sqlite3.connect("bookstore.db")
            cursor = conn.cursor()
            cursor.execute(sql)
            results = cursor.fetchall()
            conn.close()
            self.show_top_customers_overlay(results)
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось получить топ клиентов: {e}")
    def query_employee_efficiency(self):
        try:
            with open("sql/employee_efficiency.sql", "r", encoding="utf-8") as file:
                sql = file.read()
            conn = sqlite3.connect("bookstore.db")
            cursor = conn.cursor()
            cursor.execute(sql)
            results = cursor.fetchall()
            conn.close()
            self.show_employee_efficiency_overlay(results)
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось получить эффективность сотрудников: {e}")
    def query_revenue_by_employee(self):
        try:
            with open("sql/revenue_by_employee.sql", "r", encoding="utf-8") as file:
                sql = file.read()
            conn = sqlite3.connect("bookstore.db")
            cursor = conn.cursor()
            cursor.execute(sql)
            results = cursor.fetchall()
            conn.close()
            self.show_revenue_by_employee_overlay(results)
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось получить выручку по сотрудникам: {e}")
    def query_monthly_revenue_by_value(self, year_month: str):
        try:
            conn = sqlite3.connect("bookstore.db")
            cursor = conn.cursor()

            cursor.execute("""
                SELECT SUM(OrderItems.amount * OrderItems.unit_price)
                FROM Orders
                JOIN OrderItems ON Orders.id = OrderItems.order_id
                WHERE strftime('%Y-%m', Orders.order_date) = ?
            """, (year_month,))
            result = cursor.fetchone()
            monthly_total = result[0] if result[0] is not None else 0
            conn.close()
            self.show_monthly_revenue_overlay(monthly_total, year_month)
        except Exception as e:
            messagebox.showerror("Ошибка", f"Не удалось получить выручку за месяц: {e}")

    def _build_table_view(self, parent):
        for widget in parent.winfo_children():
            widget.destroy()

        ctk.CTkLabel(parent, text="Просмотр таблиц", font=ctk.CTkFont(size=20, weight="bold")).pack(pady=10)

        conn = sqlite3.connect("bookstore.db")
        cursor = conn.cursor()
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table'")
        tables = [row[0] for row in cursor.fetchall()]
        conn.close()

        # Меню выбора таблицы
        table_combo = ctk.CTkOptionMenu(parent, values=tables)
        table_combo.pack(pady=10)

        # Область для таблицы
        table_frame = ctk.CTkFrame(parent)
        table_frame.pack(fill="both", expand=True, padx=10, pady=10)

        def display_table(table_name):
            # Очистить старое содержимое
            for widget in table_frame.winfo_children():
                widget.destroy()

            conn = sqlite3.connect("bookstore.db")
            cursor = conn.cursor()

            cursor.execute(f"PRAGMA table_info({table_name})")
            columns = [col[1] for col in cursor.fetchall()]

            cursor.execute(f"SELECT * FROM {table_name}")
            rows = cursor.fetchall()
            conn.close()

            tree = ttk.Treeview(table_frame, columns=columns, show="headings")
            for col in columns:
                tree.heading(col, text=col)
                tree.column(col, width=100, anchor="center")

            for row in rows:
                tree.insert("", "end", values=row)

            tree.pack(side="left", fill="both", expand=True)

            scrollbar = ttk.Scrollbar(table_frame, orient="vertical", command=tree.yview)
            tree.configure(yscrollcommand=scrollbar.set)
            scrollbar.pack(side="right", fill="y")

        table_combo.configure(command=display_table)
        table_combo.set(tables[0])
        display_table(tables[0])
        conn.close()

if __name__ == "__main__":
    app = TabApp()
    app.mainloop()
