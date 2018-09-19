<HTML>

<HEAD>

<%@ page
    import = "java.io.*"
    import = "java.lang.*"
    import = "java.sql.*"
%>

<title>Design...</title>

</HEAD>

<BODY>

<%

    String design_text = request.getParameter("design_id");
    String product_name = request.getParameter("product_name");
    String product_sku = request.getParameter("product_sku");

try {
    // Establish a connection to the database.
        Class.forName("org.postgresql.Driver"); //load the driver
        Connection conn=DriverManager.getConnection("jdbc:postgresql://localhost/?user=kr623&password=SK0246NFalIj");

    // Create a statement for the SQL query
    java.sql.Statement stmt = conn.createStatement();

    // Create a string containing the SQL statement
   // String sqlUpdate = "INSERT INTO customer_design(customer_id, design_custom_text, design_date, template_id) VALUES (" + customer_id + ", '" +design_text + "'," + design_date "," + template_id + ")";
    //String sqlQuery = "SELECT design_custom_text FROM customer_design WHERE design_custom_text = '" + design_text + "'";

    //out.println(sqlUpdate);

    // Execute the query
   // int works = 0;
   // works = stmt.executeUpdate(sqlUpdate);
    //java.sql.ResultSet results = stmt.executeQuery(sqlQuery);


%>

<CENTER>
<TABLE CELLPADDING=1 BORDER=10>
<TR>


<%

out.println("<TR><TD>" + " Design added: " + design_text);
out.println("<TR><TD>" + "Product name: " + product_name);
out.println("<TR><TD>" + "Product sku: " + product_sku);

%>


</TABLE>
</CENTER>

<%

    stmt.close();
    conn.close();

}
catch(SQLException e)
{
    out.println("<P><B>SQLException: " + e.getMessage() + "<BR>");
    while((e = e.getNextException()) != null)
    out.println(e.getMessage() + "<BR>");
}


%>




</BODY>


</HTML>
