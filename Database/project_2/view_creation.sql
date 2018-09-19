CREATE VIEW num_sold_products_each_day AS 
SELECT t.transaction_date AS Date, cp.product_sku AS Product_ID, count(cp.product_sku) AS Sold 
FROM customer_product_details cp 
LEFT OUTER JOIN Transaction t 
ON t.transaction_id = cp.transaction_id 
GROUP BY t.transaction_date, cp.product_sku;



CREATE VIEW products_by_type AS 
SELECT product_name, product_price, product_category 
FROM product p 
JOIN product_category pc 
ON p.product_sku = pc.product_sku 
GROUP BY product_category, product_name, product_price;
