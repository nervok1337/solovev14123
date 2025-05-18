SELECT 
    Employees.last_name || ' ' || Employees.first_name || 
    COALESCE(' ' || Employees.middle_name, '') AS full_name,
    COUNT(Orders.id) AS orders_count
FROM Orders
JOIN Employees ON Orders.employee_id = Employees.id
GROUP BY Employees.id
ORDER BY orders_count DESC
LIMIT 5;
