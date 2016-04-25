package chobits.log;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import org.jdom.Element;

import chobits.common.DataUtil;
import chobits.common.DateUtil;
import chobits.init.ChobitsConfig;


public class Logger {
	
	public static String PrintScrean = "";
	public static String PrintScreanEncoded = "";
	public static String WritedFileEncoded = "";
	
	public static String Encode_File_FormatCharSet = DataUtil.CharSet_GBK;
	public static String Encode_File_newCharSet = DataUtil.CharSet_ISO8859_1;
	public static String Encode_Screen_FormatCharSet = DataUtil.CharSet_GBK;
	public static String Encode_Screen_newCharSet = DataUtil.CharSet_GBK;
	
	private static String logFolderPath = "./log";
	private static String LogFileLink = "";
	private static String FileName = "";
	
	private static String Rule_FileName = "chobits_";
	private static List<String> Title_List = new ArrayList<String>();
	
	private static String Allow_Debug = null;
	private static String Allow_Info = null;
	private static String Allow_Error = null;
	private static String Allow_Warn = null;
	private static String Allow_Fatal = null;
	
	private String className = null;
	
	public Logger(String className){
		this.className = className;
	}
	
	public static void init(Element root_element){
		
		String path = root_element.getChildText("path").trim();		
		Logger.logFolderPath = path;
		String level = root_element.getChildText("level").trim();
		if(level != null && level.trim().length() > 0){
			String[] elem = level.split(",");
			Vector<String> vector = new Vector<String>();
			if(elem.length > 0){
				for(int i=0;i<elem.length;i++){
					vector.add(elem[i]);
				}
				if(vector.contains("debug")){
					Logger.Allow_Debug = "debug";
				}
				if(vector.contains("info")){
					Logger.Allow_Info = "info";
				}
				if(vector.contains("error")){
					Logger.Allow_Error = "error";
				}
				if(vector.contains("warn")){
					Logger.Allow_Warn = "warn";
				}
				if(vector.contains("fatal")){
					Logger.Allow_Fatal = "fatal";
				}
			}			
		}
		Element element_out = root_element.getChild("out");
		String doPrintScreen = element_out.getAttributeValue("doPrintScrean").trim();
		Element element_screen = element_out.getChild("screen");
		String PrintScreanEncoded = element_screen.getAttributeValue("PrintScreanEncoded");
		String Encode_Screen_FormatCharSet = element_screen.getChildTextTrim("Encode_Screen_FormatCharSet");
		String Encode_Screen_newCharSet = element_screen.getChildTextTrim("Encode_Screen_newCharSet");
		Logger.setScreenCharSet(Encode_Screen_FormatCharSet, Encode_Screen_newCharSet, PrintScreanEncoded, doPrintScreen);
		
		Element element_file = element_out.getChild("file");
		String WritedFileEncoded = element_file.getAttributeValue("WritedFileEncoded");
		String Encode_File_FormatCharSet = element_screen.getChildTextTrim("Encode_File_FormatCharSet");
		String Encode_File_newCharSet = element_screen.getChildTextTrim("Encode_File_newCharSet");
		Logger.setFileCharSet(Encode_File_FormatCharSet, Encode_File_newCharSet, WritedFileEncoded);
		
		
		Logger.Title_List.add("=========================Chobits Log===========================");
		Logger.Title_List.add("=                 Version 2.0_build20081222100300             =");
		Logger.Title_List.add("=                 Author takuya                               =");
		Logger.Title_List.add("=                 Company China                               =");
		Logger.Title_List.add("=========================Chobits Log===========================");
		
		File folder = new File(logFolderPath);
		if(!folder.exists()){
			folder.mkdirs();
		}
		if(Logger.checkFile()){
			Logger.creatNewFile();
		}
		
		Logger.startClearServer();
	}
	
	public static void startClearServer(){
		LoggCleanerServer server = new LoggCleanerServer(Logger.logFolderPath,ChobitsConfig.TempFileForderPath);
		Thread thread = new Thread(server);
		thread.start();
	}
	
	public static void setRule(List<String> titleRule, String fileNameRule){
		if(titleRule != null && titleRule.size()>0){
			Logger.Title_List = titleRule;
		}
		if(fileNameRule != null && fileNameRule.trim().length()>0){
			Logger.Rule_FileName = fileNameRule;
		}		
	}
	
	public static void setFileCharSet(String old_charset, String new_charset,String encoded){
		if(old_charset != null){
			Logger.Encode_File_FormatCharSet = old_charset;
		}
		if(new_charset != null){
			Logger.Encode_File_newCharSet = new_charset;
		}	
		Logger.WritedFileEncoded = encoded;
	}
	
	public static void setScreenCharSet(String old_charset, String new_charset,String encoded,String doPrintScreen){
		if(old_charset != null){
			Logger.Encode_Screen_FormatCharSet = old_charset;
		}		
		if(new_charset != null){
			Logger.Encode_Screen_newCharSet = new_charset;
		}	
		Logger.PrintScreanEncoded = encoded;
		Logger.PrintScrean = doPrintScreen;
	}
	
	private static boolean checkFile(){
		boolean ok = false;
		File folder = new File(Logger.logFolderPath);
		File file[] = folder.listFiles();
		if (file.length < 1){
			ok = true;
		}else{
			long tempTime = 0L;
			for (int i = 0; i < file.length; i++){
				long lastModify = file[i].lastModified();
				if (lastModify >= tempTime)
					tempTime = lastModify;
			}
			DateUtil date_builder = new DateUtil();
			long nowTime = date_builder.getMillisecondTime();
			ok = nowTime >= tempTime;
		}
		return ok;
	}
	
	public void writeFile(Object object,String format){
		File file = new File(LogFileLink);
		long length = file.length();
		if (length >= 0xa00000L || !file.exists())
			creatNewFile();
		DateUtil date_builder = new DateUtil();
		String time = date_builder.getYMDHMSTime(DateUtil.Format_EN_yyyy_MM_dd_HH_mm_ss);
		RandomAccessFile rf = null;
		try{
			rf = new RandomAccessFile(LogFileLink, "rw");
			rf.seek(rf.length());
			if(Logger.PrintScrean.equals("true")){
				this.println(object.toString());
			}
			String text = "["+time+"]["+format+"]["+this.className+"] "+object.toString()+"\n";
			if(Logger.WritedFileEncoded.equals("true")){
				DataUtil data_builder = new DataUtil();
				text = data_builder.getEncodeString(text, Logger.Encode_File_FormatCharSet, Logger.Encode_File_newCharSet);
			}			
			rf.writeBytes(text);
			rf.close();
		}catch (Exception e){
			Logger logger = new Logger(Logger.class.getName());
			logger.println(e.getMessage());
		}
	}
	
	private static void creatNewFile(){
		BufferedWriter out = null;
		try{
			DateUtil date_builder = new DateUtil();
			String time = date_builder.getYMDHMSTime(DateUtil.Format_EN_yyyyMMddHHmmss);
			Logger.FileName = Logger.getFileName(time);
			Logger.LogFileLink = logFolderPath+"/"+FileName;
			out = new BufferedWriter(new FileWriter(Logger.LogFileLink));
			out.newLine();
			out.write("Provider:Chobits		CreatTime:  "+date_builder.getYMDHMSTime(DateUtil.Format_EN_yyyy_MM_dd_HH_mm_ss)+"\n");
			out.newLine();
			for (int i = 0; i < Logger.Title_List.size(); i++){	
				String text = (String) Logger.Title_List.get(i);
				DataUtil data_builder = new DataUtil();
				text = data_builder.getEncodeString(text, Logger.Encode_File_FormatCharSet, Logger.Encode_File_newCharSet);
				out.write(text);
				out.newLine();
			}
			out.newLine();
			out.newLine();
			out.close();
		}catch (IOException e){
			Logger logger = new Logger(Logger.class.getName());
			logger.println(e.getMessage());
		}
	}
	
	private static String getFileName(String time){
		return Logger.Rule_FileName + time + ".log";
	}
	
	public void debug(Object object){
		if(Logger.Allow_Debug != null){
			this.writeFile(object, "debug");
		}
	}
	
	public void info(Object object){
		if(Logger.Allow_Info != null){
			this.writeFile(object, "info");
		}
	}

	public void warn(Object object){
		if(Logger.Allow_Warn != null){
			this.writeFile(object, "warn");
		}
	}
	
	public void error(Object object){
		if(Logger.Allow_Error != null){
			this.writeFile(object, "error");
		}
	}
	
	public void fatal(Object object){
		if(Logger.Allow_Fatal != null){
			this.writeFile(object, "fatal");
		}
	}
	
	public void println(Object object){
		String text = null;
		DateUtil date_builder = new DateUtil();
		String time = date_builder.getYMDHMSTime(DateUtil.Format_EN_yyyy_MM_dd_HH_mm_ss);
		text = "["+time+"][screen]["+this.className+"] "+object.toString();
		if(Logger.PrintScreanEncoded.equals("true")){	
			DataUtil data_builder = new DataUtil();
			text = data_builder.getEncodeString(text, Logger.Encode_Screen_FormatCharSet, Logger.Encode_Screen_newCharSet);
		}
		System.out.println(text);
	}
}
