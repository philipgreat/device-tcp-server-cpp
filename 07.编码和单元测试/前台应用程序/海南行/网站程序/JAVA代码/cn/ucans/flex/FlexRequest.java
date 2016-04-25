package cn.ucans.flex;

import java.util.Map;

public class FlexRequest{
	
	public int ope_id = -1;
	public String menu_id = "";
	public String user_id = "";		
	public String org_id = "";	
	public int type = -1;
	public Map<String,Object> form = null;
	
	public FlexRequest(){
		
	}
	
	public boolean checkRole(){
		this.user_id = user_id;
		this.menu_id = menu_id;
		this.ope_id = ope_id;
		System.out.println("²Ù×÷ÀàÐÍ£º"+this.ope_id+","+this.menu_id+","+this.user_id);
		return false;
	}
	
	public Object getBean(String key){
		if(this.form.containsKey(key)){
			return this.form.get(key);
		}else{
			return null;
		}
	}
}
