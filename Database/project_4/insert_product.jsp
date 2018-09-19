
<html>
<head>
<%@ page
    import = "java.io.*"
    import = "java.lang.*"
    import = "java.sql.*"
%>
<title>Insert Product</title>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">
  <style>
        td{
            padding: 3px;
        }
    </style>
</head>
<body>

<P>
This JSP establishes a connection to kr623 and allows for an addition of a product to the database
</P>

<P>
<%

    String product_name = request.getParameter("product_name");
    String product_price = request.getParameter("product_price");
    String product_description = request.getParameter("product_description");

    // Note: for some reason this wasn't working, but it is supposed to allow for proper interfacing of psql with string values that have apostraphes
    //product_name = product_name.replace("'","''");
   // product_type = product_type.replace("'","''");

try {
    // Establish a connection to the database.
        Class.forName("org.postgresql.Driver");     //load the driver
        Connection conn=DriverManager.getConnection("jdbc:postgresql://localhost/?user=kr623&password=SK0246NFalIj");

    // Create a statement for the SQL query
    java.sql.Statement stmt = conn.createStatement();

    // Create a string containing the SQL statement
    String sqlInsert = "INSERT INTO product(product_name, product_price, product_description)  VALUES ('" + product_name + "'," + product_price + ",'" + product_description + "')";

    // Execute the query
    int works = 0;
    works = stmt.executeUpdate(sqlInsert);

    if (works != 0) {
        out.println("<P><B>Insert successful</B>");

    }
%>

<CENTER>
<TABLE CELLPADDING=1 BORDER=10>

<TR>
    <TD><B>Product Name</B></TD>

    <TD>
<%
    out.println(product_name);
%>
    </TD>

<TR>
    <TD><B>Product Price</B></TD>

    <TD>
<%
    out.println(product_price);
%>
    </TD>

<TR>
    <TD><B>Product Description</B></TD>

    <TD>
<%
    out.println(product_description);
%>
    </TD>


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



</body>
</html>
