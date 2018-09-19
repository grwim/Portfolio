CREATE TABLE temp2_user (
email                   VARCHAR(65)	 NOT NULL,
password                VARCHAR(65)	 ,
email_2                 VARCHAR(65)	 ,
first_name              VARCHAR(65)	 ,
last_name               VARCHAR(65)	 ,
middle_initial          VARCHAR(65)
--PRIMARY KEY   (email, password, last_name)
          );

CREATE TABLE temp2_product (
product_category        VARCHAR(65)	 NOT NULL,
product_name            VARCHAR(100)     NOT NULL,
product_description     VARCHAR(65)	 ,
price                   VARCHAR(65)	 ,
product_option_1        VARCHAR(65)	 NOT NULL,
option_value_1          VARCHAR(65)	 NOT NULL,
nothing_usefull         VARCHAR(65)	 ,
product_option_2        VARCHAR(65)	 ,
option_value_2          VARCHAR(65)
--PRIMARY KEY (product_name, option_value_1, option_value_2)
 );

CREATE TABLE temp2_address (
email                      VARCHAR(65)       NOT NULL,
street                     VARCHAR(65),
city                       VARCHAR(65),
zip                        VARCHAR(65),
PRIMARY KEY (email)                        );

CREATE TABLE temp2_zip ( 
zip_code                   VARCHAR(65)       NOT NULL,
state                      VARCHAR(65)       ,
country                    VARCHAR(65)       NOT NULL ) ;







  -- remove rows with  bad emails
UPDATE temp_user
SET email = 'invalid'
WHERE email !~'^[A-Za-z0-9._%-]+@[A-Za-z0-9.-]+[.][A-Za-z]+$';

UPDATE temp_user
SET email_2 = 'invalid'
WHERE email !~'^[A-Za-z0-9._%-]+@[A-Za-z0-9.-]+[.][A-Za-z]+$';

--remove duplicates in user
INSERT INTO temp2_user
SELECT DISTINCT email
FROM temp_user
WHERE temp_user.email <> 'invalid';

UPDATE temp2_user
SET
password = temp_user.password
FROM temp_user
WHERE temp2_user.email = temp_user.email;

UPDATE temp2_user
SET
email_2 = temp_user.email_2
FROM temp_user
WHERE temp2_user.email = temp_user.email;

UPDATE temp2_user
SET
first_name = temp_user.first_name
FROM temp_user
WHERE temp2_user.email = temp_user.email;

UPDATE temp2_user
SET
last_name = temp_user.last_name
FROM temp_user
WHERE temp2_user.email = temp_user.email;

UPDATE temp2_user
SET
middle_initial = temp_user.middle_initial
FROM temp_user
WHERE temp2_user.email = temp_user.email;

DROP TABLE temp_user;
ALTER TABLE temp2_user RENAME TO temp_user;






--remove duplicates, bad zip and email values in address
UPDATE temp_address SET zip='invalid'
    WHERE zip !~E'^\d{5}$|^\d{9}$|^\d{5}-\d{4}$';

UPDATE temp_address SET email='invalid'
  WHERE email !~E'^[A-Za-z0-9._%-]+@[A-Za-z0-9.-]+[.][A-Za-z]+$';

INSERT INTO temp2_address 
SELECT * 
FROM temp_address
WHERE temp_address.email <> 'invalid' OR temp_address.zip <> 'invalid'; 

DROP TABLE temp_address;
ALTER TABLE temp2_address RENAME TO temp_address;




--remove duplicates in zip




UPDATE temp_zip SET zip_code='invalid'
  WHERE country!='USA' OR zip_code !~E'^[0-9]{5}$|^[0-9]{9}$|^[0-9]{5}-[0-9]{4}$';


INSERT INTO temp2_zip 
SELECT * 
FROM temp_zip
WHERE temp_zip.zip_code <> 'invalid';

DROP TABLE temp_zip;
ALTER TABLE temp2_zip RENAME TO temp_zip;
--end remove duplicates from address






   -- remove non numeric price values 
UPDATE temp_product 
SET price='invalid'
WHERE price !~E'^[0-9]+$';



  -- remove header, duplicates for products 
INSERT INTO temp2_product 
SELECT * FROM temp_product
WHERE product_category <> 'product category' AND temp_product.price <> 'invalid';

DROP TABLE temp_product;
ALTER TABLE temp2_product RENAME TO temp_product;



--cast as correct data types and clean capitalization 
UPDATE temp_product
SET product_category = lower( product_category),
    product_option_1 = lower(product_option_1),
    option_value_1 = lower( option_value_1 ),
    product_option_2 = lower( product_option_2 ),
    option_value_2 = lower( option_value_2 );

  --temp_user
UPDATE temp_user
SET first_name = lower( first_name ),
    last_name = lower( last_name ),
    middle_initial = lower( middle_initial);

  --temp_zip
UPDATE temp_zip
SET state = upper( state ),
    country = lower( country);

  --temp_address
UPDATE temp_address
SET city = lower( city );






