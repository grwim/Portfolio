CREATE TABLE product_archive (
date_deleted DATE,
product_sku INT,
product_name VARCHAR(100),
product_price DECIMAL(7, 2),
product_image VARCHAR(100),
product_description VARCHAR(100) );

CREATE FUNCTION archiveProduct() RETURNS trigger AS $$
     BEGIN
         DELETE FROM product_category WHERE product_sku = OLD.product_sku;
         DELETE FROM product_options WHERE product_sku = OLD.product_sku;
         INSERT INTO product_archive VALUES(CURRENT_DATE, OLD.product_sku, OLD.product_name, OLD.product_price,
                OLD.product_image, OLD.product_description);
         RETURN OLD;
     END
$$ LANGUAGE plpgsql;

CREATE TRIGGER archiveProduct
BEFORE DELETE ON product
FOR EACH ROW
EXECUTE PROCEDURE archiveProduct();



