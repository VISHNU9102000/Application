package data.ProcessManager;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.regex.Pattern;

public class Windows{
	public static ArrayList<String> value()throws IOException 
	{
		Process pd;
		 ArrayList<String> ar = new ArrayList<>();
		try{
           //String command = "powershell.exe -File 'Data.ps1'";
           String command = "powershell.exe Get-Process -Name * | Select-Object -Property Name, Id,@{Label = 'Memory'; Expression = {[math]::round($_.privatememorysize / 1mb,2)}},@{Name = 'CPUPercent';Expression = {$TotalSec = (New-TimeSpan -Start $_.StartTime).TotalSeconds;[Math]::Round( ($_.CPU * 100 / $TotalSec), 1)}}, Path|format-table -autosize";
           pd = Runtime.getRuntime().exec(command);
           BufferedReader std = new BufferedReader(new InputStreamReader(pd.getInputStream()));
			String line;
			int j=0;
			while((line = std.readLine())!=null)
    		{
    			String std1 = (String)line.replaceAll("\\s+", " ");
    			System.out.println(std1);
    			String[] arr = std1.split("\\s",5);
    			/*for(String w:arr)
    			{
    				System.out.print(w+"<-->");
    			}*/
    			//System.out.println();
    			++j;
    			if(j>=4)
    			{
    				try{
    					if(Pattern.matches("[a-zA-Z]*",arr[1]))
   		                {
   		                  //System.out.println("true");
   		                  std1 = std1.replaceFirst(" ", "");
                        }
    				}catch(Exception e)
                        {
                        	continue;
                        }
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
