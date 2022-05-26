package data.ProcessManager;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class Linux {
	public static ArrayList<String> value()throws IOException
	{
		Process pd;
		 ArrayList<String> ar = new ArrayList<>();
		try{
          String command = "ps -eo user,pid,%mem,%cpu,cmd ";
          pd = Runtime.getRuntime().exec(command);
          BufferedReader std = new BufferedReader(new InputStreamReader(pd.getInputStream()));
			String line;
			int j=0;
			while((line = std.readLine())!=null)
    		{
    			String std1 = (String)line.replaceAll("\\s+", " ");
    			System.out.println(std1);
    			String[] arr = std1.split("\\s",5);
    			for(String w:arr)
    			{
    				System.out.print(w+"<-->");
    			}
    			System.out.println();
    			++j;
    			if(j>=2)
    			{
    				ar.add(std1);
    			}
  		}
	   }catch(Exception e)
		{
		e.printStackTrace();
		}
		return ar;
	}

}
