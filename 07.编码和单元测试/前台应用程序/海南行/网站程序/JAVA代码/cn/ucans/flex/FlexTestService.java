package cn.ucans.flex;

import cn.ucans.bean.T_USER_INFO;

public class FlexTestService {
	
	public FlexTestService(){
		
	}
	
	public FlexReponse trackLogin(FlexRequest request){
		FlexReponse reponse = new FlexReponse(request,0,"ˮҰ����");
		T_USER_INFO user = new T_USER_INFO();
		user.USER_NAME = "��Ұ��";
		reponse.set("user", user);
		return reponse;
	}
}
