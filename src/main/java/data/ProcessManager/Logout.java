package data.ProcessManager;

import java.io.IOException;

import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
@WebServlet("/Logout")
public class Logout extends HttpServlet{
	protected void doGet(HttpServletRequest req,HttpServletResponse res) throws IOException
	{
		HttpSession session = req.getSession();
		session.removeAttribute("username");
		session.removeAttribute("Role");
		session.removeAttribute("url");
		/*Cookie[] cookies = req.getCookies();
		for (Cookie ck : cookies) {
			if (ck.getName().equalsIgnoreCase("Cookuname")) {
				ck.setMaxAge(0);
				res.addCookie(ck);
			}
			if (ck.getName().equalsIgnoreCase("Cookpass")) {
				ck.setMaxAge(0);
				res.addCookie(ck);

			}
			if (ck.getName().equalsIgnoreCase("Cookrem")) {
				ck.setMaxAge(0);
				res.addCookie(ck);

			}
		}*/
		session.invalidate();
		res.sendRedirect("Login.jsp");
		res.setHeader("Cache-Control", "no-cache, no-store, must-revalidate"); // HTTP 1.1.
		res.setHeader("Pragma", "no-cache"); // HTTP 1.0.
		res.setDateHeader("Expires", 0);
		
	}

}
