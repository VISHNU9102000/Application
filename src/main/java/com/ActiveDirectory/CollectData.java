package com.ActiveDirectory;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import jakarta.servlet.RequestDispatcher;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
@WebServlet("/CollectData")
public class CollectData extends HttpServlet {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException
	{
		//String os=null;
		//String role = null;
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
		  else
		  {
		  response.setContentType("text/html");
	      PrintWriter out = response.getWriter();
	      ArrayList<String> data2 = new ArrayList<>();
	 
			GetADdata detail1 = new GetADdata();
		     data2 = detail1.GetData("Entering into c++");
		  request.setAttribute("data", data2);
		  RequestDispatcher rd = request.getRequestDispatcher("PrintUserData.jsp");
		  rd.include(request, response);
		  }
	}

}
