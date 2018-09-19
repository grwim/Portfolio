CREATE TABLE temp_product (
product_category        VARCHAR(65)      NOT NULL,
product_name            VARCHAR(100)     NOT NULL,
product_description     VARCHAR(65)      ,
price                   VARCHAR(65)      ,
product_option_1        VARCHAR(65)      NOT NULL,
option_value_1          VARCHAR(65)      NOT NULL,
nothing_usefull         VARCHAR(65)      ,
product_option_2        VARCHAR(65)      ,
option_value_2          VARCHAR(65)      
--PRIMARY KEY (product_name, option_value_1, option_value_2)  
 );

CREATE TABLE temp_user (
email                   VARCHAR(65)      NOT NULL,
password                VARCHAR(65)      NOT NULL,
email_2                 VARCHAR(65)      NOT NULL,
first_name              VARCHAR(65)      NOT NULL,
last_name               VARCHAR(65)      NOT NULL,
middle_initial          VARCHAR(65)      
--PRIMARY KEY   (email, password, last_name)
          );

CREATE TABLE temp_zip (
zip_code                VARCHAR(65)       NOT NULL,
state                   VARCHAR(65)       ,
country                 VARCHAR(65)       NOT NULL

--PRIMARY KEY   (zip_code) 
         );

CREATE TABLE temp_address (
email                   VARCHAR(65)       NOT NULL,
street                  VARCHAR(65),
city                    VARCHAR(65),
zip                     VARCHAR(65)
--PRIMARY KEY (email)                
        );

