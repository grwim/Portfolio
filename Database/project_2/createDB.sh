#!/bin/sh




psql kr623 << EOF

--update encoding

\encoding KOI8




--your different scripts

\i tableDeletion.sql

\i temp_table_creation.sql 

\i load_temp_tables.sql

\i clean_tables.sql

\i final_table_creation.sql

\i view_creation.sql

EOF
