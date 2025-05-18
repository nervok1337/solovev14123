SELECT SUM(OrderItems.amount * OrderItems.unit_price) AS monthly_revenue
FROM Orders
JOIN OrderItems ON Orders.id = OrderItems.order_id
WHERE strftime('%Y-%m', Orders.order_date) = strftime('%Y-%m', 'now');