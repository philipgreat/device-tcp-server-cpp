package cn.ucans.flex.services;

import java.util.List;
import java.util.Map;

import chobits.log.Logger;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;
import cn.ucans.param.ParamHandler;

public class ParamFlexService extends FlexService{
	
	private Logger logger = new Logger(ParamFlexService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------����ѡ�������ʼ������ ����-------------------------------");
		FlexReponse reponse = null;
		
		try{
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9001:
					reponse = this.init_AddUserInfoDialog_Param(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------����ѡ�������ʼ������ �ɹ����-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------����ѡ�������ʼ������ ִ��ʧ��-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse init_AddUserInfoDialog_Param(FlexRequest request) throws Exception{
		
		String type_01 = (String)request.getBean("type_01");
		String type_02 = (String)request.getBean("type_02");
		
		ParamHandler handler = new ParamHandler();
		List<Map<String,Object>> list_role_id = handler.getT_Role_Info__Role_ID(type_01);
		if(handler.Error_Code != 0){
			throw new Exception("����û���Ϣ����ʼ��ѡ���������ɫ��ţ�ʧ�ܣ�ԭ��"+handler.Error_Code+" "+handler.Error_Message);
		}
		
		List<Map<String,Object>> list_area_code = handler.getT_T_Area_code_Param__Area_Code(type_02);
		if(handler.Error_Code != 0){
			throw new Exception("����û���Ϣ����ʼ��ѡ��������������룩ʧ�ܣ�ԭ��"+handler.Error_Code+" "+handler.Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list_role_id", list_role_id);
		reponse.set("list_area_code", list_area_code);
		return reponse;
	}
}
