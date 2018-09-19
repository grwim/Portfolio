\copy temp_product FROM '/home/kr623/final-data/products.dat' ( FORMAT CSV,  DELIMITER (';') );
--\copy temp_product (product_category, product_name, product_description, price, product_option_1, option_value_1, nothing_usefull, product_option_2, option_value_2) FROM '/home/kr623/final-data/products.dat' WITH CSV HEADER DELIMITER ';'  

\copy temp_user FROM '/home/kr623/final-data/user.dat' ( FORMAT CSV,  DELIMITER ('|') );

\copy temp_zip FROM '/home/kr623/final-data/zip.dat' ( FORMAT CSV,  DELIMITER ('|') );

--\copy temp_address FROM '/home/kr623/final-data/address.dat' ( FORMAT CSV,  DELIMITER (';') );
\copy temp_address (email, street, city, zip) FROM '/home/kr623/final-data/address.dat' WITH CSV HEADER DELIMITER '|'
