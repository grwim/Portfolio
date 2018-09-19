
CREATE INDEX productProductPrice_index ON product (product_price);
CREATE INDEX productProductName_index ON product (product_name);
CREATE INDEX orderOrderDate_index ON "Order" (order_date);
CREATE INDEX orderTotalPrice_index ON "Order" (total_price);
CREATE INDEX orderCustomerId_index ON "Order" USING Hash (customer_id);
CREATE INDEX customerProductDetailsProductPrice_index ON customer_product_details(product_price);
CREATE INDEX customerFirstName_index ON customer USING Hash (first_name);
CREATE INDEX customerLastName_index ON customer USING Hash (last_name);
CREATE INDEX customerDesignUserDesignName_index ON customer_design USING Hash (user_design_name);
CREATE INDEX customerDesignDesignDate_index ON customer_design USING Hash (design_date);
CREATE INDEX customerDesignDesignFontSize_index ON customer_design USING Hash (design_font_size);
CREATE INDEX customerDesignDesignFontSizeColor_index ON customer_design USING Hash (design_font_size_color);
CREATE INDEX designTemplateTemplateName_index ON design_template(template_name);
