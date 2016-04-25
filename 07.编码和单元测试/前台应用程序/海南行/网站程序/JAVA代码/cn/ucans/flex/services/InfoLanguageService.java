package cn.ucans.flex.services;

import java.util.List;
import java.util.Map;
import chobits.db.DBBuilder;
import chobits.log.Logger;
import cn.ucans.bean.T_INFO_LANGUAGE_LIST;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;

public class InfoLanguageService extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------语言类型配置管理任务 启动-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9080:
					reponse = this.initInfoLanguageType(request);
					break;
				case 9081:
					reponse = this.addInfoLanguageType(request);
					break;
				case 9082:
					break;
				case 9083:
					reponse = this.updateInfoLanguageType(request);
					break;
				case 9084:
					reponse = this.deleteInfoLanguageType(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------语言类型配置管理任务 成功完成-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------语言类型配置管理任务 执行失败-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initInfoLanguageType(FlexRequest request) throws Exception{
		
		String select_InfoLanguageTypeSql="Select Language,Language_No,Language_Name,User_ID,Insert_Time  From T_Info_Language_List";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_InfoLanguageTypeSql);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("查询语言类型配置信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse addInfoLanguageType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		
		T_INFO_LANGUAGE_LIST infoLanguageTypeForAdd = (T_INFO_LANGUAGE_LIST) request.getBean("infoLanguageTypeForAdd");
		
		
		int count=builder.getCount("T_Info_Language_List", " Language='"+infoLanguageTypeForAdd.LANGUAGE+"' or Language_Name='"+infoLanguageTypeForAdd.LANGUAGE_NAME+"'  or Language_No="+ infoLanguageTypeForAdd.LANGUAGE_NO);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("添加的语言类型配置信息在查询语言类型配置信息时出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		if(count > 0){
			throw new Exception("您添加的语言类型或语言类型名称已经存在！语言类型,语言类型名称及语言ID不允许重复！添加失败！！");
		}else{
			int lang_No=0;
			try{
				lang_No= Integer.parseInt(infoLanguageTypeForAdd.LANGUAGE_NO);
			}catch(Exception e){
				throw new Exception("您添加的语言ID必须为大于零小于100的整数！添加失败！！");
			}
			String sql = "insert into T_Info_Language_List values('"+infoLanguageTypeForAdd.LANGUAGE+"',"+lang_No+",'"+infoLanguageTypeForAdd.LANGUAGE_NAME+"','"+infoLanguageTypeForAdd.USER_ID+"',sysdate)";
			 
			builder.excute(sql);
			
			if(builder.SQL_Error_Code != 0){			
				throw new Exception("添加的语言类型配置信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
		}	
		
		FlexReponse reponse = new FlexReponse(request,0,"添加语言类型配置信息成功！");
		return reponse;
	}
	
	
	
	
	private FlexReponse updateInfoLanguageType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();

		T_INFO_LANGUAGE_LIST languageInfoTypeForUpdate = (T_INFO_LANGUAGE_LIST) request.getBean("languageInfoTypeForUpdate");
		T_INFO_LANGUAGE_LIST oldLanguageInfoTypeForUpdate = (T_INFO_LANGUAGE_LIST) request.getBean("oldLanguageInfoTypeForUpdate");
		
		int count = builder.getCount("T_INFO_LANGUAGE_LIST", " Language_No="+oldLanguageInfoTypeForUpdate.LANGUAGE_NO +" and Language='"+oldLanguageInfoTypeForUpdate.LANGUAGE+"' and Language_Name='"+oldLanguageInfoTypeForUpdate.LANGUAGE_NAME+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("修改语言类型配置信息，获取语言类型配置信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count<=0){
				throw new Exception("修改语言类型配置信息，该语言类型配置信息已经不存在或已经被修改，请确认后再修改！");
			}
		}
		
		int countN =0;
		
		if(!oldLanguageInfoTypeForUpdate.LANGUAGE_NAME.equals(languageInfoTypeForUpdate.LANGUAGE_NAME) 
				&& !oldLanguageInfoTypeForUpdate.LANGUAGE.equals(languageInfoTypeForUpdate.LANGUAGE)){
			
			countN = builder.getCount("T_Info_Language_List", " Language='"+languageInfoTypeForUpdate.LANGUAGE
					+"' or Language_Name='"+languageInfoTypeForUpdate.LANGUAGE_NAME+"'");
			
		}else if(oldLanguageInfoTypeForUpdate.LANGUAGE_NAME.equals(languageInfoTypeForUpdate.LANGUAGE_NAME)&&!oldLanguageInfoTypeForUpdate.LANGUAGE.equals(languageInfoTypeForUpdate.LANGUAGE)){
			
			countN = builder.getCount("T_Info_Language_List", " Language='"+languageInfoTypeForUpdate.LANGUAGE+"'");
		}else if(!oldLanguageInfoTypeForUpdate.LANGUAGE_NAME.equals(languageInfoTypeForUpdate.LANGUAGE_NAME)&&oldLanguageInfoTypeForUpdate.LANGUAGE.equals(languageInfoTypeForUpdate.LANGUAGE)){
			
			countN = builder.getCount("T_Info_Language_List", " Language_Name='"+languageInfoTypeForUpdate.LANGUAGE_NAME+"'");
		}else if(oldLanguageInfoTypeForUpdate.LANGUAGE_NAME.equals(languageInfoTypeForUpdate.LANGUAGE_NAME) 
				&& oldLanguageInfoTypeForUpdate.LANGUAGE.equals(languageInfoTypeForUpdate.LANGUAGE)){
			 
			return new FlexReponse(request,0,"修改语言类型配置信息成功！");
		}
		
		
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("修改的语言类型配置信息在查询语言类型配置信息时出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		if(countN > 0){
			throw new Exception("您修改的语言类型或语言类型名称已经存在！语言类型和语言类型名称不允许重复！添加失败！！");
		}else{
		
		
			String update_sql="";
			update_sql = "update T_INFO_LANGUAGE_LIST set ";
			update_sql += " Language ='"+languageInfoTypeForUpdate.LANGUAGE+"',";
			update_sql += " Language_Name ='"+languageInfoTypeForUpdate.LANGUAGE_NAME+"',";
			update_sql += " Language_No ="+languageInfoTypeForUpdate.LANGUAGE_NO;
			update_sql += ", User_ID ='"+languageInfoTypeForUpdate.USER_ID+"',";
			update_sql += " Insert_Time =sysdate";
			update_sql += " where Language_No="+oldLanguageInfoTypeForUpdate.LANGUAGE_NO +" and Language_Name='"+oldLanguageInfoTypeForUpdate.LANGUAGE_NAME+"' and Language='"+oldLanguageInfoTypeForUpdate.LANGUAGE+"'";
			builder.excute(update_sql);
				
			if(builder.SQL_Error_Code != 0){			
				throw new Exception("修改语言类型配置信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"修改语言类型配置信息成功！");
		
		return reponse;
	}
	
	private FlexReponse deleteInfoLanguageType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_INFO_LANGUAGE_LIST languageInfoTypeForDel = (T_INFO_LANGUAGE_LIST) request.getBean("languageInfoTypeForDel");
		
		int count = builder.getCount("T_INFO_LANGUAGE_LIST", " Language_No="+languageInfoTypeForDel.LANGUAGE_NO +" and Language='"+languageInfoTypeForDel.LANGUAGE+"' and Language_Name='"+languageInfoTypeForDel.LANGUAGE_NAME+"'");
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("修改语言类型配置信息，获取语言类型配置信息出错，原因："+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count<=0){
				throw new Exception("修改语言类型配置信息，该语言类型配置信息已经不存在或已经被修改，请确认后再删除！");
			}
		}
		
		String sql="delete T_INFO_LANGUAGE_LIST where Language_No="+languageInfoTypeForDel.LANGUAGE_NO;
		builder.excute(sql);
		FlexReponse reponse = new FlexReponse(request,0,"删除语言类型配置信息成功！");
		return reponse;
	}
}
