package com.ActiveDirectory;

import java.util.ArrayList;

public class GetADdata
{
	//C:\ADUsersDataEclipse\x64\Debug
 static {
	 try {
      //System.loadLibrary("ADUsersDataEclipse");
		 System.load("C:\\ADUsersDataEclipse.dll");
		 }catch(UnsatisfiedLinkError e) {
	        System.err.println("Native code library failed to load.\n" + e);
	 }
   }
   public native ArrayList<String> GetData(String alert);
   public static void main(String[] args)
     {
	   GetADdata detail = new GetADdata();
	  ArrayList<String> data = detail.GetData("Entering into c++");
	  int i=0;
	  for(String user:data)
	  {
		  System.out.println("<<<<<<<<<<<<<<<< user " + i+" is >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		  //System.out.println(user);
		  String arr[] = user.split("\n");
		  System.out.println("Size of array is : "+arr.length);		  
		  for(String innerdata:arr)
		  {
			  System.out.println(innerdata);
		  }
		  i=i+1;
	  }
    }
}
