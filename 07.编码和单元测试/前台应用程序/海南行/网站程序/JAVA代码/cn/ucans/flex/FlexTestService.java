package cn.ucans.flex;

import cn.ucans.bean.T_USER_INFO;

public class FlexTestService {
	
	public FlexTestService(){
		
	}
	
	public FlexReponse trackLogin(FlexRequest request){
		FlexReponse reponse = new FlexReponse(request,0,"水野亚美");
		T_USER_INFO user = new T_USER_INFO();
		user.USER_NAME = "月野兔";
		reponse.set("user", user);
		return reponse;
	}
}
