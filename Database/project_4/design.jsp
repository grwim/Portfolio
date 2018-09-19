<html>
<head>
<%@ page
	import = "java.io.*"
	import = "java.lang.*"
	import = "java.sql.*"
%>
<title>Designs...</title>
</head>
<body>

<CENTER>

<%

String product_sku = request.getParameter("product_sku");

Connection conn = null;
java.sql.Statement stmt = null;

String sqlQuery = "SELECT product_name FROM product WHERE product_sku= " + product_sku ;
String sqlQuery2 = "SELECT user_design_name FROM customer_design";

String result_product_name = "";

try{

Class.forName("org.postgresql.Driver"); //load the driver
conn=DriverManager.getConnection("jdbc:postgresql://localhost/?user=kr623&password=SK0246NFalIj");

stmt = conn.createStatement();

java.sql.ResultSet results = stmt.executeQuery(sqlQuery);



while(results.next()){

	result_product_name = results.getString("product_name");

}

}

catch(SQLException e)
{
	out.println("<P><B>SQLException: " + e.getMessage() + "<BR>");
	while((e = e.getNextException()) != null)
	out.println(e.getMessage() + "<BR>");
}


try{
	java.sql.ResultSet results2 = stmt.executeQuery(sqlQuery2);





%>

<form method = "POST" action = "designconfirmation.jsp">
<p>Enter new design:
<input type="text" name="design_id" size="15"></p>

<input type = "hidden" value = "<%=product_sku%>" name = "product_sku">
<input type = "hidden" value = "<%=result_product_name%>" name = "product_name">
<p><input type="submit" value="Submit" name="B1"></p>

</form>

<form method = "POST" action = "existingdesignconfirmation.jsp">

<p>Select from existing designs</p>
<input type = "hidden" value = "<%=product_sku%>" name = "product_sku">
<input type = "hidden" value = "<%=result_product_name%>" name = "product_name">

<select name = "design_id" class = "form_control">
<%String result_design_name = "";
while(results2.next()){

	result_design_name = results2.getString("user_design_name");
	out.println("<option value '" + result_design_name + "'>" + result_design_name + "</option>");
}

%>

</select>

<input type = "submit" value = "Submit">

</form>


<BR>

<P>

</P>

<%

}

catch(SQLException e)
{
	out.println("<P><B>SQLException: " + e.getMessage() + "<BR>");
	while((e = e.getNextException()) != null)
	out.println(e.getMessage() + "<BR>");
}

finally{

	stmt.close();
    conn.close();

}



%>



</body>
</html>
