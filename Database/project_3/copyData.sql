\encoding KOI8
\copy customer from 'customer.dat' with delimiter '|';
\copy product from 'product.dat' with delimiter '|';
\copy product_category from 'product_category.dat' with delimiter '|';
\copy product_options from 'product_options.dat' with delimiter '|';
\copy "Order" from 'order.dat' with delimiter '|';
\copy design_template from 'design_template.dat' with delimiter '|';
\copy customer_design from 'customer_design.dat' with delimiter '|';
\copy customer_product_details from 'customer_product_details.dat' with delimiter '|';
