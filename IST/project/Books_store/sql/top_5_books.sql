SELECT Books.title, SUM(OrderItems.amount) AS total_sold
FROM OrderItems
JOIN Books ON OrderItems.book_id = Books.id
GROUP BY OrderItems.book_id
ORDER BY total_sold DESC
LIMIT 5;