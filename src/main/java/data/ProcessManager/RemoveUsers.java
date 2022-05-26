package data.ProcessManager;

import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;

import java.io.IOException;
@WebServlet("/Removeusers")
public class RemoveUsers extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
  
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
	{
		String role = null;
		 HttpSession session=request.getSession(true); 
		 response.setHeader("Cache-Control", "no-cache, no-store, must-revalidate"); // HTTP 1.1.
		 response.setHeader("Pragma", "no-cache"); // HTTP 1.0.
		 response.setDateHeader("Expires", 0);
		  if(session.getAttribute("username")==null || session.getAttribute("Role")==null)
		  {
			  StringBuffer url = request.getRequestURL();
			  String URL = url.toString();
			  session.setAttribute("url", URL);
			  response.sendRedirect("Login.jsp");
		  }
		  else if(session.getAttribute("Role")=="Admin")
		  {
		  }
	}
	

}
