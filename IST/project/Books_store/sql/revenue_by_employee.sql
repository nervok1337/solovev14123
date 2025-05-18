SELECT
    Employees.last_name || ' ' || Employees.first_name AS employee_name,
    SUM(OrderItems.unit_price * OrderItems.amount) AS total_revenue
FROM Orders
JOIN Employees ON Orders.employee_id = Employees.id
JOIN OrderItems ON Orders.id = OrderItems.order_id
GROUP BY Employees.id
ORDER BY total_revenue DESC;
