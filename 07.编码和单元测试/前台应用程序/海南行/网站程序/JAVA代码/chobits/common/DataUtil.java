package chobits.common;

import java.io.UnsupportedEncodingException;

import chobits.log.Logger;

public class DataUtil {
	
	public static String CharSet_GBK = "GBK";
	public static String CharSet_GB2312 = "GB2312";
	public static String CharSet_ISO8859_1 = "ISO8859-1";
	public static String CharSet_UTF_8 = "UTF-8";
	
	public DataUtil(){}
	
	public String getEncodeString(String plainText,String formatCharSetType,String newCharSetType){
		String text = null;
		if(plainText == null || plainText.trim().length() == 0){
			text = "";
		}else{
			try {
				text = new String(plainText.getBytes(formatCharSetType),newCharSetType);
			} catch (UnsupportedEncodingException e) {
				Logger logger = new Logger(Logger.class.getName());
				logger.println(e.getMessage());
			}
		}
		return text;
	}
}
