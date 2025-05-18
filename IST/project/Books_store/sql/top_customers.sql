SELECT 
    Customers.last_name || ' ' || Customers.first_name || 
    COALESCE(' ' || Customers.middle_name, '') AS full_name,
    SUM(OrderItems.amount * OrderItems.unit_price) AS total_spent
FROM Orders
JOIN Customers ON Orders.customer_id = Customers.id
JOIN OrderItems ON Orders.id = OrderItems.order_id
GROUP BY Customers.id
ORDER BY total_spent DESC
LIMIT 5;