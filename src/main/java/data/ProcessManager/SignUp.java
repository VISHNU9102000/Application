package data.ProcessManager;


import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;

import java.io.IOException;
import java.io.PrintWriter;

import org.json.simple.JSONObject;
@WebServlet("/Signup")
public class SignUp extends HttpServlet {
	private static final long serialVersionUID = 1L;
    //private static final long serialVersionUID = 1 L;
    private LoginDao loginDao;
    public void init() {
        loginDao = new LoginDao();
    }
	@SuppressWarnings("unchecked")
	protected void doPost(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException
	{
		boolean status = false;
		System.out.println("Entering into SignUp.java");
		String msg = null;
		PrintWriter out = res.getWriter();
		JSONObject object1 = new JSONObject();
		String username = req.getParameter("username");
        String password = req.getParameter("password");
        String remember = req.getParameter("rem");
        LoginBean loginBean = new LoginBean();
        loginBean.setUsername(username);
        loginBean.setPassword(password);
        HttpSession session = req.getSession();
        String link=null;
        link = (String) session.getAttribute("url");
        try {
        	status = loginDao.CheckUser(loginBean);
		} catch (ClassNotFoundException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
        if(status)
        {
        	msg="1";
        }
        else
        {
					try {
						if(loginDao.CreateUser(loginBean))
						{
							boolean remember1 = req.getParameter("rem") != null;
				            System.out.println(remember1);
				            if (req.getParameter("rem") != null) 
				            {
								Cookie checkuser = new Cookie("Cookuname", username);
								checkuser.setMaxAge(5000);
								res.addCookie(checkuser);
								Cookie checkpswd = new Cookie("Cookpass", password);
								checkpswd.setMaxAge(5000);
								res.addCookie(checkpswd);
								Cookie ckReme = new Cookie("CookRem", remember);
								ckReme.setMaxAge(5000);
								res.addCookie(ckReme);
				                session.setAttribute("username",username);
				                if(loginDao.Role(loginBean))
				                {
				            	   System.out.println("Hello Administrator yes cache");
				            	   session.setAttribute("Role", "Admin");
				                }
				            	else 
				                {
				               	   System.out.println("Hello User yes cache");
				               	   session.setAttribute("Role", "User");
				                }
				            } else {
				        	  if(loginDao.Role(loginBean))
				               {
				        	      System.out.println("Hello Administrator no cache");
				        	      session.setAttribute("Role", "Admin");
				               }
				        	   else 
				               {
				           	   System.out.println("Hello User no cache");
				           	   session.setAttribute("Role", "User");
				               }
				             }
				             if(link==null)
				    	      {
				    	   	     msg="3";
				    		     object1.put("msg",msg);
				    	     }
				    	     else
				    	      {
				    		    msg="4";
				    		    object1.put("msg",msg);
				    	     }
				             
				        } 
				        else {
				        	msg="2";
				        }
				      }catch (ClassNotFoundException e) {
				        e.printStackTrace();
				      }
        }
               object1.put("msg",msg);
			   System.out.println(msg);
	           System.out.println(object1);
	        
	           out.println(object1);
	           out.flush();
	        System.out.println(msg);
   }	
}
