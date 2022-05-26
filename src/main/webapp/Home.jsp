<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<style type="text/css">
body {
  margin: 0;
  font-family: Arial, Helvetica, sans-serif;
}
#one{
font-family: Arial, Helvetica, sans-serif;
max-width: 500px;
margin: 10px;
padding:20px;
border-radius: 25px;;
background-color: Grey;
}
.button {
  background-color: #4CAF50;
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  border-radius :15px;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
}
.topnav {
  overflow: hidden;
  background-color: #333;
}

.topnav a {
  float: left;
  color: #f2f2f2;
  text-align: center;
  padding: 14px 16px;
  text-decoration: none;
  font-size: 17px;
}

.topnav a:hover {
  background-color: #ddd;
  color: black;
}
.topnav a.split {
  float: right;
  background-color: #04AA6D;
  color: white;
}
</style>
<meta charset="ISO-8859-1">
<title>TASK MANAGER</title>
</head>
<body style='align:center; background-color :#f1f1f1'>
<%
response.setHeader("Cache-Control", "no-cache, no-store, must-revalidate"); // HTTP 1.1.
response.setHeader("Pragma", "no-cache"); // HTTP 1.0.
response.setDateHeader("Expires", 0);
  if(session.getAttribute("username")==null)
  {
	  response.sendRedirect("Login.jsp");
  }
   %> 
   <div class="topnav">
  <a class="active">Welcome ${username} </a>
  <a class="active"href="<%=request.getContextPath()%>/CollectData">ACTIVE DIRECTORY USER DATA</a>
  <%if(session.getAttribute("Role")=="Admin"){ %>
  <a class="active"href="<%=request.getContextPath()%>/CollectData">CREATE USER</a>
  <%} %>
  <a href="<%=request.getContextPath()%>/Logout" class="split">Logout</a>
</div>
<div id="one">
<h1 style="text-align:center;">CLICK TO VIEW TASK MANAGER</h1>
  <form style="text-align:center;" action="<%=request.getContextPath()%>/Welcome" method="Get">
  <input type="submit" class="button" value="CLICK HERE">
  </form>
 </div>
</body>
</html>