DROP TABLE IF EXISTS Customer_Preferences;
DROP TABLE IF EXISTS Customer_Artwork;
DROP TABLE IF EXISTS Customer_Product_Details;
DROP TABLE IF EXISTS Customer_Design;
DROP TABLE IF EXISTS Design_Template;
DROP TABLE IF EXISTS Product_Category;
DROP TABLE IF EXISTS Product_Options;
DROP TABLE IF EXISTS Product;
DROP TABLE IF EXISTS "Order";
DROP TABLE IF EXISTS Customer;

-- create permanent tables

CREATE TABLE Customer (
	customer_id SERIAL PRIMARY KEY NOT NULL,
	customer_email VARCHAR(100) NOT NULL, 
	password VARCHAR(100) NOT NULL,
	first_name VARCHAR(100) NOT NULL,
	last_name VARCHAR(100) NOT NULL,
	street_address VARCHAR(100),
	city VARCHAR(100),
	state VARCHAR(2),
	zip_code VARCHAR(15),
	UNIQUE (customer_email)


);

CREATE TABLE Customer_Preferences (
	customer_id INT NOT NULL,
	preference_type VARCHAR(100) NOT NULL,
	preference VARCHAR(100) NOT NULL,
	PRIMARY KEY( customer_id, preference_type, preference),
	FOREIGN KEY ( customer_id ) REFERENCES Customer (customer_id)
	ON DELETE SET NULL
	ON UPDATE CASCADE
);

CREATE TABLE Design_Template (
	template_id SERIAL PRIMARY KEY NOT NULL,
	template_name VARCHAR(100) ,
	template_description VARCHAR(100) ,
	template_filename VARCHAR(100) 
);

CREATE TABLE Customer_Design (
	design_id SERIAL PRIMARY KEY NOT NULL,
	customer_id INT NOT NULL,
	template_id INT NOT NULL,
	user_design_name VARCHAR(100) ,
	design_date DATE NOT NULL,
	design_filename VARCHAR(100) ,
	design_custom_text VARCHAR(100) ,
	design_font VARCHAR(100) DEFAULT  'Times',
	design_font_size INT DEFAULT '12',
	design_font_size_color VARCHAR(100) DEFAULT 'Black',
	FOREIGN KEY ( customer_id ) REFERENCES Customer (customer_id)
           ON DELETE SET NULL
	ON UPDATE CASCADE,
	FOREIGN KEY ( template_id ) REFERENCES Design_Template (template_id)
	ON DELETE SET NULL
	ON UPDATE CASCADE,
	UNIQUE (customer_id, template_id, design_date)
);

CREATE TABLE Customer_Artwork (
	artwork_id SERIAL PRIMARY KEY,
	customer_id INT NOT NULL,
	design_id INT NOT NULL,
	art_filename VARCHAR(100) NOT NULL,
	FOREIGN KEY ( customer_id ) REFERENCES Customer (customer_id)
	ON DELETE SET NULL
	ON UPDATE CASCADE,
      	FOREIGN KEY ( design_id ) REFERENCES Customer_Design (design_id)
	ON DELETE SET NULL
	ON UPDATE CASCADE,
	UNIQUE (customer_id, design_id, art_filename)

);

CREATE TABLE Product (
	product_sku SERIAL PRIMARY KEY NOT NULL,
	product_name VARCHAR(100) NOT NULL,
	product_price DECIMAL(7, 2) ,
	product_image VARCHAR(100) ,
	product_description VARCHAR(100) 
);

CREATE TABLE Product_Category (
	product_sku INT NOT NULL,
	product_category VARCHAR(20) NOT NULL,
	PRIMARY KEY( product_sku, product_category),
      	FOREIGN KEY ( product_sku ) REFERENCES Product (product_sku)
	ON DELETE SET NULL
	ON UPDATE CASCADE
);

CREATE TABLE Product_Options (
	product_sku INT NOT NULL,
	product_option_type VARCHAR(100) NOT NULL,
	option_value VARCHAR(100) NOT NULL,
	PRIMARY KEY( product_sku, product_option_type, option_value),
      	FOREIGN KEY ( product_sku ) REFERENCES Product (product_sku)
	ON DELETE SET NULL
	ON UPDATE CASCADE
);

CREATE TABLE "Order" (
	order_id SERIAL PRIMARY KEY NOT NULL,
	order_date DATE DEFAULT CURRENT_DATE,
	total_price DECIMAL(10,2) CHECK(total_price > 0),
	customer_id INT NOT NULL,
	street_address_1 VARCHAR(100) ,
	street_address_2 VARCHAR(100), 
	city VARCHAR(50) ,
	state CHAR(2) ,
	zip_code VARCHAR(15) ,
	credit_card_number INT CHECK( length(credit_card_number::VARCHAR) = 15 OR length(credit_card_number::VARCHAR) = 16),
	credit_card_expiration_date DATE CHECK( credit_card_expiration_date > '2017-3-31'),
	credit_card_security_code INT CHECK( length(credit_card_security_code::VARCHAR) = 3 OR length(credit_card_security_code::VARCHAR) = 4) ,
	approval_code INT,
      	FOREIGN KEY ( customer_id ) REFERENCES Customer (customer_id) 
	ON DELETE SET NULL
	ON UPDATE CASCADE
);

CREATE TABLE Customer_Product_Details (
	customer_id INT NOT NULL,
	product_sku INT NOT NULL,
	order_id INT NOT NULL,
	design_id INT NOT NULL,
	product_price DECIMAL(7, 2) CHECK(product_price > 0), 
	product_size VARCHAR(5) DEFAULT 'M',
	product_quantity INT NOT NULL CHECK(product_quantity > 0),
	PRIMARY KEY( customer_id, product_sku, order_id, design_id),
      	FOREIGN KEY ( customer_id ) REFERENCES Customer (customer_id)
	ON DELETE SET NULL
	ON UPDATE CASCADE,
      	FOREIGN KEY ( product_sku ) REFERENCES Product (product_sku)
	ON DELETE SET NULL
	ON UPDATE CASCADE,
      	FOREIGN KEY ( order_id ) REFERENCES "Order"(order_id)
	ON DELETE SET NULL
	ON UPDATE CASCADE,
      	FOREIGN KEY ( design_id ) REFERENCES Customer_Design (design_id)
	ON DELETE SET NULL
	ON UPDATE CASCADE
);

