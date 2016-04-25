package chobits.common;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RegularExpression {
	
	public RegularExpression(){}
	
	public boolean isNumeric(String str){
		boolean ok = true;
		if(str == null || str.trim().length()==0){
			ok = false;
		}else{
			Pattern pattern = Pattern.compile("[0-9]*");
			Matcher isNum = pattern.matcher(str);
			if(!isNum.matches()){
				ok = false;
			}
		}
		return ok;
	} 
}
