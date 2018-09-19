
--tableDeletion.sql--
drops tables from previous run of the shell script 

--temp_table_creation.sql-- 
creates temp tables that correspond to the data files

--load_temp_tables.sql--
loads the temp tables from the data files

--clean_tables.sql--
cleans the temp tables: 
	remove duplicates from user data.
	removed rows with invalid emails (not having @, for exmample)
	remove rows with invalid zip codes (spaces and hyphens, and canadian)
	remove header row from products 
	remove non-numeric price values 
	make categorial text fields lower case
	make state values upper case
	product names that have different prices --> Give each of them a different sku number

--final_table_creation.sql--
creates final tables, loads data into final tables from clean temp tables, and casts correct data types.


--view_creation.sql--
creates views specified in assignment. 

--DataDictionary.txt--
dictionary of tables and their elements
