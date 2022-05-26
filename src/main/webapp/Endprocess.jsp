<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<%@ page import="java.util.*" %>   
<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<title>Insert title here</title>
</head>
<body>
<%
   String id = (String)request.getParameter("param");
    
   System.out.println(id);
  
	try{
		String end;
		if(System.getProperty("os.name").contains("Windows"))
		{	
			 end  = "powershell.exe Stop-process -Id "+id;
			 System.out.println("Entering into stop process");
		}
		else
		{
			end = "kill -15 "+id;
		}
	    
	     Process pd = Runtime.getRuntime().exec(end);
		Thread.sleep(3000);
	     response.sendRedirect("DataServlet");
	     System.out.println(pd);
	     }catch(Exception e)
	     {
	         e.printStackTrace();
	     }
%>

</body>
</html>