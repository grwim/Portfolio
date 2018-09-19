<html>
<head>
<%@ page
    import = "java.io.*"
    import = "java.lang.*"
    import = "java.sql.*"
%>
<title>Products...</title>
</head>
<body>

<BR>

<P>
This JSP establishes a connection to kr623 and
gets data from an product table
</P>

<%

    String product_price = request.getParameter("product_price");
    String product_type = request.getParameter("product_type");
    String product_name = request.getParameter("product_name");

    if(product_name == null || product_name.isEmpty())
        product_name = null;
    else
        product_name = product_name.replace("'","''"); //replacing single quote with two single quotes to escape
    if(product_price == null || product_price.isEmpty())
        product_price = null;
    if(product_type == null || product_type.isEmpty())
        product_type = null;
    else
        product_type = product_type.replace("'","''"); //replacing single quote with two single quotes to escape
    //product_type = product_type.replace("'","''");

    out.println(product_name);

    // Note: for some reason this wasn't working, but it is supposed to allow for proper interfacing of psql with string values that have apostraphes
    //product_name = product_name.replace("'","''");
   // product_type = product_type.replace("'","''");

try {
    // Establish a connection to the database.
    Class.forName("org.postgresql.Driver");     //load the driver
    Connection dbConnect =DriverManager.getConnection("jdbc:postgresql://localhost/?user=kr623&password=SK0246NFalIj");

    // Create a statement for the SQL query
    java.sql.Statement stmt = dbConnect.createStatement();

    // Create a string containing the SQL statement
    //String sqlQuery = "SELECT product.product_name, product.product_sku FROM product INNER JOIN product_category USING (product_sku) WHERE (product_name = '" + product_name + "' ) OR (product_category ='" + product_type + "' ) OR (product.product_price = " + product_price + ")";

    String sqlQuery = "SELECT p.product_sku, product_name, product_price, product_category FROM product p join product_category pc on p.product_sku = pc.product_sku WHERE product_name ='" + product_name + "' OR product_price = " + product_price + " OR product_category = '" + product_type + "' order by product_sku";

    // Execute the query
    java.sql.ResultSet results = stmt.executeQuery(sqlQuery);


%>

<CENTER>
<TABLE CELLPADDING=1 BORDER=10>
<TR>
    <TD><B>Product Name</B></TD>


<%
    // Declare local variables
    String result_product_name = "";
    int result_product_sku;

    // Print each tuple
    while (results.next()) {
        result_product_sku = results.getInt("product_sku");
        result_product_name = results.getString("product_name");
        out.println("</TR><TD><a href='design.jsp?product_sku="+result_product_sku+"'>"+ result_product_name+"</a>");
        out.println("</TD>");
    }
%>


</TABLE>
</CENTER>

<%

    results.close();
    stmt.close();
    dbConnect.close();

}
catch(SQLException myException)
{
    out.println("SQLException: " + myException.getMessage() + "<BR>");
    while((myException = myException.getNextException()) != null)
    out.println(myException.getMessage() + "<BR>");
}


%>



</body>
</html>
