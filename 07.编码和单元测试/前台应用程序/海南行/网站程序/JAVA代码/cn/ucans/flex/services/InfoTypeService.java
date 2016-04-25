package cn.ucans.flex.services;

import java.util.List;
import java.util.Map;
import chobits.db.DBBuilder;
import chobits.log.Logger;
import cn.ucans.bean.T_INFO_TYPE;
import cn.ucans.flex.FlexReponse;
import cn.ucans.flex.FlexRequest;

public class InfoTypeService extends FlexService{
	
	private Logger logger = new Logger(TerminalTypeService.class.getName());
	
	public FlexReponse doTransaction(FlexRequest request){
		logger.debug("-----------------------------��Ϣ�������ù������� ����-------------------------------");
		FlexReponse reponse = null;
		
		try{
			super.checkRole(request);
			int ope_id = request.ope_id;	
			switch(ope_id){
				case 9070:
					reponse = this.initInfoType(request);
					break;
				case 9071:
					reponse = this.addInfoType(request);
					break;
				case 9072:
					break;
				case 9073:
					reponse = this.updateInfoType(request);
					break;
				case 9074:
					reponse = this.deleteInfoType(request);
					break;
				default:
					break;
			}
			logger.debug("-----------------------------��Ϣ�������ù������� �ɹ����-------------------------------");
		}catch(Exception e){
			logger.error(e.getMessage());
			reponse = new FlexReponse(request,-1,e.getMessage());	
			logger.debug("-----------------------------��Ϣ�������ù������� ִ��ʧ��-------------------------------");
		}
		
		return reponse;
	}
	
	private FlexReponse initInfoType(FlexRequest request) throws Exception{
		
		String select_InfoTypeSql="Select Info_Type,Info_Name,User_ID,Create_Time  From T_Info_Type";
		DBBuilder builder = new DBBuilder();
		List<Map<String,Object>> list = builder.queryList(select_InfoTypeSql);
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ѯ��Ϣ����������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		FlexReponse reponse = new FlexReponse(request,0,null);	
		reponse.set("list", list);
		return reponse;
	}
	
	private FlexReponse addInfoType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		
		T_INFO_TYPE infoTypeForAdd = (T_INFO_TYPE) request.getBean("infoTypeForAdd");
		
		
		int count=builder.getCount("T_Info_Type", " Info_Name='"+infoTypeForAdd.INFO_NAME+"'");
		
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("��ӵ���Ϣ����������Ϣ�ڲ�ѯ��Ϣ����������Ϣʱ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		if(count > 0){
			throw new Exception("����ӵ���Ϣ����������Ϣ�Ѿ����ڣ����ʧ�ܣ���");
		}else{
			int countN=builder.getCount("T_Info_Type",null);
			String sql = "";
			if(countN==0){
				sql = "insert into T_Info_Type values(1,'"+infoTypeForAdd.INFO_NAME+"','"+infoTypeForAdd.USER_ID+"',sysdate)";
			}else{
				sql="insert into T_Info_Type values((select max(Info_Type)+1 from T_INFO_TYPE),'"+infoTypeForAdd.INFO_NAME+"','"+infoTypeForAdd.USER_ID+"',sysdate)";
			}
			 
			builder.excute(sql);
			
			if(builder.SQL_Error_Code != 0){			
				throw new Exception("��ӵ���Ϣ����������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
			}
		}	
		
		FlexReponse reponse = new FlexReponse(request,0,"�����Ϣ����������Ϣ�ɹ���");
		return reponse;
	}
	
	
	
	
	private FlexReponse updateInfoType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();

		T_INFO_TYPE infoTypeForUpdate = (T_INFO_TYPE) request.getBean("infoTypeForUpdate");
		T_INFO_TYPE oldInfoTypeForUpdate = (T_INFO_TYPE) request.getBean("oldInfoTypeForUpdate");
		
		int count = builder.getCount("T_Info_Type", " Info_Type="+oldInfoTypeForUpdate.INFO_TYPE + " and Info_Name='"+oldInfoTypeForUpdate.INFO_NAME+"'");
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�޸���Ϣ����������Ϣ����ȡ��Ϣ����������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count<=0){
				throw new Exception("�޸���Ϣ����������Ϣ������Ϣ����������Ϣ�Ѿ������ڻ��Ѿ����޸ģ���ȷ�Ϻ����޸ģ�");
			}
		}
		
		String update_sql="";
		update_sql = "update T_Info_Type set ";
		update_sql += " Info_Name ='"+infoTypeForUpdate.INFO_NAME+"',";
		update_sql += " User_ID ='"+infoTypeForUpdate.USER_ID+"',";
		update_sql += " Create_Time =sysdate";
		update_sql += " where Info_Type="+infoTypeForUpdate.INFO_TYPE;
		builder.excute(update_sql);
			
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("�޸���Ϣ����������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}
		
		FlexReponse reponse = new FlexReponse(request,0,"�޸���Ϣ����������Ϣ�ɹ���");
		
		return reponse;
	}
	
	private FlexReponse deleteInfoType(FlexRequest request) throws Exception{
		DBBuilder builder = new DBBuilder();
		T_INFO_TYPE Info_Type_util = (T_INFO_TYPE) request.getBean("Info_Type_util");
		
		int count = builder.getCount("T_Info_Type", " Info_Type="+Info_Type_util.INFO_TYPE + " and Info_Name='"+Info_Type_util.INFO_NAME+"'" );
		if(builder.SQL_Error_Code != 0){			
			throw new Exception("ɾ����Ϣ����������Ϣ����ȡ��Ϣ����������Ϣ����ԭ��"+builder.SQL_Error_Code+" "+builder.SQL_Error_Message);
		}else{
			if(count<=0){
				throw new Exception("ɾ����Ϣ����������Ϣ������Ϣ����������Ϣ�Ѿ������ڻ��Ѿ����޸ģ���ȷ�Ϻ���ɾ����");
			}
		}
		
		String sql="delete T_Info_Type where Info_Type="+Info_Type_util.INFO_TYPE + " and Info_Name='"+Info_Type_util.INFO_NAME+"'" ;
		builder.excute(sql);
		FlexReponse reponse = new FlexReponse(request,0,"ɾ����Ϣ����������Ϣ�ɹ���");
		return reponse;
	}
}
