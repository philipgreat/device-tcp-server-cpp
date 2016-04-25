package chobits.common;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.Locale;

import chobits.log.Logger;

public class DateUtil {
	
	public static int NOTBEFORE = 1;
	public static int NOTAFTER = 2;
	
	public static  int Format_EN_yyyyMMddHHmmss = 1;
	public static  int Format_EN_yyyy_MM_dd_HH_mm_ss = 2;
	public static  int Format_CN_yyyy_MM_dd_HH_mm_ss = 3;
	public static  int Format_EN_HH_mm_ss = 4;
	public static  int Format_EN_HHmmss = 5;
	
	public static int Open = 1;
	public static int Closed = 1;
	
	public DateUtil(){}
	
	public long getMillisecondTime(){
		Date date = new Date();
		return date.getTime();
	}
	
	public boolean compared(String ymdhmsTime01, String ymdhmsTime02,int formatType ,int comparedType){
		long time01 = this.getMillisecondTime(ymdhmsTime01, formatType);
		long time02 = this.getMillisecondTime(ymdhmsTime02, formatType);
		if(comparedType == 1){
			return time01 > time02;
		}else{
			return time01 >= time02;
		}
	}
	
	public boolean compared(long millisecondTime01, long millisecondTime02 ,int comparedType){
		if(comparedType == 1){
			return millisecondTime01 > millisecondTime02;
		}else{
			return millisecondTime01 >= millisecondTime02;
		}
	}
	
	public String max(List<String> list_ymdhmsTime,int formatType){
		List<String> list = new ArrayList<String>();
		for(int i=0;i<list_ymdhmsTime.size();i++){
			list.add(list_ymdhmsTime.get(i));
		}
		long maxTime_long = Long.parseLong(Collections.max(list).toString());
		String maxTime = this.getYMDHMSTime(maxTime_long, formatType);
		return maxTime;
	}
	
	public long min(List<Long> list_millisecondTime){
		long maxTime_long = Long.parseLong(Collections.min(list_millisecondTime).toString());
		return maxTime_long;
	}
	
	public String min(List<String> list_ymdhmsTime,int formatType){
		List<Long> list = new ArrayList<Long>();
		for(int i=0;i<list_ymdhmsTime.size();i++){
			list.add(Long.parseLong(list_ymdhmsTime.get(i)));
		}
		long maxTime_long = Long.parseLong(Collections.min(list).toString());
		String maxTime = this.getYMDHMSTime(maxTime_long, formatType);
		return maxTime;
	}
	
	public long max(List<Long> list_millisecondTime){
		long maxTime_long = Long.parseLong(Collections.max(list_millisecondTime).toString());
		return maxTime_long;
	}
	
	public long getMillisecondTime(String time,int formatType){
		String pattern = null;
		if(formatType == 1){
			pattern = "yyyyMMddHHmmss";			
		}else if(formatType == 2){
			pattern = "yyyy-MM-dd HH:mm:ss";			
		}else if(formatType == 3){
			pattern = "yyyy年MM月dd日 HH时mm分ss秒";			
		}else if(formatType == 4){
			pattern = "HH:mm:ss";		
		}else if(formatType == 5){
			pattern = "HHmmss";
		}else{
			pattern = "yyyy-MM-dd HH:mm:ss";			
		}	
		SimpleDateFormat sdf = new SimpleDateFormat(pattern, Locale.SIMPLIFIED_CHINESE);
		Date date = null;		  
		try{			  
			date = sdf.parse(time);  
		} catch (ParseException e){			  
			Logger logger = new Logger(Logger.class.getName());
			logger.println(e.getMessage());
		}		  
		return date.getTime();		  
	 } 
	
	 public long getMillisecondTime(int formatType){
		String pattern = null;
		if(formatType == 1){
			pattern = "yyyyMMddHHmmss";			
		}else if(formatType == 2){
			pattern = "yyyy-MM-dd HH:mm:ss";			
		}else if(formatType == 3){
			pattern = "yyyy年MM月dd日 HH时mm分ss秒";			
		}else if(formatType == 4){
			pattern = "HH:mm:ss";		
		}else if(formatType == 5){
			pattern = "HHmmss";
		}else{
			pattern = "yyyy-MM-dd HH:mm:ss";			
		}	
		SimpleDateFormat sdf = new SimpleDateFormat(pattern, Locale.SIMPLIFIED_CHINESE);
		Date date = new Date();		  
		try{			  
			date = sdf.parse(String.valueOf(date.getTime()));  
		} catch (ParseException e){			  
			Logger logger = new Logger(Logger.class.getName());
			logger.println(e.getMessage());  
		}		  
		return date.getTime();		  
	 } 
	 
	 public String getYMDHMSTime(long millisecondtime, int formatType){
		 String pattern = null;
		 if(formatType == 1){
			 pattern = "yyyyMMddHHmmss";			
		 }else if(formatType == 2){
			 pattern = "yyyy-MM-dd HH:mm:ss";			
		 }else if(formatType == 3){
			 pattern = "yyyy年MM月dd日 HH时mm分ss秒";			
		 }else if(formatType == 4){
			 pattern = "HH:mm:ss";		
		 }else if(formatType == 5){
			 pattern = "HHmmss";
		 }else{
			 pattern = "yyyy-MM-dd HH:mm:ss";			
		 }	
		 Date date = new Date(millisecondtime);        
		 SimpleDateFormat format = new SimpleDateFormat();
		 format.applyPattern(pattern);
		 String strDate = format.format(date);						
		 return strDate;
	 }
	 
	 public String getYMDHMSTime(int formatType){
		 String pattern = null;
		 if(formatType == 1){
			 pattern = "yyyyMMddHHmmss";			
		 }else if(formatType == 2){
			 pattern = "yyyy-MM-dd HH:mm:ss";			
		 }else if(formatType == 3){
			 pattern = "yyyy年MM月dd日 HH时mm分ss秒";			
		 }else if(formatType == 4){
			 pattern = "HH:mm:ss";		
		 }else if(formatType == 5){
			 pattern = "HHmmss";
		 }else{
			 pattern = "yyyy-MM-dd HH:mm:ss";			
		 }	
		 Date date = new Date();        
		 SimpleDateFormat format = new SimpleDateFormat();
		 format.applyPattern(pattern);
		 String strDate = format.format(date);						
		 return strDate;
	 }
	 
	 public String getStringTime(String pattern){
		 Date date = new Date();        
		 SimpleDateFormat format = new SimpleDateFormat();
		 format.applyPattern(pattern);
		 String strDate = format.format(date);						
		 return strDate;
	 }
	
	 public static void main(String[] args)throws Exception{
		 DateUtil du = new DateUtil();
		 System.out.println(du.getStringTime("yyyyMM"));
	 }
}
