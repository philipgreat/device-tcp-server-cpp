package cn.ucans.flex;

import java.util.HashMap;
import java.util.Map;

public class FlexReponse{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	public int sign = -1;
	public String message = null;
	
	public int ope_id = -1;
	public String menu_id = "";
	public String user_id = "";
	public String org_id = "";	
	
	public Map<String, Object> result = null; 
	
	public FlexReponse(){
		result = new HashMap<String,Object>();
	}
	
	public FlexReponse(FlexRequest request,int sign,String message){
		result = new HashMap<String,Object>();
		this.ope_id = request.ope_id;
		this.user_id = request.user_id;
		this.menu_id = request.menu_id;
		this.sign = sign;
		this.message = message;
	}
	
	public void set(String key, Object object){
		result.put(key, object);
	}
	
	public void remove(String key){
		if(result.containsKey(key)){
			result.remove(key);
		}
	}
}
