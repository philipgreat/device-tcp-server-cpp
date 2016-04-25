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
		logger.debug("-----------------------------界面选择参数初始化任务 启动-------------------------------");
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
			logger.debug("-----------------------------界面选择参数初始化任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------界面选择参数初始化任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse init_AddUserInfoDialog_Param(FlexRequest request) throws Exception{
		
		String type_01 = (String)request.getBean("type_01");
		String type_02 = (String)request.getBean("type_02");
		
		ParamHandler handler = new ParamHandler();
		List<Map<String,Object>> list_role_id = handler.getT_Role_Info__Role_ID(type_01);
		if(handler.Error_Code != 0){
			throw new Exception("添加用户信息，初始化选择参数（角色编号）失败，原因："+handler.Error_Code+" "+handler.Error_Message);
		}
		
		List<Map<String,Object>> list_area_code = handler.getT_T_Area_code_Param__Area_Code(type_02);
		if(handler.Error_Code != 0){
			throw new Exception("添加用户信息，初始化选择参数（地区代码）失败，原因："+handler.Error_Code+" "+handler.Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);
		reponse.set("list_role_id", list_role_id);
		reponse.set("list_area_code", list_area_code);
		return reponse;
	}
}
