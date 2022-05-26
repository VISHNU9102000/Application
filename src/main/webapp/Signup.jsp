<!DOCTYPE html>
<html>
<style>
body {font-family: Arial, Helvetica, sans-serif;
max-width: 500px;
margin: auto;
padding:20px;}

/* Full-width input fields */
input[type=text], input[type=password] {
 width: 100%;
  padding: 12px 20px;
   border-radius:10px;
  margin: 8px 0;
  display: inline-block;
  border: 1px solid #ccc;
  box-sizing: border-box;
}

input[type=text]:focus, input[type=password]:focus {
  background-color: #ddd;
  outline: none;
}

hr {
  border: 1px solid #f1f1f1;
  margin-bottom: 25px;
}

/* Set a style for all buttons */
button {
  background-color: #04AA6D;
   border-radius:10px;
  color: white;
  padding: 14px 20px;
  margin: 8px 0;
  border: none;
  cursor: pointer;
  width: 100%;
  opacity: 0.9;
}

button:hover {
  opacity:1;
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
</style>
<body>
<%
response.setHeader("Cache-Control", "no-cache, no-store, must-revalidate"); // HTTP 1.1.
response.setHeader("Pragma", "no-cache"); // HTTP 1.0.
response.setDateHeader("Expires", 0);
%>
  <div class="container">
    <h1>Sign Up</h1>
    <p>Please fill in this form to create an account.</p>
    <span style='color:red' id='exist'></span>
    <hr>
    <label for="username"><b>USERNAME</b></label>
    <input type="text" placeholder="USERNAME" name="username" id="username" required>

    <label for="psw"><b>PASSWORD</b></label>
    <input type="password" placeholder="ENTER PASSWORD" name="password" id="password" required>

    <label for="psw-repeat"><b>REPEAT PASSWORD <span id='message'></span></b></label>
    <input type="password" placeholder="REPEAT PASSWORD" name="psw-repeat" id="confirmpassword" required>
    
    <label>
      <input type="checkbox" name="rem" value="1"/> Remember Me <br>
      </label>
    <button type="button" onclick="login()">SIGN UP</button>
    </div>
    <div class="container" style="background-color:#f1f1f1">
  <a class="button" href="<%=request.getContextPath()%>/Login.jsp">CANCEL</a>
  </div>
 <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
<script>
var msg=null;
function login()
{
		      var username = $("#username").val();
		      var password = $("#password").val();
		      var rem = $("#myCheck").prop('checked');
		      $.ajax({
				  url:"Signup",
				  type:"post",
		          data:{username:username,password:password,rem:rem},
			  success: function(responseText)
			  {
				  //alert(responseText);
				  var a=JSON.parse(responseText);
				  //alert(a);
				  var b=a["msg"];
				  //alert(b);
					   
					   if(b=="1")
						   {
						    $('#exist').html("Username already exists");
						   }
					   else if(b=="2")
						   {
						   $('#exist').html("Could not create account");
						   }
					   else if(b=="3")
						   {
						    alert("Account Created Sucessfully");
						      window.location.href= "Home.jsp"; 
						   }
					   else if(b=="4")
						   {
						   alert("Account Created Sucessfully");
						   window.location.href= "Home.jsp"; 
						   }
			  }
		  })   
}
</script>   