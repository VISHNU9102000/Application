package data.ProcessManager;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;


public class LoginDao {
	public static Connection connect()throws ClassNotFoundException
	{
		Connection connection = null;

	        try {
					Class.forName("com.mysql.cj.jdbc.Driver");
	        	 connection= DriverManager.getConnection("jdbc:mysql://localhost:3306/taskmanager", "root", "vishnu");
	        	}catch(SQLException e){
	        		e.printStackTrace();
	        }
	        return connection;
	}

    public boolean validate(LoginBean loginBean) throws ClassNotFoundException {
        boolean status = true;
          Connection con = LoginDao.connect();
            try(PreparedStatement preparedStatement = con
            .prepareStatement("select * from people where Name = ? and password = ? ")) {
            preparedStatement.setString(1, loginBean.getUsername());
            preparedStatement.setString(2, loginBean.getPassword());

            System.out.println(preparedStatement);
            ResultSet rs = preparedStatement.executeQuery();
            status = rs.next();

        } catch (SQLException e) {
            // process sql exception
            printSQLException(e);
        }
        return status;
    }
    public boolean Role(LoginBean loginBean) throws ClassNotFoundException {
        boolean status = false;
        String role = null;
          Connection connect = LoginDao.connect();
            try(PreparedStatement preparedStatement2 = connect.prepareStatement("SELECT Role FROM people WHERE Name=?")) {
            	 preparedStatement2.setString(1, loginBean.getUsername());
                 System.out.println(preparedStatement2);
                 ResultSet res = preparedStatement2.executeQuery();
                 
                 while(res.next() != false){

                     role = res.getString("Role");
                     System.out.println("Role : "+role);
                 }
                 
                 if(role.equals("Admin"))
                 {
                	 //System.out.println(role);
                	 status=true;
                 }

        } catch (SQLException e) {
            // process sql exception
            printSQLException(e);
        }
        return status;
    }
    public boolean CreateUser(LoginBean loginBean) throws ClassNotFoundException {
    	boolean pass = false;
    	int success=0;
    	Connection getCon = LoginDao.connect();
		try(PreparedStatement preparedStatement3 = getCon.prepareStatement("insert into people values(?,?,?)")) {
       	preparedStatement3.setString(1, loginBean.getUsername());
       	preparedStatement3.setString(2, loginBean.getPassword());
       	preparedStatement3.setString(3, "User");
        success= preparedStatement3.executeUpdate();
        if(success>0)
        {
        	pass=true;
        }
   } catch (SQLException e) {
       // process sql exception
       printSQLException(e);
   }
		return pass;
}
    public boolean CheckUser(LoginBean loginBean) throws ClassNotFoundException {
    	boolean check =false;
    	Connection set = LoginDao.connect();
    	try{
    		Statement st = set.createStatement();
    		ResultSet rs = st.executeQuery("select * from people");
    		
    		while (rs.next()) 
            {  
    			String name = rs.getString("Name");
    			System.out.println("query output : "+name);
    			String checkname = loginBean.getUsername();
    			System.out.println("name from loginbean : "+checkname);
    			if(loginBean.getUsername().equals(name))
    			{
    				check=true;
    				break;
    			}
            }
    	}catch (Exception e) 
            {  
            	e.printStackTrace();  
            }
    	System.out.println("Check status " +check);
		return check;  
    }

    private void printSQLException(SQLException ex) {
        for (Throwable e: ex) {
            if (e instanceof SQLException) {
                e.printStackTrace(System.err);
                System.err.println("SQLState: " + ((SQLException) e).getSQLState());
                System.err.println("Error Code: " + ((SQLException) e).getErrorCode());
                System.err.println("Message: " + e.getMessage());
                Throwable t = ex.getCause();
                while (t != null) {
                    System.out.println("Cause: " + t);
                    t = t.getCause();
                }
            }
        }
    }
}

	

