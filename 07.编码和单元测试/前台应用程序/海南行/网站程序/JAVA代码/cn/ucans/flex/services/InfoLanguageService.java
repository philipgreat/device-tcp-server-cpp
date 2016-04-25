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
		logger.debug("-----------------------------�����������ù������� ����-------------------------------");
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
			logger.debug("-----------------------------�����������ù������� �ɹ����-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------�����������ù������� ִ��ʧ��-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initInfoLanguageType(FlexRequest request) throws Exception{
		
		String select_InfoLanguageTypeSql="Select Language,Language_No,Language_Name,User_ID,Insert_Time  From T_Info_Language_List";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_InfoLanguageTypeSql);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ѯ��������������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
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
			throw new Exception("��ӵ���������������Ϣ�ڲ�ѯ��������������Ϣʱ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		if(count > 0){
			throw new Exception("����ӵ��������ͻ��������������Ѿ����ڣ���������,�����������Ƽ�����ID�������ظ������ʧ�ܣ���");
		}else{
			int lang_No=0;
			try{
				lang_No= Integer.parseInt(infoLanguageTypeForAdd.LANGUAGE_NO);
			}catch(Exception e){
				throw new Exception("����ӵ�����ID����Ϊ������С��100�����������ʧ�ܣ���");
			}
			String sql = "insert into T_Info_Language_List values('"+infoLanguageTypeForAdd.LANGUAGE+"',"+lang_No+",'"+infoLanguageTypeForAdd.LANGUAGE_NAME+"','"+infoLanguageTypeForAdd.USER_ID+"',sysdate)";
			 
			builder.excute(sql);
			
			if(builder.SQL_Error_Code != 0){			
				throw new Exception("��ӵ���������������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
		}	
		
		FlexReponse reponse = new FlexReponse(request,0,"�����������������Ϣ�ɹ���");
		return reponse;
	}
	
	
	
	
	private FlexReponse updateInfoLanguageType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();

		T_INFO_LANGUAGE_LIST languageInfoTypeForUpdate = (T_INFO_LANGUAGE_LIST) request.getBean("languageInfoTypeForUpdate");
		T_INFO_LANGUAGE_LIST oldLanguageInfoTypeForUpdate = (T_INFO_LANGUAGE_LIST) request.getBean("oldLanguageInfoTypeForUpdate");
		
		int count = builder.getCount("T_INFO_LANGUAGE_LIST", " Language_No="+oldLanguageInfoTypeForUpdate.LANGUAGE_NO +" and Language='"+oldLanguageInfoTypeForUpdate.LANGUAGE+"' and Language_Name='"+oldLanguageInfoTypeForUpdate.LANGUAGE_NAME+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�޸���������������Ϣ����ȡ��������������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count<=0){
				throw new Exception("�޸���������������Ϣ������������������Ϣ�Ѿ������ڻ��Ѿ����޸ģ���ȷ�Ϻ����޸ģ�");
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
			 
			return new FlexReponse(request,0,"�޸���������������Ϣ�ɹ���");
		}
		
		
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�޸ĵ���������������Ϣ�ڲ�ѯ��������������Ϣʱ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		if(countN > 0){
			throw new Exception("���޸ĵ��������ͻ��������������Ѿ����ڣ��������ͺ������������Ʋ������ظ������ʧ�ܣ���");
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
				throw new Exception("�޸���������������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"�޸���������������Ϣ�ɹ���");
		
		return reponse;
	}
	
	private FlexReponse deleteInfoLanguageType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_INFO_LANGUAGE_LIST languageInfoTypeForDel = (T_INFO_LANGUAGE_LIST) request.getBean("languageInfoTypeForDel");
		
		int count = builder.getCount("T_INFO_LANGUAGE_LIST", " Language_No="+languageInfoTypeForDel.LANGUAGE_NO +" and Language='"+languageInfoTypeForDel.LANGUAGE+"' and Language_Name='"+languageInfoTypeForDel.LANGUAGE_NAME+"'");
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�޸���������������Ϣ����ȡ��������������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count<=0){
				throw new Exception("�޸���������������Ϣ������������������Ϣ�Ѿ������ڻ��Ѿ����޸ģ���ȷ�Ϻ���ɾ����");
			}
		}
		
		String sql="delete T_INFO_LANGUAGE_LIST where Language_No="+languageInfoTypeForDel.LANGUAGE_NO;
		builder.excute(sql);
		FlexReponse reponse = new FlexReponse(request,0,"ɾ����������������Ϣ�ɹ���");
		return reponse;
	}
}
