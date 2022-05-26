<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>   
<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
font-family: Arial, Helvetica, sans-serif;
max-width: 500px;
margin: auto;
padding:20px;
}
input[type=text], input[type=password] {
  width: 100%;
  padding: 12px 20px;
  margin: 8px 0;
  display: inline-block;
  border: 1px solid #ccc;
  border-radius:10px;
  box-sizing: border-box;
}

button {
  background-color: #04AA6D;
   border-radius:10px;
  color: white;
  padding: 14px 20px;
  margin: 8px 0;
  border: none;
  cursor: pointer;
  width: 100%;
}

button:hover {
  opacity: 0.8;
}
.imgcontainer {
  text-align: center;
  margin: 24px 0 12px 0;
}
img.user {
  width: 40%;
  border-radius: 50%;
}
.a{
  padding:10px;
  border-radius:10px;
 }
 .button {
  background-color: #f44336;
   border-radius:10px;
  text-decoration: none;
  border: none;
  color: white;
  padding: 10px 20px;
 }
.container {
  padding: 16px;
}
. {
  width: auto;
  padding: 10px 18px;
  background-color: #f44336;
}
span.psw {
  float: right;
  padding-top: 16px;
}
@media screen and (max-width: 300px) {
  span.psw {
     display: block;
     float: none;
  }
  .signup {
     width: 100%;
  }
}

</style>
<title>WELCOME</title>
</head>
<body>
<div id="dvBrowserDetails"></div>
<%
response.setHeader("Cache-Control", "no-cache, no-store, must-revalidate"); // HTTP 1.1.
response.setHeader("Pragma", "no-cache"); // HTTP 1.0.
response.setDateHeader("Expires", 0);
response.setDateHeader("Expires",-1);
String uname = "", password = "", remember = "";
System.out.println(remember);
Cookie[] cookies = request.getCookies();
if (cookies != null) {
	for (Cookie cook : cookies) {
		if (cook.getName().equals("Cookuname"))
	uname = cook.getValue();
		else if (cook.getName().equals("Cookpass"))
	password = cook.getValue();
		else if (cook.getName().equals("Cookrem"))
	remember = cook.getValue();
	}
}
%>
<form name="form-login" id="form-login">
  <h1 style="text-align:center">WELCOME USER</h1>
  <div class="imgcontainer">
    <img src="images.jpg" alt="User" class="user">
  </div>
  <div class="container">
    <label for="uname"><b>USERNAME</b></label>
    <input type="text" id="username" name="username" placeholder="username"
			value="<%=uname%>" required />

    <label for="psw"><b>PASSWORD</b></label>
    <input type="password" name="password" id="password" placeholder="password" value="<%=password%>" required />
    <div style='color:red' id="message"></div>
    <label>
      <input type="checkbox" name="rem" id="rem"
			<%="1".equals(remember) ? "checked='/checked'" : " "%> /> RememberMe <br>
    </label>
    <button type="button" onclick="login()">LOGIN</button>
  </div>
</form>
  <div class="container" style="background-color:#f1f1f1">
  <a class="button" href="<%=request.getContextPath()%>/Signup.jsp">SIGNUP</a>
    <span class="psw">Forgot <a href="#">password?</a></span>
  </div>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js"></script>
 <script>
 var msg=null;
 function login()
 {
	  var username = $("#username").val();
	  var password = $("#password").val();
	  var rem = $("#rem").prop('checked');
	  //alert(rem);
	  //string returnString = "";
	  //returnString = Convert.ToString(Session["url"]);
	  $.ajax({
		  url:"login",
		  type:"post",
		  data:{username:username,password:password,rem:rem},
		  success: function(responseText)
		  {
			  var a=JSON.parse(responseText)
			  //alert(a);
			  var b=a["msg"];
			  //alert(b);
				   
				   if(b=="0")
					   {
					    $('#message').html("Username or password incorrect");
					   }
				   else if(b=="1")
					   {
					      //alert("Hello"+ username);
					      window.location.href="Home.jsp";
					   }
				   else if(b=="2")
					   {
					   var session ='<%= session.getAttribute("url")%>';
					   window.location.href=session;
					   }
		  }
	  })
  }
 </script> 
</body>
</html>