

CREATE TABLE Customer (
   customer_id       SERIAL        PRIMARY KEY,
   customer_email    TEXT          NOT NULL, --candidate key
   password          TEXT          ,
   first_name        TEXT          NOT NULL,
   last_name         TEXT          NOT NULL,
   street_address    TEXT          ,
   city              TEXT          ,
   state             TEXT          ,
   zip_code          TEXT          ,
   UNIQUE            (customer_email) );

--get number of customers (unique emails)

--use that number to make table of random set of X numbers from 0-X with attribute of customer_id for this id

INSERT INTO Customer(customer_email, password, first_name, last_name, street_address, city, zip_code  )
SELECT  cast( u.email AS TEXT), cast(u.password AS TEXT),  cast( u.first_name AS TEXT), cast( u.last_name AS TEXT),
cast( a.street AS TEXT), cast( a.city AS TEXT), cast( a.zip AS TEXT)
FROM temp_user AS u, temp_address AS a
WHERE u.email = a.email;

UPDATE Customer
SET
state = cast( temp_zip.state AS TEXT)
FROM temp_zip
WHERE Customer.zip_code = temp_zip.zip_code;


/*
INSERT INTO Customer(customer_email, password, first_name, last_name, street_address, city, state, zip_code  )
SELECT  cast( u.email AS TEXT), cast(u.password AS TEXT),  cast( u.first_name AS TEXT), cast( u.last_name AS TEXT),
cast( a.street AS TEXT), cast( a.city AS TEXT), cast( z.state AS TEXT),  cast( a.zip AS TEXT)
FROM temp_user AS u, temp_address AS a, temp_zip AS z
WHERE u.email = a.email AND a.zip = z.zip_code;
*/


  --BEGIN Customer_Preferences
CREATE TABLE Customer_Preferences (
   customer_id       INT           NOT NULL,
   preference_type   TEXT          NOT NULL,
   preference        TEXT          NOT NULL,
   PRIMARY KEY       (customer_id, preference_type, preference),
   FOREIGN KEY       (customer_id) REFERENCES Customer(customer_id)  );

CREATE TABLE Design_Template (
   template_id             SERIAL          NOT NULL,
   template_name           TEXT            NOT NULL,
   template_description    TEXT            ,
   template_filename       TEXT            ,
   PRIMARY KEY     (template_id)    );

CREATE TABLE Product (
   product_sku           SERIAL          NOT NULL,
   product_name          TEXT            NOT NULL,
   product_price         TEXT            NOT NULL,
   product_image         TEXT            ,
   product_description   TEXT            ,
   PRIMARY KEY   (product_sku)    );

INSERT INTO Product (product_name, product_price, product_description)
SELECT cast( p.product_name AS TEXT), cast( p.price AS INT), cast( p.product_description AS TEXT)
FROM temp_product AS p
WHERE 
p.price <> 'null';


CREATE TABLE Transaction (
   transaction_id             SERIAL         NOT NULL,
   transaction_date           TEXT           NOT NULL,
   total_price          INT            NOT NULL,
   customer_id          INT            NOT NULL,
   street_address_1     TEXT           NOT NULL,
   street_address_2     TEXT           NOT NULL,
   city                 TEXT           NOT NULL,
   state                TEXT           NOT NULL,
   zip_code             TEXT           NOT NULL,
   credit_card_number   INT                 ,
   credit_card_expiration_date   DATE       ,
   credit_card_security_code     INT        ,
   approval_code                 TEXT       ,
   PRIMARY KEY    (transaction_id),
   FOREIGN KEY    (customer_id) REFERENCES Customer(customer_id)
      ON UPDATE CASCADE
);


CREATE TABLE Customer_Design (
   design_id         SERIAL         NOT NULL,
   customer_id       INT            NOT NULL,
   template_id       INT            NOT NULL,
   user_design_name  TEXT           NOT NULL,
   design_date       DATE          ,
   UNIQUE            (customer_id, template_id, design_date),
   PRIMARY KEY       (design_id),
   FOREIGN KEY       (customer_id) REFERENCES Customer(customer_id),
   FOREIGN KEY       (template_id) REFERENCES Design_Template(template_id)  
);


CREATE TABLE Customer_Artwork (
   artwork_id        SERIAL        NOT NULL,
   customer_id       INT           NOT NULL,
   design_id         INT           NOT NULL,
   art_filename      TEXT          ,
   UNIQUE            (customer_id, design_id, art_filename),
   PRIMARY KEY       (artwork_id),
   FOREIGN KEY       (customer_id) REFERENCES Customer(customer_id),
   FOREIGN KEY       (design_id) REFERENCES Customer_Design(design_id)   
);


CREATE TABLE Product_Category (
   product_sku         SERIAL             NOT NULL,
   product_category    TEXT            NOT NULL,
   PRIMARY KEY        (product_sku, product_category)
-- FOREIGN KEY    (product_sku) REFERENCES Product(product_sku)
--       ON UPDATE CASCADE  
);

INSERT INTO Product_Category  (product_category)
SELECT cast( p.product_category AS TEXT)
FROM temp_product AS p;



CREATE TABLE Product_Options (
   product_sku            INT     NOT NULL,
   product_option_type    TEXT    NOT NULL,
   option_value           TEXT    NOT NULL,
   PRIMARY KEY    (product_sku, product_option_type, option_value),
   FOREIGN KEY    (product_sku) REFERENCES Product(product_sku)
         ON UPDATE CASCADE  
 );


CREATE TABLE Customer_Product_Details (
   customer_id          INT                NOT NULL,
   product_sku          INT                NOT NULL,
   transaction_id       INT                NOT NULL,
   design_id            INT                NOT NULL,
   product_price        INT                NOT NULL,
   product_size         INT                NOT NULL,
   product_quantity     INT                NOT NULL,
   PRIMARY KEY    (customer_id, product_sku, transaction_id, design_id),
   FOREIGN KEY    (customer_id) REFERENCES Customer(customer_id)
         ON UPDATE CASCADE,
   FOREIGN KEY    (product_sku) REFERENCES Product(product_sku)
         ON UPDATE CASCADE,
   FOREIGN KEY    (transaction_id) REFERENCES Transaction(transaction_id)
         ON UPDATE CASCADE,
   FOREIGN KEY    (design_id) REFERENCES Customer_Design(design_id)
         ON UPDATE CASCADE
);













